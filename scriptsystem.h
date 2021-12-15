#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H


#include "constants.h"
#include <string>
#include <qjsonvalue.h>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJSEngine>
#include <string>
#include <vector>
#include "vector3d.h"
#include "altypes.h"



class ScriptSystem
{
public:
    //! \brief creating static pointer for instance
    static ScriptSystem* getInstance()          ///< Get pointer to singleton instance.
    {
        if (!mInstance)
        {
            mInstance = new ScriptSystem();
            mInstance->init();
        }
        return mInstance;
    }
    //! \brief empty construct
    ScriptSystem();
    //! \brief init script
    bool init();
    //! \brief reading data from javascript
    //! \param scriptN  - script name of file used
    void readData(std::string scriptN);
    //! \brief setting data from script
    //! \param entityID - used to link to which entity the script is for
    //! \param scriptN - script name of file used
    void setData(MAX_ENTITIES_TYPE entityID, std::string scriptN);

    //! copy for struct member
    QJSEngine mScriptEngine;
private:
    static ScriptSystem* mInstance;         ///< Singleton instance pointer.
    ALCdevice* mDevice;                 ///< Pointer to the ALC Device.
    ALCcontext* mContext;               ///< Pointer to the ALC Context.
};

#endif // SCRIPTSYSTEM_H
