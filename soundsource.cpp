//#include "soundsource.h"
//#include "soundhandler.h"

//#include <sstream>
//#include <iostream>
//#include <QDebug>

//SoundSource::SoundSource(std::string name, bool loop, float gain) :
//    mName(name),
//    mSource(0),
//    mBuffer(0),
//    mPosition(0.0f, 0.0f, 0.0f),
//    mVelocity(0.0f, 0.0f, 0.0f)
//{
//    alGetError();
//    alGenBuffers(1, &mBuffer);
//    checkError("alGenBuffers");
//    alGenSources(1, &mSource);
//    checkError("alGenSources");
//    alSourcef(mSource, AL_PITCH, 1.0f);
//    alSourcef(mSource, AL_GAIN, gain);

//    ALfloat temp[3] = {mPosition.x, mPosition.y, mPosition.z};
//    alSourcefv(mSource, AL_POSITION, temp);
//    ALfloat temp2[3] = {mVelocity.x, mVelocity.y, mVelocity.z};
//    alSourcefv(mSource, AL_VELOCITY, temp2);

//    alSourcei(mSource, AL_LOOPING, loop);
//}
//SoundSource::~SoundSource()
//{
//    qDebug() << "Destroying SoundSource " << mName.c_str();
//    stop();
//    alGetError();
//    alSourcei(mSource, AL_BUFFER, 0);
//    checkError("alSourcei");
//    alDeleteSources(1, &mSource);
//    checkError("alDeleteSources");
//    alDeleteBuffers(1, &mBuffer);
//    checkError("alDeleteBuffers");
//}

//bool SoundSource::loadWave(std::string fileName)
//{
//    qDebug() << "Loading wave file!";
//    ALuint frequency{};
//    ALenum format{};
//    WaveRawData* waveData = SoundHandler::loadWave(fileName);
//    if (!waveData)
//    {
//        qDebug() << "Error loading wave file!";
//        return false; // error loading wave file data
//    }

//    frequency = waveData->sampleRate;

//    switch (waveData->bitsPerSample)
//    {
//    case 8:
//        switch (waveData->channels)
//        {
//        case 1: format = AL_FORMAT_MONO8;
//            qDebug() << "Format: 8bit Mono";
//            break;
//        case 2: format = AL_FORMAT_STEREO8;
//            qDebug() << "Format: 8bit Stereo";
//            break;
//        default: break;
//        }
//        break;
//    case 16:
//        switch (waveData->channels)
//        {
//        case 1: format = AL_FORMAT_MONO16;
//            qDebug() << "Format: 16bit Mono";
//            break;
//        case 2: format = AL_FORMAT_STEREO16;
//            qDebug() << "Format: 16bit Stereo";
//            break;
//        default: break;
//        }
//        break;
//    default: break;
//    }

//    if (waveData->audioData == NULL)
//    {
//        qDebug() << "NO WAVE DATA!";
//    }

//    std::ostringstream i2s;
//    i2s << waveData->dataSize;
//    qDebug() << "DataSize: " << i2s.str().c_str() << " bytes";

//    alGetError();
//    alBufferData(mBuffer, format, waveData->audioData, waveData->dataSize, frequency);
//    checkError("alBufferData");
//    alSourcei(mSource, AL_BUFFER, mBuffer);
//    checkError("alSourcei (loadWave)");

//    qDebug() << "Loading complete!";
//    if (waveData->audioData) delete waveData->audioData;
//    if (waveData) delete waveData;
//    return true;
//}

//void SoundSource::play()
//{
//    alSourcePlay(mSource);
//}
//void SoundSource::pause()
//{
//    alSourcePause(mSource);
//}
//void SoundSource::stop()
//{
//    alSourceStop(mSource);
//}

//void SoundSource::setPosition(gsl::Vector3D newPos)
//{
//    mPosition = newPos;
//    ALfloat temp[3] = {mPosition.x, mPosition.y, mPosition.z};
//    alSourcefv(mSource, AL_POSITION, temp);
//}
//void SoundSource::setVelocity(gsl::Vector3D newVel)
//{
//    mVelocity = newVel;
//    ALfloat temp[3] = {mVelocity.x, mVelocity.y, mVelocity.z};
//    alSourcefv(mSource, AL_VELOCITY, temp);
//}

//bool SoundSource::checkError(std::string name)
//{
//    switch (alGetError())
//    {
//    case AL_NO_ERROR:
//        break;
//    case AL_INVALID_NAME:
//        qDebug() << "OpenAL Error: " << name.c_str() << ": Invalid name!";
//        return false;
//    case AL_INVALID_ENUM:
//        qDebug() << "OpenAL Error: " << name.c_str() << ": Invalid enum!";
//        return false;
//    case AL_INVALID_VALUE:
//        qDebug() << "OpenAL Error: " << name.c_str() << ": Invalid value!";
//        return false;
//    case AL_INVALID_OPERATION:
//        qDebug() << "OpenAL Error: " << name.c_str() << ": Invalid operation!";
//        return false;
//    case AL_OUT_OF_MEMORY:
//        qDebug() << "OpenAL Error: " << name.c_str() << ": Out of memory!";
//        return false;
//    default: break;
//    }
//    return true;
//}
