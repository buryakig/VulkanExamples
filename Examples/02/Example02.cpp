
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
      Clear();

      if (!CreateSwapChain()) {
          return false;
      }
      if (!CreateCommandBuffers()) {
          return false;
      }
      return true;
  }

  bool Example02::Draw()
  {
      uint32_t image_index;
      VkResult result = vkAcquireNextImageKHR(VulkanDescriptor.Device, VulkanDescriptor.SwapChain, UINT64_MAX, VulkanDescriptor.ImageAvailableSemaphore, VK_NULL_HANDLE, &image_index);
      switch (result)
      {
      case VK_SUCCESS:
      case VK_SUBOPTIMAL_KHR:
          break;
      case VK_ERROR_OUT_OF_DATE_KHR:
          return OnWindowResized();
      default:
          std::cout << "Problem occured during chain image acquisition!" << std::endl;
          return false;
      }

      VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT;
      VkSubmitInfo submit_info = {
          VK_STRUCTURE_TYPE_SUBMIT_INFO,
          nullptr,
          1,
          &VulkanDescriptor.ImageAvailableSemaphore,
          &wait_dst_stage_mask,
          1,
          &VulkanDescriptor.PresentQueueCmdBuffers[image_index],
          1,
          &VulkanDescriptor.RenderingFinishedSemaphore
      };

      if (vkQueueSubmit(VulkanDescriptor.PresentQueue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS)
      {
          return false;
      }

      VkPresentInfoKHR present_info = {
          VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
          nullptr,
          1,
          &VulkanDescriptor.RenderingFinishedSemaphore,
          1,
          &VulkanDescriptor.SwapChain,
          &image_index,
          nullptr
      };
      result = vkQueuePresentKHR(VulkanDescriptor.PresentQueue, &present_info);

      switch (result)
      {
      case VK_SUCCESS:
          break;
      case VK_ERROR_OUT_OF_DATE_KHR:
      case VK_SUBOPTIMAL_KHR:
          return OnWindowResized();
      default:
          std::cout << "Problem occured during image presentation!" << std::endl;
          return false;
      }

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
    if( !CreateSemaphores() ) {
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

      uint32_t selected_graphics_queue_family_index = UINT32_MAX;
      uint32_t selected_present_queue_family_index = UINT32_MAX;
      for (uint32_t i = 0; i < num_devices; ++i)
      {
          if (CheckPhysicalDeviceProperties(vk_physical_devices[i], selected_graphics_queue_family_index, selected_present_queue_family_index))
          {
              VulkanDescriptor.PhysicalDevice = vk_physical_devices[i];
              break;
          }
      }

      if (VulkanDescriptor.PhysicalDevice == VK_NULL_HANDLE)
      {
#ifdef _DEBUG  
          std::cout << "Could not select physical device based on the chosen properties!" << std::endl;
#endif
          return false;
      }

      std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
      std::vector<float> queue_priorities = { 1.0f };

      queue_create_infos.push_back( {
          VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
          nullptr,
          0,
          selected_graphics_queue_family_index,
          static_cast<uint32_t>(queue_priorities.size()),
          &queue_priorities[0]
      });

      if (selected_graphics_queue_family_index != selected_present_queue_family_index)
      {
          queue_create_infos.push_back({
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            nullptr,
            0,
            selected_present_queue_family_index,
            static_cast<uint32_t>(queue_priorities.size()),
            &queue_priorities[0]
          });
      }


      std::vector<const char*> extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
      };

      VkDeviceCreateInfo device_create_info = {
          VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
          nullptr,
          0,
          static_cast<uint32_t>(queue_create_infos.size()),
          & queue_create_infos[0],
          0,
          nullptr,
          static_cast<uint32_t>(extensions.size()),
          &extensions[0],
          nullptr
      };

      if (vkCreateDevice(VulkanDescriptor.PhysicalDevice, &device_create_info, nullptr, &VulkanDescriptor.Device) != VK_SUCCESS)
      {
#ifdef _DEBUG  
          std::cout << "Could not create Vulkan device!" << std::endl;
#endif
          return false;
      }

      VulkanDescriptor.GraphicsQueueFamilyIndex = selected_present_queue_family_index;
      VulkanDescriptor.PresentQueueFamilyIndex = selected_present_queue_family_index;
      return true;
  }

  bool Example02::CheckPhysicalDeviceProperties(VkPhysicalDevice physical_device, uint32_t& selected_graphics_queue_family_index, uint32_t& selected_present_queue_family_index)
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

      std::vector<VkQueueFamilyProperties>  queue_family_properties(queue_families_count);
      std::vector<VkBool32>                 queue_presentation_support(queue_families_count);

      vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, &queue_family_properties[0]);

      uint32_t graphics_queue_family_index = UINT32_MAX;
      uint32_t present_queue_family_index = UINT32_MAX;

      for (uint32_t i = 0; i < queue_families_count; ++i)
      {
          vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, VulkanDescriptor.PresentationSurface, &queue_presentation_support[i]);

          if ((queue_family_properties[i].queueCount > 0) &&
              (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
          {
              if (graphics_queue_family_index == UINT32_MAX) 
              {
                  graphics_queue_family_index = i;
              }

              if (queue_presentation_support[i])
              {
                  selected_graphics_queue_family_index = i;
                  selected_present_queue_family_index = i;
                  return true;
              }
          }
      }

      // We don't have queue that supports both graphics and present so we have to use separate queues
      for (uint32_t i = 0; i < queue_families_count; ++i)
      {
          if (queue_presentation_support[i])
          {
              present_queue_family_index = i;
              break;
          }
      }
      // If this device doesn't support queues with graphics and present capabilities don't use it
      if ((graphics_queue_family_index == UINT32_MAX) || (present_queue_family_index == UINT32_MAX)) 
      {
          std::cout << "Could not find queue family with required properties on physical device " << physical_device << "!" << std::endl;
          return false;
      }

      selected_graphics_queue_family_index = graphics_queue_family_index;
      selected_present_queue_family_index = present_queue_family_index;
      return true;
  }

  bool Example02::LoadDeviceLevelFunctions()
  {
    VK_LOAD_DEVICE_LEVEL_FUN(vkGetDeviceQueue)
    VK_LOAD_DEVICE_LEVEL_FUN(vkDestroyDevice)
    VK_LOAD_DEVICE_LEVEL_FUN(vkDeviceWaitIdle)

    VK_LOAD_DEVICE_LEVEL_FUN(vkCreateSwapchainKHR)
    VK_LOAD_DEVICE_LEVEL_FUN(vkDestroySwapchainKHR)
    VK_LOAD_DEVICE_LEVEL_FUN(vkGetSwapchainImagesKHR)
    VK_LOAD_DEVICE_LEVEL_FUN(vkAcquireNextImageKHR)
    VK_LOAD_DEVICE_LEVEL_FUN(vkQueuePresentKHR)
    VK_LOAD_DEVICE_LEVEL_FUN(vkQueuePresentKHR)
    VK_LOAD_DEVICE_LEVEL_FUN(vkGetSwapchainImagesKHR)

    VK_LOAD_DEVICE_LEVEL_FUN(vkCreateSemaphore)
    VK_LOAD_DEVICE_LEVEL_FUN(vkCreateCommandPool)
    VK_LOAD_DEVICE_LEVEL_FUN(vkAllocateCommandBuffers)
    VK_LOAD_DEVICE_LEVEL_FUN(vkBeginCommandBuffer)
    VK_LOAD_DEVICE_LEVEL_FUN(vkCmdPipelineBarrier)
    VK_LOAD_DEVICE_LEVEL_FUN(vkCmdClearColorImage)
    VK_LOAD_DEVICE_LEVEL_FUN(vkEndCommandBuffer)

    VK_LOAD_DEVICE_LEVEL_FUN(vkAcquireNextImageKHR)
    VK_LOAD_DEVICE_LEVEL_FUN(vkQueueSubmit)
    VK_LOAD_DEVICE_LEVEL_FUN(vkQueuePresentKHR)
    VK_LOAD_DEVICE_LEVEL_FUN(vkFreeCommandBuffers)
    VK_LOAD_DEVICE_LEVEL_FUN(vkDestroyCommandPool)
    VK_LOAD_DEVICE_LEVEL_FUN(vkDestroySemaphore)


    return true;
  }

  bool Example02::GetDeviceQueue()
  {
      vkGetDeviceQueue(VulkanDescriptor.Device, VulkanDescriptor.GraphicsQueueFamilyIndex, 0, &VulkanDescriptor.GraphicsQueue);
      vkGetDeviceQueue(VulkanDescriptor.Device, VulkanDescriptor.PresentQueueFamilyIndex, 0, &VulkanDescriptor.PresentQueue);
      return true;
  }

  bool Example02::CreateSemaphores()
  {
      VkSemaphoreCreateInfo semaphore_create_info = {
          VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
          nullptr,
          0
      };

      VkDevice dev = VulkanDescriptor.Device;
      VkSemaphore* sem = &VulkanDescriptor.RenderingFinishedSemaphore;

      if (vkCreateSemaphore(VulkanDescriptor.Device, &semaphore_create_info, nullptr, &VulkanDescriptor.ImageAvailableSemaphore) != VK_SUCCESS ||
          vkCreateSemaphore(VulkanDescriptor.Device, &semaphore_create_info, nullptr, &VulkanDescriptor.RenderingFinishedSemaphore) != VK_SUCCESS)
      {
          std::cout << "Could not create semaphores!" << std::endl;
          return false;
      }

      return true;
  }

  bool Example02::CreateSwapChain()
  {
      isRunning = false;

      if (VulkanDescriptor.Device != VK_NULL_HANDLE)
          vkDeviceWaitIdle(VulkanDescriptor.Device);

      VkSurfaceCapabilitiesKHR surface_capabilities;
      if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanDescriptor.PhysicalDevice, VulkanDescriptor.PresentationSurface, &surface_capabilities) != VK_SUCCESS)
      {
          std::cout << "Could not check presentation surface capabilities!" << std::endl;
          return false;
      }

      // Format mode
      uint32_t formats_count;
      if (vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanDescriptor.PhysicalDevice, VulkanDescriptor.PresentationSurface, &formats_count, nullptr) != VK_SUCCESS)
      {
          std::cout << "Error occurred during presentation surface formats enumeration!" << std::endl;
          return false;
      }

      std::vector<VkSurfaceFormatKHR> surface_formats(formats_count);
      if (vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanDescriptor.PhysicalDevice, VulkanDescriptor.PresentationSurface, &formats_count, surface_formats.data()) != VK_SUCCESS)
      {
          std::cout << "Error occurred during presentation surface formats extraction!" << std::endl;
          return false;
      }

      // Present mode
      uint32_t present_modes_count;
      if (vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanDescriptor.PhysicalDevice, VulkanDescriptor.PresentationSurface, &present_modes_count, nullptr) != VK_SUCCESS ||
          present_modes_count == 0)
      {
          std::cout << "Error occurred during presentation surface formats enumeration!" << std::endl;
      }

      std::vector<VkPresentModeKHR> present_modes(present_modes_count);
      if (vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanDescriptor.PhysicalDevice, VulkanDescriptor.PresentationSurface, &present_modes_count, present_modes.data()) != VK_SUCCESS)
      {
          std::cout << "Error occurred during presentation surface present modes extraction!" << std::endl;
          return false;
      }

      uint32_t          desired_number_of_images = GetSwapChainNumImages(surface_capabilities);
      VkSurfaceFormatKHR            desired_format = GetSwapChainFormat(surface_formats);
      VkExtent2D                    desired_extent = GetSwapChainExtent(surface_capabilities);
      VkImageUsageFlags             desired_usage = GetSwapChainUsageFlags(surface_capabilities);
      VkSurfaceTransformFlagBitsKHR desired_transform = GetSwapChainTransform(surface_capabilities);
      VkPresentModeKHR              desired_present_mode = GetSwapChainPresentMode(present_modes);
      VkSwapchainKHR                old_swap_chain = VulkanDescriptor.SwapChain;

      if (static_cast<int>(desired_usage) == -1) {
          return false;
      }
      if (static_cast<int>(desired_present_mode) == -1) {
          return false;
      }
      if ((desired_extent.width == 0) || (desired_extent.height == 0)) {
          // Current surface size is (0, 0) so we can't create a swap chain and render anything (CanRender == false)
          // But we don't wont to kill the application as this situation may occur i.e. when window gets minimized
          return true;
      }

      VkSwapchainCreateInfoKHR swap_chain_create_info = {
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,  // VkStructureType                sType
        nullptr,                                      // const void                    *pNext
        0,                                            // VkSwapchainCreateFlagsKHR      flags
        VulkanDescriptor.PresentationSurface,                   // VkSurfaceKHR                   surface
        desired_number_of_images,                     // uint32_t                       minImageCount
        desired_format.format,                        // VkFormat                       imageFormat
        desired_format.colorSpace,                    // VkColorSpaceKHR                imageColorSpace
        desired_extent,                               // VkExtent2D                     imageExtent
        1,                                            // uint32_t                       imageArrayLayers
        desired_usage,                                // VkImageUsageFlags              imageUsage
        VK_SHARING_MODE_EXCLUSIVE,                    // VkSharingMode                  imageSharingMode
        0,                                            // uint32_t                       queueFamilyIndexCount
        nullptr,                                      // const uint32_t                *pQueueFamilyIndices
        desired_transform,                            // VkSurfaceTransformFlagBitsKHR  preTransform
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,            // VkCompositeAlphaFlagBitsKHR    compositeAlpha
        desired_present_mode,                         // VkPresentModeKHR               presentMode
        VK_TRUE,                                      // VkBool32                       clipped
        old_swap_chain                                // VkSwapchainKHR                 oldSwapchain
      };

      if (vkCreateSwapchainKHR(VulkanDescriptor.Device, &swap_chain_create_info, nullptr, &VulkanDescriptor.SwapChain) != VK_SUCCESS) {
          std::cout << "Could not create swap chain!" << std::endl;
          return false;
      }
      if (old_swap_chain != VK_NULL_HANDLE) {
          vkDestroySwapchainKHR(VulkanDescriptor.Device, old_swap_chain, nullptr);
      }


      isRunning = true;
      return true;
  }

  uint32_t Example02::GetSwapChainNumImages(VkSurfaceCapabilitiesKHR& surface_capabilities)
  {
      uint32_t image_count = surface_capabilities.minImageCount + 1;
      if (surface_capabilities.maxImageCount > 0 && image_count > surface_capabilities.maxImageCount)
      {
          image_count = surface_capabilities.maxImageCount;
      }

      return image_count;
  }

  VkSurfaceFormatKHR Example02::GetSwapChainFormat(std::vector<VkSurfaceFormatKHR>& surface_formats)
  {
      if (surface_formats.size() == 1 && surface_formats[0].format == VK_FORMAT_UNDEFINED)
      {
          return{ VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
      }

      for (VkSurfaceFormatKHR& surface_format : surface_formats) 
      {
          if (surface_format.format == VK_FORMAT_R8G8B8A8_UNORM) 
              return surface_format;
      }

      return surface_formats[0];
  }

  VkExtent2D Example02::GetSwapChainExtent(VkSurfaceCapabilitiesKHR& surface_capabilities)
  {
      if (surface_capabilities.currentExtent.width == -1)
      {
          VkExtent2D swap_chain_extent = {640, 480};
          if (swap_chain_extent.width < surface_capabilities.minImageExtent.width) {
              swap_chain_extent.width = surface_capabilities.minImageExtent.width;
          }
          if (swap_chain_extent.height < surface_capabilities.minImageExtent.height) {
              swap_chain_extent.height = surface_capabilities.minImageExtent.height;
          }
          if (swap_chain_extent.width > surface_capabilities.maxImageExtent.width) {
              swap_chain_extent.width = surface_capabilities.maxImageExtent.width;
          }
          if (swap_chain_extent.height > surface_capabilities.maxImageExtent.height) {
              swap_chain_extent.height = surface_capabilities.maxImageExtent.height;
          }

          return swap_chain_extent;
      }

      return surface_capabilities.currentExtent;
  }

  VkImageUsageFlags Example02::GetSwapChainUsageFlags(VkSurfaceCapabilitiesKHR& surface_capabilities) 
  {
      if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
          return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
      }
      std::cout << "VK_IMAGE_USAGE_TRANSFER_DST image usage is not supported by the swap chain!" << std::endl
          << "Supported swap chain's image usages include:" << std::endl
          << (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT ? "    VK_IMAGE_USAGE_TRANSFER_SRC\n" : "")
          << (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT ? "    VK_IMAGE_USAGE_TRANSFER_DST\n" : "")
          << (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT ? "    VK_IMAGE_USAGE_SAMPLED\n" : "")
          << (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT ? "    VK_IMAGE_USAGE_STORAGE\n" : "")
          << (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_COLOR_ATTACHMENT\n" : "")
          << (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT\n" : "")
          << (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT\n" : "")
          << (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_INPUT_ATTACHMENT" : "")
          << std::endl;
      return static_cast<VkImageUsageFlags>(-1);
  }


  VkSurfaceTransformFlagBitsKHR Example02::GetSwapChainTransform(VkSurfaceCapabilitiesKHR& surface_capabilities)
  {
      if (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
          return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
      else
          return surface_capabilities.currentTransform;
  }

  VkPresentModeKHR Example02::GetSwapChainPresentMode(std::vector<VkPresentModeKHR>& present_modes)
  {
      for (VkPresentModeKHR& present_mode : present_modes)
      {
          if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
              return present_mode;
      }
      for (VkPresentModeKHR& present_mode : present_modes)
      {
          if (present_mode == VK_PRESENT_MODE_FIFO_KHR)
              return present_mode;
      }

      std::cout << "FIFO present mode is not supported by the swap chain!" << std::endl;
      return static_cast<VkPresentModeKHR>(-1);
  }

  bool Example02::CreateCommandBuffers()
  {
      // Creating memory pool for command buffer
      VkCommandPoolCreateInfo cmd_pool_create_info = {
          VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
          nullptr,
          0,
          VulkanDescriptor.PresentQueueFamilyIndex
      };

      if (vkCreateCommandPool(VulkanDescriptor.Device, &cmd_pool_create_info, nullptr, &VulkanDescriptor.PresentQueueCmdPool) != VK_SUCCESS)
      {
          std::cout << "Could not create a command pool!" << std::endl;
          return false;
      }

      uint32_t image_count = 0;
      if(vkGetSwapchainImagesKHR(VulkanDescriptor.Device, VulkanDescriptor.SwapChain, &image_count, nullptr) != VK_SUCCESS
          || image_count == 0)
      {
          std::cout << "Could not get the number of swap chain images!" << std::endl;
          return false;
      }

      VulkanDescriptor.PresentQueueCmdBuffers.resize(image_count);


      VkCommandBufferAllocateInfo cmd_buffer_allocate_info = {
          VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
          nullptr,
          VulkanDescriptor.PresentQueueCmdPool,
          VK_COMMAND_BUFFER_LEVEL_PRIMARY,
          image_count
      };

      if (vkAllocateCommandBuffers(VulkanDescriptor.Device, &cmd_buffer_allocate_info, &VulkanDescriptor.PresentQueueCmdBuffers[0]) != VK_SUCCESS)
      {
          std::cout << "Could not allocate command buffers!" << std::endl;
          return false;
      }

      if (!RecordCommandBuffers()) {
          std::cout << "Could not record command buffers!" << std::endl;
          return false;
      }

      return true;
  }

  bool Example02::RecordCommandBuffers()
  {
      uint32_t image_count = static_cast<uint32_t>(VulkanDescriptor.PresentQueueCmdBuffers.size());

      std::vector<VkImage> swap_chain_images(image_count);
      if (vkGetSwapchainImagesKHR(VulkanDescriptor.Device, VulkanDescriptor.SwapChain, &image_count, &swap_chain_images[0]) != VK_SUCCESS)
      {
          std::cout << "Could not get swap chain images!" << std::endl;
          return false;
      }

      VkCommandBufferBeginInfo cmd_buffer_begin_info = {
          VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
          nullptr,
          VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
          nullptr
      };

      VkClearColorValue clear_color = {
          {1.0f, 0.8f, 0.4f, 0.0f}
      };

      VkImageSubresourceRange image_subresource_range = {
          VK_IMAGE_ASPECT_COLOR_BIT,                    // VkImageAspectFlags   aspectMask
          0,                                            // uint32_t             baseMipLevel
          1,                                            // uint32_t             levelCount
          0,                                            // uint32_t             baseArrayLayer
          1                                             // uint32_t             layerCount
      };

      for (uint32_t i = 0; i < image_count; ++i)
      {
          VkImageMemoryBarrier barrier_from_present_to_clear = {
              VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
              nullptr,
              VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags  srcAccessMask
              VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags  dstAccessMask
              VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout  oldLayout
              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout  newLayout
              VulkanDescriptor.PresentQueueFamilyIndex,                     
              VulkanDescriptor.PresentQueueFamilyIndex,                     
              swap_chain_images[i],                                         
              image_subresource_range                                       
          };                                                                
                                                                            
          VkImageMemoryBarrier barrier_from_clear_to_present = {            
              VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,                       
              nullptr,                                      
              VK_ACCESS_TRANSFER_WRITE_BIT,               // VkAccessFlags                                          
              VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags  dstAccessMask
              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,       // VkImageLayout  newLayout
              VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout  oldLayout
              VulkanDescriptor.PresentQueueFamilyIndex,
              VulkanDescriptor.PresentQueueFamilyIndex,
              swap_chain_images[i],
              image_subresource_range
          };

          vkBeginCommandBuffer(VulkanDescriptor.PresentQueueCmdBuffers[i], &cmd_buffer_begin_info);
          vkCmdPipelineBarrier(VulkanDescriptor.PresentQueueCmdBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_present_to_clear);
          vkCmdClearColorImage(VulkanDescriptor.PresentQueueCmdBuffers[i], swap_chain_images[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clear_color, 1, &image_subresource_range);
          vkCmdPipelineBarrier(VulkanDescriptor.PresentQueueCmdBuffers[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_clear_to_present);
          if (vkEndCommandBuffer(VulkanDescriptor.PresentQueueCmdBuffers[i]) != VK_SUCCESS) {
              std::cout << "Could not record command buffers!" << std::endl;
              return false;
          }
      }

      return true;
  }



  bool Example02::Clear()
  {
      if (VulkanDescriptor.Device != VK_NULL_HANDLE) {
          vkDeviceWaitIdle(VulkanDescriptor.Device);

          if (VulkanDescriptor.PresentQueueCmdBuffers.size() > 0 && (VulkanDescriptor.PresentQueueCmdBuffers[0] !=
              VK_NULL_HANDLE))
          {
              vkFreeCommandBuffers(VulkanDescriptor.Device, VulkanDescriptor.PresentQueueCmdPool, static_cast<uint32_t>(VulkanDescriptor.PresentQueueCmdBuffers.size()), &VulkanDescriptor.PresentQueueCmdBuffers[0]);
              VulkanDescriptor.PresentQueueCmdBuffers.clear();
          }

          if (VulkanDescriptor.PresentQueueCmdPool != VK_NULL_HANDLE)
          {
              vkDestroyCommandPool(VulkanDescriptor.Device, VulkanDescriptor.PresentQueueCmdPool, nullptr);
              VulkanDescriptor.PresentQueueCmdPool = VK_NULL_HANDLE;
          }
      }

      return true;
  }

  Example02::~Example02() 
  {
      Clear();

      if (VulkanDescriptor.Device != VK_NULL_HANDLE)
      {
          vkDeviceWaitIdle(VulkanDescriptor.Device);

          if (VulkanDescriptor.ImageAvailableSemaphore != VK_NULL_HANDLE) {
              vkDestroySemaphore(VulkanDescriptor.Device, VulkanDescriptor.ImageAvailableSemaphore, nullptr);
          }
          if (VulkanDescriptor.RenderingFinishedSemaphore != VK_NULL_HANDLE) {
              vkDestroySemaphore(VulkanDescriptor.Device, VulkanDescriptor.RenderingFinishedSemaphore, nullptr);
          }
          if (VulkanDescriptor.SwapChain != VK_NULL_HANDLE) {
              vkDestroySwapchainKHR(VulkanDescriptor.Device, VulkanDescriptor.SwapChain, nullptr);
          }
          vkDestroyDevice(VulkanDescriptor.Device, nullptr);
      }

      if (VulkanDescriptor.PresentationSurface != VK_NULL_HANDLE) {
          vkDestroySurfaceKHR(VulkanDescriptor.Instance, VulkanDescriptor.PresentationSurface, nullptr);
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