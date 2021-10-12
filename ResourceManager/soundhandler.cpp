#include "soundhandler.h"
#include <iostream>
#include <sstream>
#include <QDebug>

#include "altypes.h"
#include "constants.h"
#include "logger.h"

SoundHandler::SoundHandler()
{
    //Hm - functions are static, so can not use this pointer...
    //mLogger = Logger::getInstance();    //Have to do this, else program will crash
}

WaveRawData *SoundHandler::loadWave(std::string fileName)
{
    WaveRawData* wavePtr{nullptr};

    Logger::getInstance()->logText("Reading " + fileName + " from disk");
    FILE* fp = NULL;
    fp = std::fopen((gsl::SoundFilePath + fileName).c_str(), "rb");
    if (fp == NULL)
    {
        endOnFileReadError("Sound file error: " + fileName + " not found.");
        return wavePtr;
    }

    char type[4];

    std::fread(type, sizeof(char), 4, fp);
    if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')
    {
        endOnFileReadError("Sound file error: RIFF header missing or invalid.");
        return wavePtr;
    }

    //Starting to use the WavRawData pointer:
    //File found so hopefully it is OK:
    wavePtr = new WaveRawData();

    std::fread(&wavePtr->size, sizeof(uint32_t), 1, fp);
    std::fread(type, sizeof(char), 4, fp);
    if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')
    {
        delete wavePtr;
        wavePtr = nullptr;
        endOnFileReadError("Sound file error: WAVE header missing or invalid.");
        return wavePtr;
    }

    std::fread(type, sizeof(char), 4, fp);
    if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')
    {
        delete wavePtr;
        wavePtr = nullptr;
        endOnFileReadError("Sound file error: fmt header missing or invalid.");
        return wavePtr;
    }

    std::fread(&wavePtr->chunkSize,       sizeof(uint32_t), 1, fp);
    std::fread(&wavePtr->formatType,      sizeof(short), 1, fp);
    std::fread(&wavePtr->channels,        sizeof(short), 1, fp);
    std::fread(&wavePtr->sampleRate,      sizeof(uint32_t), 1, fp);
    std::fread(&wavePtr->avgBytesPerSec,  sizeof(uint32_t), 1, fp);
    std::fread(&wavePtr->bytesPerSample,  sizeof(short), 1, fp);
    std::fread(&wavePtr->bitsPerSample,   sizeof(short), 1, fp);

    std::fread(type, sizeof(char), 4, fp);
    if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')
    {
        delete wavePtr;
        wavePtr = nullptr;
        endOnFileReadError("Sound file error: data header missing or invalid.");
        return wavePtr;
    }

    std::fread(&wavePtr->dataSize, sizeof(uint32_t), 1, fp);

    wavePtr->audioData = new unsigned char[wavePtr->dataSize];
    uint32_t result = std::fread(wavePtr->audioData, sizeof(char), wavePtr->dataSize, fp);
    if (result != wavePtr->dataSize)
    {
        delete wavePtr;
        wavePtr = nullptr;
        endOnFileReadError("Sound file error: fread result mismatch.");
        return wavePtr;
    }

    if (ferror(fp))
    {
        delete wavePtr;
        wavePtr = nullptr;
        endOnFileReadError("Sound file error: fstream error.");
        return wavePtr;
    }

    if (wavePtr->audioData == NULL)
    {
        delete wavePtr;
        wavePtr = nullptr;
        endOnFileReadError("Sound file error: Wave Data pointer is NULL.");
        return wavePtr;
    }

    std::fclose(fp);

    Logger::getInstance()->logText("Reading of " + fileName + " complete!");

    if(!makeALBuffer(wavePtr))
    {
        delete wavePtr;
        wavePtr = nullptr;
    }

    return wavePtr;
}

bool SoundHandler::makeALBuffer(WaveRawData *waveData)
{
    std::string tempText("  Making ALBuffer | ");
    alGetError();   //empty error buffer before we use it
    alGenBuffers(1, &waveData->mALBuffer);
    if(!checkALError("alGenBuffers"))
        return false;

    ALuint frequency{};
    ALenum format{};

    frequency = waveData->sampleRate;

    switch (waveData->bitsPerSample){
        case 8:
            switch (waveData->channels){
                case 1:
                    format = AL_FORMAT_MONO8;
                    tempText += " Format: 8bit Mono | ";
                    break;
                case 2:
                    format = AL_FORMAT_STEREO8;
                    tempText += " Format: 8bit Stereo | ";
                    break;
                default: break;
            }
            break;
        case 16:
            switch (waveData->channels){
                case 1:
                    format = AL_FORMAT_MONO16;
                    tempText += " Format: 16bit Mono | ";
                    break;
                case 2:
                    format = AL_FORMAT_STEREO16;
                    tempText += " Format: 16bit Stereo | ";
                    break;
                default: break;
            }
            break;
        default: break;
    }

    std::ostringstream i2s;
    i2s << waveData->dataSize;
    tempText += " DataSize: " + i2s.str() + " bytes";

    Logger::getInstance()->logText(tempText);

    alGetError();
    alBufferData(waveData->mALBuffer, format, waveData->audioData, waveData->dataSize, frequency);
    checkALError("alBufferData");
    if(!checkALError("alBufferData"))
        return false;
//    alSourcei(mSource, AL_BUFFER, waveData->mALBuffer);
//    checkALError("alSourcei (loadWave)");

    Logger::getInstance()->logText("  Making ALBuffer complete!");

    //Should we delete the raw data from RAM?
//    if (waveData->audioData)
//        delete waveData->audioData;
    return true;
}

int SoundHandler::makeALSource(ALuint bufferIn)
{
    ALuint tempSource;
    alGetError();
    alGenSources(1, &tempSource);
    if(!checkALError("alGenSources"))
        return -1;   //Fix - not totally correct. 0 is probably a
    alSourcei(tempSource, AL_BUFFER, bufferIn);
    if(!checkALError("alSourcei (makeALSource)"))
        return -1;

    return tempSource;
}

bool SoundHandler::endOnFileReadError(std::string errmsg)
{
    Logger::getInstance()->logText(errmsg, LColor::DAMNERROR);
    return false;
}

bool SoundHandler::checkALError(std::string name)
{
    switch (alGetError())
    {
    case AL_NO_ERROR:
        break;
    case AL_INVALID_NAME:
        Logger::getInstance()->logText("OpenAL Error: " + name + ": Invalid name!", LColor::DAMNERROR);
        return false;
    case AL_INVALID_ENUM:
        Logger::getInstance()->logText("OpenAL Error: " + name + ": Invalid enum!", LColor::DAMNERROR);
        return false;
    case AL_INVALID_VALUE:
        Logger::getInstance()->logText("OpenAL Error: " + name + ": Invalid value!", LColor::DAMNERROR);
        return false;
    case AL_INVALID_OPERATION:
        Logger::getInstance()->logText("OpenAL Error: " + name + ": Invalid operation!", LColor::DAMNERROR);
        return false;
    case AL_OUT_OF_MEMORY:
        Logger::getInstance()->logText("OpenAL Error: " + name + ": Out of memory!", LColor::DAMNERROR);
        return false;
    }

    return true;
}
