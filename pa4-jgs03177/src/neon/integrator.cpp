#include "integrator.hpp"
#include "neon/intersection.hpp"
#include "neon/material.hpp"
#include "neon/scene.hpp"

namespace ne {

namespace core {

glm::vec3 Integrator::integrate(const ne::Ray &ray,
                                std::shared_ptr<ne::Scene> scene) {
  glm::vec3 K{1.0f};
  glm::vec3 L{0.0f};
  glm::vec3 C{0.0f};
  ne::Ray r{ray};
  ne::Intersection hit;
  int hitcount = 0;
  bool ishit;

  // if ray hit object in scene, apply shading
  while (ishit = scene->rayIntersect(r, hit)) {
    MaterialPointer material = hit.material;
    const glm::vec3 albedo = material->attenuation();
    K = glm::min(K, albedo);
    // emit
    L += material->emitted();
    hitcount++;
    if (hitcount > 10) {
      break;
    }
    if (!material->scatter(r, hit, r)) {
      break;
    }
  }
  if (!ishit) {
    L += scene->sampleBackgroundLight(r.dir);
  }
  C.r = K.r * L.r;
  C.g = K.g * L.g;
  C.b = K.b * L.b;

  return C;
}

glm::vec3 Integrator::integrate2(const ne::Ray &ray,
                                std::shared_ptr<ne::Scene> scene) {
  glm::vec3 K{1.0f};
  glm::vec3 L{0.0f};
  glm::vec3 C{0.0f};
  ne::Ray r{ray};
  ne::Intersection hit;
  int hitcount = 0;
  bool ishit;

  // if ray hit object in scene, apply shading
  while (ishit = scene->rayIntersect(r, hit)) {
    MaterialPointer material = hit.material;
    const glm::vec3 albedo = material->attenuation();
    K = glm::min(K, albedo);
    // emit
    L += material->emitted();
	L += scene->sampleDirectLight(r, hit);
    hitcount++;
    if (hitcount > 10) {
      break;
    }
    if (!material->scatter(r, hit, r)) {
      break;
    }
  }
  if (!ishit) {
    L += scene->sampleBackgroundLight2(r.dir);
  }
  C.r = K.r * L.r;
  C.g = K.g * L.g;
  C.b = K.b * L.b;

  return C;
}

} // namespace core
} // namespace ne
