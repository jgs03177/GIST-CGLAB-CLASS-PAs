#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "neon/blueprint.hpp"
#include "neon/ray.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

namespace ne {

class Camera {
public:
  Camera() {}

  Camera(const glm::vec3 &origin, const glm::vec3 &lookat,
         const glm::vec3 &world_up, float vfov, float aspect = 1.0f,
         float aperture = 60.0f, float focusDist = 2.0f)
      : origin(origin) {
    w = glm::normalize(lookat - origin);
    u = glm::normalize(glm::cross(w, world_up));
    v = glm::normalize(glm::cross(u, w));

    float theta = glm::radians(vfov);
    float half_height = glm::tan(theta / 2.0f) * focusDist;
    float half_width = aspect * half_height;

    // 3D world position of bottom left corner pixel
    bottomLeft = origin - (half_width * u) - (half_height * v) + focusDist * w;
    horizontal = 2 * half_width * u;
    vertical = 2 * half_height * v;

    parameters.lensRadius = aperture / 2.0f;
    parameters.focalLength = focusDist;
    parameters.vfov = vfov;
  }

  inline ne::Ray sample(float s, float t) const {
    return ne::Ray(origin, bottomLeft + s * horizontal + t * vertical - origin);
  }

  // blurred by the constants
  inline ne::Ray sample_with_blur(float s, float t, float blurr) const {
    glm::vec3 origin = this->origin + glm::mat2x3(u,v) * glm::diskRand(blurr);
    return ne::Ray(origin, bottomLeft + s * horizontal + t * vertical - origin);
  }

  struct {
    float vfov;
    float lensRadius;
    float focalLength; // focus distance
  } parameters;

  glm::vec3 origin;
  glm::vec3 bottomLeft;
  glm::vec3 horizontal;
  glm::vec3 vertical;
  glm::vec3 u, v, w;
};

} // namespace ne

#endif // __CAMERA_H_
