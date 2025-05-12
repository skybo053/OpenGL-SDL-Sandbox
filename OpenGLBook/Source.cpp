#include <iostream>

#include <SDL/SDL.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_sdl2.h>
#include <ImGUI/imgui_impl_opengl3.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <set>
#include <thread>
#include <deque>
#include <sstream>
#include <vector>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Camera.h"
#include "Vertex.h"
#include "HL_MDL_Model.h"
#include "ImGuiManager.h"

struct DrawData
{
  unsigned int vaoID;
  unsigned int programID;
  int numIndices;
};


std::string getShader(const char*);
unsigned char* loadImage(const char*);
GLuint attachAndLinkShaders(SDL_Window*, const char*, const char*);
void checkShaderError(GLuint, SDL_Window*);
void checkProgramError(GLuint, SDL_Window*);
void handleSDLEvents(bool& pRunning, std::unordered_set<unsigned char>& pKeys, Camera& pCamera, SDL_Window* pWindow, bool& pWindowEntered);
DrawData* drawTriangle(SDL_Window* pWindow, const char* pVertexShader, const char* pFragmentShader);
DrawData* drawBlockWithHole(SDL_Window* pWindow, const char* pVertexShader, const char* pFragmentShader);
DrawData* drawSquare(SDL_Window* pWindow, const char* pVertexShader, const char* pFragmentShader);
DrawData* drawCube(SDL_Window* pWindow, const char* pVertexShader, const char* pFragmentShader);
DrawData* drawFullScreenQuad(SDL_Window* pWindow, const char* pVertexShader, const char* pFragmentShader);

int main(int argc, char** argv)
{
  SDL_Window*   vWindow  = nullptr;
  SDL_GLContext vContext = nullptr;
  bool          vRunning = false;
  bool          vWindowEntered = false;

  //init stuff
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  vWindow  = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
  vContext = SDL_GL_CreateContext(vWindow);
  
  ImGuiManager vImGuiManager(vWindow, vContext);

  gladLoadGL();

  Camera vCamera({ 0.0F, 1.0F, 0.0F }, { 0.0F, 0.0F, -1.0F }, {0.0F, 0.0F, 2.0F});
  std::unordered_set<unsigned char> keys;
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  //glEnable(GL_CULL_FACE);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  DrawData* vTriangleDrawData = nullptr;
  DrawData* vBlockDrawData    = nullptr;
  DrawData* vSquareDrawData   = nullptr;
  DrawData* vCubeDrawData     = nullptr;
  DrawData* vFullScreenQuad   = nullptr;
  DrawData* vRedCubeDrawData  = nullptr;

  //vTriangleDrawData = drawTriangle(vWindow, "VertexShader.glsl", "FragmentShader.glsl");
  // vBlockDrawData = drawBlockWithHole(vWindow, "VertexShader.glsl", "FragmentShader.glsl");
  // vSquareDrawData = drawSquare(vWindow, "VertexShader.glsl", "FragmentShader.glsl");
  vCubeDrawData    = drawCube(vWindow, "VertexShader.glsl", "FragmentShader.glsl");
  vRedCubeDrawData = drawCube(vWindow, "RedCubeVertexShader.glsl", "RedCubeFragmentShader.glsl");
  //vFullScreenQuad = drawFullScreenQuad(vWindow, "FrameBufferVertexShader.glsl", "FrameBufferFragmentShader.glsl");

  

  //set up cubemap textures


  GLuint vSkyboxID;
  glGenTextures(1, &vSkyboxID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, vSkyboxID);
  
  std::vector<std::string> vCubeMapPaths = 
  {
    "Textures/cubemap/lake/right.jpg",
    "Textures/cubemap/lake/left.jpg",
    "Textures/cubemap/lake/top.jpg",
    "Textures/cubemap/lake/bottom.jpg",
    "Textures/cubemap/lake/front.jpg",
    "Textures/cubemap/lake/back.jpg",
  };

  int width, height, channels;
  for(int vIndex = 0; vIndex < vCubeMapPaths.size(); ++vIndex)
  {
    unsigned char* vTextureData = stbi_load(vCubeMapPaths[vIndex].c_str(), &width, &height, &channels, 0);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + vIndex, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, vTextureData);
  }

  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

  vRunning = true;
  while(vRunning)
  {
    handleSDLEvents(vRunning, keys, vCamera, vWindow, vWindowEntered);

    //Set up ImGUI frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //ImGui::Begin("Move Square"); 
    //ImGui::End();

    vCamera.updatePositionVector(keys);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //draw red cube

    glUseProgram(vRedCubeDrawData->programID);
    glBindVertexArray(vRedCubeDrawData->vaoID);

    GLint vMvpLocationId = glGetUniformLocation(vRedCubeDrawData->programID, "mvp");
    glUniformMatrix4fv(vMvpLocationId, 1, GL_FALSE, glm::value_ptr(
      glm::perspective(glm::radians(50.0F), 1.0F, 0.1F, 25.0F) * vCamera.getLookAtMatrix() * glm::translate(glm::mat4(1), glm::vec3(0.0F, 0.0F, -4.0F))
    ));

    glDrawElements(GL_TRIANGLES, vRedCubeDrawData->numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //draw cubemap
    glUseProgram(vCubeDrawData->programID);
    glBindVertexArray(vCubeDrawData->vaoID);

    GLint vProjectionLocation = glGetUniformLocation(vCubeDrawData->programID, "projection");
    GLint vViewLocation       = glGetUniformLocation(vCubeDrawData->programID, "view");
    GLint vTransformLocation  = glGetUniformLocation(vCubeDrawData->programID, "transform");

    glUniformMatrix4fv(vProjectionLocation, 1, GL_FALSE, glm::value_ptr(glm::perspective(glm::radians(50.0F), 1.0F, 0.1F, 25.0F)));
    glUniformMatrix4fv(vViewLocation, 1, GL_FALSE, glm::value_ptr(vCamera.getLookAtMatrix()));
    glUniformMatrix4fv(vTransformLocation, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1), glm::vec3(3.0F, 3.0F, 3.0F))));

    glDepthFunc(GL_LEQUAL);
    glDrawElements(GL_TRIANGLES, vCubeDrawData->numIndices, GL_UNSIGNED_INT, 0);
    glDepthFunc(GL_LESS);

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


void handleSDLEvents(bool& pRunning, std::unordered_set<unsigned char>& pKeys, Camera& pCamera, SDL_Window* pWindow, bool& pWindowEntered)
{
  SDL_Event vEvent;

  while(SDL_PollEvent(&vEvent))
  {
    ImGui_ImplSDL2_ProcessEvent(&vEvent);

    switch(vEvent.type)
    {
      case SDL_QUIT:
      {
        pRunning = false;
        break;
      }
      case SDL_KEYUP:
      {
        SDL_KeyboardEvent vKeyEvent = vEvent.key;

        pKeys.erase(vKeyEvent.keysym.sym);

        break;
      }
      case SDL_KEYDOWN:
      {
        SDL_KeyboardEvent vKeyEvent = vEvent.key;

        if(vKeyEvent.keysym.sym == SDLK_ESCAPE)
        {
          pRunning = false;
        }

        pKeys.insert(vKeyEvent.keysym.sym);

        break;
      }
      case SDL_MOUSEMOTION:
      {
        SDL_MouseMotionEvent vMouseMotionEvent = vEvent.motion;

        if(vMouseMotionEvent.windowID == SDL_GetWindowID(pWindow))
        {
          glm::vec2 vCurrMousePosition = glm::vec2(vMouseMotionEvent.x, vMouseMotionEvent.y);

          pCamera.updateDirectionVector(vCurrMousePosition, pWindowEntered, pKeys);

          if(pWindowEntered)
          {
            pWindowEntered = false;
          }
        }

        break;
      }
      case SDL_WINDOWEVENT:
      {
        SDL_WindowEvent vWindowEvent = vEvent.window;

        if(vWindowEvent.event == SDL_WINDOWEVENT_ENTER &&
          vWindowEvent.windowID == SDL_GetWindowID(pWindow))
        {
          pWindowEntered = true;
        }

        break;
      }
    }
  }
}


DrawData* drawBlockWithHole(SDL_Window* pWindow, const char* pVertexShader, const char* pFragmentShader)
{
  DrawData* vDrawData = new DrawData;

  float vVertices[] = 
  {
    -2.0F, 2.0F, -8.0F,
    -2.0F, 1.0F, -8.0F,
    2.0F, 1.0F, -8.0F,
    2.0F, 2.0F, -8.0F,
    -2.0F, -2.0F, -8.0F,
    -1.0F, 2.0F, -8.0F,
    -1.0F, -2.0F, -8.0F,
    -2.0F, -1.0F, -8.0F,
    2.0F, -1.0F, -8.0F,
    2.0F, -2.0F, -8.0F,
    1.0F, 2.0F, -8.0F,
    2.0F, -2.0F, -8.0F,
    1.0F, -2.0F, -8.0F
  };

  int vIndices[] =
  {
    0, 1, 2,
    0, 2, 3,

    0, 4, 5,
    5, 4, 6,

    4, 7, 8,
    4, 8, 9,

    3, 9, 10,
    10, 11, 12
  };

  GLuint vertexArrayId;
  GLuint vertexBufferId;
  GLuint indexBufferId;

  glGenVertexArrays(1, &vertexArrayId);
  glGenBuffers(1, &vertexBufferId);
  glGenBuffers(1, &indexBufferId);

  glBindVertexArray(vertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vIndices), vIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLuint vBoxProgramId = attachAndLinkShaders(pWindow, pVertexShader, pFragmentShader);

  vDrawData->vaoID = vertexArrayId;
  vDrawData->programID = vBoxProgramId;
  vDrawData->numIndices = 24;

  return vDrawData;
}


DrawData* drawTriangle(SDL_Window* pWindow, const char* pVertexShader, const char* pFragmentShader)
{
  DrawData* vDrawData = new DrawData;

  float vVertices[] = 
  {
    -0.5F,    -0.5F,    -3.0F,
     0.0F,     0.5F,    -3.0F,
     0.5F,    -0.5F,    -3.0F
  };

  int vIndices[] = {0, 1, 2};

  GLuint vertexArrayId;
  GLuint vertexBufferId;
  GLuint indexBufferId;

  glGenVertexArrays(1, &vertexArrayId);
  glGenBuffers(1, &vertexBufferId);
  glGenBuffers(1, &indexBufferId);

  glBindVertexArray(vertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vIndices), vIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLuint vTriangleProgramId = attachAndLinkShaders(pWindow, pVertexShader, pFragmentShader);

  vDrawData->vaoID = vertexArrayId;
  vDrawData->programID = vTriangleProgramId;
  vDrawData->numIndices = 3;

  return vDrawData;
}

DrawData* drawSquare(SDL_Window* pWindow, const char* pVertexShader, const char* pFragmentShader)
{
  DrawData* vDrawData = nullptr;

  vDrawData = new DrawData;

  float vVertices[] = 
  {
     0.5F,   0.5F,  -3.0F,     1.0F, 0.0F,     // top right
    -0.5F,   0.5F,  -3.0F,     0.0F, 0.0F,     // top left
    -0.5F,  -0.5F,  -3.0F,     0.0F, 1.0F,     // bottom left
     0.5F,  -0.5F,  -3.0F,     1.0F, 1.0F      // bottom right
  };

  int vIndices[]
  {
    0, 1, 2,
    0, 2, 3
  };

  GLuint vSquareBufferId;
  GLuint vIndexBufferId;
  GLuint vVaoId;

  glGenBuffers(1, &vSquareBufferId);
  glGenBuffers(1, &vIndexBufferId);
  glGenVertexArrays(1, &vVaoId);

  glBindVertexArray(vVaoId);
  glBindBuffer(GL_ARRAY_BUFFER, vSquareBufferId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vIndexBufferId);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vIndices), vIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLuint vSquareProgramId = attachAndLinkShaders(pWindow, pVertexShader, pFragmentShader);

  vDrawData->numIndices = 6;
  vDrawData->programID = vSquareProgramId;
  vDrawData->vaoID = vVaoId;

  return vDrawData;
}

DrawData* drawCube(SDL_Window* pWindow, const char* pVertexShader, const char* pFragmentShader)
{
  DrawData* vDrawData = new DrawData();
  
  float     vVertices[] = 
  {
    -1.0F,  1.0F, 1.0F,      1.0F, 0.0F, 0.0F, 1.0F,    // 0 front top left
    -1.0F, -1.0F, 1.0F,      0.0F, 1.0F, 0.0F, 1.0F,    // 1 front bottom left
     1.0F, -1.0F, 1.0F,      0.0F, 0.0F, 1.0F, 1.0F,    // 2 front bottom right
     1.0F,  1.0F, 1.0F,      1.0F, 0.0F, 0.0F, 1.0F,    // 3 front top right

     1.0F,  1.0F, -1.0F,     0.0F, 1.0F, 0.0F, 1.0F,    // 4 back top left
     1.0F, -1.0F, -1.0F,     0.0F, 0.0F, 1.0F, 1.0F,    // 5 back bottom left
    -1.0F, -1.0F, -1.0F,     1.0F, 0.0F, 0.0F, 1.0F,    // 6 back bottom right
    -1.0F,  1.0F, -1.0F,     0.0F, 1.0F, 0.0F, 1.0F    // 7 back top right
  };

  int vIndices[] = 
  {
    //front face
    0,1,2,
    3,0,2,

    //right side
    3,2,5,
    4,3,5,

    //left side
    7,6,1,
    0,7,1,

    //back face
    4,5,6,
    7,4,6,

    //top face
    7, 0, 3,
    4, 7, 3,

    //bottom face
    1, 6, 5,
    2, 1,5,
  };

  GLuint vVaoID;
  GLuint vVboID;
  GLuint vEboID;

  glGenVertexArrays(1, &vVaoID);
  glGenBuffers(1, &vVboID);
  glGenBuffers(1, &vEboID);

  glBindVertexArray(vVaoID);
  glBindBuffer(GL_ARRAY_BUFFER, vVboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vEboID);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_DYNAMIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vIndices), vIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(sizeof(float) * 3));

  vDrawData->numIndices = 36;
  vDrawData->vaoID      = vVaoID;
  vDrawData->programID  = attachAndLinkShaders(pWindow, pVertexShader, pFragmentShader);

  return vDrawData;
}

DrawData* drawFullScreenQuad(SDL_Window* pWindow, const char* pVertexShader, const char* pFragmentShader)
{
  DrawData* vDrawData = new DrawData;

  float vFullScreenQuad[] =
  {
      1.0F, 1.0F, 0.9F,   1.0F, 0.0F, 0.0F, 1.0F,    1.0F, 1.0F,   // 0  top right
      1.0F,  -1.0F, 0.9F,   1.0F, 0.0F, 0.0F, 1.0F,    1.0F, 0.0F,  // 1  bottom right
     -1.0F, -1.0F, 0.9F,   1.0F, 0.0F, 0.0F, 1.0F,    0.0F, 0.0F,   // 2  bottom left
      -1.0F,  1.0f, 0.9F,   1.0F, 0.0F, 0.0F, 1.0F,    0.0F, 1.0F, // 3  top left
  };

  unsigned int vIndices[] = 
  {
    0,1,2,
    0,2,3
  };


  GLuint vId;
  GLuint vVAO;
  GLuint vEAO;

  glGenBuffers(1, &vEAO);
  glGenBuffers(1, &vId);
  glGenVertexArrays(1, &vVAO);
  
  glBindVertexArray(vVAO);
  glBindBuffer(GL_ARRAY_BUFFER, vId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vEAO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vFullScreenQuad), vFullScreenQuad, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vIndices), vIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 3));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(sizeof(float) * 7));

  glBindVertexArray(0);

  vDrawData->numIndices = 6;
  vDrawData->vaoID = vVAO;
  vDrawData->programID = attachAndLinkShaders(pWindow, pVertexShader, pFragmentShader);

  return vDrawData;
}