#ifndef VULKAN_FUNCTIONS_H
#define VULKAN_FUNCTIONS_H

#include "vulkan.h"

// Note: Always put declarations & definitions to Vulkan functions inside a namespace, as if they are global they can cause issues on some OSs.
// Source: Vulkan Cookbook [2016], p20
namespace VulkanFunctionLoaders
{
	// Note: The ## operator in the preprocessor concatenates things, so for example if we specified:
	//			EXPORTED_VULKAN_FUNCTION(dave) then the extern name of the function would be "PFN_dave"

#define EXPORTED_VULKAN_FUNCTION( name ) extern PFN_##name name;

// Note: In Vulkan there are only 3 global-level functions: `vkEnumerateInstanceExtensionProperties`, `vkEnumerateInstanceLayerProperties`, and `vkCreateInstance`.
//       These are used during Instance creation to check what instance-level extensions and layers are available and to create the Instance itself.
#define GLOBAL_LEVEL_VULKAN_FUNCTION( name ) extern PFN_##name name;

#define INSTANCE_LEVEL_VULKAN_FUNCTION( name ) extern PFN_##name name;

#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) extern PFN_##name name;

#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) extern PFN_##name name;

#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) extern PFN_##name name;

#include "ListOfVulkanFunctions.inl"

} // End of namespace VulkanFunctionLoaders

#endif