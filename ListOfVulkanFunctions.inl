#ifndef EXPORTED_VULKAN_FUNCTION 
#define EXPORTED_VULKAN_FUNCTION( function ) 
#endif 

// Note: This instance process address is the gateway through which we load all other Vulkan functions
EXPORTED_VULKAN_FUNCTION( vkGetInstanceProcAddr )

#undef EXPORTED_VULKAN_FUNCTION
//
#ifndef GLOBAL_LEVEL_VULKAN_FUNCTION
#define GLOBAL_LEVEL_VULKAN_FUNCTION(function)
#endif

// Note: These 3 are the only global level Vulkan functions that exist!
GLOBAL_LEVEL_VULKAN_FUNCTION( vkEnumerateInstanceExtensionProperties )
GLOBAL_LEVEL_VULKAN_FUNCTION( vkEnumerateInstanceLayerProperties )
GLOBAL_LEVEL_VULKAN_FUNCTION( vkCreateInstance )

#undef GLOBAL_LEVEL_VULKAN_FUNCTION
//
#ifndef INSTANCE_LEVEL_VULKAN_FUNCTION
#define INSTANCE_LEVEL_VULKAN_FUNCTION(function)
#endif
#undef INSTANCE_LEVEL_VULKAN_FUNCTION
//
#ifndef INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(function, extension)
#endif
#undef INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
//
#ifndef DEVICE_LEVEL_VULKAN_FUNCTION
#define DEVICE_LEVEL_VULKAN_FUNCTION(function)
#endif
#undef DEVICE_LEVEL_VULKAN_FUNCTION
//
#ifndef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(function, extension)
#endif
#undef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION