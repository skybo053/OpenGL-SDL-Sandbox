#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <unordered_set>


class Camera
{
public:

  Camera(glm::vec3 pUpVector, glm::vec3 pDirectionVector, glm::vec3 pPositionVector);

  void updateDirectionVector(glm::vec2 pCurrentMousePosition, bool pWindowEntered, const std::unordered_set<unsigned char>& pKeys);
  void updatePositionVector(const std::unordered_set<unsigned char>& pKeys);

  glm::vec3 getPositionVector() const;
  glm::vec3 getDirectionVector() const;
  glm::mat4 getLookAtMatrix() const;
  glm::mat4 getReverseLookAtMatrix() const;

private:

  glm::vec3 upVector;
  glm::vec3 directionVector;
  glm::vec3 positionVector;

  glm::vec2 lastMousePosition;
};


#endif

