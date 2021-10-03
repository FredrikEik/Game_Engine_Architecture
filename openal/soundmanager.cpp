#include "soundmanager.h"
#include "wavfilehandler.h"
#include <iostream>
#include <vector3d.h>

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

void SoundManager::updateListener(Vector3 pos, Vector3 vel, Vector3 dir, Vector3 up)
{
    ALfloat posVec[3];
    ALfloat velVec[3];
    ALfloat headVec[6];
    posVec[0] = pos.x;
    posVec[1] = pos.y;
    posVec[2] = pos.z;
    velVec[0] = vel.x;
    velVec[1] = vel.y;
    velVec[2] = vel.z;
    headVec[0] = dir.x;
    headVec[1] = dir.y;
    headVec[2] = dir.z;
    headVec[3] = up.x;
    headVec[4] = up.y;
    headVec[5] = up.z;
    alListenerfv(AL_POSITION, posVec);
    alListenerfv(AL_VELOCITY, velVec);
    alListenerfv(AL_ORIENTATION, headVec);
}

void SoundManager::updateListener(gsl::Vector3D pos, gsl::Vector3D  vel, gsl::Vector3D  dir, gsl::Vector3D  up)
{
    ALfloat posVec[3];
    ALfloat velVec[3];
    ALfloat headVec[6];
    posVec[0] = pos.getX();
    posVec[1] = pos.getY();
    posVec[2] = pos.getZ();
    velVec[0] = vel.getX();
    velVec[1] = vel.getY();
    velVec[2] = vel.getZ();
    headVec[0] = dir.getX();
    headVec[1] = dir.getY();
    headVec[2] = dir.getZ();
    headVec[3] = up.getX();
    headVec[4] = up.getY();
    headVec[5] = up.getZ();
    alListenerfv(AL_POSITION, posVec);
    alListenerfv(AL_VELOCITY, velVec);
    alListenerfv(AL_ORIENTATION, headVec);
}


