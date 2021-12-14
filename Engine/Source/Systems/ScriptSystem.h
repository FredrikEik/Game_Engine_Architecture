#pragma once
#include "BaseSystem.h"
#include <iostream>
#include <mono/metadata/environment.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/debug-helpers.h>
#include "../Components/Components.h"



class ScriptSystem final : public BaseSystem
{

public:
	static void Init();

	static void Invoke(const std::string& functionToCall, class ECSManager* manager);
	static void Invoke(uint32 entity, const std::string& functionToCall, class ECSManager* manager);

	//Namespace.Class::Method + a Function pointer with the actual definition
	static void BindInternalFunction(const std::string& name, const void* functionPtr);
	static std::string ScriptSystem::GetName(MonoMethod* method);
	static void InitScriptObject(ScriptComponent* scriptComp, std::string className = "Unit");

	static void setScriptClassName(uint32 entityID, const std::string& className, ECSManager* ECS);

	static MonoObject* getObject_Internal(uint32 entity);
private:

};

