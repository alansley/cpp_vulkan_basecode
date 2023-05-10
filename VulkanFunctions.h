#pragma once

#include "vulkan.h"

// Note: Always put declarations & definitions to Vulkan functions inside a namespace, as if they are global they can cause issues on some OSs.
// Source: Vulkan Cookbook [2016], p20
namespace VulkanCookbook
{
	// Note: The ## operator in the preprocessor concatenates things, so for example if we specified:
	//			EXPORTED_VULKAN_FUNCTION(dave) then the extern name of the function would be "PFN_dave"

#define EXPORTED_VULKAN_FUNCTION(name) extern PFN_##name name;

#define GLOBAL_LEVEL_VULKAN_FUNCTION(name) extern PFN_##name name;

#define INSTANCE_LEVEL_VULKAN_FUNCTION(name) extern PFN_##name name;

#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name,	extension) extern PFN_##name name;

#define DEVICE_LEVEL_VULKAN_FUNCTION(name) extern PFN_##name name;

#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension) extern PFN_##name name;

#include "ListOfVulkanFunctions.inl"

} // namespace VulkanCookbook