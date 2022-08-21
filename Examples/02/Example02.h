#pragma once

#include "vulkan_core.h"
#include "OS.h"

namespace VulkanExamples 
{

  struct VulkanExample02Parameters 
  {
    VkInstance          Instance;
    VkDevice            Device;
    VkPhysicalDevice    PhysicalDevice;
    uint32_t            QueueFamilyIndex;
    VkQueue             Queue;
    VkSurfaceKHR        PresentationSurface;

    VulkanExample02Parameters() :
      Instance( VK_NULL_HANDLE ),
      Device( VK_NULL_HANDLE ),
      PhysicalDevice(VK_NULL_HANDLE),
      QueueFamilyIndex( 0 ),
      PresentationSurface(VK_NULL_HANDLE),
      Queue( VK_NULL_HANDLE ) {
    }
  };

  class Example02 : public OS::ApplicationBase 
  {
  public:
    Example02();
    ~Example02();

    bool OnWindowResized() override;
    bool Draw() override;
    inline bool Running() override;
    inline void ShutDown() override;

    bool PrepareVulkan(const OS::WindowParameters& window_desc);

  private:

    OS::WindowParameters        WindowDescriptor;
    OS::LibraryHandle           VulkanLibrary;
    VulkanExample02Parameters  VulkanDescriptor;

    bool LoadVulkanLibrary();
    bool LoadExportedFunctions();
    bool LoadGlobalLevelFunctions();
    bool CreateInstance();
    bool CreatePresentationSurface();
    bool CheckExtensionAvailability(const char* extension_name, const std::vector<VkExtensionProperties>& available_extensions);
    bool LoadInstanceLevelFunctions();
    bool CreateDevice();
    bool CheckPhysicalDeviceProperties( VkPhysicalDevice physical_device, uint32_t &queue_family_index );
    bool LoadDeviceLevelFunctions();
    bool GetDeviceQueue();
  };

} // namespace VulkanExamples
