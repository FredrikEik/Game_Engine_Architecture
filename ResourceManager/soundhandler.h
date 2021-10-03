#ifndef SOUNDHANDLER_H
#define SOUNDHANDLER_H

#include <cstdint>    //does not work well on mac
//#include <stdint.h>     //using this on Mac instead of cstdint. not a totally clean solution
#include <string>
#include <cstdio>
#include "altypes.h"

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

    unsigned char* audioData{nullptr};      ///< Buffer to hold audio data.

    //OpenAL buffer assign to this wav data
    //Originally uses ALuint == unsigned int
    ALuint mALBuffer{0};
};

/// Class to handle loading of Microsoft signed 16-bit PCM WAVE audio files.
/// Also set up ALBuffer and can connect ALBuffer to ALSource
class SoundHandler
{
public:
    /// Loads a given wave file, fills WaveRawData structure with wave data.
    /**
        \param File path relative to execution directory.
        \param Pointer to a wave_t structure to contain the wave data.
    **/
     // set volume, currently global volume
    static WaveRawData* loadWave(std::string fileName);
    static int makeALSource(ALuint bufferIn, float vol = 0.1f);
    static void setVolume(ALuint temp, float vol);

private:
    /// Error handling function.
    /**
        Always returns false. This returns false from loadWave function to indicate
        the file was not loaded.
        Internally outputs error string to the command line.
        \param Error message to output.
    **/
    static bool endOnFileReadError(std::string errmsg);
    static bool checkALError(std::string name);

    static bool makeALBuffer(WaveRawData *waveData);

};

#endif
