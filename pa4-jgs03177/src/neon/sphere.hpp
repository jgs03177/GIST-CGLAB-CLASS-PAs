#ifndef __SPHERE_H_
#define __SPHERE_H_

#include "neon/blueprint.hpp"
#include "neon/rendable.hpp"

namespace ne {

class Sphere final : public ne::abstract::Rendable {
public:
  explicit Sphere(glm::vec3 c, float r, MaterialPointer m = nullptr)
      : ne::abstract::Rendable(m), center_(c), radius_(r) {}

  bool rayIntersect(ne::Ray &ray, Intersection &hit) override;
  
  // sample a point of this sphere to use in direct light sampling.
  // we sample point that can be seen by ray if rayintersection is given.
  // returns probability and point.
  float sample_point(ne::Intersection& point, const ne::Intersection &inter) override;
  float sample_point(ne::Intersection& point) override;

  glm::vec3 center_;
  float radius_;
};

} // namespace ne
#endif // __SPHERE_H_
