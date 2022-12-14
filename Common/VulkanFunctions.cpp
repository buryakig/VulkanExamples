#include "vulkan_core.h"

#ifdef VK_USE_PLATFORM_WIN32_KHR 
	#include <Windows.h>
	#include "vulkan_win32.h"
#endif

#define VK_FUN_DEF(vk_fun) PFN_##vk_fun vk_fun;

namespace VulkanExamples
{
// Exported functions                                           
	VK_FUN_DEF(vkGetInstanceProcAddr)

// Global level functions                                       
	VK_FUN_DEF(vkCreateInstance)
	VK_FUN_DEF(vkEnumerateInstanceExtensionProperties)
	VK_FUN_DEF(vkEnumerateInstanceLayerProperties)

// Instance level functions 
	// Swapchain extension
	VK_FUN_DEF(vkDestroySurfaceKHR)
	VK_FUN_DEF(vkGetPhysicalDeviceSurfaceSupportKHR)
	VK_FUN_DEF(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
	VK_FUN_DEF(vkGetPhysicalDeviceSurfaceFormatsKHR)
	VK_FUN_DEF(vkGetPhysicalDeviceSurfacePresentModesKHR)
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		VK_FUN_DEF(vkCreateWin32SurfaceKHR)
#endif

	// Default
	VK_FUN_DEF(vkEnumerateDeviceExtensionProperties)
	VK_FUN_DEF(vkEnumeratePhysicalDevices)
	VK_FUN_DEF(vkGetPhysicalDeviceProperties)
	VK_FUN_DEF(vkGetPhysicalDeviceFeatures)
	VK_FUN_DEF(vkGetPhysicalDeviceQueueFamilyProperties)
	VK_FUN_DEF(vkCreateDevice)
	VK_FUN_DEF(vkGetDeviceProcAddr)
	VK_FUN_DEF(vkDestroyInstance)

// Device level functions                                       
	VK_FUN_DEF(vkGetDeviceQueue)
	VK_FUN_DEF(vkDeviceWaitIdle)
	VK_FUN_DEF(vkDestroyDevice)
	VK_FUN_DEF(vkCreateSemaphore)
	VK_FUN_DEF(vkCreateSwapchainKHR)
	VK_FUN_DEF(vkDestroySwapchainKHR)
	VK_FUN_DEF(vkGetSwapchainImagesKHR)
	VK_FUN_DEF(vkAcquireNextImageKHR)
	VK_FUN_DEF(vkQueuePresentKHR)
	VK_FUN_DEF(vkQueueSubmit)
	VK_FUN_DEF(vkCreateCommandPool)
	VK_FUN_DEF(vkAllocateCommandBuffers)
	VK_FUN_DEF(vkBeginCommandBuffer)
	VK_FUN_DEF(vkCmdPipelineBarrier)
	VK_FUN_DEF(vkCmdClearColorImage)
	VK_FUN_DEF(vkEndCommandBuffer)
	VK_FUN_DEF(vkFreeCommandBuffers)
	VK_FUN_DEF(vkDestroyCommandPool)
	VK_FUN_DEF(vkDestroySemaphore)



} // namespace VulkanExamples