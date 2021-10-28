#include "soundsystem.h"
#include "soundhandler.h"

#include "rendersystem.h"   //should not be needed when update is fixed
#include "gameobject.h"

SoundSystem* SoundSystem::mInstance = nullptr;    //static pointer to instance

SoundSystem::SoundSystem() : mDevice{nullptr}, mContext{nullptr}
{
    mLogger = Logger::getInstance();
}

bool SoundSystem::init()
{
    mLogger->logText("Intializing OpenAL!", LColor::HIGHLIGHT);
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
        mLogger->logText("Device not made!");
        return false;
    }
    else
        mLogger->logText("  OpenAL intialization complete!");

    const ALCchar *defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    std::string temp = defaultDeviceName;
    mLogger->logText("  Device name: " + temp);

    //this prints out the same on my PC at least...
//    const ALCchar *actualDeviceName = alcGetString(mDevice, ALC_DEVICE_SPECIFIER);
//    temp = actualDeviceName;
//    mLogger->logText("  " + temp);

    /*Start listing of found sound devices:
    //Not jet implemented
    //ALDeviceList *pDeviceList = NULL;
    //ALCcontext *pContext = NULL;
    //ALCdevice *pDevice = NULL;
    //ALint i;	//will hold the number of the preferred device
    //ALboolean bReturn = AL_FALSE;*/

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
        mLogger->logText("Invalid name!", LColor::DAMNERROR);
        return false;
    case AL_INVALID_ENUM:
        mLogger->logText("Invalid enum!", LColor::DAMNERROR);
        return false;
    case AL_INVALID_VALUE:
        mLogger->logText("Invalid value!", LColor::DAMNERROR);
        return false;
    case AL_INVALID_OPERATION:
        mLogger->logText("Invalid operation!", LColor::DAMNERROR);
        return false;
    case AL_OUT_OF_MEMORY:
        mLogger->logText("Out of memory!", LColor::DAMNERROR);
        return false;
    default:
        mLogger->logText("Some unknown OpenAL error in SoundSystem!", LColor::DAMNERROR);
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

//SoundSource* SoundSystem::createSource(std::string name, gsl::Vector3D pos, std::string fileName, bool loop, float gain)
//{
//    SoundSource* tempPtr = new SoundSource(name, loop, gain);
//    tempPtr->setPosition(pos);
//    if (fileName != "")
//        tempPtr->loadWave(fileName);
//    return tempPtr;
//}

void SoundSystem::updateListener(gsl::Vector3D &pos, gsl::Vector3D &dir, gsl::Vector3D &up)
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
