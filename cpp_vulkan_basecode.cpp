// cpp_vulkan_basecode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using std::cout, std::endl, std::string;

const string ApplicationName = "cpp_vulkan_basecode";
const string EngineName = "ACL_vulkan_engine";

// Note: Windows.h is required to call `LoadLibrary` to pull in `vulkan-1.dll`
// Also: When we build for x64 `_WIN32` is still defined - so take this as "We're on Windows" rather than "We're on Windows building in x86 / 32-bit mode"
#if defined _WIN32
	#include <Windows.h>
#endif





//#include "include/vulkan/vk_platform.h"
#define VK_NO_PROTOTYPES   // In this example we'll load the functions that we need only, rather than pull in all the prototypes from vulkan.h
#include <vector>

#include "vulkan/vulkan.h" // Note: "vulkan.h" includes "vk_platform.h" amongst other things

// ----- Platform-specific Defines ------

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

	return vulkan_library != nullptr; // Returns false if the vulkan_library is null, true (i.e., success) otherwise
}

#include "VulkanFunctions.h"



// Note: I fixed this via the commend from `bodyaka` at: https://stackoverflow.com/questions/63587107/undeclared-identifier-when-defining-macro-to-load-vulkan-function-pointers
// Be sure to thank him & maybe link to this repo at some point.
namespace VulkanFunctionLoaders
{
	bool LoadFunctionExportedFromVulkanLoaderLibrary(LIBRARY_TYPE const& vulkan_library)
	{

#define EXPORTED_VULKAN_FUNCTION( name )                                         \
VulkanFunctionLoaders::name = (PFN_##name)LoadFunction( vulkan_library, #name ); \
if( VulkanFunctionLoaders::name == nullptr ) {                                   \
  std::cout << "Could not load exported Vulkan function named: "                 \
    #name << std::endl;                                                          \
  return false;                                                                  \
}

#include "ListOfVulkanFunctions.inl"

		return true;
	}

	bool LoadVulkanGlobalFunctions()
	{
#define GLOBAL_LEVEL_VULKAN_FUNCTION( name )                \
name = (PFN_##name)vkGetInstanceProcAddr( nullptr, #name ); \
if( name == nullptr ) {                                     \
std::cout << "Could not load global-level function named: " \
#name << std::endl; \
return false; \
}
#include "ListOfVulkanFunctions.inl"
		return true;
	}

	bool IsExtensionSupported(std::vector<VkExtensionProperties> const& available_extensions, char const* const extension)
	{
		for (auto & available_extension : available_extensions)
		{
			if (strstr(available_extension.extensionName, extension)) {	return true; }
		}
		return false;
	}

} // End of namespace VulkanFunctionLoaders


int main()
{
	constexpr bool VERBOSE = true;

	// Connect to the Vulkan loader library. Note: `LIBRARY_TYPE` is a macro that makes the result a `HMODULE` on Windows and a `void*` on Linux.
	LIBRARY_TYPE vulkanLibrary;
	const bool connectedToVulkanLoader = ConnectWithVulkanLoaderLibrary(vulkanLibrary);
	if (!connectedToVulkanLoader)
	{
		cout << "[FAIL] Could not connect to Vulkan loader library." << endl;
		return -1;
	}
	if (VERBOSE) { cout << "[OK] Connected to Vulkan loader library." << endl; }

	// Load all functions we've specified in `ListOfVulkanFunctions.ink` automatically
	const bool loadFunctionSuccess = VulkanFunctionLoaders::LoadFunctionExportedFromVulkanLoaderLibrary(vulkanLibrary);
	if (!loadFunctionSuccess)
	{
		cout << "[FAIL] Failed to load Vulkan loader function." << endl;
		return -1;		
	}
	if (VERBOSE) { cout << "[OK] Successfully loaded Vulkan loader function." << endl; }

	// Load all the global functions
	const bool loadGlobalFunctionsSuccess = VulkanFunctionLoaders::LoadVulkanGlobalFunctions();
	if (!loadGlobalFunctionsSuccess)
	{
		cout << "[FAIL] Failed to load Vulkan global functions." << endl;
		return -2;
	}
	if (VERBOSE) { cout << "[OK] Successfully loaded Vulkan global functions." << endl; }

	// Get a count of all the Vulkan instance extensions available
	uint32_t instanceExtensionsCount = 0;
	VkResult result = VK_SUCCESS;
	// Note: Providing null to the third (`pProperties`) argument makes the `vkEnumerateInstanceExtensionProperties` method fill the count at the 2nd argument with the number of extensions found.
	result = VulkanFunctionLoaders::vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionsCount, nullptr);
	if (result != VK_SUCCESS ||	instanceExtensionsCount == 0)
	{
		cout << "[FAIL] Could not get the number of Vulkan Instance extensions." << endl;
		return -3;
	}
	else
	{
		if (VERBOSE) { cout << "Found " << instanceExtensionsCount << " Vulkan instance extensions." << endl; }
	}

	// Obtain the details of all available Vulkan instance extensions
	std::vector<VkExtensionProperties> availableExtensions(instanceExtensionsCount);	
	result = VulkanFunctionLoaders::vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionsCount, &availableExtensions[0]);
	if (result != VK_SUCCESS || instanceExtensionsCount == 0)
	{
		std::cout << "[FAIL] Could not enumerate Instance extension details." << std::endl;
		return -4;
	}
	else
	{
		if (VERBOSE)
		{
			std::cout << "Extension details:" << std::endl;
			for (int i = 0; i < instanceExtensionsCount; ++i)
			{
				cout << "\t" << availableExtensions[i].extensionName << " - version: " << availableExtensions[i].specVersion << endl;
			}
		}
	}

	// Make all the extensions available for use
	// Note: If we only wanted to make SOME extensions available then we would add them to the desiredExtensions vector directly then check that they ARE available
	std::vector<char const*> desiredExtensions(instanceExtensionsCount);
	for (int i = 0; i < instanceExtensionsCount; ++i) { desiredExtensions[i] = availableExtensions[i].extensionName; }

	// Check that all the desired extensions are available. If a desiredExtension was "DoesNotExist" or such then we'll return a failure code.
	for (int i = 0; i < instanceExtensionsCount; ++i)
	{
		const bool supported = VulkanFunctionLoaders::IsExtensionSupported(availableExtensions, desiredExtensions[i]);
		if (!supported)
		{
			cout << "[FAIL]: Vulkan extension: " << desiredExtensions[i] << " is not available." << endl;
			return -5;
		}
	}
	if (VERBOSE) { cout << "[OK] All desired extensions are available." << endl; }

	// Construct the application info required to initialise Vulkan
	VkApplicationInfo applicationInfo = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		nullptr,
		ApplicationName.c_str(),
		VK_MAKE_VERSION(1, 0, 0),
		EngineName.c_str(),
		VK_MAKE_VERSION(1, 0, 0),
		VK_MAKE_VERSION(1, 0, 0)
	};

	// Construct the creation info using the above application info.
	// Note: We provide all the desired extension names as the final argument, assuming there are any (and there should be!)
	const VkInstanceCreateInfo instanceCreateInfo = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		nullptr,
		0,
		&applicationInfo,
		0,
		nullptr,
		static_cast<uint32_t>(desiredExtensions.size()),
		!desiredExtensions.empty() ? desiredExtensions.data() : nullptr
	};

	// Actually create the Vulkan instance!
	VkInstance vulkanInstance;
	const VkResult instanceCreationResult = VulkanFunctionLoaders::vkCreateInstance(&instanceCreateInfo, nullptr, &vulkanInstance);
	if (instanceCreationResult != VK_SUCCESS || vulkanInstance == VK_NULL_HANDLE)
	{
		cout << "[FAIL] Could not create Vulkan Instance." << endl;
		return -6;
	}

}