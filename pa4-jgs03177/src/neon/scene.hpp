#ifndef __SCENE_H_
#define __SCENE_H_

#include "neon/blueprint.hpp"
#include "neon/intersection.hpp"
#include "neon/rendable.hpp"

#include <memory>
#include <vector>

namespace ne {

class Scene {

public:
  void add(RendablePointer object);
  glm::vec3 background(ne::Ray &ray);
  bool rayIntersect(ne::Ray &ray, ne::Intersection &hit);
  glm::vec3 sampleBackgroundLight2(const glm::vec3 &dir) const;
  glm::vec3 sampleDirectLight(ne::Ray &ray, ne::Intersection &hit) const;
  glm::vec3 sampleBackgroundLight(const glm::vec3 &dir) const;

private:
  std::vector<ne::RendablePointer> objects_;
  std::vector<ne::RendablePointer> lights_;
};

} // namespace ne

#endif // __SCENE_H_
