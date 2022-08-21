#pragma once

#include "vulkan_core.h"

#ifdef VK_USE_PLATFORM_WIN32_KHR 
	#include <Windows.h>
	#include "vulkan_win32.h"
#endif

#define VK_FUN_DECL(vk_fun) extern PFN_##vk_fun vk_fun;

namespace VulkanExamples 
{
// Exported functions                                           
	VK_FUN_DECL(vkGetInstanceProcAddr)

// Global level functions                                       
	VK_FUN_DECL(vkCreateInstance)
	VK_FUN_DECL(vkEnumerateInstanceExtensionProperties)
	VK_FUN_DECL(vkEnumerateInstanceLayerProperties)

// Instance level functions 
	// Swapchain extension
	VK_FUN_DECL(vkDestroySurfaceKHR)
	VK_FUN_DECL(vkGetPhysicalDeviceSurfaceSupportKHR)
	VK_FUN_DECL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
	VK_FUN_DECL(vkGetPhysicalDeviceSurfaceFormatsKHR)
	VK_FUN_DECL(vkGetPhysicalDeviceSurfacePresentModesKHR)
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		VK_FUN_DECL(vkCreateWin32SurfaceKHR)
#endif
	
	// Default
	VK_FUN_DECL(vkEnumerateDeviceExtensionProperties)
	VK_FUN_DECL(vkEnumeratePhysicalDevices)
	VK_FUN_DECL(vkGetPhysicalDeviceProperties)
	VK_FUN_DECL(vkGetPhysicalDeviceFeatures)
	VK_FUN_DECL(vkGetPhysicalDeviceQueueFamilyProperties)
	VK_FUN_DECL(vkCreateDevice)
	VK_FUN_DECL(vkGetDeviceProcAddr)
	VK_FUN_DECL(vkDestroyInstance)

// Device level functions                                       
	VK_FUN_DECL(vkGetDeviceQueue)
	VK_FUN_DECL(vkDeviceWaitIdle)
	VK_FUN_DECL(vkDestroyDevice)

} 