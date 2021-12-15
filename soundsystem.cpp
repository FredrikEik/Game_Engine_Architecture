#include "soundsystem.h"
#include "soundhandler.h"
#include <iostream>
#include <QDebug>

#include "rendersystem.h"   //should not be needed when update is fixed
#include "gameobject.h"
#include "components.h"

/********************************************//**
* ... I have not worked on the sound system. Was a part of the cooperation for the
* mandatory tasks in the subject i did with Fredrik Eik Søgnen.
***********************************************/

SoundSystem* SoundSystem::mInstance = nullptr;    //static pointer to instance

SoundSystem::SoundSystem() : mDevice{nullptr}, mContext{nullptr} {}

bool SoundSystem::init()
{
    qDebug() << "Intializing OpenAL!";
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
        qDebug() << "Device not made!";
    }
    else
        qDebug() << "OpenAL intialization complete!";


    return true;
}

void SoundSystem::cleanUp()
{
    mContext = alcGetCurrentContext();
    mDevice = alcGetContextsDevice(mContext);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
    alcCloseDevice(mDevice);
}

void SoundSystem::update(RenderSystem *renderWindowIn)
{
    //Super hack - should only be set once!
    mRenderSystem = renderWindowIn;

    if(shouldPlaySounds)
    {
        alGetError();   //clears earlier errors
        for (auto gob : renderWindowIn->mGameObjects)
        {
            if(gob->mSoundComponent)
            {
                if(gob->mSoundComponent->shouldPlay)
                {
                    //set positions
                    gsl::Vector3D pos= gob->mTransform->mMatrix.getPosition();
                    ALfloat temp[3] = {pos.x, pos.y, pos.z};
                    alSourcefv(gob->mSoundComponent->mSource, AL_POSITION, temp);

                    //Start playing if not
                    ALint value;
                    alGetSourcei(gob->mSoundComponent->mSource, AL_SOURCE_STATE, &value);
                    if (value != AL_PLAYING)
                        alSourcePlay(gob->mSoundComponent->mSource);
                }
            }
            checkError();
        }
    }
}

bool SoundSystem::checkError()
{
    switch (alGetError())
    {
    case AL_NO_ERROR:
        break;
    case AL_INVALID_NAME:
        qDebug() << "Invalid name!";
        return false;
    case AL_INVALID_ENUM:
        qDebug() << "Invalid enum!";
        return false;
    case AL_INVALID_VALUE:
        qDebug() << "Invalid value!";
        return false;
    case AL_INVALID_OPERATION:
        qDebug() << "Invalid operation!";
        return false;
    case AL_OUT_OF_MEMORY:
        qDebug() << "Out of memory!";
        return false;
    default:
        qDebug() << "Some unknown OpenAL error in SoundSystem!";
        break;
    }
    return true;
}

void SoundSystem::togglePlaySounds(bool shouldPlay)
{
    shouldPlaySounds = shouldPlay;

    if(mRenderSystem && !shouldPlay)
    {
        for (auto gob : mRenderSystem->mGameObjects)
        {
            if(gob->mSoundComponent)
            {
                if(gob->mSoundComponent->shouldPlay)
                {
                    alSourcePause(gob->mSoundComponent->mSource);
                }
            }
            checkError();
        }
    }
}

void SoundSystem::updateListener(gsl::Vector3D &pos, gsl::Vector3D &dir, gsl::Vector3D &up)
{
    ALfloat posVec[3]{pos.x, pos.y, pos.z};
    alListenerfv(AL_POSITION, posVec);

    ALfloat headVec[6]{dir.x, dir.y, dir.z, up.x, up.y, up.z};
    alListenerfv(AL_ORIENTATION, headVec);


}
