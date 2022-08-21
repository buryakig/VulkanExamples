#include "Example01.h"

int main( int argc, char **argv ) {
    VulkanExamples::OS::Window window;
    VulkanExamples::Example01 Example01;

  // Window creation
  if( !window.Create( "01 - The Beginning" ) ) 
  {
    return -1;
  }

  // Vulkan preparations and initialization
  if( !Example01.PrepareVulkan() ) 
  {
    return -1;
  }

  // Rendering loop
  if( !window.RenderingLoop( Example01 ) ) 
  {
    return -1;
  }

  return 0;
}
