#pragma once
#include "glm/glm.hpp"
#include "../CoreMinimal.h"
#include "mono/jit/jit.h"
#include <mono/metadata/assembly.h>

class ScriptEngine
{
public:
	/// <summary>
	/// Gets the instance.
	/// </summary>
	/// <returns></returns>
	static ScriptEngine* GetInstance()
	{
		if (!m_ScriptEngineInst)
			m_ScriptEngineInst = new ScriptEngine();
		return m_ScriptEngineInst;
	}

	/// <summary>
	/// Finalizes an instance of the <see cref="ScriptEngine"/> class.
	/// </summary>
	~ScriptEngine();

	/// <summary>
	/// Gets the domain.
	/// </summary>
	/// <returns></returns>
	MonoDomain* getDomain() const { return m_Domain; }
	/// <summary>
	/// Gets the asm.
	/// </summary>
	/// <returns></returns>
	MonoAssembly* getAsm() const { return m_CSharpAssembly; }
	/// <summary>
	/// Gets the image.
	/// </summary>
	/// <returns></returns>
	MonoImage* getImage() const { return m_MonoImage; }
	/// <summary>
	/// Gets the name of the asm.
	/// </summary>
	/// <returns></returns>
	MonoAssemblyName* getAsmName() const { return m_AsmName; }
private:
	static ScriptEngine* m_ScriptEngineInst;
	/// <summary>
	/// Initializes the script engine.
	/// </summary>
	/// <returns></returns>
	bool InitScriptEngine();

	MonoDomain* m_Domain;
	MonoAssembly* m_CSharpAssembly;
	MonoImage* m_MonoImage;
	MonoAssemblyName* m_AsmName;
	bool bUseMainAsEntry{ false };


	ScriptEngine();
};