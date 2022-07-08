add_executable(glskeleton
  main.cpp)

target_link_libraries(glskeleton
  PRIVATE
  glfw::glfw
  glad::glad
  glm::glm
  tinyobjloader::tinyobjloader
  )
