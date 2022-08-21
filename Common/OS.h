#pragma once

#if defined(VK_USE_PLATFORM_WIN32_KHR)
	#include <Windows.h>
#endif

#include <string>
#include <vector>
#include <set>
#include <iostream>

namespace VulkanExamples
{
	namespace OS
	{
		// Platfomr dependent type of Vulkan Dynamic library

		#if defined(VK_USE_PLATFORM_WIN32_KHR)
			typedef HMODULE LibraryHandle;
		#endif


		class ApplicationBase
		{
		public:
			ApplicationBase()
				: isRunning{true}
			{}

			virtual ~ApplicationBase()
			{}

			virtual bool OnWindowResized() = 0;
			virtual bool Draw() = 0;
			virtual bool Running() = 0;
			virtual void ShutDown() = 0;

		protected:
			bool isRunning;
		};

		struct WindowParameters
		{
			#if defined(VK_USE_PLATFORM_WIN32_KHR)
			HINSTANCE Instance;
			HWND Handle;
			
			WindowParameters()
				: Instance()
				, Handle()
			{}

			#endif
		};

		class Window
		{
		public:
			Window();
			~Window();

			bool Create(const char *title);
			bool RenderingLoop(ApplicationBase&baseApp) const;
			inline WindowParameters GetParameters() const { return parameters; }

		private:
			WindowParameters parameters;
		};

	}
}
