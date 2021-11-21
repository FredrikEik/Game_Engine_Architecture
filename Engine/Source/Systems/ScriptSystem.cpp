#include "ScriptSystem.h"
#include <string>
#include <iterator>
#include <vector>
#include <iostream>
#include "../ECSManager.h"
#include "../Engine/ScriptEngine.h"


void ScriptSystem::Init()
{
	//ScriptEngine::GetInstance();
}

void ScriptSystem::Invoke(const std::string& functionToCall, class ECSManager* manager)
{	
	ComponentManager<ScriptComponent>* scriptManager = manager->getComponentManager<ScriptComponent>();
	auto& scriptArray = scriptManager->getComponentArray();

	for (auto ScriptComp : scriptArray)
	{
		mono_runtime_invoke(ScriptComp.m_Methods[std::hash<std::string>{}(functionToCall)], ScriptComp.m_Object, nullptr, nullptr);
	}
}

void ScriptSystem::BindInternalFunction(const std::string& name, const void* functionPtr)
{
	mono_add_internal_call(name.c_str(), functionPtr);
}

std::string ScriptSystem::GetName(MonoMethod* method)
{
	
	return std::string(std::string(mono_class_get_name(mono_method_get_class(method)))
		+ "::"
		+ std::string(mono_method_get_name(method)));
		
}

void ScriptSystem::InitScriptObject(ScriptComponent* scriptComp)
{
	ScriptEngine* SE = ScriptEngine::GetInstance();
	scriptComp->m_Class = mono_class_from_name(SE->getImage(), "Game", scriptComp->ScriptClassName.c_str());

	void* pIterator = 0;
	MonoMethod* monoMethod;
	while ((monoMethod = mono_class_get_methods(scriptComp->m_Class, &pIterator)) != nullptr)
	{
		size_t hashVal = std::hash<std::string>{}(std::string(mono_method_get_name(monoMethod)));
		scriptComp->m_Methods.insert(std::make_pair<size_t, MonoMethod*>(std::move(hashVal), std::move(monoMethod)));
	}


	scriptComp->m_Object = mono_object_new(SE->getDomain(), scriptComp->m_Class);
	mono_runtime_object_init(scriptComp->m_Object);


	// parameters
	//fields
}