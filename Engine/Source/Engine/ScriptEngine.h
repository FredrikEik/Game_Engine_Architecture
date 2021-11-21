#pragma once
#include "glm/glm.hpp"
#include "../CoreMinimal.h"
#include "mono/jit/jit.h"
#include <mono/metadata/assembly.h>

class ScriptEngine
{
public:
	// TODO: Remove or fix up the Get function

	static ScriptEngine* GetInstance()
	{
		if (!m_ScriptEngineInst)
			m_ScriptEngineInst = new ScriptEngine();
		return m_ScriptEngineInst;
	}



	~ScriptEngine();


	MonoDomain* getDomain() const { return m_Domain; }
	MonoAssembly* getAsm() const { return m_CSharpAssembly; }
	MonoImage* getImage() const { return m_MonoImage; }
	MonoAssemblyName* getAsmName() const { return m_AsmName; }
private:
	static ScriptEngine* m_ScriptEngineInst;
	bool InitScriptEngine();

	// mono
	MonoDomain* m_Domain;
	MonoAssembly* m_CSharpAssembly;
	MonoImage* m_MonoImage;
	MonoAssemblyName* m_AsmName;
	bool bUseMainAsEntry{ false };


	ScriptEngine();
};