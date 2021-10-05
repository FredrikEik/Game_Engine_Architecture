//#include "wavfilehandler.h"
//#include <iostream>
#include "soundmanager.h"
#include "gameobject.h"
#include "components.h"
#include "soundhandler.h"
#include <iostream>
#include <QDebug>

SoundManager* SoundManager::mInstance = NULL;

SoundManager::SoundManager() :
    mDevice(NULL),
    mContext(NULL)
{

}

bool SoundManager::init()
{
    std::cout << "Intializing OpenAL!\n";
    mDevice = alcOpenDevice(NULL);
    if (mDevice)
    {
        mContext = alcCreateContext(mDevice, NULL);
        alcMakeContextCurrent(mContext);
    }

    // Generate buffers
    alGetError();

    if (!mDevice)
    {
        std::cout << "Device not made!\n";
    }
    else
        std::cout << "Intialization complete!\n";

    //Start listing of found sound devices:
    //Not jet implemented
    //ALDeviceList *pDeviceList = NULL;
    //ALCcontext *pContext = NULL;
    //ALCdevice *pDevice = NULL;
    //ALint i;	//will hold the number of the preferred device
    //ALboolean bReturn = AL_FALSE;

    return true;
}

void SoundManager::cleanUp()
{
    mContext = alcGetCurrentContext();
    mDevice = alcGetContextsDevice(mContext);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}

bool SoundManager::checkError()
{
    switch (alGetError())
    {
    case AL_NO_ERROR:
        break;
    case AL_INVALID_NAME:
        std::cout << "Invalid name!\n";
        return false;
    case AL_INVALID_ENUM:
        std::cout << "Invalid enum!\n";
        return false;
    case AL_INVALID_VALUE:
        std::cout << "Invalid value!\n";
        return false;
    case AL_INVALID_OPERATION:
        std::cout << "Invalid operation!\n";
        return false;
    case AL_OUT_OF_MEMORY:
        std::cout << "Out of memory!\n";
        return false;
    default: break;
    }
    return true;
}

SoundSource* SoundManager::createSource(std::string name, Vector3 pos, std::string filePath, bool loop, float gain)
{
    SoundSource* tempPtr = new SoundSource(name, loop, gain);
    tempPtr->setPosition(pos);
    if (filePath != "")
        tempPtr->loadWave(filePath);
    return tempPtr;
}

void SoundManager::updateListener(gsl::Vector3D &pos, gsl::Vector3D &dir, gsl::Vector3D &up)
{
    ALfloat posVec[3]{pos.x, pos.y, pos.z};
    alListenerfv(AL_POSITION, posVec);

    ALfloat headVec[6]{dir.x, dir.y, dir.z, up.x, up.y, up.z};
    alListenerfv(AL_ORIENTATION, headVec);

    //    gsl::Vector3D vel
    //    ALfloat velVec[3];,
    //    velVec[0] = vel.x;
    //    velVec[1] = vel.y;
    //    velVec[2] = vel.z;
    //    alListenerfv(AL_VELOCITY, velVec);

}
