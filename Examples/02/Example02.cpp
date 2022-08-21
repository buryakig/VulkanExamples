
#include "Example02.h"
#include "VulkanFunctions.h"
#include "VulkanDefines.h"


namespace VulkanExamples 
{

  Example02::Example02() 
      : VulkanLibrary()
      , VulkanDescriptor()
  {
  }

  bool Example02::OnWindowResized()
  {
#ifdef _DEBUG
      std::cout << "Window resized." << std::endl;
#endif
      return true;
  }

  bool Example02::Draw()
  {
#ifdef _DEBUG
      //std::cout << "Draw function called." << std::endl;
#endif
      return true;
  }

  inline void Example02::ShutDown()
  {
#ifdef _DEBUG
      std::cout << "ShutDown function called." << std::endl;
#endif

      isRunning = false;
  }

  inline bool Example02::Running()
  {
      return isRunning;
  }

  bool Example02::PrepareVulkan(const OS::WindowParameters& window_desc)
  {
    WindowDescriptor = window_desc;

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
    if (!CreatePresentationSurface()) {
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

  bool Example02::LoadVulkanLibrary() 
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

  bool Example02::LoadExportedFunctions() 
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

  bool Example02::LoadGlobalLevelFunctions()
  {
      
    VK_LOAD_GLOBAL_LEVEL_FUN(vkCreateInstance)
    VK_LOAD_GLOBAL_LEVEL_FUN(vkEnumerateInstanceExtensionProperties)
    VK_LOAD_GLOBAL_LEVEL_FUN(vkEnumerateInstanceLayerProperties)

    return true;
  }

  bool Example02::CreateInstance() 
  {
      uint32_t extensions_count = 0;
      if (vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr) != VK_SUCCESS || extensions_count == 0)
      {
#ifdef _DEBUG
          std::cout << "Instance extensions enumeration failed!" << std::endl;
#endif
          return false;
      }

      std::vector<VkExtensionProperties> available_extensions(extensions_count);
      if ( vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, available_extensions.data()) != VK_SUCCESS)
      {
#ifdef _DEBUG
          std::cout << "Error occurred during instance extensions enumeration!" << std::endl;
#endif
          return false;
      }

#ifdef _DEBUG
      std::cout << "Found " << available_extensions.size() << " avaliable extensions." << std::endl;
#endif

      std::vector<const char*> required_extentions =
      {
          VK_KHR_SURFACE_EXTENSION_NAME,
    #if defined(VK_USE_PLATFORM_WIN32_KHR)
          VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    #endif
      };

      for (size_t i = 0; i < required_extentions.size(); ++i)
      {
          if (!CheckExtensionAvailability(required_extentions[i], available_extensions))
          {
#ifdef _DEBUG
              std::cout << "Could not find instance extension named \"" << required_extentions[i] << "\"!" << std::endl;
#endif
              return false;
          }
      }

      // Instance-level extensions are required during instance creation - we create an instance with a list of extensions that should be enabled

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
          nullptr,
          static_cast<uint32_t>(required_extentions.size()),
          &required_extentions[0]
      };

      if (vkCreateInstance(&instance_create_info, nullptr, &VulkanDescriptor.Instance) != VK_SUCCESS)
      {
#ifdef _DEBUG
          std::cout << "Could not create Vulkan instance!" << std::endl;
#endif
          return false;
      }
  
      return true;
  }

  bool Example02::CheckExtensionAvailability(const char* extension_name, const std::vector<VkExtensionProperties>& available_extensions)
  {
      for (size_t i = 0; i < available_extensions.size(); ++i) {
          if (strcmp(available_extensions[i].extensionName, extension_name) == 0) {
              return true;
          }
      }
      return false;
  }

  bool Example02::LoadInstanceLevelFunctions() 
  {    
    // Swapchain extension functions
    VK_LOAD_INSTANCE_LEVEL_FUN(vkDestroySurfaceKHR)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkGetPhysicalDeviceSurfaceSupportKHR)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkGetPhysicalDeviceSurfaceFormatsKHR)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkGetPhysicalDeviceSurfacePresentModesKHR)
#if defined(VK_USE_PLATFORM_WIN32_KHR)
        VK_LOAD_INSTANCE_LEVEL_FUN(vkCreateWin32SurfaceKHR)
#endif

    // Default instance-level functios
    VK_LOAD_INSTANCE_LEVEL_FUN(vkEnumerateDeviceExtensionProperties)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkEnumeratePhysicalDevices)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkGetPhysicalDeviceProperties)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkGetPhysicalDeviceFeatures)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkGetPhysicalDeviceQueueFamilyProperties)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkCreateDevice)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkGetDeviceProcAddr)
    VK_LOAD_INSTANCE_LEVEL_FUN(vkDestroyInstance)

    return true;
  }

  bool Example02::CreatePresentationSurface()
  {
#if defined(VK_USE_PLATFORM_WIN32_KHR)
      VkWin32SurfaceCreateInfoKHR surface_create_info = {
          VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
          nullptr,
          0,
          WindowDescriptor.Instance,
          WindowDescriptor.Handle
      };

      if (vkCreateWin32SurfaceKHR(VulkanDescriptor.Instance, &surface_create_info, nullptr, &VulkanDescriptor.PresentationSurface) == VK_SUCCESS)
      {
          return true;
      }
#endif

std::cout << "Could not create presentation surface!" << std::endl;
return false;
  }

  bool Example02::CreateDevice()
  {
      uint32_t num_devices = 0;
      if (vkEnumeratePhysicalDevices(VulkanDescriptor.Instance, &num_devices, nullptr) != VK_SUCCESS)
      {
#ifdef _DEBUG  
          std::cout << "Error occurred during physical devices enumeration!" << std::endl;
#endif
          return false;
      }

      if (num_devices == 0)
      {
#ifdef _DEBUG  
          std::cout << "No physical devicces were found!" << std::endl;
#endif
          return false;
      }

      std::vector<VkPhysicalDevice> vk_physical_devices(num_devices);
      if (vkEnumeratePhysicalDevices(VulkanDescriptor.Instance, &num_devices, &vk_physical_devices[0]) != VK_SUCCESS)
      {
#ifdef _DEBUG  
          std::cout << "Error occurred during physical devices extraction!" << std::endl;
#endif
          return false;
      }

      VkPhysicalDevice selected_physical_device = VK_NULL_HANDLE;
      uint32_t selected_queue_family_index = UINT32_MAX;
      for (uint32_t i = 0; i < num_devices; ++i)
      {
          if (CheckPhysicalDeviceProperties(vk_physical_devices[i], selected_queue_family_index))
          {
              selected_physical_device = vk_physical_devices[i];
              break;
          }
      }

      if (selected_physical_device == VK_NULL_HANDLE)
      {
#ifdef _DEBUG  
          std::cout << "Could not select physical device based on the chosen properties!" << std::endl;
#endif
          return false;
      }

      if (selected_queue_family_index == UINT32_MAX)
      {
#ifdef _DEBUG  
          std::cout << "Could not select queue family based on the chosen properties!" << std::endl;
#endif
          return false;
      }

      std::vector<float> queue_priorities = { 1.0f };

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
#ifdef _DEBUG  
          std::cout << "Could not create Vulkan device!" << std::endl;
#endif
          return false;
      }

      VulkanDescriptor.QueueFamilyIndex = selected_queue_family_index;
      return true;
  }

  bool Example02::CheckPhysicalDeviceProperties(VkPhysicalDevice physical_device, uint32_t& queue_family_index)
  {
      // Getting extension count
      uint32_t extensions_count = 0;
      if (vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, nullptr) != VK_SUCCESS ||
          extensions_count == 0 )
      {
          std::cout << "Error occurred during physical device " << physical_device << " extensions number retrieving!" << std::endl;
          return false;
      }

      // Getting extensions 
      std::vector<VkExtensionProperties> available_device_extensions(extensions_count);
      if (vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensions_count, available_device_extensions.data()) != VK_SUCCESS ||
          extensions_count == 0)
      {
          std::cout << "Error occurred during physical device " << physical_device << " extensions enumeration!" << std::endl;
          return false;
      }

      std::vector<const char*> required_device_extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
      };

      for (size_t i = 0; i < required_device_extensions.size(); i++)
      {
          if (!CheckExtensionAvailability(required_device_extensions[i], available_device_extensions))
          {
              std::cout << "Physical device " << physical_device << " doesn`t support extension named \"" << required_device_extensions[i] << "\"!" << std::endl;
              return false;
          }
      }


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

  bool Example02::LoadDeviceLevelFunctions()
  {
      VK_LOAD_DEVICE_LEVEL_FUN(vkGetDeviceQueue)
      VK_LOAD_DEVICE_LEVEL_FUN(vkDestroyDevice)
      VK_LOAD_DEVICE_LEVEL_FUN(vkDeviceWaitIdle)

    return true;
  }

  bool Example02::GetDeviceQueue()
  {
      vkGetDeviceQueue(VulkanDescriptor.Device, VulkanDescriptor.QueueFamilyIndex, 0, &VulkanDescriptor.Queue);
      return true;
  }

  Example02::~Example02() 
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