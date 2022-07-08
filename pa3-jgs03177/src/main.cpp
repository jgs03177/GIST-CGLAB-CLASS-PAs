#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>
#include <tinyobjloader/tiny_obj_loader.h>

#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif

// callback
double mouse_x = 0.0;
double mouse_y = 0.0;
double scroll_dy = 0.0;
bool clicked = false;
bool clicking = false;
bool scrolled = false;
bool zin = false;
bool zout = false;
// point light / directional light
bool plon = true;
bool dlon = true;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

void drawobj(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes);
glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

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
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

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
	drawobj(attrib, shapes);
	glEnd();
	glEndList();

	glm::mat4 matModel1 = glm::mat4(1.0); // I_4
	glm::vec3 axisCam = glm::vec3(0, 1, 1);  // camera axis (direction from origin)
	float dCam = 5.5;  // camera distance
	float degFoV = 60.0f;  // camera FoV

	float sphere_r = 2.0f;
	glm::vec3 rotstart;

	float pointLight[] = { 0.0, 10.0, 0.0, 1.0 }; // point light
	float directionalLight[] = { 0.0, -10.0, 0.0, 0.0 }; // directional light (w == 0.0)
	
	float La[] = { 0.1, 0.1, 0.1, 1.0 };
	float Ld[] = { 1.0, 1.0, 1.0, 1.0 };
	float Ls[] = { 1.0, 1.0, 1.0, 1.0 };

	float ka[] = { 0.5, 0.0, 0.1, 1.0 };
	float kd[] = { 0.5, 0.0, 0.1, 1.0 };
	float ks[] = { 0.7, 0.3, 0.7, 1.0 };
	
	float shininess[] = { 30.0 };

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		glfwWaitEvents();

		// dolling
		if (scrolled) {
			float dCamNew = dCam - static_cast<float>(0.5f * scroll_dy);
			if (dCamNew > 0) {
				dCam = dCamNew;
			}
			std::cout << "cam distance : " << dCam << std::endl;
			scrolled = false;
		}

		// zooming
		if (zin || zout) {
			float degFoVNew = degFoV + 10.0f * zout - 10.0f * zin;
			// if FoV is out of range, do not change FoV
			if (0.01f < degFoVNew && degFoVNew < 179.99f) {
				degFoV = degFoVNew;
			}
			std::cout << "Fov : " << degFoV << "degree" << std::endl;
			zin = false;
			zout = false;
		}

		// trackballcam.
		if (clicked || clicking) {
			// upper left based coordinate.
			std::cout << mouse_x << ", " << mouse_y << std::endl;

			float sphere_x = (1.0f - mouse_x * 2/WIDTH);
			float sphere_y = (1.0f - mouse_y * 2/HEIGHT);
			float sphere_z = glm::sqrt(sphere_r * sphere_r - (sphere_x * sphere_x + sphere_y * sphere_y));
			glm::vec3 rotdest = glm::vec3(sphere_x, sphere_y, sphere_z);
			if (!clicking) {
				clicking = true;
			}
			else {
				const glm::mat4 R = glm::toMat4(RotationBetweenVectors(rotstart, rotdest));
				axisCam = R * glm::vec4(axisCam.x, axisCam.y, axisCam.z, 1.0);
			}
			rotstart = rotdest;
			clicked = false;
		}

		glm::mat4 matView = glm::lookAt(dCam * glm::normalize(axisCam), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 matProj = glm::perspective(glm::radians(degFoV), (float)WIDTH / HEIGHT, 0.1f, 100.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set projection matrix for this frame
		glMatrixMode(GL_PROJECTION); // set projection matrix
		// set the value of projection matrix
		glLoadMatrixf(glm::value_ptr(matProj)); // should include glm/gtc/type_ptr for glm::value_ptr

		// set modelview matrix for the model 1
		glm::mat4 modelView1 = matView * matModel1;
		glMatrixMode(GL_MODELVIEW);
		glDrawBuffer(GL_FRONT);

		// lighting
		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_POSITION, pointLight);
		glLightfv(GL_LIGHT0, GL_AMBIENT, La);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, Ld);
		glLightfv(GL_LIGHT0, GL_SPECULAR, Ls);
		if (plon) {
			glEnable(GL_LIGHT0);
		}
		else {
			glDisable(GL_LIGHT0);
		}
		glLightfv(GL_LIGHT1, GL_POSITION, directionalLight);
		glLightfv(GL_LIGHT1, GL_AMBIENT, La);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, Ld);
		glLightfv(GL_LIGHT1, GL_SPECULAR, Ls);
		if (dlon) {
			glEnable(GL_LIGHT1);
		}
		else {
			glDisable(GL_LIGHT1);
		}
		glLoadMatrixf(glm::value_ptr(modelView1));
		// draw your model 1 (front = red if highlighted, blue if not)
		glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
		glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glColor3b(127, 0, 0);
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
  // close window
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (action == GLFW_PRESS) {
	  switch (key) {
	  case GLFW_KEY_1:
		  // point light on
		  plon = true;
		  break;

	  case GLFW_KEY_2:
		  // point light off
		  plon = false;
		  break;

	  case GLFW_KEY_3:
		  // directional light on
		  dlon = true;
		  break;

	  case GLFW_KEY_4:
		  // directional light off
		  dlon = false;
		  break;

	  case GLFW_KEY_Q:
		  // zoom in
		  zin = true;
		  break;

	  case GLFW_KEY_W:
		  // zoom out
		  zout = true;
		  break;

	  default:
		  // do nothing
		  break;
	  }
  }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_LEFT)
	{
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		if (action == GLFW_PRESS) {
			clicked = true;
		}
		else if (action == GLFW_RELEASE) {
			clicking = false;
		}
	}
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	mouse_x = xpos;
	mouse_y = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	scroll_dy = yoffset;	// either -1 or 1
	scrolled = true;
	//std::cout << xoffset << ", " << yoffset << std::endl;
}

void drawobj(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes) {
	// Loop over shapes
	size_t shapesize = shapes.size();
	glBegin(GL_TRIANGLES);
	for (size_t s = 0; s < shapesize; s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		size_t verticesize = shapes[s].mesh.num_face_vertices.size();
		for (size_t f = 0; f < verticesize; f++) {
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
				//tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				//tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				// Optional: vertex colors
				// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
				// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
				// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
				
				glNormal3f(nx, ny, nz);
				glVertex3f(vx, vy, vz);
				//glTexCoord2f(tx, ty);
			}
			index_offset += fv;

			// per-face material
			// shapes[s].mesh.material_ids[f];
		}
	}
}

// reference
// 1st: http://www.opengl-tutorial.org/kr/intermediate-tutorials/tutorial-17-quaternions/
// 2nd: pa3.pdf

glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {
		// special case when vectors in opposite directions:
		// there is no ideal rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length2(rotationAxis) < 0.01) { // bad luck, they were parallel, try again!
			rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
		}

		rotationAxis = normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(s * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);
}