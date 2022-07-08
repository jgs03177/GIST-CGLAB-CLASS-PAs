#include "test.hpp"

#include "neon/material.hpp"
#include "neon/scene.hpp"
#include "neon/sphere.hpp"
#include <memory>

// Factory function for simple test scene1
std::shared_ptr<ne::Scene> testScene1() {
  // Define materials
  const ne::MaterialPointer mat1 =
      std::make_shared<ne::Lambertian>(glm::vec3{0.8f, 0.3f, 0.3f});
  const ne::MaterialPointer mat2 =
      std::make_shared<ne::Lambertian>(glm::vec3{0.8f, 0.8f, 0.0f});
  const ne::MaterialPointer mat3 =
      std::make_shared<ne::Metal>(glm::vec3{0.8f, 0.6f, 0.2f});
  const ne::MaterialPointer mat4 =
      std::make_shared<ne::Dielectric>(glm::vec3{0.8f, 0.8f, 0.8f}, 1.5f);
  const ne::MaterialPointer mat5 =
      std::make_shared<ne::DiffuseLight>(glm::vec3{2.0, 2.0, 2.0});

  // Define rendable geometries and bind materials
  const ne::RendablePointer s1 =
      std::make_shared<ne::Sphere>(glm::vec3(0, 0, -1), 0.5f, mat1);
  const ne::RendablePointer s2 =
      std::make_shared<ne::Sphere>(glm::vec3(0, -100.5, -1), 100.f, mat2);
  const ne::RendablePointer s3 =
      std::make_shared<ne::Sphere>(glm::vec3(1, 0, -1), 0.5f, mat3);
  const ne::RendablePointer s4 =
      std::make_shared<ne::Sphere>(glm::vec3(-1, 0, -1), 0.5f, mat4);
  const ne::RendablePointer s5 =
      std::make_shared<ne::Sphere>(glm::vec3(0, 1, -1), 0.5f, mat5);

  // Assemble the scene
  std::shared_ptr<ne::Scene> scene = std::make_shared<ne::Scene>();
  scene->add(s1);
  scene->add(s2);
  scene->add(s3);
  scene->add(s4);
  scene->add(s5);

  return scene;
}

// Factory function for simple test scene2
std::shared_ptr<ne::Scene> testScene2() {
  ne::MaterialPointer materials[] = {
      std::make_shared<ne::Lambertian>(glm::vec3(0.8f, 0.8f, 0.8f)),
      std::make_shared<ne::Lambertian>(glm::vec3(0.8f, 0.4f, 0.4f)),
      std::make_shared<ne::Lambertian>(glm::vec3(0.4f, 0.8f, 0.4f)),
      std::make_shared<ne::Metal>(glm::vec3(0.4f, 0.4f, 0.8f)),
      std::make_shared<ne::Metal>(glm::vec3(0.4f, 0.8f, 0.4f)),
      std::make_shared<ne::Metal>(glm::vec3(0.4f, 0.8f, 0.4f), 0.2f),
      std::make_shared<ne::Metal>(glm::vec3(0.4f, 0.4f, 0.4f), 0.6f),
      std::make_shared<ne::Dielectric>(glm::vec3(0.4f, 0.4f, 0.4f), 1.5f),
      std::make_shared<ne::DiffuseLight>(glm::vec3(1.2f, 1.2f, 1.2f)),
  };
  ne::RendablePointer objects[] = {
      std::make_shared<ne::Sphere>(glm::vec3(0, -100.5, -1), 100, materials[0]),
      std::make_shared<ne::Sphere>(glm::vec3(2, 0, -1), 0.5f, materials[1]),
      std::make_shared<ne::Sphere>(glm::vec3(0, 0, -1), 0.5f, materials[2]),
      std::make_shared<ne::Sphere>(glm::vec3(-2, 0, -1), 0.5f, materials[3]),
      std::make_shared<ne::Sphere>(glm::vec3(2, 0, 1), 0.5f, materials[4]),
      std::make_shared<ne::Sphere>(glm::vec3(0, 0, 1), 0.5f, materials[5]),
      std::make_shared<ne::Sphere>(glm::vec3(-2, 0, 1), 0.5f, materials[6]),
      std::make_shared<ne::Sphere>(glm::vec3(0.5f, 1.0, -1), 0.5f,
                                   materials[7]),
      std::make_shared<ne::Sphere>(glm::vec3(-1.5f, 1.5, 0), 0.3f,
                                   materials[8]),
  };

  // Assemble the scene
  std::shared_ptr<ne::Scene> scene = std::make_shared<ne::Scene>();

  for (int i = 0; i < 9; ++i) {
    scene->add(objects[i]);
  }

  return scene;
}
