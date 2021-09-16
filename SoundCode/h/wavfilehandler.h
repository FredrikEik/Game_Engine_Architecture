#ifndef WAVFILEHANDLER_H
#define WAVFILEHANDLER_H

#include <cstdint>
#include <string>
#include <cstdio>

struct wave_t
{
    uint32_t size;             //WAVE file size
    uint32_t chunkSize;        //fmt chunk size
    uint32_t sampleRate;        //Sample rate of audio
    uint32_t avgBytesPerSec;   //Average byte size
    uint32_t dataSize;         //Size of data chunk
    short formatType;          //Audio file format
    short channels;            //Nr of audio channels
    short bytesPerSample;      //Nr of bytes per sample
    short bitsPerSample;       //Nr of bits per sample

    unsigned char* buffer;     //Buffer to hold audio data
    wave_t() : buffer(NULL) {} //Structure Constructor. Initialises data buffer to NULL
};

//Class that handels loading of MS signed 16-bit PCM WAVE audio files.
class WavFileHandler
{
public:
    //Loads the wave file and fills wave_t with data
    static bool loadWave(std::string filePath, wave_t* wavePtr);

private:
    //Error function to output message
    static bool endOnError(std::string errmsg);
};

#endif // WAVFILEHANDLER_H
