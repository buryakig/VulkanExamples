#include "Example02.h"

int main( int argc, char **argv ) {
    VulkanExamples::OS::Window window;
    VulkanExamples::Example02 Example02;

  // Window creation
  if( !window.Create( "01 - The Beginning" ) ) 
  {
    return -1;
  }

  // Vulkan preparations and initialization
  if( !Example02.PrepareVulkan(window.GetParameters()) )
  {
    return -1;
  }

  // Rendering loop
  if( !window.RenderingLoop( Example02 ) ) 
  {
    return -1;
  }

  return 0;
}
