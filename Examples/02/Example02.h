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
    uint32_t            GraphicsQueueFamilyIndex;
    uint32_t            PresentQueueFamilyIndex;
    VkQueue             GraphicsQueue;
    VkQueue             PresentQueue;
    VkSwapchainKHR      SwapChain;
    VkSurfaceKHR        PresentationSurface;
    VkSemaphore         ImageAvailableSemaphore;
    VkSemaphore         RenderingFinishedSemaphore;
    VkCommandPool       PresentQueueCmdPool;
    std::vector<VkCommandBuffer>  PresentQueueCmdBuffers;

    VulkanExample02Parameters() :
      Instance( VK_NULL_HANDLE ),
      Device( VK_NULL_HANDLE ),
      PhysicalDevice(VK_NULL_HANDLE),
      GraphicsQueueFamilyIndex( 0 ),
      PresentQueueFamilyIndex( 0 ),
      PresentQueueCmdBuffers( 0 ),
      PresentationSurface(VK_NULL_HANDLE),
      GraphicsQueue(VK_NULL_HANDLE),
      PresentQueue(VK_NULL_HANDLE),
      SwapChain(VK_NULL_HANDLE),
      PresentQueueCmdPool(VK_NULL_HANDLE),
        ImageAvailableSemaphore(VK_NULL_HANDLE),
      RenderingFinishedSemaphore( VK_NULL_HANDLE ) {
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
    bool CreateSwapChain();
    bool CreateCommandBuffers();
    bool Clear();

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
    bool CheckPhysicalDeviceProperties(VkPhysicalDevice physical_device, uint32_t& selected_graphics_queue_family_index, uint32_t& selected_present_queue_family_index);
    bool LoadDeviceLevelFunctions();
    bool GetDeviceQueue();
    bool CreateSemaphores();
    bool RecordCommandBuffers();
    uint32_t GetSwapChainNumImages(VkSurfaceCapabilitiesKHR& surface_capabilities);

    VkSurfaceFormatKHR GetSwapChainFormat(std::vector<VkSurfaceFormatKHR>& surface_formats);

    VkExtent2D GetSwapChainExtent(VkSurfaceCapabilitiesKHR& surface_capabilities);

    VkImageUsageFlags GetSwapChainUsageFlags(VkSurfaceCapabilitiesKHR& surface_capabilities);

    VkSurfaceTransformFlagBitsKHR GetSwapChainTransform(VkSurfaceCapabilitiesKHR& surface_capabilities);
    VkPresentModeKHR GetSwapChainPresentMode(std::vector<VkPresentModeKHR>& present_modes);
  };

} // namespace VulkanExamples
