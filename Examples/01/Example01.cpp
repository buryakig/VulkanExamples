#include <vector>
#include "Example01.h"
#include "VulkanFunctions.h"
#include "VulkanDefines.h"

namespace VulkanExamples 
{

  Example01::Example01() 
      : VulkanLibrary()
      , VulkanDescriptor()
  {
  }

  bool Example01::OnWindowResized()
  {
#ifdef _DEBUG
      std::cout << "Window resized." << std::endl;
#endif
      return true;
  }

  bool Example01::Draw()
  {
#ifdef _DEBUG
      //std::cout << "Draw function called." << std::endl;
#endif
      return true;
  }

  inline void Example01::ShutDown()
  {
#ifdef _DEBUG
      std::cout << "ShutDown function called." << std::endl;
#endif

      isRunning = false;
  }

  inline bool Example01::Running()
  {
      return isRunning;
  }

  bool Example01::PrepareVulkan() {
    if( !LoadVulkanLibrary() ) {
      return false;
    }
    if( !LoadExportedFunctions() ) {
      return false;
    }
    if( !LoadGlobalLevelFunctions() ) {
      return false;
    }
    if( !CreateInstance() ) {
      return false;
    }
    if( !LoadInstanceLevelFunctions() ) {
      return false;
    }
    if( !CreateDevice() ) {
      return false;
    }
    if( !LoadDeviceLevelFunctions() ) {
      return false;
    }
    if( !GetDeviceQueue() ) {
      return false;
    }
    return true;
  }

  bool Example01::LoadVulkanLibrary() 
  {
  #if defined(VK_USE_PLATFORM_WIN32_KHR)
    VulkanLibrary = LoadLibrary( "vulkan-1.dll" );
  #endif
    if (VulkanLibrary == nullptr)
    {
        std::cout << "Could not load Vulkan library!" << std::endl;
        return false;
    }
    return true;
  }

  bool Example01::LoadExportedFunctions() 
  {
  #if defined(VK_USE_PLATFORM_WIN32_KHR)
    #define LoadProcAddress GetProcAddress
  #endif
                                                
    if( !(vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)LoadProcAddress( VulkanLibrary, "vkGetInstanceProcAddr")))
    {
      std::cout << "Could not load exported function: " << "vkGetInstanceProcAddr" << "!" << std::endl;
      return false;                                                                   
    }

    return true;
  }

  bool Example01::LoadGlobalLevelFunctions() 
  {

    VK_LOAD_GLOBAL_LEVEL_FUN(vkCreateInstance)
    VK_LOAD_GLOBAL_LEVEL_FUN(vkEnumerateInstanceExtensionProperties)
    VK_LOAD_GLOBAL_LEVEL_FUN(vkEnumerateInstanceLayerProperties)

          return true;
  }

  bool Example01::CreateInstance() 
  {
      VkApplicationInfo  application_info = {
          VK_STRUCTURE_TYPE_APPLICATION_INFO,
          nullptr,
          "Vulkan Examples",
          VK_MAKE_VERSION(1, 0, 0),
          "Vulkan Examples Engine",
          VK_MAKE_VERSION(1, 0, 0),
          VK_MAKE_VERSION(1, 0, 0)
      };

      VkInstanceCreateInfo instance_create_info = {
          VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
          nullptr,
          0,
          &application_info,
          0,
          nullptr
      };

      if (vkCreateInstance(&instance_create_info, nullptr, &VulkanDescriptor.Instance) != VK_SUCCESS)
      {
          std::cout << "Could not create Vulkan instance!" << std::endl;
          return false;
      }
  
      return true;
  }

  bool Example01::LoadInstanceLevelFunctions() 
  {        
    // Default instance-level functios
    VK_LOAD_INSTANCE_LEVEL_FUN(vkEnumeratePhysicalDevices)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkGetPhysicalDeviceProperties)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkGetPhysicalDeviceFeatures)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkGetPhysicalDeviceQueueFamilyProperties)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkCreateDevice)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkGetDeviceProcAddr)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkDestroyInstance)

    return true;
  }

  bool Example01::CreateDevice() 
  {
    uint32_t num_devices = 0;
    if (vkEnumeratePhysicalDevices(VulkanDescriptor.Instance, &num_devices, nullptr) != VK_SUCCESS)
    {
        std::cout << "Error occurred during physical devices enumeration!" << std::endl;
        return false;
    }

    if (num_devices == 0)
    {
        std::cout << "No physical devicces were found!" << std::endl;
        return false;
    }

    std::vector<VkPhysicalDevice> vk_physical_devices(num_devices);
    if (vkEnumeratePhysicalDevices(VulkanDescriptor.Instance, &num_devices, &vk_physical_devices[0]) != VK_SUCCESS)
    {
        std::cout << "Error occurred during physical devices extraction!" << std::endl;
        return false;
    }

    VkPhysicalDevice selected_physical_device = VK_NULL_HANDLE;
    uint32_t selected_queue_family_index = UINT32_MAX;
    for( uint32_t i = 0; i < num_devices; ++i ) 
    {
      if( CheckPhysicalDeviceProperties(vk_physical_devices[i], selected_queue_family_index ) ) 
      {
        selected_physical_device = vk_physical_devices[i];
        break;
      }
    }

    if (selected_physical_device == VK_NULL_HANDLE)
    {
        std::cout << "Could not select physical device based on the chosen properties!" << std::endl;
    }

    if (selected_queue_family_index == UINT32_MAX)
    {
        std::cout << "Could not select queue family based on the chosen properties!" << std::endl;
    }
    
    std::vector<float> queue_priorities = {1.0f};

    VkDeviceQueueCreateInfo device_queue_create_info = {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        nullptr,
        0,
        selected_queue_family_index,
        static_cast<uint32_t>(queue_priorities.size()),
        &queue_priorities[0]
    };

    VkDeviceCreateInfo device_create_info = {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        nullptr,
        0,
        1,
        &device_queue_create_info,
        0,
        nullptr,
        0,
        nullptr,
        nullptr
    };

    if (vkCreateDevice(selected_physical_device, &device_create_info, nullptr, &VulkanDescriptor.Device) != VK_SUCCESS)
    {
        std::cout << "Could not create Vulkan device!" << std::endl;
        return false;
    }

    VulkanDescriptor.QueueFamilyIndex = selected_queue_family_index;
    return true;
  }

  bool Example01::CheckPhysicalDeviceProperties(VkPhysicalDevice physical_device, uint32_t &queue_family_index)
  {
      VkPhysicalDeviceProperties device_properties;
      VkPhysicalDeviceFeatures device_features;

      vkGetPhysicalDeviceProperties(physical_device, &device_properties);
      vkGetPhysicalDeviceFeatures(physical_device, &device_features);

      uint32_t major_version = VK_VERSION_MAJOR(device_properties.apiVersion);
      uint32_t minor_version = VK_VERSION_MINOR(device_properties.apiVersion);
      uint32_t patch_version = VK_VERSION_PATCH(device_properties.apiVersion);

      if ((major_version < 1) && (device_properties.limits.maxImageDimension2D < 4096))
      {
          std::cout << "Physical device" << physical_device << "doesn't support required parameters!" << std::endl;
          return false;
      }

      uint32_t queue_families_count = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, nullptr);
      if (queue_families_count == 0)
      {
          std::cout << "Physical device" << physical_device << "doesn't have any queue families!" << std::endl;
          return false;
      }

      std::vector<VkQueueFamilyProperties> queue_family_properties(queue_families_count);
      vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, &queue_family_properties[0]);
      for (uint32_t i = 0; i < queue_families_count; ++i)
      {
          if ((queue_family_properties[i].queueCount > 0) &&
              (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
          {
              queue_family_index = i;
              return true;
          }
      }

      std::cout << "Cpuld not find queue family with required properties on physical device " << physical_device << "!" << std::endl;

      return false;
  }

  bool Example01::LoadDeviceLevelFunctions()
  {
    VK_LOAD_DEVICE_LEVEL_FUN(vkGetDeviceQueue)
    VK_LOAD_DEVICE_LEVEL_FUN(vkDestroyDevice)
    VK_LOAD_DEVICE_LEVEL_FUN(vkDeviceWaitIdle)

    return true;
  }

  bool Example01::GetDeviceQueue()
  {
      vkGetDeviceQueue(VulkanDescriptor.Device, VulkanDescriptor.QueueFamilyIndex, 0, &VulkanDescriptor.Queue);
      return true;
  }

  Example01::~Example01() 
  {
      if (VulkanDescriptor.Device != VK_NULL_HANDLE)
      {
          vkDeviceWaitIdle(VulkanDescriptor.Device);
          vkDestroyDevice(VulkanDescriptor.Device, nullptr);
      }

      if (VulkanDescriptor.Instance != VK_NULL_HANDLE)
      {
          vkDestroyInstance(VulkanDescriptor.Instance, nullptr);
      }

      if (VulkanLibrary)
      {
          #if defined(VK_USE_PLATFORM_WIN32_KHR)
            FreeLibrary(VulkanLibrary);
          #endif
      }

  }

} // namespace VulkanExamples