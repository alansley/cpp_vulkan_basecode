// cpp_vulkan_basecode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

// Note: Windows.h is required to call `LoadLibrary` to pull in `vulkan-1.dll`
// Also: When we build for x64 `_WIN32` is still defined - so take this as "We're on Windows" rather than "We're on Windows building in x86 / 32-bit mode"
#if defined _WIN32
	#include <Windows.h>
#endif

// Vulkan library type
#ifdef _WIN32
	#define LIBRARY_TYPE HMODULE
#elif defined __linux
	#define LIBRARY_TYPE void*
#endif

// Vulkan function loader command
#ifdef _WIN32
	#define LoadFunction GetProcAddress
#elif defined __linux
	#define LoadFunction dlsym
#endif


#ifdef _WIN32
	#define NAME "We got loaded in _WIN32!"
#else
	#define NAME "We DID NOT got loaded in _WIN32!"
#endif


//#include "include/vulkan/vk_platform.h"
#define VK_NO_PROTOTYPES   // In this example we'll load the functions that we need only, rather than pull in all the prototypes from vulkan.h
#include "vulkan/vulkan.h" // Note: "vulkan.h" includes "vk_platform.h" amongst other things

// Method to connect to the Vulkan loader library
bool ConnectWithVulkanLoaderLibrary(LIBRARY_TYPE& vulkan_library)
{
#if defined _WIN32
	// IMPORTANT: Do not try to connect to the Vulkan DLL with: `LPCWSTR("vulkan-1.dll")` - this won't work! Instead have the type cast as `TEXT` as per below!
	// Source: https://github.com/KhronosGroup/Vulkan-Hpp/issues/452	
	vulkan_library = LoadLibrary(TEXT("vulkan-1.dll"));
#elif defined __linux
	vulkan_library = dlopen("libvulkan.so.1", RTLD_NOW);
#endif

	if (vulkan_library == nullptr) {
		std::cout << "[FAIL] Could not connect with a Vulkan Runtime library." << std::endl;
		return false;
	}
	// Implied else
	std::cout << "[OK] Successfully connected to the Vulkan Runtime Library!" << std::endl;
	return true;
}

#include "VulkanFunctions.h"




namespace VulkanFunctionLoaders
{
	bool LoadFunctionExportedFromVulkanLoaderLibrary(LIBRARY_TYPE const& vulkan_library)
	{

		// Vulkan function loader command
#ifdef _WIN32
#define LOAD_FUNCTION GetProcAddress
#elif defined __linux
#define LOAD_FUNCTION dlsym
#endif

#define EXPORTED_VULKAN_FUNCTION( name )                                \
VulkanFunctionLoaders::name = (PFN_##name)LOAD_FUNCTION( vulkan_library, #name ); \
if( VulkanFunctionLoaders::name == nullptr ) {                                    \
  std::cout << "Could not load exported Vulkan function named: "        \
    #name << std::endl;                                                 \
  return false;                                                         \
}

#include "ListOfVulkanFunctions.inl"

		return true;
	}

} // End of namespace VulkanFunctionLoaders



template <typename functionType>
functionType performLoadFunction(LIBRARY_TYPE const& vulkanLibrary, std::string functionName)
{
#define FUCK_EXPORTED_VULKAN_FUNCTION(name) PFN_##name functionName;


	//#define FunctionTypeZ PFN_##functionName functionName;

	//auto vkGetInstanceProcAddr = (FUCK_EXPORTED_VULKAN_FUNCTION(functionName))LoadFunction(vulkanLibrary, functionName.c_str());

	auto vkGetInstanceProcAddr = const_cast<functionType>(LoadFunction(vulkanLibrary, functionName.c_str()));
	
	//auto vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(LoadFunction(vulkanLibrary, functionName.c_str()));

	if (vkGetInstanceProcAddr == nullptr)
	{
		std::cout << "[FAIL] Could not find address of function: " << functionName << std::endl;
	}
	else
	{
		std::cout << "[OK] Successfully found address of function: " << functionName << " (" << vkGetInstanceProcAddr << ")" << std::endl;
	}
	return vkGetInstanceProcAddr; // This will be nullptr if we could not load the function
}

int main()
{
	//auto vulkan_library = LoadLibrary("vulkan-1.dll");

	//HMODULE vulkan_library = LoadLibrary(LPCWSTR("C:\\Users\\r3dux\\Desktop\\cpp_vulkan_basecode\\x64\\Debug\\vulkan-1.dll"));

	// IMPORTANT: Do not try to connect to the Vulkan DLL with: `LPCWSTR("vulkan-1.dll")` - this won't work! Instead have the type cast as `TEXT` as per below!
	/*
	LIBRARY_TYPE vulkan_library = LoadLibrary(TEXT("vulkan-1.dll"));
	if (vulkan_library == nullptr)
	{
		std::cout << "[FAIL] Could not connect with a Vulkan Runtime library." << std::endl;
		return -1;
	}
	else
	{
		std::cout << "[OK] Successfully connected to the Vulkan Runtime Library!" << std::endl;
	}
	*/


	LIBRARY_TYPE vulkanLibrary;
	bool connectedToVulkanLoader = ConnectWithVulkanLoaderLibrary(vulkanLibrary);


	/*** STRUGGLING WITH AUTOMATICALLY LOADING ALL FUNCTIONS DECLARED IN `ListOfVulkanFunctions.inl` - BUT WE CAN USE THE BELOW ONE-AT-A-TIME! ***/
	// Load all functions we've specified in `ListOfVulkanFunctions.ink` automatically
	bool loadFunctionSuccess = VulkanFunctionLoaders::LoadFunctionExportedFromVulkanLoaderLibrary(vulkanLibrary);
	if (loadFunctionSuccess)
	{
		std::cout << "NAILED IT!" << std::endl;
	}
		


	// To load a function manually, use:
	std::string functionName = "vkGetInstanceProcAddr";
	//auto vkGetInstanceProcAddre = performLoadFunction<>()

	/*
	auto vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)LoadFunction(vulkanLibrary, functionName.c_str());
	if (vkGetInstanceProcAddr == nullptr)
	{
		std::cout << "[FAIL] Could not find address of function: " << functionName << std::endl;
	}
	std::cout << "[OK] Successfully found address of function: " << functionName << " (" << vkGetInstanceProcAddr << ")" << std::endl;
	*/

	std::cout << "Name define is: " << NAME << std::endl;
	
}