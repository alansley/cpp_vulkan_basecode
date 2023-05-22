#ifndef EXPORTED_VULKAN_FUNCTION 
#define EXPORTED_VULKAN_FUNCTION( function ) 
#endif 

// Note: This instance process address is the gateway through which we load all other Vulkan functions
EXPORTED_VULKAN_FUNCTION( vkGetInstanceProcAddr )

#undef EXPORTED_VULKAN_FUNCTION

// ------------------------------

#ifndef GLOBAL_LEVEL_VULKAN_FUNCTION
#define GLOBAL_LEVEL_VULKAN_FUNCTION(function)
#endif

// Note: These 3 are the only global level Vulkan functions that exist!
GLOBAL_LEVEL_VULKAN_FUNCTION( vkEnumerateInstanceExtensionProperties )
GLOBAL_LEVEL_VULKAN_FUNCTION( vkEnumerateInstanceLayerProperties )
GLOBAL_LEVEL_VULKAN_FUNCTION( vkCreateInstance )

#undef GLOBAL_LEVEL_VULKAN_FUNCTION

// ------------------------------

#ifndef INSTANCE_LEVEL_VULKAN_FUNCTION
#define INSTANCE_LEVEL_VULKAN_FUNCTION(function)
#endif

// Note: Instance-level functions are mainly used for operations on physical devices (e.g., checking their
// properties & abilities, and creating logical devices on which we can actually draw!).
INSTANCE_LEVEL_VULKAN_FUNCTION( vkEnumeratePhysicalDevices )
INSTANCE_LEVEL_VULKAN_FUNCTION( vkEnumerateDeviceExtensionProperties )
INSTANCE_LEVEL_VULKAN_FUNCTION( vkGetPhysicalDeviceProperties )
INSTANCE_LEVEL_VULKAN_FUNCTION( vkGetPhysicalDeviceQueueFamilyProperties )
INSTANCE_LEVEL_VULKAN_FUNCTION( vkGetPhysicalDeviceFeatures )
INSTANCE_LEVEL_VULKAN_FUNCTION( vkCreateDevice )
INSTANCE_LEVEL_VULKAN_FUNCTION( vkGetDeviceProcAddr )

#undef INSTANCE_LEVEL_VULKAN_FUNCTION

// ------------------------------

#ifndef INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(function, extension)
#endif

INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( vkGetPhysicalDeviceSurfaceSupportKHR,      VK_KHR_SURFACE_EXTENSION_NAME)
INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( vkGetPhysicalDeviceSurfaceCapabilitiesKHR, VK_KHR_SURFACE_EXTENSION_NAME)
INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( vkGetPhysicalDeviceSurfaceFormatsKHR,      VK_KHR_SURFACE_EXTENSION_NAME)
INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( vkGetPhysicalDeviceSurfacePresentModesKHR, VK_KHR_SURFACE_EXTENSION_NAME)
INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( vkDestroySurfaceKHR,                       VK_KHR_SURFACE_EXTENSION_NAME)

// Put this somewhere: Logical devices represent physical devices for which a set of features and extensions are enabled

// Note: These platform specific instance functions from extensions are for Windows (1st) and Linux (2nd and 3rd).
// XCB is the most recent of the Linux pair, as XLIB is basically deprecated. There's a some good reading at the
// following reddit post about it (latest comments circa 2022):
// https://www.reddit.com/r/vulkan/comments/3lxt1y/vulkan_on_linux_xcb_vs_xlib/
//
// Further... It's possible that I don't need these if I don't need to load these if I don't define "VK_NO_PROTOTYPES" as they're 'part of the core'.
// See: https://stackoverflow.com/a/43901600
#ifdef VK_USE_PLATFORM_WIN32_KHR
	//INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( vkCreateWin32SurfaceKHR, VK_KHR_WIN32_SURFACE_EXTENSION_NAME)
#elif defined VK_USE_PLATFORM_XCB_KHR
	INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(vkCreateXcbSurfaceKHR, VK_KHR_XLIB_SURFACE_EXTENSION_NAME)
#elif defined VK_USE_PLATFORM_XLIB_KHR
	INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(vkCreateXlibSurfaceKHR, VK_KHR_XCB_SURFACE_EXTENSION_NAME)
#endif

#undef INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION

// ------------------------------

#ifndef DEVICE_LEVEL_VULKAN_FUNCTION
#define DEVICE_LEVEL_VULKAN_FUNCTION(function)
#endif

// Note: It is DEVICE LEVEL FUNCTIONS that perform almost all the typical work done in 3D rendering.
// Also: To know if a function is instance-level or device-level, keep in mind that all device-level
// functions have their first argument of type VkDevice, VkQueue, or VKCommandBuffer.
// Also: To avoid the step of `vkGetInstanceProcAdder` redirecting us to the correct function we can
// use `vkGetDeviceProcAddr` to get each function on each device directly - but this does mean we have
// to obtain function pointers for each device created in an applications (and as such need a separate
// list for each logical device - as we can't use functions acquired from one device to perform
// operations on another device). See: Vulkan Cookbook, p56-57.
DEVICE_LEVEL_VULKAN_FUNCTION(vkGetDeviceQueue)
DEVICE_LEVEL_VULKAN_FUNCTION(vkDeviceWaitIdle)
DEVICE_LEVEL_VULKAN_FUNCTION(vkDestroyDevice)
DEVICE_LEVEL_VULKAN_FUNCTION(vkCreateBuffer)
DEVICE_LEVEL_VULKAN_FUNCTION(vkGetBufferMemoryRequirements)

#undef DEVICE_LEVEL_VULKAN_FUNCTION

// ------------------------------

#ifndef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(function, extension)
#endif

DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(vkCreateSwapchainKHR,    VK_KHR_SWAPCHAIN_EXTENSION_NAME)
DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(vkGetSwapchainImagesKHR, VK_KHR_SWAPCHAIN_EXTENSION_NAME)
DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(vkAcquireNextImageKHR,   VK_KHR_SWAPCHAIN_EXTENSION_NAME)
DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(vkQueuePresentKHR,       VK_KHR_SWAPCHAIN_EXTENSION_NAME)
DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(vkDestroySwapchainKHR,   VK_KHR_SWAPCHAIN_EXTENSION_NAME)

#undef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION