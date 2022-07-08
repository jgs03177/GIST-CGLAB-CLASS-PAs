#include "neon/material.hpp"
#include "neon/scene.hpp"

#include <glm/gtx/string_cast.hpp>

#include <iostream>

namespace ne {

void Scene::add(ne::RendablePointer object) {
  objects_.push_back(object);
  if (glm::length(object->material_->emitted()) > 0.0f) {
    lights_.push_back(object);
  }
}

// if hit, returns true and update inter to the nearest intersection.
bool Scene::rayIntersect(ne::Ray &ray, ne::Intersection &inter) {
  bool foundIntersection = false;

  // Do not change order between a || b.
  // sometimes result(a || b) != result (b || a) and this is the case!
  // this is because b will not evaluated if a is true in this statment.
  // e.g. a = a || b (b will not be evaluated because a is true)
  for (const auto o : objects_) {
    foundIntersection = o->rayIntersect(ray, inter) || foundIntersection;
  }

  return foundIntersection;
}

glm::vec3 Scene::sampleBackgroundLight(const glm::vec3 &dir) const {
  glm::vec3 unit = glm::normalize(dir);
  float t = 0.5f * (unit.y + 1.0f);
  return ((1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5, 0.5, 0.9));
}

glm::vec3 Scene::sampleBackgroundLight2(const glm::vec3 &dir) const {
  return glm::vec3(0.2, 0.2, 0.2);
}

glm::vec3 Scene::sampleDirectLight(ne::Ray &ray, ne::Intersection &hit) const {
  glm::vec3 Ls_sum = glm::vec3(0.0f);
  glm::vec3 wo = -ray.dir;
  for (auto l : lights_) {
    ne::Intersection point;
    ne::Intersection inter;
    float prob = l->sample_point(point, hit);
    glm::vec3 wi(point.p - hit.p);
    glm::vec3 win = glm::normalize(wi);
    ne::Ray ray_wi(hit.p, win);
    
	// check if the ray hits light (ie hitpoint and lightpoint can see each other).
    bool visible = l->rayIntersect(ray_wi, inter);
    float t = ray_wi.t;
	for (const auto o : objects_) {
      o->rayIntersect(ray_wi, inter);
	}
	// if not, skip.
    if (ray_wi.t < t) {
      continue;
	}

    glm::vec3 Ls = l->material_->emitted();
    float brdf = hit.material->brdf(hit.n, win, wo);
    float costh1 = glm::dot(hit.n, win);
    float costh2 = glm::abs(glm::dot(point.n, -win));
    float distancesq = glm::dot(wi, wi);
    Ls_sum += brdf * Ls * costh1 * costh2 / distancesq;
    //std::cout << Ls_sum.x << Ls_sum.y << Ls_sum.z << std::endl;
  }
  
  return Ls_sum;
}

} // namespace ne
