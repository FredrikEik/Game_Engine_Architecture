#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

//Forward declaration
class RenderSystem;

/// Class handling OpenAL setup and plays sound sources.
/**
    Singleton class that plays soundsources, and updates listener.
**/
class SoundSystem
{
public:
    static SoundSystem* getInstance()          ///< Get pointer to singleton instance.
    {
        if (!mInstance)
        {
            mInstance = new SoundSystem();
            mInstance->init();
        }
        return mInstance;
    }

    /// Initialises OpenAL.
    bool init();
    /// Cleans up and shuts down OpenAL.
    void cleanUp();

    void update(RenderSystem *renderWindowIn);

    /// Creates a new SoundSource with the given parameters.
    /**
        \param The name of the sound. (Not in use.)
        \param The source position as Vector3.
        \param File path relative to execution directory.
        \param Boolean to see if sound should loop or not.
    **/
//    SoundSource* createSource(std::string name, gsl::Vector3D pos, std::string fileName = "", bool loop = false, float gain = 1.0);
    void updateListener(gsl::Vector3D &pos, gsl::Vector3D &dir, gsl::Vector3D &up);

    void togglePlaySounds(bool shouldPlay);

private:
    SoundSystem();                         ///< Private constructor.
    SoundSystem(SoundSystem const&);      ///< Private copy constructor.
    void operator=(SoundSystem const&);    ///< Private class assignment operator.
    static SoundSystem* mInstance;         ///< Singleton instance pointer.
    /// Debug function.
    /**
        Checks for and outputs OpenAL errors.
    **/
    bool checkForALerrors();

    ALCdevice* mDevice{nullptr};                 ///< Pointer to the ALC Device.
    ALCcontext* mContext{nullptr};               ///< Pointer to the ALC Context.

    bool shouldPlaySounds{false};

    RenderSystem* mRenderSystem{nullptr};     //Should not be necessary
    class Logger* mLogger{nullptr};
};

#endif
