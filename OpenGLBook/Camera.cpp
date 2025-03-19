#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <SDL/SDL_keycode.h>

#define EPSILON 0.0001
#define SPEED 0.2F


Camera::Camera(glm::vec3 pUpVector, glm::vec3 pDirectionVector, glm::vec3 pPositionVector) :
  upVector(pUpVector), directionVector(pDirectionVector), positionVector(pPositionVector),
  lastMousePosition(0.0F, 0.0F)
{

}

void Camera::updateDirectionVector(glm::vec2 pCurrentMousePosition, bool pWindowEntered, const std::unordered_set<unsigned char>& pKeys)
{
  glm::vec2 vMouseDelta = pCurrentMousePosition - lastMousePosition;

  lastMousePosition = pCurrentMousePosition;

  //return if window was entered to avoid a jump because of large mouse delta for direction vector calculation
  if(pKeys.find(SDLK_SPACE) != pKeys.end() || pWindowEntered) return;

  float vAbs  = std::abs(directionVector.y);
  float vDiff = 1 - vAbs;

    //for camera looking down                                              //for camera looking up
    if((directionVector.y < 0 && vDiff <= EPSILON && vMouseDelta.y >= 0) || (directionVector.y > 0 && vDiff <= EPSILON && vMouseDelta.y <= 0))
    {
      directionVector = glm::rotate(glm::mat4(1), glm::radians(-(vMouseDelta.x * SPEED)), glm::vec3(0.0F, 1.0F, 0.0F)) *
        glm::vec4(directionVector, 1.0F);
    }
    else
    {
      glm::vec3 XZToRotateAround = glm::normalize(glm::cross(directionVector, glm::vec3(0.0F, 1.0F, 0.0F)));

      directionVector = glm::rotate(glm::mat4(1), glm::radians(-(vMouseDelta.x * SPEED)), glm::vec3(0.0F, 1.0F, 0.0F)) *
        glm::rotate(glm::mat4(1), glm::radians(-(vMouseDelta.y * SPEED)), XZToRotateAround) *
        glm::vec4(directionVector, 1.0F);
    }

    directionVector = glm::normalize(directionVector);
}

void Camera::updatePositionVector(const std::unordered_set<unsigned char>& pKeys)
{
  static float speed = 0.001F;
  
  if(pKeys.find(SDLK_w) != pKeys.end())
  {
    positionVector += directionVector * speed;
  }

  if(pKeys.find(SDLK_s) != pKeys.end())
  {
    positionVector -= directionVector * speed;
  }
  
  if(pKeys.find(SDLK_d) != pKeys.end())
  {
    glm::vec3 strafeVector = glm::normalize(glm::cross(directionVector, glm::vec3(0.0F, 1.0F, 0.0F)));

    positionVector += strafeVector * speed;
  }

  if(pKeys.find(SDLK_a) != pKeys.end())
  {
    glm::vec3 strafeVector = glm::normalize(glm::cross(directionVector, glm::vec3(0.0F, 1.0F, 0.0F)));

    positionVector -= strafeVector * speed;
  }
}
 
glm::mat4 Camera::getLookAtMatrix() const
{
  return glm::lookAt(positionVector, positionVector + directionVector, upVector);
}

glm::mat4 Camera::getReverseLookAtMatrix() const
{
  glm::vec3 vReverseDirectionVector(-directionVector.x, -directionVector.y, -directionVector.z);

  return glm::lookAt(positionVector, positionVector + vReverseDirectionVector, upVector);
}

glm::vec3 Camera::getPositionVector() const
{
  return positionVector;
}

glm::vec3 Camera::getDirectionVector() const
{
  return directionVector;
}
