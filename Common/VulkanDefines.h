#pragma once


#define VK_INSTANCE_FUN_LOAD_WITH_DEBUG_CHECK_RETURN(vk_fun, debug_str) \
    if (!(vk_fun = (PFN_##vk_fun)vkGetInstanceProcAddr(VulkanDescriptor.Instance, #vk_fun)))   \
    {   \
        std::cout << debug_str << #vk_fun << "!" << std::endl;   \
        return false;   \
    }   \

#define VK_INSTANCE_FUN_LOAD_WITH_CHECK_RETURN(vk_fun) \
    if (!(vk_fun = (PFN_##vk_fun)vkGetInstanceProcAddr(VulkanDescriptor.Instance, #vk_fun)))   \
    {   \
        return false;   \
    }   \

// Device
#define VK_DEVICE_FUN_LOAD_WITH_DEBUG_CHECK_RETURN(vk_fun, debug_str) \
    if (!(vk_fun = (PFN_##vk_fun)vkGetDeviceProcAddr(VulkanDescriptor.Device, #vk_fun)))   \
    {   \
        std::cout << debug_str << #vk_fun << "!" << std::endl;   \
        return false;   \
    }   \

#define VK_DEVICE_FUN_LOAD_WITH_CHECK_RETURN(vk_fun) \
    if (!(vk_fun = (PFN_##vk_fun)vkGetDeviceProcAddr(VulkanDescriptor.Device, #vk_fun)))   \
    {   \
        return false;   \
    }   \

#ifdef _DEBUG
    #define VK_LOAD_GLOBAL_LEVEL_FUN(vk_fun) VK_INSTANCE_FUN_LOAD_WITH_DEBUG_CHECK_RETURN(vk_fun, "Could not load global level function: ")
#else
    #define VK_LOAD_GLOBAL_LEVEL_FUN(vk_fun) VK_INSTANCE_FUN_LOAD_WITH_CHECK_RETURN(vk_fun)
#endif

#ifdef _DEBUG
    #define VK_LOAD_INSTANCE_LEVEL_FUN(vk_fun) VK_INSTANCE_FUN_LOAD_WITH_DEBUG_CHECK_RETURN(vk_fun, "Could not load instance level function: ")
#else
    #define VK_LOAD_INSTANCE_LEVEL_FUN(vk_fun) VK_INSTANCE_FUN_LOAD_WITH_CHECK_RETURN(vk_fun)
#endif

#ifdef _DEBUG
    #define VK_LOAD_DEVICE_LEVEL_FUN(vk_fun) VK_DEVICE_FUN_LOAD_WITH_DEBUG_CHECK_RETURN(vk_fun,  "Could not load device level function: ")
#else
    #define VK_LOAD_DEVICE_LEVEL_FUN(vk_fun) VK_DEVICE_FUN_LOAD_WITH_CHECK_RETURN(vk_fun)
#endif