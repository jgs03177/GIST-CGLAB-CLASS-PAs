include(FetchContent)

# define external projects
# FetchContent_Declare(
#   glfw
#   GIT_REPOSITORY https://github.com/glfw/glfw.git
#   GIT_TAG 3.2)

# FetchContent_Declare(
#   glad
#   GIT_REPOSITORY https://github.com/Dav1dde/glad.git
#   GIT_TAG v0.1.29)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.5)

FetchContent_Declare(
  cpp-taskflow
  GIT_REPOSITORY https://github.com/cpp-taskflow/cpp-taskflow.git
  GIT_TAG v2.1.0
  )


# fetch and import external projects
# FetchContent_GetProperties(glfw)
# if(NOT glfw_POPULATED)
#   FetchContent_Populate(glfw)
#   set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#   set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#   add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
#   add_library(extern::glfw ALIAS glfw)
# endif()

# FetchContent_GetProperties(glad)
# if(NOT glad_POPULATED)
#   FetchContent_Populate(glad)
#   add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})
#   add_library(extern::glad ALIAS glad)
# endif()

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
  FetchContent_Populate(glm)
  set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
  add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
  add_library(extern::glm ALIAS glm)
endif()

FetchContent_GetProperties(cpp-taskflow)
if(NOT cpp-taskflow_POPULATED)
  FetchContent_Populate(cpp-taskflow)
  set(TF_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(TF_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  add_subdirectory(${cpp-taskflow_SOURCE_DIR} ${cpp-taskflow_BINARY_DIR})

  find_package(Threads REQUIRED)
  target_link_libraries(Cpp-Taskflow INTERFACE Threads::Threads)
  add_library(extern::taskflow ALIAS Cpp-Taskflow)
endif()

add_library(lodepng SHARED
  lodepng/lodepng.h
  lodepng/lodepng.cpp)
target_include_directories(lodepng
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR})
add_library(extern::lodepng ALIAS lodepng)


