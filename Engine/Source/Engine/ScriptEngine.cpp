#include "ScriptEngine.h"
#include <iostream>


ScriptEngine* ScriptEngine::m_ScriptEngineInst = nullptr;
ScriptEngine::ScriptEngine()
{

	bool bInitedScriptEngine = InitScriptEngine();
	if (!bInitedScriptEngine)
		std::cout << "failed initializing script engine" << '\n';

}

ScriptEngine::~ScriptEngine()
{
	mono_jit_cleanup(m_Domain);
	mono_assembly_name_free(m_AsmName);
}

bool ScriptEngine::InitScriptEngine()
{
	mono_set_dirs("Mono/lib", "Mono/etc");
	m_Domain = mono_jit_init_version("CSharp_Domain", "v4.0.30319");

	//Load the binary file as an Assembly
	m_CSharpAssembly = mono_domain_assembly_open(m_Domain, "Build/bin/Game.dll");
	if (!m_CSharpAssembly)
		return false;

	// calls main entry point
	if (bUseMainAsEntry)
	{
		int argc = 1;
		char* argv[1] = { (char*)"CSharp" };
		mono_jit_exec(m_Domain, m_CSharpAssembly, argc, argv);
	}

	m_MonoImage = mono_assembly_get_image(m_CSharpAssembly);

	MonoAssemblyName* aname = mono_assembly_name_new("mscorlib");

	return true;
}
