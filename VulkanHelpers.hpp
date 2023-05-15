#pragma once

#include <iostream>
#include <string>

using std::cout, std::endl, std::string;

#include <vulkan_core.h>

class VulkanHelpers
{
public:

	// Method to return a human-readable error message from a VkResult
	// See: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkResult.html
	static string getFriendlyResultString(VkResult result)
	{
		string msg;
		switch (result)
		{
			// ----- Non-Errors -----
		case VK_SUCCESS:                    msg = "VK_SUCCESS";                    break;
		case VK_NOT_READY:                  msg = "VK_NOT_READY";                  break;
		case VK_TIMEOUT:                    msg = "VK_TIMEOUT";                    break;
		case VK_EVENT_SET:                  msg = "VK_EVENT_SET";                  break;
		case VK_EVENT_RESET:                msg = "VK_EVENT_RESET";                break;
		case VK_INCOMPLETE:                 msg = "VK_INCOMPLETE";                 break;
		case VK_THREAD_IDLE_KHR:            msg = "VK_THREAD_IDLE_KHR";            break;
		case VK_THREAD_DONE_KHR:            msg = "VK_THREAD_DONE_KHR";            break;
		case VK_OPERATION_DEFERRED_KHR:     msg = "VK_OPERATION_DEFERRED_KHR";     break;
		case VK_OPERATION_NOT_DEFERRED_KHR: msg = "VK_OPERATION_NOT_DEFERRED_KHR"; break;
		case VK_RESULT_MAX_ENUM:            msg = "VK_RESULT_MAX_ENUM";            break;

			// ----- Errors -----
		case VK_ERROR_OUT_OF_HOST_MEMORY:                           msg = "VK_ERROR_OUT_OF_HOST_MEMORY";                           break;
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:                         msg = "VK_ERROR_OUT_OF_DEVICE_MEMORY";                         break;
		case VK_ERROR_INITIALIZATION_FAILED:                        msg = "VK_ERROR_INITIALIZATION_FAILED";                        break;
		case VK_ERROR_DEVICE_LOST:                                  msg = "VK_ERROR_DEVICE_LOST";                                  break;
		case VK_ERROR_MEMORY_MAP_FAILED:                            msg = "VK_ERROR_MEMORY_MAP_FAILED";                            break;
		case VK_ERROR_LAYER_NOT_PRESENT:                            msg = "VK_ERROR_LAYER_NOT_PRESENT";                            break;
		case VK_ERROR_EXTENSION_NOT_PRESENT:                        msg = "VK_ERROR_EXTENSION_NOT_PRESENT";                        break;
		case VK_ERROR_FEATURE_NOT_PRESENT:                          msg = "VK_ERROR_FEATURE_NOT_PRESENT";                          break;
		case VK_ERROR_INCOMPATIBLE_DRIVER:                          msg = "VK_ERROR_INCOMPATIBLE_DRIVER";                          break;
		case VK_ERROR_TOO_MANY_OBJECTS:                             msg = "VK_ERROR_TOO_MANY_OBJECTS";                             break;
		case VK_ERROR_FORMAT_NOT_SUPPORTED:                         msg = "VK_ERROR_FORMAT NOT SUPPORTED";                         break;
		case VK_ERROR_FRAGMENTED_POOL:                              msg = "VK_ERROR_FRAGMENTED_POOL";                              break;
		case VK_ERROR_UNKNOWN:                                      msg = "VK_ERROR_UNKNOWN";                                      break;
		case VK_ERROR_SURFACE_LOST_KHR:                             msg = "VK_ERROR_SURFACE_LOST_KHR";                             break;
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:                     msg = "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";                     break;
		case VK_SUBOPTIMAL_KHR:                                     msg = "VK_SUBOPTIMAL_KHR";                                     break;
		case VK_ERROR_OUT_OF_DATE_KHR:                              msg = "VK_ERROR_OUT_OF_DATE_KHR";                              break;
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:                     msg = "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";                     break;
		case VK_ERROR_VALIDATION_FAILED_EXT:                        msg = "VK_ERROR_VALIDATION_FAILED_EXT";                        break;
		case VK_ERROR_INVALID_SHADER_NV:                            msg = "VK_ERROR_INVALID_SHADER_NV";                            break;
		case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:                msg = "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";                break;
		case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:       msg = "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";       break;
		case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:    msg = "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";    break;
		case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:       msg = "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";       break;
		case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:        msg = "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";        break;
		case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:          msg = "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";          break;
		case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: msg = "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT"; break;
		case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:          msg = "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";          break;
		case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:                    msg = "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";                    break;
		case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT:               msg = "VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT";               break;

			// ----- Errors with Duplicate Enums -----
		case VK_ERROR_OUT_OF_POOL_MEMORY:             msg = "VK_ERROR_OUT_OF_POOL_MEMORY or (..._KHR) - same enum value.";                         break;
		case VK_ERROR_INVALID_EXTERNAL_HANDLE:        msg = "VK_ERROR_INVALID_EXTERNAL_HANDLE or (..._KHR) - same enum value.";                    break;
		case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: msg = "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS or (..._KHR) or (...EXT) - same enum value."; break;
		case VK_ERROR_FRAGMENTATION:                  msg = "VK_ERROR_FRAGMENTATION or (...EXT) - same enum value.";                               break;
		case VK_ERROR_NOT_PERMITTED_EXT:              msg = "VK_ERROR_NOT_PERMITTED_EXT or (..._KHR) - same enum value.";                          break;

			// ----- Non-Errors with Duplicate Enums -----
		case VK_PIPELINE_COMPILE_REQUIRED:            msg = "VK_PIPELINE_COMPILE_REQUIRED or (..._EXT) - same enum value."; break;

		default: msg = "Unknown VkResult enum value - code is: " + result;
		}

		return msg;
	}

	// Method to print out Vulkan physical device features.
	// See: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceFeatures.html
	static void printPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures& features)
	{
		cout << "----- Physical Device Features (1 = available, 0 = unavailable) -----" << endl;
		cout << "alphaToOne:                              " << features.alphaToOne << endl;
		cout << "depthBiasClamp:                          " << features.depthBiasClamp << endl;
		cout << "depthBounds:                             " << features.depthBounds << endl;
		cout << "depthClamp:                              " << features.depthClamp << endl;
		cout << "drawIndirectFirstInstance:               " << features.drawIndirectFirstInstance << endl;
		cout << "dualSrcBlend:                            " << features.dualSrcBlend << endl;
		cout << "fillModeNonSolid:                        " << features.fillModeNonSolid << endl;
		cout << "fragmentStoresAndAtomics:                " << features.fragmentStoresAndAtomics << endl;
		cout << "fullDrawIndexUint32:                     " << features.fullDrawIndexUint32 << endl;
		cout << "geometryShader:                          " << features.geometryShader << endl;
		cout << "imageCubeArray:                          " << features.imageCubeArray << endl;
		cout << "independentBlend:                        " << features.independentBlend << endl;
		cout << "inheritedQueries:                        " << features.inheritedQueries << endl;
		cout << "largePoints:                             " << features.largePoints << endl;
		cout << "logicOp:                                 " << features.logicOp << endl;
		cout << "multiDrawIndirect:                       " << features.multiDrawIndirect << endl;
		cout << "multiViewport:                           " << features.multiViewport << endl;
		cout << "occlusionQueryPrecise:                   " << features.occlusionQueryPrecise << endl;
		cout << "pipelineStatisticsQuery:                 " << features.pipelineStatisticsQuery << endl;
		cout << "robustBufferAccess:                      " << features.robustBufferAccess << endl;
		cout << "samplerAnisotropy:                       " << features.samplerAnisotropy << endl;
		cout << "sampleRateShading:                       " << features.sampleRateShading << endl;
		cout << "shaderClipDistance:                      " << features.shaderClipDistance << endl;
		cout << "shaderCullDistance:                      " << features.shaderCullDistance << endl;
		cout << "shaderFloat64:                           " << features.shaderFloat64 << endl;
		cout << "shaderImageGatherExtended:               " << features.shaderImageGatherExtended << endl;
		cout << "shaderInt16:                             " << features.shaderInt16 << endl;
		cout << "shaderInt64:                             " << features.shaderInt64 << endl;
		cout << "shaderResourceMinLod:                    " << features.shaderResourceMinLod << endl;
		cout << "shaderResourceResidency:                 " << features.shaderResourceResidency << endl;
		cout << "shaderSampledImageArrayDynamicIndexing:  " << features.shaderSampledImageArrayDynamicIndexing << endl;
		cout << "shaderStorageBufferArrayDynamicIndexing: " << features.shaderStorageBufferArrayDynamicIndexing << endl;
		cout << "shaderStorageImageExtendedFormats:       " << features.shaderStorageImageExtendedFormats << endl;
		cout << "shaderStorageImageMultisample:           " << features.shaderStorageImageMultisample << endl;
		cout << "shaderStorageImageReadWithoutFormat:     " << features.shaderStorageImageReadWithoutFormat << endl;
		cout << "shaderStorageImageWriteWithoutFormat:    " << features.shaderStorageImageWriteWithoutFormat << endl;
		cout << "shaderTessellationAndGeometryPointSize:  " << features.shaderTessellationAndGeometryPointSize << endl;
		cout << "shaderUniformBufferArrayDynamicIndexing: " << features.shaderUniformBufferArrayDynamicIndexing << endl;
		cout << "sparseBinding:                           " << features.sparseBinding << endl;
		cout << "sparseResidency16Samples:                " << features.sparseResidency16Samples << endl;
		cout << "sparseResidency2Samples:                 " << features.sparseResidency2Samples << endl;
		cout << "sparseResidency4Samples:                 " << features.sparseResidency4Samples << endl;
		cout << "sparseResidency8Samples:                 " << features.sparseResidency8Samples << endl;
		cout << "sparseResidencyAliased:                  " << features.sparseResidencyAliased << endl;
		cout << "sparseResidencyBuffer:                   " << features.sparseResidencyBuffer << endl;
		cout << "sparseResidencyImage2D:                  " << features.sparseResidencyImage2D << endl;
		cout << "sparseResidencyImage3D:                  " << features.sparseResidencyImage3D << endl;
		cout << "tessellationShader:                      " << features.tessellationShader << endl;
		cout << "textureCompressionASTC_LDR:              " << features.textureCompressionASTC_LDR << endl;
		cout << "textureCompressionBC:                    " << features.textureCompressionBC << endl;
		cout << "textureCompressionETC2:                  " << features.textureCompressionETC2 << endl;
		cout << "variableMultisampleRate:                 " << features.variableMultisampleRate << endl;
		cout << "vertexPipelineStoresAndAtomics:          " << features.vertexPipelineStoresAndAtomics << endl;
		cout << "wideLines:                               " << features.wideLines << endl;
	}

	// Method to print out Vulkan physical device features.
	// See : https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceProperties.html
	// Also: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceType.html
	// Also: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceSparseProperties.html
	// Also: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceLimits.html
	static void printPhysicalDeviceProperties(const VkPhysicalDeviceProperties& properties)
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
		cout << "apiVersion:        " << properties.apiVersion << endl;
		cout << "driverVersion:     " << properties.driverVersion << endl;
		cout << "vendorID:          " << properties.vendorID << endl;
		cout << "deviceID:          " << properties.deviceID << endl;
		cout << "deviceType:        " << deviceTypeString << endl;
		cout << "deviceName:        " << properties.deviceName << endl;
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
		cout << "bufferImageGranularity:                          " << limits.bufferImageGranularity << endl;
		cout << "discreteQueuePriorities:                         " << limits.discreteQueuePriorities << endl;
		cout << "framebufferColorSampleCounts:                    " << limits.framebufferColorSampleCounts << endl;
		cout << "framebufferDepthSampleCounts:                    " << limits.framebufferDepthSampleCounts << endl;
		cout << "framebufferNoAttachmentsSampleCounts:            " << limits.framebufferNoAttachmentsSampleCounts << endl;
		cout << "framebufferStencilSampleCounts:                  " << limits.framebufferStencilSampleCounts << endl;
		cout << "lineWidthGranularity:                            " << limits.lineWidthGranularity << endl;
		cout << "lineWidthRange - Min:                            " << limits.lineWidthRange[0] << ", Max: " << limits.lineWidthRange[1] << endl;
		cout << "maxBoundDescriptorSets:                          " << limits.maxBoundDescriptorSets << endl;
		cout << "maxClipDistances:                                " << limits.maxClipDistances << endl;
		cout << "maxColorAttachments:                             " << limits.maxColorAttachments << endl;
		cout << "maxCombinedClipAndCullDistances:                 " << limits.maxCombinedClipAndCullDistances << endl;
		cout << "maxComputeSharedMemorySize:                      " << limits.maxComputeSharedMemorySize << endl;
		cout << "maxComputeWorkGroupCount                      X: " << limits.maxComputeWorkGroupCount[0] << ", Y: " << limits.maxComputeWorkGroupCount[1] << ", Z: " << limits.maxComputeWorkGroupCount[2] << endl;
		cout << "maxComputeWorkGroupInvocations:                  " << limits.maxComputeWorkGroupInvocations << endl;
		cout << "maxComputeWorkGroupSize                       X: " << limits.maxComputeWorkGroupSize[0] << ", Y: " << limits.maxComputeWorkGroupSize[1] << ", Z: " << limits.maxComputeWorkGroupSize[2] << endl;
		cout << "maxCullDistances:                                " << limits.maxCullDistances << endl;
		cout << "maxDescriptorSetInputAttachments:                " << limits.maxDescriptorSetInputAttachments << endl;
		cout << "maxDescriptorSetSampledImages:                   " << limits.maxDescriptorSetSampledImages << endl;
		cout << "maxDescriptorSetSamplers:                        " << limits.maxDescriptorSetSamplers << endl;
		cout << "maxDescriptorSetStorageBuffers:                  " << limits.maxDescriptorSetStorageBuffers << endl;
		cout << "maxDescriptorSetStorageBuffersDynamic:           " << limits.maxDescriptorSetStorageBuffersDynamic << endl;
		cout << "maxDescriptorSetStorageImages:                   " << limits.maxDescriptorSetStorageImages << endl;
		cout << "maxDescriptorSetUniformBuffers:                  " << limits.maxDescriptorSetUniformBuffers << endl;
		cout << "maxDescriptorSetUniformBuffersDynamic:           " << limits.maxDescriptorSetUniformBuffersDynamic << endl;
		cout << "maxDrawIndexedIndexValue:                        " << limits.maxDrawIndexedIndexValue << endl;
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

	// Method to return a human-readable string of what Vulkan queue flags are set in a given VkQueueFlags mask.
	// See: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkQueueFlagBits.html
	static string getFriendlyQueueFlags(VkQueueFlags flagMask)
	{
		std::string msg;
		if ((flagMask & VK_QUEUE_GRAPHICS_BIT) != 0) { msg += "VK_QUEUE_GRAPHICS_BIT "; }
		if ((flagMask & VK_QUEUE_COMPUTE_BIT) != 0) { msg += "VK_QUEUE_COMPUTE_BIT "; }
		if ((flagMask & VK_QUEUE_TRANSFER_BIT) != 0) { msg += "VK_QUEUE_TRANSFER_BIT "; }
		if ((flagMask & VK_QUEUE_SPARSE_BINDING_BIT) != 0) { msg += "VK_QUEUE_SPARSE_BINDING_BIT "; }
		if ((flagMask & VK_QUEUE_PROTECTED_BIT) != 0) { msg += "VK_QUEUE_PROTECTED_BIT "; } // Provided by VK_VERSION_1_1
		if ((flagMask & VK_QUEUE_VIDEO_DECODE_BIT_KHR) != 0) { msg += "VK_QUEUE_VIDEO_DECODE_BIT_KHR "; } // Provided by VK_KHR_video_decode_queue

#ifdef VK_ENABLE_BETA_EXTENSIONS
		if ((flagMask & 0x00000040) != 0) { msg += "VK_QUEUE_VIDEO_ENCODE_BIT_KHR "; } // Provided by VK_KHR_video_encode_queue
#endif

		if ((flagMask & VK_QUEUE_OPTICAL_FLOW_BIT_NV) != 0) { msg += "VK_QUEUE_OPTICAL_FLOW_BIT_NV "; } // Provided by VK_NV_optical_flow

		return msg;
	}
	
};
