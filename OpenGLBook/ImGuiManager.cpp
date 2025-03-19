#include "ImGuiManager.h"

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_sdl2.h>
#include <ImGUI/imgui_impl_opengl3.h>


ImGuiManager::ImGuiManager(SDL_Window* pWindow, void* pGlContext)
{
  ImGui::CreateContext();

  ImGuiIO& vImGuiIO = ImGui::GetIO();
  vImGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  vImGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  vImGuiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
  vImGuiIO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows


  ImGui_ImplSDL2_InitForOpenGL(pWindow, pGlContext);
  ImGui_ImplOpenGL3_Init();
}
