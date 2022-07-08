#ifndef __RAY_H_
#define __RAY_H_

#include <glm/glm.hpp>
#include <limits>
#include <cassert>

namespace ne {

class Ray {
 public:
  explicit Ray(glm::vec3 origin = glm::vec3(0.0f),
               glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f))
      : o(origin), dir(glm::normalize(direction))
  {
    
  }

  glm::vec3 o;
  glm::vec3 dir;
  float t = std::numeric_limits<float>::max();


  inline glm::vec3 at(float t_eval) const {
    return o + dir * t_eval;
  }

  inline glm::vec3 eval() const {
    return o + dir * t;
  }
};




} // namespace ne

#endif // __RAY_H_
