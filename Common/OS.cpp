#include <thread>
#include <chrono>
#include "OS.h"

namespace VulkanExamples 
{

  namespace OS 
  {

    Window::Window() 
        : parameters() 
    {}

#if defined(VK_USE_PLATFORM_WIN32_KHR)

#define WINDOW_NAME "Vulkan Examples"

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_SIZE:
        case WM_EXITSIZEMOVE:
            PostMessage(hWnd, WM_USER + 1, wParam, lParam);
            break;
        case WM_KEYDOWN:
        case WM_CLOSE:
            PostMessage(hWnd, WM_USER + 2, wParam, lParam);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        return 0;
    }

    Window::~Window()
    {
        if (parameters.Handle)
        {
            DestroyWindow(parameters.Handle);
        }

        if (parameters.Handle)
        {
            UnregisterClass(WINDOW_NAME, parameters.Instance);
        }
    }

    bool Window::Create(const char* title)
    {
        parameters.Instance = GetModuleHandle(nullptr);

        // Register window class
        WNDCLASSEX wclassex;

        wclassex.cbSize = sizeof(WNDCLASSEX);

        wclassex.style = CS_HREDRAW | CS_VREDRAW;
        wclassex.lpfnWndProc = WndProc;
        wclassex.cbClsExtra = 0;
        wclassex.cbWndExtra = 0;
        wclassex.hInstance = parameters.Instance;
        wclassex.hIcon = NULL;
        wclassex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wclassex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wclassex.lpszMenuName = NULL;
        wclassex.lpszClassName = WINDOW_NAME;
        wclassex.hIconSm = NULL;

        if (!RegisterClassEx(&wclassex))
        {
            std::cout << "Window class registration failed!" << std::endl;
            return false;
        }

        // Create window
        parameters.Handle = CreateWindow(WINDOW_NAME, title, WS_OVERLAPPEDWINDOW, 20, 20, 500, 500, nullptr, nullptr, parameters.Instance, nullptr);
        if (!parameters.Handle)
        {
            std::cout << "Window creation failed!" << std::endl;
            return false;
        }
        return true;
    }

    bool Window::RenderingLoop(ApplicationBase &baseApp) const
    {
        ShowWindow(parameters.Handle, SW_SHOWNORMAL);
        UpdateWindow(parameters.Handle);

        MSG message;

        while (baseApp.Running())
        {
            if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
            {
                switch (message.message)
                {
                    // Resize
                case WM_USER + 1:
                    baseApp.OnWindowResized();
                    break;

                    // Close
                case WM_USER + 2:
                    baseApp.ShutDown();
                    break;
                }

                TranslateMessage(&message);
                DispatchMessage(&message);
            }

            baseApp.Draw();
        }

        return true;
    }

#endif    //APPLICATION_PLATFORM_WIN32

  } // namespace OS

} // namespace VulkanExamples
