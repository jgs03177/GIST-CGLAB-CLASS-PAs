add_library(neon SHARED
  image.hpp
  image.cpp
  integrator.cpp
  integrator.hpp
  scene.hpp
  scene.cpp
  sphere.hpp
  sphere.cpp
  intersection.hpp
  rendable.hpp
  ray.hpp
  material.hpp
  material.cpp
  utils.hpp
  utils.cpp
  )

target_link_libraries(neon
  PUBLIC
  extern::lodepng
  extern::glm
  extern::taskflow)
