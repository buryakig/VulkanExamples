#include "vulkan_core.h"

namespace VulkanExamples 
{

	// Exported functions                                           
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

	// Global level functions                                       
	PFN_vkCreateInstance vkCreateInstance;
	PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
	PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;

	// Instance level functions                                     
	PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
	PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
	PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
	PFN_vkCreateDevice vkCreateDevice;
	PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	PFN_vkDestroyInstance vkDestroyInstance;

	// Device level functions                                       
	PFN_vkGetDeviceQueue vkGetDeviceQueue;
	PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
	PFN_vkDestroyDevice vkDestroyDevice;



} // namespace VulkanExamples