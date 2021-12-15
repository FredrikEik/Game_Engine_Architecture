#pragma once
#include "BaseSystem.h"
#include <iostream>
#include <mono/metadata/environment.h>
#include <mono/utils/mono-publib.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/debug-helpers.h>
#include "../Components/Components.h"



/// <summary>
/// Script system handles bindings between c# and c++
/// And invocations
/// </summary>
/// <seealso cref="BaseSystem" />
class ScriptSystem final : public BaseSystem
{
public:
	/// <summary>
	/// Initializes this instance.
	/// </summary>
	static void Init();

	/// <summary>
	/// Invokes the specified function to call.
	/// </summary>
	/// <param name="functionToCall">The function to call.</param>
	/// <param name="manager">The manager.</param>
	static void Invoke(const std::string& functionToCall, class ECSManager* manager);
	/// <summary>
	/// Invokes the specified entity.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <param name="functionToCall">The function to call.</param>
	/// <param name="manager">The manager.</param>
	static void Invoke(uint32 entity, const std::string& functionToCall, class ECSManager* manager);

	//Namespace.Class::Method + a Function pointer with the actual definition
/// <summary>
/// Binds the internal function.
/// </summary>
/// <param name="name">The name.</param>
/// <param name="functionPtr">The function PTR.</param>
	static void BindInternalFunction(const std::string& name, const void* functionPtr);
	/// <summary>
	/// Gets the name.
	/// </summary>
	/// <param name="method">The method.</param>
	/// <returns></returns>
	static std::string ScriptSystem::GetName(MonoMethod* method);
	/// <summary>
	/// Initializes the script object.
	/// </summary>
	/// <param name="scriptComp">The script comp.</param>
	/// <param name="className">Name of the class.</param>
	static void InitScriptObject(ScriptComponent* scriptComp, std::string className = "Unit");
	/// <summary>
	/// Uninitializes all components.
	/// </summary>
	/// <param name="manager">The manager.</param>
	static void uninitializeAllComponents(class ECSManager* manager);

	/// <summary>
	/// Sets the name of the script class.
	/// </summary>
	/// <param name="entityID">The entity identifier.</param>
	/// <param name="className">Name of the class.</param>
	/// <param name="ECS">The ecs.</param>
	static void setScriptClassName(uint32 entityID, const std::string& className, ECSManager* ECS);

	/// <summary>
	/// Gets the object internal.
	/// </summary>
	/// <param name="entity">The entity.</param>
	/// <returns></returns>
	static MonoObject* getObject_Internal(uint32 entity);

};

