#ifndef __INTEGRATOR_H_
#define __INTEGRATOR_H_

#include "neon/blueprint.hpp"

#include <glm/glm.hpp>
#include <memory>

namespace ne {

namespace core {

class Integrator {
public:
  // integration part of rendering equation
  virtual glm::vec3 integrate(const ne::Ray &ray,
                              std::shared_ptr<ne::Scene> scene);
  virtual glm::vec3 integrate2(const ne::Ray &ray,
                              std::shared_ptr<ne::Scene> scene);
};

} // namespace core

} // namespace ne

#endif // __INTEGRATOR_H_
