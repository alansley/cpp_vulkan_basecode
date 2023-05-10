// cpp_vulkan_basecode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

// Note: Windows.h is required to call `LoadLibrary` to pull in `vulkan-1.dll`
#if defined _WIN32
	#include <Windows.h>
#elif defined _WIN64
	#include <Windows.h>
#endif

// Vulkan library type
#ifdef _WIN32
	#define LIBRARY_TYPE HMODULE
#elif defined __linux
	#define LIBRARY_TYPE void*
#endif

// Vulkan

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

/***
----- THIS PART IS NOT WORKING PROPERLY TO LOAD ALL THE FUNCTIONS LISTED IN `ListOfVulkanFunctions.inl` -----

bool LoadFunctionExportedFromVulkanLoaderLibrary(LIBRARY_TYPE const& vulkan_library)
{
#if defined _WIN32
	#define LoadFunction GetProcAddress
#elif defined __linux
	#define LoadFunction dlsym
#endif

#define EXPORTED_VULKAN_FUNCTION(name)                                \
    name = (PFN_##name)LoadFunction(vulkan_library, #name);           \
    if (name == nullptr) {                                            \
      std::cout << "Could not load exported Vulkan function named: "  \
        #name << std::endl;                                           \
      return false;                                                   \
    }

#include "ListOfVulkanFunctions.inl"

	return true;
}
*/

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
	//bool loadFunctionSuccess = LoadFunctionExportedFromVulkanLoaderLibrary(vulkanLibrary);
	
	
	// To load a function manually, use:
	std::string functionName = "vkGetInstanceProcAddr";
	auto vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vulkanLibrary, functionName.c_str());
	if (vkGetInstanceProcAddr == nullptr)
	{
		std::cout << "[FAIL] Could not find address of function: " << functionName << std::endl;
	}
	std::cout << "[OK] Successfully found address of function: " << functionName << " (" << vkGetInstanceProcAddr << ")" << std::endl;


	
}