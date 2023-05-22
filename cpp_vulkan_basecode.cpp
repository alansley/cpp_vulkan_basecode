// cpp_vulkan_basecode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdint>
#include <vector>

// Our `VulkanHelpers` are just some static utility functions to do things like print out details or human-friendly strings of things
#include "VulkanHelpers.hpp"

using std::cout, std::endl, std::string;

// Vulkan requires an application name and engine name when creating an instance. This is so that AAA games
// can provide specific details which driver vendors like Nvidia and AMD can then check for to enable or
// disable specific driver settings. That's it - that's the entire purpose.
const string ApplicationName = "cpp_vulkan_basecode";
const string EngineName = "ACL_vulkan_engine";

// Note: Windows.h is required to call `LoadLibrary` to pull in `vulkan-1.dll`
// Also: When we build for x64 `_WIN32` is still defined - so take this as "We're on Windows" rather than "We're on Windows building in x86 / 32-bit mode"
#if defined _WIN32
	#include <Windows.h>
#endif

//#include "include/vulkan/vk_platform.h"
#define VK_NO_PROTOTYPES   // In this example we'll load the functions that we need only, rather than pull in all the prototypes from vulkan.h
#include "vulkan/vulkan.h" // Note: "vulkan.h" includes "vk_platform.h" amongst other things

// Custom struct for queue details
struct QueueInfo
{
	uint32_t FamilyIndex;
	std::vector<float> Priorities;
};

constexpr bool VERBOSE      = true;  // Prints out verbose details of what we're doing if true
constexpr bool VERY_VERBOSE = false; // Prints out additional details of what we're doing if true (e.g., prints all physical device properties, extensions etc.)

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

// TODO: If we look at the Vulkan SDK `Templates` folder they use Vulkan.hpp rather than this, and I believe that will allow us to load functions without all the templating craziness. Check it out.
#include "VulkanFunctions.h"

// TODO: I fixed this templating stuff via the commend from `bodyaka` at:
// https://stackoverflow.com/questions/63587107/undeclared-identifier-when-defining-macro-to-load-vulkan-function-pointers
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
if (name == nullptr) {                                      \
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

	// Method to load Vulkan instance-level functions for the provided instance
	bool LoadInstanceLevelFunctions(VkInstance &vulkanInstance)
	{
#define INSTANCE_LEVEL_VULKAN_FUNCTION( name )                      \
name = (PFN_##name)vkGetInstanceProcAddr( vulkanInstance, #name );  \
if( name == nullptr ) {                                             \
std::cout << "Could not load instance-level Vulkan function named: "\
#name << " for instance: " << vulkanInstance << std::endl;          \
return false;                                                       \
}
#include "ListOfVulkanFunctions.inl"

		return true;
	}

	// Method to load an instance-level function from a specific extension (which must be available and loaded)
	bool LoadInstanceLevelFunctionFromExtension(VkInstance& vulkanInstance, std::vector<char const*> enabledExtensions)
	{
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )  \
for (auto &enabledExtension : enabledExtensions) {                        \
if (std::string(enabledExtension) == std::string(extension)) {            \
name = (PFN_##name)vkGetInstanceProcAddr(vulkanInstance, #name);          \
if (name == nullptr) {                                                    \
std::cout << "Could not load instance-level Vulkan function from EXTENSION named: "      \
#name << std::endl;                                                       \
return false;                                                             \
}                                                                         \
}                                                                         \
}
#include "ListOfVulkanFunctions.inl"
		return true;
	}

	// Method to load a device level function
	bool LoadDeviceLevelFunction(VkDevice vulkanDevice, char const* name)
	{
#define DEVICE_LEVEL_VULKAN_FUNCTION( name )                                              \
name = (PFN_##name)vkGetDeviceProcAddr( vulkanDevice, #name );                            \
if( name == nullptr ) {                                                                   \
std::cout << "Could not load device-level Vulkan function named: " << #name << std::endl; \
			return false;                                                                 \
}
#include "ListOfVulkanFunctions.inl"
	return true;
	}

	bool LoadDeviceLevelFunctionFromExtension(VkDevice logicalDevice, char const* name, std::vector<char const*> enabledExtensions)
	{
#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name,	extension)                                                    \
		for (auto &enabledExtension : enabledExtensions) {			                                                      \
				if (std::string(enabledExtension) == std::string(extension)) {		                                      \
						name = (PFN_##name)vkGetDeviceProcAddr(logicalDevice, #name);                                     \
						if (name == nullptr) {							                                                  \
								std::cout << "Could not load device-level Vulkan function named: " << #name << std::endl; \
								return false;                                                                             \
						}                                                                                                 \
				}                                                                                                         \
		}
#include "ListOfVulkanFunctions.inl"
		return true;
	}

} // End of namespace VulkanFunctionLoaders


int main()
{
	// Set a flag & use validation layers if this is a debug build.
	// Note: Although we have `_DEBUG` defined in debug builds, we may want to output code-debugging details (as
	// opposed to
	//bool DEBUGGING = false;



	std::vector<const char*> vulkanLayers;
#if defined(_DEBUG)
	vulkanLayers.push_back("VK_LAYER_KHRONOS_validation");
	//DEBUGGING = true;
#else
	// If not debugging we assign nullptr to our vulkanLayers - this way we can always pass it to vulkan functions and it's always the right thing
	const char* vulkanLayers = nullptr;
#endif

	// ----- Step 1 -----
	// Connect to the Vulkan loader library. Note: `LIBRARY_TYPE` is a macro that makes the result a `HMODULE` on Windows and a `void*` on Linux.
	LIBRARY_TYPE vulkanLibrary;
	const bool connectedToVulkanLoader = ConnectWithVulkanLoaderLibrary(vulkanLibrary);
	if (!connectedToVulkanLoader)
	{
		cout << "[FAIL] Could not connect to Vulkan loader library." << endl;
		return -1;
	}
	if (VERBOSE) { cout << "[OK] Connected to Vulkan loader library." << endl; }

	// ----- Step 2 -----
	// Load all functions we've specified in `ListOfVulkanFunctions.inl` automatically
	const bool loadFunctionSuccess = VulkanFunctionLoaders::LoadFunctionExportedFromVulkanLoaderLibrary(vulkanLibrary);
	if (!loadFunctionSuccess)
	{
		cout << "[FAIL] Failed to load Vulkan loader function." << endl;
		return -1;		
	}
	if (VERBOSE) { cout << "[OK] Successfully loaded Vulkan loader function." << endl; }

	// ----- Step 3 -----
	// Load all the global functions
	const bool loadGlobalFunctionsSuccess = VulkanFunctionLoaders::LoadVulkanGlobalFunctions();
	if (!loadGlobalFunctionsSuccess)
	{
		cout << "[FAIL] Failed to load Vulkan global functions." << endl;
		return -2;
	}
	if (VERBOSE) { cout << "[OK] Successfully loaded Vulkan global functions." << endl; }

	// ----- Step 4 -----
	// Get a count of all the Vulkan instance extensions available..
	uint32_t instanceExtensionsCount = 0;
	VkResult result = VK_SUCCESS;
	// Note: Providing null to the third (`pProperties`) argument makes the `vkEnumerateInstanceExtensionProperties` method fill the count at the 2nd argument with the number of extensions found.
#if defined(_DEBUG) && defined(USING_VALIDATION_LAYERS)
	result = VulkanFunctionLoaders::vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionsCount, nullptr);
#else
	result = VulkanFunctionLoaders::vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionsCount, nullptr);
#endif
	
	if (result != VK_SUCCESS ||	instanceExtensionsCount == 0)
	{
		cout << "[FAIL] Could not get the number of Vulkan Instance extensions." << endl;
		return -3;
	}
	else
	{
		if (VERBOSE) { cout << "[OK] Found " << instanceExtensionsCount << " Vulkan instance extensions." << endl; }
	}
	// ..then obtain the details of all available Vulkan instance extensions.
	std::vector<VkExtensionProperties> availableExtensions(instanceExtensionsCount);	
	result = VulkanFunctionLoaders::vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionsCount, availableExtensions.data());
	if (result != VK_SUCCESS || instanceExtensionsCount == 0)
	{
		std::cout << "[FAIL] Could not enumerate Instance extension details." << std::endl;
		return -4;
	}
	else
	{
		if (VERY_VERBOSE)
		{
			for (unsigned int i = 0; i < instanceExtensionsCount; ++i)
			{
				cout << "\t" << availableExtensions[i].extensionName << " - version: " << availableExtensions[i].specVersion << endl;
			}
		}
	}

	// ----- Step 5 -----
	// Ask to make all the supported extensions we found available for use.
	// Note: If we only wanted to make SOME extensions available then we would add them to the desiredExtensions vector directly then
	// check that they ARE available.
	// Also: We have this as a pointer because later on when we create a `VkCreateDeviceInfo` object we need a pointer-to-a-pointer to the
	// list of all desired extensions we wish to load for our logical device (it gets passed to `ppEnabledExtensionNames`)!
	std::vector<char const*> desiredInstanceExtensions;
	desiredInstanceExtensions.push_back("VK_KHR_device_group_creation");             // 1
	desiredInstanceExtensions.push_back("VK_KHR_external_fence_capabilities");	      // 2
	desiredInstanceExtensions.push_back("VK_KHR_external_memory_capabilities");      // 3
	desiredInstanceExtensions.push_back("VK_KHR_external_semaphore_capabilities");   // 4	
	desiredInstanceExtensions.push_back("VK_KHR_get_physical_device_properties2");   // 5
	desiredInstanceExtensions.push_back("VK_KHR_get_surface_capabilities2");         // 6
	desiredInstanceExtensions.push_back("VK_KHR_surface");                           // 7
	//desiredInstanceExtensions.push_back("VK_KHR_surface_protected_capabilities");       // 8 - THIS IS THE ONE THAT'S CAUSING ISSUES! NOT USING FOR NOW!
	desiredInstanceExtensions.push_back("VK_KHR_win32_surface");                     // 9
	desiredInstanceExtensions.push_back("VK_EXT_debug_report");                      // 10
	desiredInstanceExtensions.push_back("VK_EXT_debug_utils");                       // 11
	desiredInstanceExtensions.push_back("VK_EXT_swapchain_colorspace");              // 12
	desiredInstanceExtensions.push_back("VK_NV_external_memory_capabilities");       // 13
	desiredInstanceExtensions.push_back("VK_KHR_portability_enumeration");           // 14
	desiredInstanceExtensions.push_back("VK_LUNARG_direct_driver_loading");          // 15

	uint32_t numDesiredInstanceExtensions = desiredInstanceExtensions.size();
	if (VERBOSE)
	{
		cout << "[OK] Requesting " << numDesiredInstanceExtensions << " of the available instance-level extensions." << endl;
		if (VERY_VERBOSE)
		{
			for (unsigned int i = 0; i < numDesiredInstanceExtensions; ++i)
			{
				cout << "\t" << desiredInstanceExtensions.at(i) << endl;
			}
		}
	}

	/*
	---------- THE ERROR WE GET WHEN WE IMPORT desiredExtension #8 - "VK_KHR_surface_protected_capabilities" ----------
	[OK] This is a a debug build - about to enable vulkan layers. Count: 1
	VUID-vkCreateInstance-ppEnabledExtensionNames-01388(ERROR / SPEC): msgNum: -437968512 - Validation Error:
	[ VUID-vkCreateInstance-ppEnabledExtensionNames-01388 ] Object 0: VK_NULL_HANDLE, type = VK_OBJECT_TYPE_INSTANCE;
	| MessageID = 0xe5e52180 | Missing extension required by the instance extension VK_KHR_surface_protected_capabilities: VK_VERSION_1_1.
	The Vulkan spec states: All required extensions for each extension in the VkInstanceCreateInfo::ppEnabledExtensionNames list must also
	be present in that list
	(https://vulkan.lunarg.com/doc/view/1.3.246.0/windows/1.3-extensions/vkspec.html#VUID-vkCreateInstance-ppEnabledExtensionNames-01388)
	Objects: 1
		[0] 0, type: 1, name: NULL
	*/

	// Check that all the desired extensions are available	
	if (numDesiredInstanceExtensions > 0)
	{
		for (unsigned int desiredExtensionIndex = 0; desiredExtensionIndex < numDesiredInstanceExtensions; ++desiredExtensionIndex)
		{
			//cout << "Checking if the following extension is supported: " << pDesiredExtensions->at(i) << endl;
			const bool supported = VulkanFunctionLoaders::IsExtensionSupported(availableExtensions, desiredInstanceExtensions.at(desiredExtensionIndex));// *pDesiredExtensions)[i]);
			if (!supported)
			{
				cout << "[FAIL]: Vulkan extension: " << desiredInstanceExtensions.at(desiredExtensionIndex) << " is not available." << endl;
				return -5;
			}
		}
		if (VERBOSE) { cout << "[OK] All " << numDesiredInstanceExtensions << " desired instance extensions are available." << endl; }
	}
	else
	{
		cout << "[WARNING]: No desired instance extensions requested!" << endl;
	}

	// ----- Step 6 -----
	// Construct the application info required to initialise Vulkan.
	// Note: The application name and engine name properties are used by graphics drivers to twiddle internal driver
	// settings, typically for specific AAA games.
	VkApplicationInfo applicationInfo = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		nullptr,
		ApplicationName.c_str(),
		VK_MAKE_VERSION(1, 0, 0),
		EngineName.c_str(),
		VK_MAKE_VERSION(1, 0, 0),
		VK_MAKE_VERSION(1, 0, 0)
	};

	// ----- Step 7 -----
	// Construct the instance creation info using the above application info.
	auto numVulkanLayers = static_cast<uint32_t>(vulkanLayers.size());
	if (VERBOSE) { cout << "[OK] About to create `InstanceCreateInfo` with: " << numVulkanLayers << " vulkan layer(s)." << endl; }
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = nullptr;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledExtensionCount = numDesiredInstanceExtensions; // static_cast<uint32_t>(desiredExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = desiredInstanceExtensions.data();
	instanceCreateInfo.enabledLayerCount = numVulkanLayers;
	instanceCreateInfo.ppEnabledLayerNames = numVulkanLayers != 0 ? vulkanLayers.data() : nullptr;

	// ----- Step 8 -----
	// Actually create the Vulkan instance! Note: The `VkInstance` returned is an `opaque handle` - we can't get any details from it directly.
	// See: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkInstance.html
	VkInstance vulkanInstance;
	const VkResult instanceCreationResult = VulkanFunctionLoaders::vkCreateInstance(&instanceCreateInfo, nullptr, &vulkanInstance);
	if (instanceCreationResult != VK_SUCCESS || vulkanInstance == VK_NULL_HANDLE)
	{
		cout << "[FAIL] Could not create Vulkan instance." << endl;
		return -6;
	}
	if (VERBOSE) { cout << "[OK] Vulkan instance created." << endl; }

	// ----- Step 9 -----
	// Load our instance-level functions.
	const bool instanceLevelFunctionsLoaded = VulkanFunctionLoaders::LoadInstanceLevelFunctions(vulkanInstance);
	if (!instanceLevelFunctionsLoaded)
	{
		cout << "[FAIL]: Vulkan instance level functions could not be loaded." << endl;
		return -7;
	}
	if (VERBOSE) { cout << "[OK] Vulkan instance-level functions loaded." << endl; }

	// ----- Step 10 -----
	// Now load the instance-level functions that are provided by our extensions.
	const bool instanceLevelExtensionFunctionsLoaded = VulkanFunctionLoaders::LoadInstanceLevelFunctionFromExtension(vulkanInstance, desiredInstanceExtensions);
	if (!instanceLevelExtensionFunctionsLoaded)
	{
		cout << "[FAIL]: Vulkan instance level functions could not be loaded from extensions." << endl;
		return -8;
	}
	if (VERBOSE) { cout << "[OK] Vulkan instance-level functions loaded from extensions." << endl; }

	// ----- Step 11 -----
	// Enumerate available physical devices (from which we will access LOGICAL devices that will perform our work!).
	// Note: Like our VkInstance, VkPhysicalDevice is an opaque handle.
	// See: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDevice.html
	// IMPORTANT: We essentially do this process twice - first to find out how many physical devices we have, and then again to actually
	// populate a vector of VkPhysicalDevice. While we could combine these two passes into a single one, we don't know the vector size
	// when we go to populate it so rather than go off-road let's stay with the book's way of doing it (Vulcan Cookbook, p36-37).37 for details.
	uint32_t physicalDeviceCount = 0;	
	result = VK_SUCCESS;
	result = VulkanFunctionLoaders::vkEnumeratePhysicalDevices(vulkanInstance, &physicalDeviceCount, nullptr);
	if (result != VK_SUCCESS || physicalDeviceCount == 0)
	{
		cout << "[FAIL] Could not enumerate physical devices. Physical devices found: " << physicalDeviceCount << endl;
		return -9;
	}
	if (VERBOSE) { cout << "[OK] Found " << physicalDeviceCount << " physical device(s)." << endl; }

	std::vector<VkPhysicalDevice> availablePhysicalDevices(physicalDeviceCount);
	result = VK_SUCCESS;
	result = VulkanFunctionLoaders::vkEnumeratePhysicalDevices(vulkanInstance, &physicalDeviceCount, availablePhysicalDevices.data());
	if (result != VK_SUCCESS || physicalDeviceCount == 0)
	{
		cout << "[FAIL] Could not populate details of physical devices. Physical devices found: " << physicalDeviceCount << endl;
		return -10;
	}
	if (VERBOSE) { cout << "[OK] Populated details of " << physicalDeviceCount << " physical device(s)." << endl; }
	
	// For now we'll only work with the first physical device found so let's keep an easy reference to it
	if (physicalDeviceCount > 1)
	{
		cout << "[WARNING] Found multiple physical devices - using physical device 0 for now to keep the code manageable." << endl;
	}
	VkPhysicalDevice activePhysicalDevice = availablePhysicalDevices[0];

	// ----- Step 11 -----
	// Populate extension properties on available physical devices.
	// Note: Again we do this as a two-step - first we find the number of extensions for a physical device, then we populate details (p40)
	uint32_t physicalDeviceExtensionCount = 0;
	result = VK_SUCCESS;
	result = VulkanFunctionLoaders::vkEnumerateDeviceExtensionProperties(activePhysicalDevice, nullptr, &physicalDeviceExtensionCount, nullptr);
	if (result != VK_SUCCESS || physicalDeviceExtensionCount == 0)
	{
		cout << "[FAIL] Could not enumerate physical device extensions. Physical devices extension count: " << physicalDeviceExtensionCount << endl;
		return -11;
	}
	if (VERBOSE) { cout << "[OK] Found " << physicalDeviceExtensionCount << " extensions for physical device 0." << endl; }

	std::vector<VkExtensionProperties> physicalDeviceExtensions(physicalDeviceExtensionCount);
	result = VK_SUCCESS;
	result = VulkanFunctionLoaders::vkEnumerateDeviceExtensionProperties(availablePhysicalDevices[0], nullptr, &physicalDeviceExtensionCount, physicalDeviceExtensions.data());
	if (result != VK_SUCCESS || physicalDeviceExtensionCount == 0)
	{
		cout << "[FAIL] Could populate physical device 0 extension properties. Physical device 0 extensions found: " << physicalDeviceExtensionCount << endl;
		return -12;
	}
	if (VERBOSE) { cout << "[OK] Populated " << physicalDeviceExtensionCount << " extension properties for physical device 0." << endl; }
	if (VERY_VERBOSE)
	{
		for (auto &pdeProperty : physicalDeviceExtensions)
		{
			cout << "\t" << pdeProperty.extensionName << " - version: " << pdeProperty.specVersion << endl;
		}
	}		

	// ----- Step 12 -----
	// Get features and properties of physical devices
	VkPhysicalDeviceFeatures activePhysicalDeviceFeatures;
	VkPhysicalDeviceProperties activePhysicalDeviceProperties;
	VulkanFunctionLoaders::vkGetPhysicalDeviceFeatures(activePhysicalDevice, &activePhysicalDeviceFeatures);
	VulkanFunctionLoaders::vkGetPhysicalDeviceProperties(activePhysicalDevice, &activePhysicalDeviceProperties);
	if (VERY_VERBOSE)
	{
		VulkanHelpers::printPhysicalDeviceFeatures(activePhysicalDeviceFeatures);
		VulkanHelpers::printPhysicalDeviceProperties(activePhysicalDeviceProperties);
	}

	// ----- Step 13 -----
	// Get details of queue families - in familiar style, we'll do this as a two-step where we first get the number of families and then
	// populate details of each queue family we found.
	uint32_t queueFamiliesCount = 0;
	VulkanFunctionLoaders::vkGetPhysicalDeviceQueueFamilyProperties(activePhysicalDevice, &queueFamiliesCount, nullptr);
	if (queueFamiliesCount == 0)
	{
		std::cout << "Could not get the number of queue families." << std::endl;
		return -13;
	}
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamiliesCount);
	VulkanFunctionLoaders::vkGetPhysicalDeviceQueueFamilyProperties(activePhysicalDevice, &queueFamiliesCount,queueFamilies.data());
	if (queueFamiliesCount == 0)
	{
		cout << "Could not acquire properties of queue families." << endl;
		return -14;
	}
	if (VERBOSE)
	{
		cout << "[OK] Queue families found: " << queueFamiliesCount << endl;
		if (VERY_VERBOSE)
		{
			for (int i = 0; i < queueFamiliesCount; ++i)
			{
				auto granularity = queueFamilies[i].minImageTransferGranularity;

				cout << "Queue family index: " << i << endl;
				cout << "\tminImageTransferGranularity - Width: " << granularity.width << ", Height: " << granularity.height << ", Depth: " << granularity.depth << endl;
				cout << "\ttimestampValidBits:                  " << queueFamilies[i].timestampValidBits << endl;
				cout << "\tqueueCount:                          " << queueFamilies[i].queueCount << endl;
				cout << "\tqueueFlags:                          " << VulkanHelpers::getFriendlyQueueFlags(queueFamilies[i].queueFlags) << endl;
			}
		}
	}

	// ----- Step 15 -----
	// Now that we have details of the available queue families, we need to choose one that has our desired capabilities - which in this
	// case will simply be that we want to draw something.	
	int numSuitableFamiliesFound = 0;
	VkQueueFamilyProperties activeQueueFamily;
	uint32_t activeQueueFamilyIndex = 0xffffffff; // Set to max possible value initially to avoid "may be uninitialised" moaning
	VkQueueFlags desiredCapabilities = VK_QUEUE_GRAPHICS_BIT;
	string desiredCapabilitiesString = VulkanHelpers::getFriendlyQueueFlags(desiredCapabilities);
	uint32_t numDesiredQueues = 1; // From a given queue family we may ask for a subset of all available queues (like if it can provide 16 queues, we may only ask for 3 for example)
	bool requestAllAvailableQueues = true;
	for (int i = 0; i < queueFamiliesCount; ++i)
	{
		// If the queue we're looking at has the flag for desired capabilities AND it has a queue available..
		if ((queueFamilies[i].queueFlags & desiredCapabilities) != 0 && queueFamilies[i].queueCount > 0 && queueFamilies[i].queueCount >= numDesiredQueues)
		{
			// ..then if we haven't already found a suitable queue family we have now! Set it! 
			if (numSuitableFamiliesFound == 0)
			{
				activeQueueFamily = queueFamilies[i];
				activeQueueFamilyIndex = i;
				++numSuitableFamiliesFound;
			}
			// Warn if we found another potential queue family that meets our needs - but to keep things simple for now we'll stick w/ the first one found
			else if (numSuitableFamiliesFound >= 1)
			{
				++numSuitableFamiliesFound;
				cout << "[WARNING] Found another queue family w/ req'd capabilities, but sticking w/ 1st found. Potential queue families avail now: " << numSuitableFamiliesFound << endl;
			}
		}
	}
	if (numSuitableFamiliesFound == 0)
	{
		cout << "[FAIL] Could not find a suitable queue family with desired capabilities: " << desiredCapabilitiesString << endl;
		return -15;
	}
	if (VERBOSE)
	{
		cout << "[OK] Found queue family with desired capabilities: " << desiredCapabilitiesString << endl;
		cout << "[OK] Using queue family at index: " << activeQueueFamilyIndex << endl;
		cout << "[OK] Active queue family queue count is: " << activeQueueFamily.queueCount << endl;
	}

	if (requestAllAvailableQueues)
	{
		numDesiredQueues = activeQueueFamily.queueCount;
		cout << "[OK] Requesting access to all available queues. Count: " << numDesiredQueues << endl;
	}
	else
	{
		cout << "[OK] Although we can request up to " << activeQueueFamily.queueCount << " we are only requesting to use: " << numDesiredQueues << " queues." << endl;
	}
	
	// ----- Step 15 -----
	// Create our `QueueInfo` struct with the details of the queue we'll use
	QueueInfo activeQueueInfo;
	activeQueueInfo.FamilyIndex = activeQueueFamilyIndex;
	std::vector<float> queuePriorities(numDesiredQueues, 0.5f); // Fill the vector of queue priorities w/ a value (multiple same values is OK)
	activeQueueInfo.Priorities = queuePriorities;
	
	// ----- Step 16 -----
	// Create the `DeviceQueueCreateInfo` structure.
	// CAREFUL: Normally we'd create a vector of `VkDeviceCreateInfo` but because we're using only a single physical device we can use just one here.
	// CAREFUL: See the Vulkan Cookbook, p51 for further details.
	VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
	deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; // Be careful here - we have to use `DEVICE_QUEUE_CREATE_INFO` not `DEVICE_CREATE_INFO`!
	deviceQueueCreateInfo.pNext = nullptr;
	deviceQueueCreateInfo.flags = 0;
	deviceQueueCreateInfo.queueFamilyIndex = activeQueueFamilyIndex;
	deviceQueueCreateInfo.queueCount = numDesiredQueues; // .queueCount; // We can ask for a smaller number of queues here, if we wish (instead of ALL queues available) - if we do so we 
	deviceQueueCreateInfo.pQueuePriorities = queuePriorities.data();

	// ----- Step 17 -----
	// Create vector of the names of all physical device extensions we wish to load then load them!
	// IMPORTANT: We NEVER want to load "all physical device extensions" because we cannot legally combine certain combinations of features & extensions!
	std::vector<char const*> requestedPhysicalDeviceExtensionNames;
	requestedPhysicalDeviceExtensionNames.push_back("VK_KHR_16bit_storage");                // 1
	requestedPhysicalDeviceExtensionNames.push_back("VK_KHR_storage_buffer_storage_class"); // 2 - Required by `VK_KHR_16bit_storage` (1)

	if (VERBOSE)
	{
		cout << "[OK] Requesting to load: " << requestedPhysicalDeviceExtensionNames.size() << " physical device extensions." << endl;
		if (VERY_VERBOSE)
		{			
			for (unsigned int i = 0; i < numDesiredInstanceExtensions; ++i)
			{
				cout << "\t" << requestedPhysicalDeviceExtensionNames.at(i) << endl;
			}
		}
	}
	
	// Create a `DeviceCreateInfo` object which we use to construct our logical device
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO; // Be careful here - we have to use `DEVICE_CREATE_INFO` not `DEVICE_QUEUE_CREATE_INFO`!
	deviceCreateInfo.pNext = nullptr;
	deviceCreateInfo.flags = 0;
	deviceCreateInfo.queueCreateInfoCount = 1; // Providing `1` as we're just using a single physical device for the time being!
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo; // This would normally be a pointer to the start of a vector of `QueueCreateInfo`s!
	deviceCreateInfo.enabledLayerCount = numVulkanLayers;
	deviceCreateInfo.ppEnabledLayerNames = numVulkanLayers == 0 ? nullptr : vulkanLayers.data();
	deviceCreateInfo.enabledExtensionCount = requestedPhysicalDeviceExtensionNames.size();
	deviceCreateInfo.ppEnabledExtensionNames = requestedPhysicalDeviceExtensionNames.data();
	deviceCreateInfo.pEnabledFeatures = &activePhysicalDeviceFeatures;
		
	// ----- Step 18 -----
	// Finally, create the logical device!
	VkDevice logicalDevice;
	result = VulkanFunctionLoaders::vkCreateDevice(activePhysicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);
	if (result != VK_SUCCESS)
	{
		cout << "[FAIL] Failed to create logical device. VkResult is: " << VulkanHelpers::getFriendlyResultString(result) << endl;
		return -16;
	}
	if (VERBOSE) { cout << "[OK] Successfully created logical device." << endl; }

	// Bad function names we can attempt to load to mke sure we handle stuff properly
	std::vector<char const*> badExtensionNames;
	requestedPhysicalDeviceExtensionNames.push_back("VK_KHR_16bit_storage666");
	requestedPhysicalDeviceExtensionNames.push_back("VK_KHR_storage_buffer_storage_class_DOES_NOT_EXIST");

	bool boolResult = VulkanFunctionLoaders::LoadDeviceLevelFunction(logicalDevice, requestedPhysicalDeviceExtensionNames[0]);
	if (!boolResult) cout << "Could not load device level function!" << endl;

	boolResult = VulkanFunctionLoaders::LoadDeviceLevelFunctionFromExtension(logicalDevice, requestedPhysicalDeviceExtensionNames[0], requestedPhysicalDeviceExtensionNames);
	if (!boolResult) cout << "Could not load device level function from extension!" << endl;

	// NEXT: Getting a device queue

	VkQueue queue;
	uint32_t activeQueueNumber = 0;
	if (activeQueueNumber > activeQueueFamily.queueCount)
	{
		cout << "[FAIL] Requested to use active queue at index: " << activeQueueNumber << " but active queue family's queue count is only: " + activeQueueFamily.queueCount << endl;
		return -17;

	}
	vkGetDeviceQueue(logicalDevice, activeQueueFamilyIndex, activeQueueNumber, &queue);
}