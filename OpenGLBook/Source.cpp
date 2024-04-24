#include <iostream>

#include <SDL/SDL.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_sdl2.h>
#include <ImGUI/imgui_impl_opengl3.h>

#include <set>
#include <thread>
#include <deque>
#include <sstream>



#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Geometry.h"
#include "Camera.h"

#define EPSILON 0.0001

std::string getShader(const char*);
unsigned char* loadImage(const char*);

GLuint attachAndLinkShaders(SDL_Window*, const char*, const char*);
void checkShaderError(GLuint, SDL_Window*);
void checkProgramError(GLuint, SDL_Window*);


int main(int argc, char** argv)
{
  SDL_Window*   vWindow  = nullptr;
  SDL_GLContext vContext = nullptr;
  bool          vRunning = false;
  bool          vWindowEntered = false;

  stbi_set_flip_vertically_on_load(true);
  vWindow  = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
  vContext = SDL_GL_CreateContext(vWindow);
  
  gladLoadGL();

  //camera vars
  Camera vCamera({ 0.0F, 1.0F, 0.0F }, { 0.0F, 0.0F, -1.0F }, {0.0F, 0.0F, 2.0F});
  std::unordered_set<unsigned char> keys;

  glm::vec3 vLightWorldPosition = {-2.0F, 2.0F, -3.0F};

  ShapeData vCube     = ShapeGenerator::makeCube();

  GLuint vCubeVertexBufferId;
  GLuint vCubeIndexBufferId;
  GLuint vCubeVertexArrayId;


  //glFrontFace(GL_CW);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);

  glGenBuffers(1, &vCubeVertexBufferId);
  glGenBuffers(1, &vCubeIndexBufferId);
  glGenVertexArrays(1, &vCubeVertexArrayId);

  //set up cube stuff
  glBindVertexArray(vCubeVertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, vCubeVertexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vCubeIndexBufferId);

  glBufferData(GL_ARRAY_BUFFER, vCube.sizeVertices, vCube.vertices, GL_DYNAMIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vCube.sizeIndices, vCube.indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(sizeof(float) * 3));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(sizeof(float) * 7));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(sizeof(float) * 9));

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLuint vShapeProgramId = attachAndLinkShaders(vWindow, "VertexShader.glsl", "FragmentShader.glsl");
  GLuint vLightProgramId = attachAndLinkShaders(vWindow, "LightVertexShader.glsl", "LightFragmentshader.glsl");

  //load textures
  GLuint vBoxTextureId;

  glGenTextures(1, &vBoxTextureId);

  int vWallTextureHeight, vWallTextureWidth, vWallTextureChannels;
  unsigned char* vWallTextureData = stbi_load("Textures/wall2.png", &vWallTextureWidth, &vWallTextureHeight, &vWallTextureChannels, 0);

  int vBoxTextureHeight, vBoxTextureWidth, vBoxTextureChannels;
  unsigned char* vBoxTextureData = stbi_load("Textures/box.png", &vBoxTextureWidth, &vBoxTextureHeight, &vBoxTextureChannels, 0);

  int vWaterTextureHeight, vWaterTextureWidth, vWaterTextureChannels;
  unsigned char* vWaterTextureData = stbi_load("Textures/water.png", &vWaterTextureWidth, &vWaterTextureHeight, &vWaterTextureChannels, 0);

  //configure 2nd texture
  glBindTexture(GL_TEXTURE_2D, vBoxTextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vBoxTextureWidth, vBoxTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, vBoxTextureData);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  vRunning = true;


  ImGui::CreateContext();

  ImGuiIO& vImGuiIO = ImGui::GetIO(); 
  vImGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  vImGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  vImGuiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
  vImGuiIO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
  

  ImGui_ImplSDL2_InitForOpenGL(vWindow, vContext);
  ImGui_ImplOpenGL3_Init();

  while(vRunning)
  {
    SDL_Event vEvent;

    while(SDL_PollEvent(&vEvent))
    {
      ImGui_ImplSDL2_ProcessEvent(&vEvent);

      switch(vEvent.type)
      {
        case SDL_QUIT:
        {
          vRunning = false;
          break;
        }
        case SDL_KEYUP:
        {
          SDL_KeyboardEvent vKeyEvent = vEvent.key;

          keys.erase(vKeyEvent.keysym.sym);

          break;
        }
        case SDL_KEYDOWN:
        {
          SDL_KeyboardEvent vKeyEvent = vEvent.key;
          
          if(vKeyEvent.keysym.sym == SDLK_ESCAPE)
          {
            vRunning = false;
          }
          
          keys.insert(vKeyEvent.keysym.sym);

          break;
        }
        case SDL_MOUSEMOTION:
        { 
          SDL_MouseMotionEvent vMouseMotionEvent = vEvent.motion;

          if(vMouseMotionEvent.windowID == SDL_GetWindowID(vWindow))
          {
            glm::vec2 vCurrMousePosition = glm::vec2(vMouseMotionEvent.x, vMouseMotionEvent.y);

            vCamera.updateDirectionVector(vCurrMousePosition, vWindowEntered, keys);

            if(vWindowEntered)
            {
              vWindowEntered = false;
            }
          }
          
          break;
        }
        case SDL_WINDOWEVENT:
        {
          SDL_WindowEvent vWindowEvent = vEvent.window;

          if(vWindowEvent.event    == SDL_WINDOWEVENT_ENTER &&
             vWindowEvent.windowID == SDL_GetWindowID(vWindow))
          {
            vWindowEntered = true;
          }
          
          break;
        }
      }
    }

    //Set up ImGUI frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow();
    ImGui::Begin("Move Light Source");

    ImGui::SliderFloat("Light X position", &vLightWorldPosition.x, -15.0F, 15.0F);
    ImGui::SliderFloat("Light Y position", &vLightWorldPosition.y, -15.0F, 15.0F);
    ImGui::SliderFloat("Light Z position", &vLightWorldPosition.z, -15.0F, 15.0F);

    ImGui::End();

    vCamera.updatePositionVector(keys);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //stay the same for each draw call
    glm::mat4 vPerspective = glm::perspective(glm::radians(60.0F), 800.0F / 600, 1.0F, 20.0F);
    glm::mat4 vLookAt      = vCamera.getLookAtMatrix();

    //cube draw call
    glUseProgram(vShapeProgramId);

    static float angle = 0.001F;
    angle += 0.01F;

    GLint vModelUniformId          = glGetUniformLocation(vShapeProgramId, "model");
    GLint vRotationUniformId       = glGetUniformLocation(vShapeProgramId, "rotation");
    GLint vViewUniformId           = glGetUniformLocation(vShapeProgramId, "view");
    GLint vProjectionUniformId     = glGetUniformLocation(vShapeProgramId, "projection");
    GLint vLightPositionUniformId  = glGetUniformLocation(vShapeProgramId, "lightPosition");
    GLint vCameraPositionUniformId = glGetUniformLocation(vShapeProgramId, "cameraPosition");

    glm::mat4 vRotation  = glm::rotate(glm::mat4(1), glm::radians(angle), glm::vec3(0.0F, 1.0F, 0.0F));
    glm::mat4 vTransform = glm::translate(glm::mat4(1), glm::vec3(2.0F, 0.0F, -5.0F));

    //set uniforms
    glUniformMatrix4fv(vModelUniformId, 1, GL_FALSE, glm::value_ptr(vTransform));
    glUniformMatrix4fv(vRotationUniformId, 1, GL_FALSE, glm::value_ptr(vRotation));
    glUniformMatrix4fv(vViewUniformId, 1, GL_FALSE, glm::value_ptr(vLookAt));
    glUniformMatrix4fv(vProjectionUniformId, 1, GL_FALSE, glm::value_ptr(vPerspective));
    glUniform3fv(vLightPositionUniformId, 1, glm::value_ptr(vLightWorldPosition));
    glUniform3fv(vCameraPositionUniformId, 1, glm::value_ptr(vCamera.getPositionVector()));

    glBindVertexArray(vCubeVertexArrayId);
    glBindTexture(GL_TEXTURE_2D, vBoxTextureId);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);



    //light draw call
    glUseProgram(vLightProgramId);

    vModelUniformId = glGetUniformLocation(vLightProgramId, "model");
    vViewUniformId = glGetUniformLocation(vLightProgramId, "view");
    vProjectionUniformId = glGetUniformLocation(vLightProgramId, "projection");

    vTransform = glm::translate(glm::mat4(1), vLightWorldPosition) * glm::scale(glm::mat4(1), glm::vec3(0.1F, 0.1F, 0.1F));

    //set uniforms
    glUniformMatrix4fv(vModelUniformId, 1, GL_FALSE, glm::value_ptr(vTransform));
    glUniformMatrix4fv(vViewUniformId, 1, GL_FALSE, glm::value_ptr(vLookAt));
    glUniformMatrix4fv(vProjectionUniformId, 1, GL_FALSE, glm::value_ptr(vPerspective));

    glBindVertexArray(vCubeVertexArrayId);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
      SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    SDL_GL_SwapWindow(vWindow);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  return 1;
}

GLuint attachAndLinkShaders(SDL_Window* pWindow, const char* pVertexShaderFile, const char* pFragmentShaderFile)
{
  GLuint vVertexShaderId   = glCreateShader(GL_VERTEX_SHADER);
  GLuint vFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  GLuint vProgramId        = glCreateProgram();

  std::string vShaderText;
  const char* vShaderTextCString;

  vShaderText        = getShader(pVertexShaderFile);
  vShaderTextCString = vShaderText.c_str();

  glShaderSource(vVertexShaderId, 1, &vShaderTextCString, NULL);
  glCompileShader(vVertexShaderId);
  checkShaderError(vVertexShaderId, pWindow);

  vShaderText        = getShader(pFragmentShaderFile);
  vShaderTextCString = vShaderText.c_str();

  glShaderSource(vFragmentShaderId, 1, &vShaderTextCString, NULL);
  glCompileShader(vFragmentShaderId);
  checkShaderError(vFragmentShaderId, pWindow);

  //link shaders

  glAttachShader(vProgramId, vVertexShaderId);
  glAttachShader(vProgramId, vFragmentShaderId);

  glLinkProgram(vProgramId);
  checkProgramError(vProgramId, pWindow);

  //glUseProgram(vProgramId);

  glDeleteShader(vVertexShaderId);
  glDeleteShader(vFragmentShaderId);
  
  return vProgramId;
}

void checkShaderError(GLuint pShaderId, SDL_Window* pWindow)
{
  int  vSuccess;
  char vErrorLog[512];

  glGetShaderiv(pShaderId, GL_COMPILE_STATUS, &vSuccess);

  if(!vSuccess)
  {
    glGetShaderInfoLog(pShaderId, sizeof(vErrorLog), NULL, vErrorLog);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Error compiling shader", vErrorLog, pWindow);
  }
}

void checkProgramError(GLuint pProgramId, SDL_Window* pWindow)
{
  int  vSuccess;
  char vErrorLog[512];

  glGetProgramiv(pProgramId, GL_LINK_STATUS, &vSuccess);

  if(!vSuccess)
  {
    glGetProgramInfoLog(pProgramId, sizeof(vErrorLog), NULL, vErrorLog);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Error linking program", vErrorLog, pWindow);
  }
}