from conans import ConanFile, CMake

class ConanPackage(ConanFile):
	name = "GameInJin"
	version = "0.1"
	settings = "os", "compiler", "build_type", "arch"
	requires = [
        ('glad/0.1.34'),
        ('glfw/3.3.4'),
		('glm/0.9.9.8'),
        #('imgui/1.83'),
		('openal/1.20.1'),
		('libsndfile/1.0.31')
    ]
	generators = {
                "cmake"
                }
	options = {
		"shared": [True, False]
	}
	default_options = {
		"shared": False,
		"glad:egl_version": None, "glad:gl_profile": "compatibility",
		"glad:gl_version": 4.6, "glad:gles1_version": None, "glad:extensions":'', "glad:gles2_version": None, "glad:glsc2_version": None, "glad:glx_version": None,
		"glad:no_loader": False, "glad:shared": False, "glad:spec": "gl", "glad:wgl_version": None, "glfw:vulkan_static": False, "glfw:shared": False
	}
	exports_sources = ["CMakeList.txt"]

	def configure_cmake(self):
		cmake = CMake(self)
		cmake.configure()
		return cmake

	def imports(self):
                self.copy("*.dll", dst="bin", src="bin")
                self.copy("*.dylib*", dst="bin", src="lib")
                self.copy("imgui_impl_glfw.cpp", dst="../Source/imgui/bindings", src="./res/bindings")
                self.copy("imgui_impl_opengl3.cpp", dst="../Source/imgui/bindings", src="./res/bindings")
                self.copy("imgui_impl_glfw.h", dst="../Source/imgui/bindings", src="./res/bindings")
                self.copy("imgui_impl_opengl3.h", dst="../Source/imgui/bindings", src="./res/bindings")	


	def build(self):
		cmake = self.configure_cmake()
		cmake.build()
		#cmake.test()


	def package(self):
                
		cmake = self.configure_cmake()
		cmake.install()

	def package_info(self):
		self.cpp_info.libs = ["GameInJin"]

    	
    	
