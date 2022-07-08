# glad
add_subdirectory(glad)
if(TARGET glad)
  # Part of the main project
  add_library(glad::glad ALIAS glad)
else()
  # Stand-alone build
  find_package(glad CONFIG REQUIRED)
endif()

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw) # has glfw target
if(TARGET glfw)
  # Part of the main project
  add_library(glfw::glfw ALIAS glfw)
else()
  # Stand-alone build
  find_package(glfw CONFIG REQUIRED)
endif()


# GLM
set(GLM_TEST_ENABLE OFF)
add_subdirectory(glm)
add_library(glm::glm ALIAS glm)


# tinyobjloader
add_library(tinyobjloader
  tinyobjloader/tinyobjloader/tiny_obj_loader.cc
  tinyobjloader/tinyobjloader/tiny_obj_loader.h)
target_include_directories(tinyobjloader
  PUBLIC
  tinyobjloader)
add_library(tinyobjloader::tinyobjloader ALIAS tinyobjloader)
