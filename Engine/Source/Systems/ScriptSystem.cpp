#include "ScriptSystem.h"
#include <string>
#include <iterator>
#include <vector>
#include <iostream>
#include "../ECSManager.h"
#include "../Engine/ScriptEngine.h"
#include "TransformSystem.h"

void PrintMethod_Internal(MonoString* string)
{
	char* cppString = mono_string_to_utf8(string);// mono_string_chars(string);

	std::cout << cppString;

	mono_free(cppString);
}

uint64 getEntityID_Internal(MonoObject* this_ptr, MonoString* str)
{
	const char* name = mono_string_to_utf8(str);


	std::string string = std::string(name);
	std::string delim = std::string(".");
	std::string name_space = string.substr(0, string.find(delim));
	std::string classname = string.erase(0, string.find(delim) + delim.length());


	uint64 entityID;
	MonoClass* klass = mono_class_from_name(ScriptEngine::GetInstance()->getImage(), name_space.c_str(), classname.c_str());
	mono_field_get_value(this_ptr, mono_class_get_field_from_name(klass, "native_handle"), &entityID);

	return entityID;
}

void ScriptSystem::Init()
{
	//Namespace.Class::Method + a Function pointer with the actual definition
	BindInternalFunction("ScriptInJin.Debug::PrintMethod_Internal", &PrintMethod_Internal);

	//mono_add_internal_call("ScriptInJin.Entity::get_EntityID", &);
	BindInternalFunction("ScriptInJin.Entity::getEntityID_internal", &getEntityID_Internal);
	BindInternalFunction("ScriptInJin.Transform::Move_Internal", &TransformSystem::move_internal);
	BindInternalFunction("ScriptInJin.Transform::getTransform_internal", &TransformSystem::getTransform_internal);
	
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

//Namespace.Class::Method + a Function pointer with the actual definition
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


void ScriptSystem::InitScriptObject(ScriptComponent* scriptComp, std::string className)
{
	ScriptEngine* SE = ScriptEngine::GetInstance();
	scriptComp->ScriptClassName = className;
	scriptComp->m_Class = mono_class_from_name(SE->getImage(), "Game", className.c_str());

	void* pIterator = 0;
	MonoMethod* monoMethod;
	while ((monoMethod = mono_class_get_methods(scriptComp->m_Class, &pIterator)) != nullptr)
	{
		size_t hashVal = std::hash<std::string>{}(std::string(mono_method_get_name(monoMethod)));
		scriptComp->m_Methods.insert(std::make_pair<size_t, MonoMethod*>(std::move(hashVal), std::move(monoMethod)));
	}


	scriptComp->m_Object = mono_object_new(SE->getDomain(), scriptComp->m_Class);
	mono_runtime_object_init(scriptComp->m_Object);

	// entity id handle
	scriptComp->entityID_handle = mono_class_get_field_from_name(scriptComp->m_Class, "native_handle");
	mono_field_set_value(scriptComp->m_Object, scriptComp->entityID_handle, &scriptComp->entityID);
	//todo 
	// parameters
	//fields
}