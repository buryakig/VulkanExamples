#pragma once

#include "vulkan_core.h"
#include "OS.h"

namespace VulkanExamples 
{

  struct VulkanExample01Parameters 
  {
    VkInstance  Instance;
    VkDevice    Device;
    uint32_t    QueueFamilyIndex;
    VkQueue     Queue;

    VulkanExample01Parameters() :
      Instance( VK_NULL_HANDLE ),
      Device( VK_NULL_HANDLE ),
      QueueFamilyIndex( 0 ),
      Queue( VK_NULL_HANDLE ) {
    }
  };

  class Example01 : public OS::ApplicationBase 
  {
  public:
    Example01();
    ~Example01();

    bool OnWindowResized() override;
    bool Draw() override;
    inline bool Running() override;
    inline void ShutDown() override;

    bool PrepareVulkan();

  private:
    OS::LibraryHandle           VulkanLibrary;
    VulkanExample01Parameters  VulkanDescriptor;

    bool LoadVulkanLibrary();
    bool LoadExportedFunctions();
    bool LoadGlobalLevelFunctions();
    bool CreateInstance();
    bool LoadInstanceLevelFunctions();
    bool CreateDevice();
    bool CheckPhysicalDeviceProperties( VkPhysicalDevice physical_device, uint32_t &queue_family_index );
    bool LoadDeviceLevelFunctions();
    bool GetDeviceQueue();
  };

} // namespace VulkanExamples