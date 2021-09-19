#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H

#include <cstdint>    //does not work well on mac
//#include <stdint.h>     //using this on Mac instead of cstdint. not a totally clean solution
#include <string>
#include <cstdio>

/// Structure to contain WAVE file data.
struct WaveRawData
{
    //Data from the header of the wav-file
    uint32_t size;              ///< Size of the WAVE file.
    uint32_t chunkSize;         ///< Size of the fmt chunk.
    uint32_t sampleRate;        ///< Sample rate of the audio.
    uint32_t avgBytesPerSec;    ///< Average byte rate.
    uint32_t dataSize;          ///< Size of the data chunk.
    short formatType;           ///< Audio format.
    short channels;             ///< Number of audio channels.
    short bytesPerSample;       ///< Number of bytes per sample.
    short bitsPerSample;        ///< Number of bits per sample.

    unsigned char* buffer{nullptr};      ///< Buffer to hold audio data.
//    WaveRawData() : buffer(NULL) {}  ///< Structure Constructor. Initialises data buffer to NULL.
};

/// Class to handle loading of Microsoft signed 16-bit PCM WAVE audio files.
class SoundHandler
{
public:
    /// Loads a given wave file, fills WaveRawData structure with wave data.
    /**
        \param File path relative to execution directory.
        \param Pointer to a wave_t structure to contain the wave data.
    **/
    static bool loadWave(std::string fileName, WaveRawData* wavePtr);

private:
    /// Error handling function.
    /**
        Always returns false. This returns false from loadWave function to indicate
        the file was not loaded.
        Internally outputs error string to the command line.
        \param Error message to output.
    **/
    static bool endOnError(std::string errmsg);
};

#endif
