// cpp_vulkan_basecode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdint>

using std::cout, std::endl, std::string;

// Vulkan requires an application name and engine name when creating an instance. This is so that AAA games
// can provide specific details which driver vendors like Nvidia and AMD can then check for to enable or
// disable specific driver logic. That's it - that's the entire purpose.
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
std::cout << "Could not load instance-level Vulkan function named: "      \
#name << std::endl;                                                       \
return false;                                                             \
}                                                                         \
}                                                                         \
}
#include "ListOfVulkanFunctions.inl"
		return true;
	}

} // End of namespace VulkanFunctionLoaders

// Method to print out Vulkan physical device features
// See: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceFeatures.html
void printPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures &features)
{
	cout << "----- Physical Device Features (1 = available, 0 = unavailable) -----"                         << endl;
	cout << "alphaToOne:                              " << features.alphaToOne                              << endl;
	cout << "depthBiasClamp:                          " << features.depthBiasClamp                          << endl;
	cout << "depthBounds:                             " << features.depthBounds                             << endl;
	cout << "depthClamp:                              " << features.depthClamp                              << endl;
	cout << "drawIndirectFirstInstance:               " << features.drawIndirectFirstInstance               << endl;
	cout << "dualSrcBlend:                            " << features.dualSrcBlend                            << endl;
	cout << "fillModeNonSolid:                        " << features.fillModeNonSolid                        << endl;
	cout << "fragmentStoresAndAtomics:                " << features.fragmentStoresAndAtomics                << endl;
	cout << "fullDrawIndexUint32:                     " << features.fullDrawIndexUint32                     << endl;
	cout << "geometryShader:                          " << features.geometryShader                          << endl;
	cout << "imageCubeArray:                          " << features.imageCubeArray                          << endl;
	cout << "independentBlend:                        " << features.independentBlend                        << endl;
	cout << "inheritedQueries:                        " << features.inheritedQueries                        << endl;
	cout << "largePoints:                             " << features.largePoints                             << endl;
	cout << "logicOp:                                 " << features.logicOp                                 << endl;
	cout << "multiDrawIndirect:                       " << features.multiDrawIndirect                       << endl;
	cout << "multiViewport:                           " << features.multiViewport                           << endl;
	cout << "occlusionQueryPrecise:                   " << features.occlusionQueryPrecise                   << endl;
	cout << "pipelineStatisticsQuery:                 " << features.pipelineStatisticsQuery                 << endl;
	cout << "robustBufferAccess:                      " << features.robustBufferAccess                      << endl;
	cout << "samplerAnisotropy:                       " << features.samplerAnisotropy                       << endl;
	cout << "sampleRateShading:                       " << features.sampleRateShading                       << endl;	
	cout << "shaderClipDistance:                      " << features.shaderClipDistance                      << endl;
	cout << "shaderCullDistance:                      " << features.shaderCullDistance                      << endl;
	cout << "shaderFloat64:                           " << features.shaderFloat64                           << endl;
	cout << "shaderImageGatherExtended:               " << features.shaderImageGatherExtended               << endl;
	cout << "shaderInt16:                             " << features.shaderInt16                             << endl;
	cout << "shaderInt64:                             " << features.shaderInt64                             << endl;
	cout << "shaderResourceMinLod:                    " << features.shaderResourceMinLod                    << endl;
	cout << "shaderResourceResidency:                 " << features.shaderResourceResidency                 << endl;
	cout << "shaderSampledImageArrayDynamicIndexing:  " << features.shaderSampledImageArrayDynamicIndexing  << endl;
	cout << "shaderStorageBufferArrayDynamicIndexing: " << features.shaderStorageBufferArrayDynamicIndexing << endl;
	cout << "shaderStorageImageExtendedFormats:       " << features.shaderStorageImageExtendedFormats       << endl;
	cout << "shaderStorageImageMultisample:           " << features.shaderStorageImageMultisample           << endl;
	cout << "shaderStorageImageReadWithoutFormat:     " << features.shaderStorageImageReadWithoutFormat     << endl;
	cout << "shaderStorageImageWriteWithoutFormat:    " << features.shaderStorageImageWriteWithoutFormat    << endl;	
	cout << "shaderTessellationAndGeometryPointSize:  " << features.shaderTessellationAndGeometryPointSize  << endl;
	cout << "shaderUniformBufferArrayDynamicIndexing: " << features.shaderUniformBufferArrayDynamicIndexing << endl;
	cout << "sparseBinding:                           " << features.sparseBinding                           << endl;
	cout << "sparseResidency16Samples:                " << features.sparseResidency16Samples                << endl;
	cout << "sparseResidency2Samples:                 " << features.sparseResidency2Samples                 << endl;
	cout << "sparseResidency4Samples:                 " << features.sparseResidency4Samples                 << endl;
	cout << "sparseResidency8Samples:                 " << features.sparseResidency8Samples                 << endl;
	cout << "sparseResidencyAliased:                  " << features.sparseResidencyAliased                  << endl;
	cout << "sparseResidencyBuffer:                   " << features.sparseResidencyBuffer                   << endl;
	cout << "sparseResidencyImage2D:                  " << features.sparseResidencyImage2D                  << endl;
	cout << "sparseResidencyImage3D:                  " << features.sparseResidencyImage3D                  << endl;
	cout << "tessellationShader:                      " << features.tessellationShader                      << endl;
	cout << "textureCompressionASTC_LDR:              " << features.textureCompressionASTC_LDR              << endl;
	cout << "textureCompressionBC:                    " << features.textureCompressionBC                    << endl;
	cout << "textureCompressionETC2:                  " << features.textureCompressionETC2                  << endl;
	cout << "variableMultisampleRate:                 " << features.variableMultisampleRate                 << endl;
	cout << "vertexPipelineStoresAndAtomics:          " << features.vertexPipelineStoresAndAtomics          << endl;
	cout << "wideLines:                               " << features.wideLines                               << endl;
}


// Method to print out Vulkan physical device features
// See : https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceProperties.html
// Also: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceType.html
// Also: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceSparseProperties.html
// Also: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceLimits.html
void printPhysicalDeviceProperties(const VkPhysicalDeviceProperties& properties)
{
	// Create a human-readable device-type string from the device-type enum
	const VkPhysicalDeviceType deviceType = properties.deviceType;
	std::string deviceTypeString;
	switch (deviceType)
	{
	case 0:
		deviceTypeString = "VK_PHYSICAL_DEVICE_TYPE_OTHER";
		break;
	case 1:
		deviceTypeString = "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU";
		break;
	case 2:
		deviceTypeString = "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU";
		break;
	case 3:
		deviceTypeString = "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU";
		break;
	case 4:
		deviceTypeString = "VK_PHYSICAL_DEVICE_TYPE_CPU";
		break;
	default:
		deviceTypeString = "Unknown VkPhysicalDeviceType enum: " + deviceType;
	}

	cout << "----- Physical Device Properties -----" << endl;
	cout << "apiVersion:        " << properties.apiVersion        << endl;
	cout << "driverVersion:     " << properties.driverVersion     << endl;
	cout << "vendorID:          " << properties.vendorID          << endl;
	cout << "deviceID:          " << properties.deviceID          << endl;
	cout << "deviceType:        " << deviceTypeString             << endl;
	cout << "deviceName:        " << properties.deviceName        << endl;
	cout << "pipelineCacheUUID: Non-print-friendly uint8_t[VK_UUID_SIZE] - use directly" << endl;

	const VkPhysicalDeviceSparseProperties sparseProperties = properties.sparseProperties;
	cout << "--- Sparse Properties ---" << endl;
	cout << "residencyStandard2DBlockShape:            " << sparseProperties.residencyStandard2DBlockShape << endl;
	cout << "residencyStandard2DMultisampleBlockShape: " << sparseProperties.residencyStandard2DMultisampleBlockShape << endl;
	cout << "residencyStandard3DBlockShape             " << sparseProperties.residencyStandard3DBlockShape << endl;
	cout << "residencyAlignedMipSize                   " << sparseProperties.residencyAlignedMipSize << endl;
	cout << "residencyNonResidentStrict                " << sparseProperties.residencyNonResidentStrict << endl;

	const VkPhysicalDeviceLimits limits = properties.limits;
	cout << "--- Limits ---" << endl;
	cout << "bufferImageGranularity:                          " << limits.bufferImageGranularity                << endl;
	cout << "discreteQueuePriorities:                         " << limits.discreteQueuePriorities               << endl;
	cout << "framebufferColorSampleCounts:                    " << limits.framebufferColorSampleCounts          << endl;
	cout << "framebufferDepthSampleCounts:                    " << limits.framebufferDepthSampleCounts          << endl;
	cout << "framebufferNoAttachmentsSampleCounts:            " << limits.framebufferNoAttachmentsSampleCounts  << endl;
	cout << "framebufferStencilSampleCounts:                  " << limits.framebufferStencilSampleCounts        << endl;
	cout << "lineWidthGranularity:                            " << limits.lineWidthGranularity                  << endl;
	cout << "lineWidthRange - Min:                            " << limits.lineWidthRange[0] << ", Max: " << limits.lineWidthRange[1] << endl;
	cout << "maxBoundDescriptorSets:                          " << limits.maxBoundDescriptorSets                << endl;
	cout << "maxClipDistances:                                " << limits.maxClipDistances                      << endl;
	cout << "maxColorAttachments:                             " << limits.maxColorAttachments                   << endl;
	cout << "maxCombinedClipAndCullDistances:                 " << limits.maxCombinedClipAndCullDistances       << endl;
	cout << "maxComputeSharedMemorySize:                      " << limits.maxComputeSharedMemorySize            << endl;
	cout << "maxComputeWorkGroupCount                      X: " << limits.maxComputeWorkGroupCount[0] << ", Y: " << limits.maxComputeWorkGroupCount[1] << ", Z: " << limits.maxComputeWorkGroupCount[2] << endl;
	cout << "maxComputeWorkGroupInvocations:                  " << limits.maxComputeWorkGroupInvocations        << endl;
	cout << "maxComputeWorkGroupSize                       X: " << limits.maxComputeWorkGroupSize[0]  << ", Y: " << limits.maxComputeWorkGroupSize[1]  << ", Z: " << limits.maxComputeWorkGroupSize[2] << endl;
	cout << "maxCullDistances:                                " << limits.maxCullDistances                      << endl;
	cout << "maxDescriptorSetInputAttachments:                " << limits.maxDescriptorSetInputAttachments      << endl;
	cout << "maxDescriptorSetSampledImages:                   " << limits.maxDescriptorSetSampledImages         << endl;
	cout << "maxDescriptorSetSamplers:                        " << limits.maxDescriptorSetSamplers              << endl;
	cout << "maxDescriptorSetStorageBuffers:                  " << limits.maxDescriptorSetStorageBuffers        << endl;
	cout << "maxDescriptorSetStorageBuffersDynamic:           " << limits.maxDescriptorSetStorageBuffersDynamic << endl;
	cout << "maxDescriptorSetStorageImages:                   " << limits.maxDescriptorSetStorageImages         << endl;
	cout << "maxDescriptorSetUniformBuffers:                  " << limits.maxDescriptorSetUniformBuffers        << endl;
	cout << "maxDescriptorSetUniformBuffersDynamic:           " << limits.maxDescriptorSetUniformBuffersDynamic << endl;
	cout << "maxDrawIndexedIndexValue:                        " << limits.maxDrawIndexedIndexValue              << endl;
	cout << "maxDrawIndirectCount:                            " << limits.maxDrawIndirectCount << endl;
	cout << "maxFragmentCombinedOutputResources:              " << limits.maxFragmentCombinedOutputResources << endl;
	cout << "maxFragmentDualSrcAttachments:                   " << limits.maxFragmentDualSrcAttachments << endl;
	cout << "maxFragmentInputComponents:                      " << limits.maxFragmentInputComponents << endl;
	cout << "maxFragmentOutputAttachments:                    " << limits.maxFragmentOutputAttachments << endl;
	cout << "maxFramebufferHeight:                            " << limits.maxFramebufferHeight << endl;
	cout << "maxFramebufferLayers:                            " << limits.maxFramebufferLayers << endl;
	cout << "maxFramebufferWidth:                             " << limits.maxFramebufferWidth << endl;
	cout << "maxGeometryInputComponents:                      " << limits.maxGeometryInputComponents << endl;
	cout << "maxGeometryOutputComponents:                     " << limits.maxGeometryOutputComponents << endl;
	cout << "maxGeometryOutputVertices:                       " << limits.maxGeometryOutputVertices << endl;
	cout << "maxGeometryShaderInvocations:                    " << limits.maxGeometryShaderInvocations << endl;
	cout << "maxGeometryTotalOutputComponents:                " << limits.maxGeometryTotalOutputComponents << endl;
	cout << "maxImageArrayLayers:                             " << limits.maxImageArrayLayers << endl;
	cout << "maxImageDimension1D:                             " << limits.maxImageDimension1D << endl;
	cout << "maxImageDimension2D:                             " << limits.maxImageDimension2D << endl;
	cout << "maxImageDimension3D:                             " << limits.maxImageDimension3D << endl;
	cout << "maxImageDimensionCube:                           " << limits.maxImageDimensionCube << endl;
	cout << "maxInterpolationOffset:                          " << limits.maxInterpolationOffset << endl;
	cout << "maxMemoryAllocationCount:                        " << limits.maxMemoryAllocationCount << endl;
	cout << "maxPerStageDescriptorInputAttachments:           " << limits.maxPerStageDescriptorInputAttachments << endl;
	cout << "maxPerStageDescriptorSampledImages:              " << limits.maxPerStageDescriptorSampledImages << endl;
	cout << "maxPerStageDescriptorSamplers:                   " << limits.maxPerStageDescriptorSamplers << endl;
	cout << "maxPerStageDescriptorStorageBuffers:             " << limits.maxPerStageDescriptorStorageBuffers << endl;
	cout << "maxPerStageDescriptorStorageImages:              " << limits.maxPerStageDescriptorStorageImages << endl;
	cout << "maxPerStageDescriptorUniformBuffers:             " << limits.maxPerStageDescriptorUniformBuffers << endl;
	cout << "maxPerStageResources:                            " << limits.maxPerStageResources << endl;
	cout << "maxPushConstantsSize:                            " << limits.maxPushConstantsSize << endl;
	cout << "maxSampleMaskWords:                              " << limits.maxSampleMaskWords << endl;
	cout << "maxSamplerAllocationCount:                       " << limits.maxSamplerAllocationCount << endl;
	cout << "maxSamplerAnisotropy:                            " << limits.maxSamplerAnisotropy << endl;
	cout << "maxSamplerLodBias:                               " << limits.maxSamplerLodBias << endl;
	cout << "maxStorageBufferRange:                           " << limits.maxStorageBufferRange << endl;
	cout << "maxTessellationControlPerPatchOutputComponents:  " << limits.maxTessellationControlPerPatchOutputComponents << endl;
	cout << "maxTessellationControlPerVertexInputComponents:  " << limits.maxTessellationControlPerVertexInputComponents << endl;
	cout << "maxTessellationControlPerVertexOutputComponents: " << limits.maxTessellationControlPerVertexOutputComponents << endl;
	cout << "maxTessellationControlTotalOutputComponents:     " << limits.maxTessellationControlTotalOutputComponents << endl;
	cout << "maxTessellationEvaluationInputComponents:        " << limits.maxTessellationEvaluationInputComponents << endl;
	cout << "maxTessellationEvaluationOutputComponents:       " << limits.maxTessellationEvaluationOutputComponents << endl;
	cout << "maxTessellationGenerationLevel:                  " << limits.maxTessellationGenerationLevel << endl;
	cout << "maxTessellationPatchSize:                        " << limits.maxTessellationPatchSize << endl;
	cout << "maxTexelBufferElements:                          " << limits.maxTexelBufferElements << endl;
	cout << "maxTexelGatherOffset:                            " << limits.maxTexelGatherOffset << endl;
	cout << "maxTexelOffset:                                  " << limits.maxTexelOffset << endl;
	cout << "maxUniformBufferRange:                           " << limits.maxUniformBufferRange << endl;
	cout << "maxVertexInputAttributeOffset:                   " << limits.maxVertexInputAttributeOffset << endl;
	cout << "maxVertexInputAttributes:                        " << limits.maxVertexInputAttributes << endl;
	cout << "maxVertexInputBindings:                          " << limits.maxVertexInputBindings << endl;
	cout << "maxVertexInputBindingStride:                     " << limits.maxVertexInputBindingStride << endl;
	cout << "maxVertexOutputComponents:                       " << limits.maxVertexOutputComponents << endl;
	cout << "maxViewportDimensions                         X: " << limits.maxViewportDimensions[0] << ", Y: " << limits.maxViewportDimensions[1] << endl;
	cout << "maxViewports:                                    " << limits.maxViewports << endl;
	cout << "minInterpolationOffset:                          " << limits.minInterpolationOffset << endl;
	cout << "minMemoryMapAlignment:                           " << limits.minMemoryMapAlignment << endl;
	cout << "minStorageBufferOffsetAlignment:                 " << limits.minStorageBufferOffsetAlignment << endl;
	cout << "minTexelBufferOffsetAlignment:                   " << limits.minTexelBufferOffsetAlignment << endl;
	cout << "minTexelGatherOffset:                            " << limits.minTexelGatherOffset << endl;
	cout << "minTexelOffset:                                  " << limits.minTexelOffset << endl;
	cout << "minUniformBufferOffsetAlignment:                 " << limits.minUniformBufferOffsetAlignment << endl;
	cout << "mipmapPrecisionBits:                             " << limits.mipmapPrecisionBits << endl;
	cout << "nonCoherentAtomSize:                             " << limits.nonCoherentAtomSize << endl;
	cout << "optimalBufferCopyOffsetAlignment:                " << limits.optimalBufferCopyOffsetAlignment << endl;
	cout << "optimalBufferCopyRowPitchAlignment:              " << limits.optimalBufferCopyRowPitchAlignment << endl;
	cout << "pointSizeGranularity:                            " << limits.pointSizeGranularity << endl;
	cout << "pointSizeRange                              Min: " << limits.pointSizeRange[0] << ", Max: " << limits.pointSizeRange[1] << endl;
	cout << "sampledImageColorSampleCounts:                   " << limits.sampledImageColorSampleCounts << endl;
	cout << "sampledImageDepthSampleCounts:                   " << limits.sampledImageDepthSampleCounts << endl;
	cout << "sampledImageIntegerSampleCounts:                 " << limits.sampledImageIntegerSampleCounts << endl;
	cout << "sampledImageStencilSampleCounts:                 " << limits.sampledImageStencilSampleCounts << endl;
	cout << "sparseAddressSpaceSize:                          " << limits.sparseAddressSpaceSize << endl;
	cout << "standardSampleLocations:                         " << limits.standardSampleLocations << endl;
	cout << "storageImageSampleCounts:                        " << limits.storageImageSampleCounts << endl;
	cout << "strictLines:                                     " << limits.strictLines << endl;
	cout << "subPixelInterpolationOffsetBits:                 " << limits.subPixelInterpolationOffsetBits << endl;
	cout << "subPixelPrecisionBits:                           " << limits.subPixelPrecisionBits << endl;
	cout << "subTexelPrecisionBits:                           " << limits.subTexelPrecisionBits << endl;
	cout << "timestampComputeAndGraphics:                     " << limits.timestampComputeAndGraphics << endl;
	cout << "timestampPeriod:                                 " << limits.timestampPeriod << endl;
	cout << "viewportBoundsRange                         Min: " << limits.viewportBoundsRange[0] << ", Max: " << limits.viewportBoundsRange[1] << endl;
	cout << "viewportSubPixelBits:                            " << limits.viewportSubPixelBits << endl;
}

// Method to return a human-readable string of what Vulkan queue flags are set
// See: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkQueueFlagBits.html
string getFriendlyQueueFlags(VkQueueFlags flagMask)
{
	string msg;
	if ((flagMask & VK_QUEUE_GRAPHICS_BIT)         != 0) { msg += "VK_QUEUE_GRAPHICS_BIT ";         }
	if ((flagMask & VK_QUEUE_COMPUTE_BIT)          != 0) { msg += "VK_QUEUE_COMPUTE_BIT ";          }
	if ((flagMask & VK_QUEUE_TRANSFER_BIT)         != 0) { msg += "VK_QUEUE_TRANSFER_BIT ";         }
	if ((flagMask & VK_QUEUE_SPARSE_BINDING_BIT)   != 0) { msg += "VK_QUEUE_SPARSE_BINDING_BIT ";   }
	if ((flagMask & VK_QUEUE_PROTECTED_BIT)        != 0) { msg += "VK_QUEUE_PROTECTED_BIT ";        } // Provided by VK_VERSION_1_1
	if ((flagMask & VK_QUEUE_VIDEO_DECODE_BIT_KHR) != 0) { msg += "VK_QUEUE_VIDEO_DECODE_BIT_KHR "; } // Provided by VK_KHR_video_decode_queue

#ifdef VK_ENABLE_BETA_EXTENSIONS
	if ((flagMask & 0x00000040) != 0) { msg += "VK_QUEUE_VIDEO_ENCODE_BIT_KHR "; } // Provided by VK_KHR_video_encode_queue
#endif

	if ((flagMask & VK_QUEUE_OPTICAL_FLOW_BIT_NV) != 0) { msg += "VK_QUEUE_OPTICAL_FLOW_BIT_NV ";  } // Provided by VK_NV_optical_flow

	return msg;
}

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
			for (unsigned int i = 0; i < instanceExtensionsCount; ++i)
			{
				cout << "\t" << availableExtensions[i].extensionName << " - version: " << availableExtensions[i].specVersion << endl;
			}
		}
	}


	// Make all the extensions available for use
	// Note: If we only wanted to make SOME extensions available then we would add them to the desiredExtensions vector directly then check that they ARE available
	std::vector<char const*> desiredExtensions(instanceExtensionsCount);
	for (unsigned int i = 0; i < instanceExtensionsCount; ++i) { desiredExtensions[i] = availableExtensions[i].extensionName; }

	// Check that all the desired extensions are available. If a desiredExtension was "DoesNotExist" or such then we'll return a failure code.
	for (unsigned int i = 0; i < instanceExtensionsCount; ++i)
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


	// Actually create the Vulkan instance! Note: The vulkan instance itself is an `opaque handle` - we can't get any details from it directly.
	// See: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkInstance.html
	VkInstance vulkanInstance;
	const VkResult instanceCreationResult = VulkanFunctionLoaders::vkCreateInstance(&instanceCreateInfo, nullptr, &vulkanInstance);
	if (instanceCreationResult != VK_SUCCESS || vulkanInstance == VK_NULL_HANDLE)
	{
		cout << "[FAIL] Could not create Vulkan Instance." << endl;
		return -6;
	}
	if (VERBOSE) { cout << "[OK] Vulkan instance created." << endl; }


	// Load our instance-level functions
	const bool instanceLevelFunctionsLoaded = VulkanFunctionLoaders::LoadInstanceLevelFunctions(vulkanInstance);
	if (!instanceLevelFunctionsLoaded)
	{
		cout << "[FAIL]: Vulkan instance level functions could not be loaded." << endl;
		return -7;
	}
	if (VERBOSE) { cout << "[OK] Vulkan instance-level functions loaded." << endl; }


	// Now load the instance-level functions that are provided by our extensions
	const bool instanceLevelExtensionFunctionsLoaded = VulkanFunctionLoaders::LoadInstanceLevelFunctionFromExtension(vulkanInstance, desiredExtensions);
	if (!instanceLevelExtensionFunctionsLoaded)
	{
		cout << "[FAIL]: Vulkan instance level functions could not be loaded from extensions." << endl;
		return -8;
	}
	if (VERBOSE) { cout << "[OK] Vulkan instance-level functions loaded from extensions." << endl; }


	// Enumerate available physical devices (from which we will access LOGICAL devices that will perform our work!)
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
		cout << "[FAIL] Could populate details of physical devices. Physical devices found: " << physicalDeviceCount << endl;
		return -10;
	}
	if (VERBOSE) { cout << "[OK] Populated details of " << physicalDeviceCount << " physical device(s)." << endl; }


	// For now we'll only work with the first physical device found so let's keep an easy reference to it
	if (physicalDeviceCount > 1)
	{
		cout << "[WARNING] Found multiple physical devices - using physical device 0 for now to keep the code manageable." << endl;
	}
	auto activePhysicalDevice = availablePhysicalDevices[0];


	// Enumerate extension properties on available physical devices.
	// Note: Again we do this as a two-step - first we find the number of extensions for a physical device, then we populate details (p40)
	uint32_t physicalDeviceExtensionCount = 0;
	result = VK_SUCCESS;
	result = VulkanFunctionLoaders::vkEnumerateDeviceExtensionProperties(availablePhysicalDevices[0], nullptr, &physicalDeviceExtensionCount, nullptr);
	if (result != VK_SUCCESS || physicalDeviceExtensionCount == 0)
	{
		cout << "[FAIL] Could not enumerate physical device extensions. Physical devices extension count: " << physicalDeviceExtensionCount << endl;
		return -11;
	}
	if (VERBOSE) { cout << "[OK] Found " << physicalDeviceExtensionCount << " extensions for physical device 0." << endl; }

	std::vector<VkExtensionProperties> physicalDeviceExtensionProperties(physicalDeviceExtensionCount);
	result = VK_SUCCESS;
	result = VulkanFunctionLoaders::vkEnumerateDeviceExtensionProperties(availablePhysicalDevices[0], nullptr, &physicalDeviceExtensionCount, physicalDeviceExtensionProperties.data());
	if (result != VK_SUCCESS || physicalDeviceExtensionCount == 0)
	{
		cout << "[FAIL] Could populate physical device 0 extension properties. Physical device 0 extensions found: " << physicalDeviceExtensionCount << endl;
		return -12;
	}
	if (VERBOSE) { cout << "[OK] Populated " << physicalDeviceExtensionCount << " extension properties for physical device 0." << endl; }


	// Get features and properties of physical devices
	VkPhysicalDeviceFeatures activePhysicalDeviceFeatures;
	VkPhysicalDeviceProperties activePhysicalDeviceProperties;
	VulkanFunctionLoaders::vkGetPhysicalDeviceFeatures(activePhysicalDevice, &activePhysicalDeviceFeatures);
	VulkanFunctionLoaders::vkGetPhysicalDeviceProperties(activePhysicalDevice, &activePhysicalDeviceProperties);
	printPhysicalDeviceFeatures(activePhysicalDeviceFeatures);
	printPhysicalDeviceProperties(activePhysicalDeviceProperties);


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
		cout << "Num. queue families found: " << queueFamiliesCount << endl;
		
		for (int i = 0; i < queueFamiliesCount; ++i)
		{
			auto granularity = queueFamilies[i].minImageTransferGranularity;

			cout << "Queue family number: " << i << endl;
			cout << "\tminImageTransferGranularity - Width: " << granularity.width << ", Height: " << granularity.height << ", Depth: " << granularity.depth << endl;
			cout << "\ttimestampValidBits:                  " << queueFamilies[i].timestampValidBits << endl;
			cout << "\tqueueCount:                          " << queueFamilies[i].queueCount << endl;
			cout << "\tqueueFlags:                          " << getFriendlyQueueFlags(queueFamilies[i].queueFlags) << endl;
		}
	}


	// Now that we have details of the available queue families, we need to choose one that has our desired capabilities - which in this
	// case will simply be that we want to draw something.
	bool foundSuitableQueueFamily = false;
	VkQueueFamilyProperties activeQueueFamily;
	uint32_t activeQueueFamilyIndex = 0xffffffff; // Set to max possible value initially to avoid "may be uninitialised" moaning
	VkQueueFlags desiredCapabilities = VK_QUEUE_GRAPHICS_BIT;
	string desiredCapabilitiesString = getFriendlyQueueFlags(desiredCapabilities);
	for (int i = 0; i < queueFamiliesCount; ++i)
	{		
		if ((queueFamilies[i].queueFlags & desiredCapabilities) != 0)
		{
			activeQueueFamily        = queueFamilies[i];
			activeQueueFamilyIndex   = i;
			foundSuitableQueueFamily = true;
			break;
		}
	}
	if (!foundSuitableQueueFamily)
	{
		cout << "Could not find a suitable queue family with desired capabilities: " << desiredCapabilitiesString << endl;
		return -15;
	}
	cout << "Found queue family with desired capabilities: " << desiredCapabilitiesString << endl;
	cout << "Using queue family at index : " << activeQueueFamilyIndex << endl;


}