#include "neon/material.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>
#include <random>

#include <iostream>

namespace ne {

bool DiffuseLight::scatter(const ne::Ray &r_in, const ne::Intersection &hit,
                           ne::Ray &r_out) const {
  // go through everything
  // r_out.o = hit.p;
  // r_out.dir = r_in.dir;
  // r_out.t = std::numeric_limits<float>::max();
  return false;
}

glm::vec3 DiffuseLight::emitted() const {
  // emits color
  // return glm::vec3(0.0f);
  return color_;
}

glm::vec3 DiffuseLight::attenuation() const {
  // has no color
  return glm::vec3(1.0f);
}

// dark
float DiffuseLight::brdf(const glm::vec3 &n, const glm::vec3 &wi,
                         const glm::vec3 &wo) const {
  return 0;
}

bool Dielectric::scatter(const ne::Ray &r_in, const ne::Intersection &hit,
                         ne::Ray &r_out) const {
  r_out.o = hit.p;
  r_out.t = std::numeric_limits<float>::max();
  float ni_over_no;
  glm::vec3 normal_vector;

  float costh = glm::dot(r_in.dir, hit.n);
  if (costh < 0) { // outside of a sphere
    ni_over_no = 1 / IOR_;
    normal_vector = hit.n;
  } else { // inside of a sphere
    ni_over_no = IOR_;
    normal_vector = -hit.n;
    costh = -costh;
  }

  float k = 1 - ni_over_no * ni_over_no * (1 - costh * costh);
  bool refraction = false;
  // check if total reflection (k < 0)
  if (k >= 0) {
    // schlick approximation
    // cos_proj = -costh
    float coeff = 1 + costh;
    float coeff_p5 = coeff * coeff * coeff * coeff * coeff;
    float Rth = R0 + (1 - R0) * coeff_p5; // reflection coefficient at theta

    // refractivity = 1 - reflectivity (consider as probability)
    float random_scalar = glm::linearRand(0.0f, 1.0f);
    if (Rth < random_scalar) {
      refraction = true;
    }
  }

  if (refraction) {
    // refraction
    r_out.dir = ni_over_no * (r_in.dir - normal_vector * costh) -
                normal_vector * glm::sqrt(k);
  } else {
    // reflection
    r_out.dir = r_in.dir - 2 * costh * normal_vector;
  }
  return true;
}

glm::vec3 Dielectric::attenuation() const { return color_; }

float Dielectric::brdf(const glm::vec3 &n, const glm::vec3 &wi,
                       const glm::vec3 &wo) const {
  float ni_over_no;
  glm::vec3 normal_vector;
  float costh = glm::dot(wi, n);
  if (costh > 0) { // outside of a sphere
    ni_over_no = 1 / IOR_;
    normal_vector = n;
  } else { // inside of a sphere
    ni_over_no = IOR_;
    normal_vector = -n;
    costh = -costh;
  }

  bool same_side = costh * glm::dot(n, wo) > 0;

  glm::vec3 wi_mirror = -wi + 2 * glm::dot(normal_vector, wi) * normal_vector;
  glm::vec3 wi_refract(0, 0, 0);

  float k = 1 - ni_over_no * ni_over_no * (1 - costh * costh);
  // Rth = Reflectivity
  float Rth = 1; // total reflection Rth=1 when k < 0
  if (k >= 0) {
    // schlick approximation
    float coeff = 1 + costh;
    float coeff_p5 = coeff * coeff * coeff * coeff * coeff;
    Rth = R0 + (1 - R0) * coeff_p5; // reflection coefficient at theta

    glm::vec3 wi_refract = ni_over_no * (-wi + normal_vector * costh) -
                           normal_vector * glm::sqrt(k);
  }

  float reflection_specular = 0;
  float refraction_specular = 0;
  // reflection
  if (same_side) {
    float costh = glm::dot(wo, wi_mirror);
    float tanth = glm::sqrt(1 - costh * costh) / costh;
    // specular : tan(theta) < specular_
    // specular_ in 1 degree
    float specular_ = 0.1f;
    reflection_specular = (costh > 0 && 0 < tanth && tanth < specular_)
                              ? 1 / (specular_ * specular_ * glm::pi<float>())
                              : 0;
    // refraction
  } else {
    float costh = glm::dot(wo, wi_refract);
    float tanth = glm::sqrt(1 - costh * costh) / costh;
    // specular : tan(theta) < specular_
    // specular_ in 1 degree
    float specular_ = 0.1f;
    refraction_specular = (costh > 0 && 0 < tanth && tanth < specular_)
                              ? 1 / (specular_ * specular_ * glm::pi<float>())
                              : 0;
  }
  return Rth * reflection_specular + (1 - Rth) * refraction_specular;
}

bool Lambertian::scatter(const ne::Ray &r_in, const ne::Intersection &hit,
                         ne::Ray &r_out) const {
  // return false;

  r_out.o = hit.p;
  glm::vec3 random_vector = glm::sphericalRand(1.0f);
  // flip random vector.
  random_vector *= glm::sign(glm::dot(random_vector, hit.n));
  r_out.dir = random_vector;
  return true;
}

glm::vec3 Lambertian::attenuation() const { return color_; }

float Lambertian::brdf(const glm::vec3 &n, const glm::vec3 &wi,
                       const glm::vec3 &wo) const {
  return (glm::dot(n, wi) > 0 && glm::dot(n, wo) > 0)
             ? 2 * glm::one_over_pi<float>()
             : 0;
}

bool Metal::scatter(const ne::Ray &r_in, const ne::Intersection &hit,
                    ne::Ray &r_out) const {
  r_out.o = hit.p;
  r_out.t = std::numeric_limits<float>::max();
  float random_scalar = glm::linearRand(0.0f, 1.0f);
  // roughness
  if (random_scalar < roughness_) {
    // lambertian
    // return false;

    glm::vec3 random_vector = glm::sphericalRand(1.0f);
    // flip random vector.
    random_vector *= glm::sign(glm::dot(random_vector, hit.n));
    r_out.dir = random_vector;
    return true;
  }
  // specular
  glm::vec2 circleconst = glm::diskRand(specular_);
  glm::vec3 out_vec = r_in.dir - 2 * glm::dot(hit.n, r_in.dir) * hit.n;
  glm::vec3 u = glm::cross(hit.n, out_vec);
  glm::vec3 v = glm::cross(out_vec, u);
  r_out.dir = glm::normalize(out_vec + glm::mat2x3(u, v) * circleconst);
  return true;
}

glm::vec3 Metal::attenuation() const { return color_; }

float Metal::brdf(const glm::vec3 &n, const glm::vec3 &wi,
                  const glm::vec3 &wo) const {
  if (glm::dot(n, wi) > 0 && glm::dot(n, wo) > 0) {
    glm::vec3 wi_mirror = -wi + 2 * glm::dot(n, wi) * n;
    float costh = glm::dot(wo, wi_mirror);
    float tanth = glm::sqrt(1 - costh * costh) / costh;
    float lambertian = 2 * glm::one_over_pi<float>();
    // specular : tan(theta) < specular_
    float specular = (costh > 0 && 0 < tanth && tanth < specular_)
                         ? 1 / (specular_ * specular_ * glm::pi<float>())
                         : 0;
    return roughness_ * lambertian + (1 - roughness_) * specular;
  } else {
    return 0;
  }
}

} // namespace ne
