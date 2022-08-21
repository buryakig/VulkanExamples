#pragma once

#include "vulkan_core.h"

namespace VulkanExamples 
{
	// Exported functions                                           
	extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

	// Global level functions                                       
	extern PFN_vkCreateInstance vkCreateInstance;
	extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
	extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;

	// Instance level functions                                     
	extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
	extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
	extern PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
	extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
	extern PFN_vkCreateDevice vkCreateDevice;
	extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	extern PFN_vkDestroyInstance vkDestroyInstance;

	// Device level functions                                       
	extern PFN_vkGetDeviceQueue vkGetDeviceQueue;
	extern PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
	extern PFN_vkDestroyDevice vkDestroyDevice;

} 