# Neon

Neon is a minimal ray tracer written in C++. Originally, It was wrrtien for my own learning
purpose. This is part of the ray tracer which can be used as base code for assignment in 
the Computer Graphics course at GIST.


# Installation 

```sh
git clone  https://github.com/CGLAB-Classes/neon-edu.git
cd neon
mkdir build 
cmake ..              # this might take a while due to installation of external library
make -j4
```


# Dependancies

- [lodepng](https://github.com/lvandeve/lodepng), Very simple png read/write library
- [glm](https://github.com/g-truc/glm.git), Math library for OpenGL and GLSL shader
- [taskflow-cpp](https://github.com/cpp-taskflow/cpp-taskflow), Task-based
  parallelism library


# System Requirements 

To use Neon, you need a C++ compiler which supports C++17 and cmake.

- C++ Compiler
  - GCC  >= 7.3 
  - Clang >= 6.0
  - Visual Studio Version >= 15.7 
- Cmake > 3.13


# License 

Neon is licensed under the [MIT License](https://github.com/CGLAB-Classes/neon-edu/blob/master/LICENSE)




