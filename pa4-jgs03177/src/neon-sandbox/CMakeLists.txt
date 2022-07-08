add_executable(neon-sandbox
  main.cpp
  test.hpp
  test.cpp)

if(MSVC)
	set_target_properties(
		neon-sandbox 
		PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()

target_link_libraries(neon-sandbox
  neon
  extern::lodepng
  extern::glm
  extern::taskflow)
