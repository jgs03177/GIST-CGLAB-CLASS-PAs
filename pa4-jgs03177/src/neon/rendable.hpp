#ifndef __RENDABLE_H_
#define __RENDABLE_H_

#include "neon/blueprint.hpp"
#include "neon/intersection.hpp"
#include "neon/ray.hpp"

namespace ne {

namespace abstract {

/// Rendable is an object which can compute rayIntersection and has material
/// pointer
class Rendable {
public:
  Rendable(MaterialPointer m = nullptr) : material_(m) {}
  virtual ~Rendable() {}
  virtual bool rayIntersect(ne::Ray &ray, ne::Intersection &inter) = 0;
  virtual float sample_point(ne::Intersection& point, const ne::Intersection &inter) = 0;
  virtual float sample_point(ne::Intersection& point) = 0;

  // You need c++ 17 compiler for inline static initilization

  /// To prevent self-intersection check, you gonna need this.
  inline static constexpr float eps_ = 0.0001f;
  MaterialPointer material_ = nullptr;
};

} // namespace abstract
} // namespace ne

#endif // __RENDABLE_H_
