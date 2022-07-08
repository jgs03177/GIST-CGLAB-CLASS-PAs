#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <tinyobjloader/tiny_obj_loader.h>
#include <fstream>

#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif

double mouse_x = 0.0;
double mouse_y = 0.0;
bool check = false;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.1" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "glskeleton", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

	// TinyObjLoader
	// https://github.com/syoyo/tinyobjloader#usage

	std::string inputfile = "../../bunny.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	auto g_bunnyID = glGenLists(1);
	glNewList(g_bunnyID, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				glVertex3f(vx, vy, vz);
				glNormal3f(nx, ny, nz);
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}
	glEnd();
	glEndList();

	glm::mat4 matModel1 = glm::mat4(1.0); // I_4
	glm::mat4 matModel2 = glm::mat4(1.0);
	glm::mat4 matView = glm::lookAt(glm::vec3(0, 4, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 matProj = glm::perspective(glm::radians(60.0f), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
	bool change_color = false;

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		glfwWaitEvents();
		
		// check if highlighted.
		if (check) {
			// upper left based coordinate.
			std::cout << mouse_x << ", " << mouse_y << std::endl;
			char pixel[3];
			glReadBuffer(GL_BACK);
			// lower left based coordinate. so invert y coordinate.
			glReadPixels(mouse_x, HEIGHT-mouse_y, 1, 1, GL_RGB, GL_BYTE, pixel);
			// check if the color of the back buffer is blue.
			if (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 127){
				change_color = true;
			}
			else{
				change_color = false;
			}
			check = false;
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set projection matrix for this frame
		glMatrixMode(GL_PROJECTION); // set projection matrix
		// set the value of projection matrix
		glLoadMatrixf(glm::value_ptr(matProj)); // should include glm/gtc/type_ptr for glm::value_ptr

		// set modelview matrix for the model 1
		glm::mat4 modelView1 = matView * matModel1;
		glMatrixMode(GL_MODELVIEW);
		// use front buffer
		glDrawBuffer(GL_FRONT);
		glLoadMatrixf(glm::value_ptr(modelView1));
		// set color
		if (change_color){
			// red
			glColor3b(127, 0, 0);
		}
		else{
			// green
			glColor3b(0, 127, 0);
		}
		// draw your model 1
		glCallList(g_bunnyID);

		// set modelview matrix for the model 2
		glm::mat4 modelView2 = matView * matModel2;
		glMatrixMode(GL_MODELVIEW);
		// use back buffer
		glDrawBuffer(GL_BACK);
		glDisable(GL_LIGHTING);
		glLoadMatrixf(glm::value_ptr(modelView2));
		// set color
		// blue
		glColor3b(0, 0, 127);
		// draw your model 2
		glCallList(g_bunnyID);
		
		//draw
		glFinish();
	}

    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if ((button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_LEFT)
		&& action == GLFW_PRESS) {
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		check = true;
	}
}
