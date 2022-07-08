#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include "neon/blueprint.hpp"
#include "neon/intersection.hpp"
#include "neon/ray.hpp"

namespace ne {

namespace abstract {

// Absract material class for inteface
// you can add/change variables/methods if you want
class Material {
public:
  virtual bool scatter(const ne::Ray &r_in, const ne::Intersection &hit,
                       ne::Ray &r_out) const = 0;

  virtual glm::vec3 emitted() const { return glm::vec3{0, 0, 0}; }

  virtual glm::vec3 attenuation() const = 0;

  virtual float brdf(const glm::vec3& n, const glm::vec3& wi, const glm::vec3& wo) const = 0;

protected:
};

} // namespace abstract

/*  Actual implementation of materials  */
// you can add/change variables/methods if you want

// Light material which glow unifomly.
class DiffuseLight final : public ne::abstract::Material {
public:
  DiffuseLight(const glm::vec3 &color = glm::vec3(1.0)) : color_(color) {}

  bool scatter(const ne::Ray &r_in, const ne::Intersection &hit,
               ne::Ray &r_out) const override;

  glm::vec3 emitted() const override;

  glm::vec3 attenuation() const override;

  float brdf(const glm::vec3 &n, const glm::vec3 &wi, const glm::vec3 &wo) const override;

private:
  glm::vec3 color_;
};

// material which obeys snells raw
//
class Dielectric final : public ne::abstract::Material {
public:
  Dielectric(const glm::vec3 &color = glm::vec3(0.7), float IOR = 0.7f)
      : color_(color), IOR_(IOR) {
    R0 = ((IOR_ - 1) / (IOR_ + 1)) * ((IOR_ - 1) / (IOR_ + 1));
  }

  bool scatter(const ne::Ray &r_in, const ne::Intersection &hit,
               ne::Ray &r_out) const override;

  glm::vec3 attenuation() const override;
  
  float brdf(const glm::vec3 &n, const glm::vec3 &wi, const glm::vec3 &wo) const override;

protected:
  glm::vec3 color_;
  // refraction index
  float IOR_;
  float R0;
};

// material which reflect ray toward random direction
class Lambertian final : public ne::abstract::Material {
public:
  Lambertian(const glm::vec3 color = glm::vec3{0.f, 0.f, 0.f})
      : color_(color) {}

  bool scatter(const ne::Ray &r_in, const ne::Intersection &hit,
               ne::Ray &r_out) const override;

  glm::vec3 attenuation() const override;

  float brdf(const glm::vec3 &n, const glm::vec3 &wi, const glm::vec3 &wo) const override;

protected:
  glm::vec3 color_;
};

// metal obeys raw of reflection
class Metal final : public ne::abstract::Material {
public:
  Metal(const glm::vec3 color = glm::vec3{0.0f, 0.0f, 0.f}, float blurr = 0.2f, float specular = 0.2f)
      : color_(color), roughness_(glm::clamp(blurr, 0.0f, 1.0f)), specular_(specular) {}

  bool scatter(const ne::Ray &r_in, const ne::Intersection &hit,
               ne::Ray &r_out) const override;

  glm::vec3 attenuation() const override;

  float brdf(const glm::vec3 &n, const glm::vec3 &wi, const glm::vec3 &wo) const override;

protected:
  glm::vec3 color_;
  // roughness == 0 is mirror-like 
  // roughness == 1 is lambertian-like
  float roughness_; 
  // a
  float specular_;
};

} // namespace ne

#endif // __MATERIAL_H_
