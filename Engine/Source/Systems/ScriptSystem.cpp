#include "ScriptSystem.h"
#include <string>
#include <iterator>
#include <vector>
#include <iostream>
#include "../ECSManager.h"
#include "../Engine/ScriptEngine.h"
#include "TransformSystem.h"
#include "SelectionSystem.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include "ParticleSystem.h"
#include "HudSystem.h"
#include "../Input/Input.h"
#include "../Engine/Engine.h"
#include "../Sound/SoundSource.h"

void PrintMethod_Internal(MonoString* string)
{
	char* cppString = mono_string_to_utf8(string);

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
	BindInternalFunction("ScriptInJin.Entity::getEntityID_internal", &getEntityID_Internal);
	BindInternalFunction("ScriptInJin.Transform::Move_Internal", &TransformSystem::move_internal);
	BindInternalFunction("ScriptInJin.Transform::getTransform_internal", &TransformSystem::getTransform_internal);
	BindInternalFunction("ScriptInJin.Transform::setPosition_internal", &TransformSystem::setPosition_internal);
	BindInternalFunction("ScriptInJin.Entity::IsEntitySelected_internal", &SelectionSystem::IsEntitySelected_internal);
	BindInternalFunction("ScriptInJin.Entity::isOverlappingEntity_Internal", &CollisionSystem::isOverlappingEntity_Internal);
	BindInternalFunction("ScriptInJin.Entity::setVelocity_Internal", &PhysicsSystem::setVelocity_Internal);
	BindInternalFunction("ScriptInJin.Entity::setParticleActive_Internal", &ParticleSystem::setParticleActive_Internal);
	BindInternalFunction("ScriptInJin.Entity::getDeltaTime_Internal", &Engine::getDeltaTime_Internal);
	BindInternalFunction("ScriptInJin.Entity::getObject_Internal", &ScriptSystem::getObject_Internal);
	BindInternalFunction("ScriptInJin.Entity::createDefaultEntity_Internal", &Engine::createDefaultEntity_Internal);
	BindInternalFunction("ScriptInJin.Entity::showHud_Internal", &HudSystem::showHud_Internal);
	BindInternalFunction("ScriptInJin.Entity::removeHud_Internal", &HudSystem::removeHud_Internal);
	BindInternalFunction("ScriptInJin.Entity::addMaterialTexture_Internal", &TextureSystem::addMaterialTexture_Internal);
	BindInternalFunction("ScriptInJin.Entity::playSound_Internal", &SoundSource::Play_Internal);


	BindInternalFunction("ScriptInJin.Input::getCursorWorldPosition_Internal", &SelectionSystem::getCursorWorldPosition_Internal);
	BindInternalFunction("ScriptInJin.Input::getMouseKeyState_Internal", &Input::getMouseKeyState_Internal);


}

void ScriptSystem::Invoke(const std::string& functionToCall, class ECSManager* manager)
{	
	ComponentManager<ScriptComponent>* scriptManager = manager->getComponentManager<ScriptComponent>();
	auto& scriptArray = scriptManager->getComponentArray();

	for (auto ScriptComp : scriptArray)
	{
		if (!&ScriptComp)
			continue;

		if (!ScriptComp.bInitialized || functionToCall == "Update")
		{
			ScriptComp.bInitialized = true;
			auto func = ScriptComp.m_Methods[std::hash<std::string>{}(functionToCall)];
			auto object = ScriptComp.m_Object;
			mono_runtime_invoke(func, object, nullptr, nullptr);
		}
	}
}

void ScriptSystem::Invoke(uint32 entity, const std::string& functionToCall, ECSManager* manager)
{
	ScriptComponent* component = manager->getComponentManager<ScriptComponent>()->getComponentChecked(entity);
	assert(component);
	if (!component->bInitialized)
	{
		component->bInitialized = true;
		mono_runtime_invoke(component->m_Methods[std::hash<std::string>{}(functionToCall)], component->m_Object, nullptr, nullptr);
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
	scriptComp->entityID_handle = mono_class_get_field_from_name(scriptComp->m_Class, "native_handle");
	mono_field_set_value(scriptComp->m_Object, scriptComp->entityID_handle, &scriptComp->entityID);
}

void ScriptSystem::uninitializeAllComponents(ECSManager* manager)
{
	auto scriptManager = manager->getComponentManager<ScriptComponent>();
	if (!scriptManager)
		return;
	auto components = scriptManager->getComponentArray();
	for (auto& it : components)
	{
		it.bInitialized = false;
	}
}

void ScriptSystem::setScriptClassName(uint32 entityID, const std::string& className, ECSManager* ECS)
{
	ScriptComponent* component = ECS->getComponentManager<ScriptComponent>()->getComponentChecked(entityID);
	assert(component);
	component->ScriptClassName = className;
}

MonoObject* ScriptSystem::getObject_Internal(uint32 entity)
{
	ScriptComponent* component = Engine::Get().getECSManager()->getComponentManager<ScriptComponent>()->getComponentChecked(entity);
	
	if(!component)
		return nullptr;
	return component->m_Object;
}
