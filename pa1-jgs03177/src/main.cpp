#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

		// precalculations
		double currentTime = glfwGetTime();
		float sinT = static_cast<float>(sin(currentTime));
		float cosT = static_cast<float>(cos(currentTime));

		// initial values
		const float v1_x0 = -0.5f;
		const float v1_y0 = -0.5f;
		const float v2_x0 = 0.5f;
		const float v2_y0 = -0.5f;
		const float v3_x0 = 0;
		const float v3_y0 = 0.5f;
		const float c_r0 = 0.7f;
		const float c_g0 = 0.3f;
		const float c_b0 = 0.3f;

		// transformation
		float v1_xt = v1_x0 * cosT + v1_y0 * (-sinT);
		float v1_yt = v1_x0 * sinT + v1_y0 * cosT;
		float v2_xt = v2_x0 * cosT + v2_y0 * (-sinT);
		float v2_yt = v2_x0 * sinT + v2_y0 * cosT;
		float v3_xt = v3_x0 * cosT + v3_y0 * (-sinT);
		float v3_yt = v3_x0 * sinT + v3_y0 * cosT;

		// distinct colors~
		float c_rt = 0.5f + (c_r0 - 0.5f) * cos(currentTime / 2.0);
		float c_gt = 0.5f + (c_g0 - 0.5f) * cos(currentTime / 3.0);
		float c_bt = 0.5f + (c_b0 - 0.5f) * cos(currentTime / 5.0);

		// Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glColor3f(c_rt, c_gt, c_bt);
        glVertex3f(v1_xt, v1_yt, 0);
        glVertex3f(v2_xt, v2_yt, 0);
        glVertex3f(v3_xt, v3_yt, 0);
        glEnd();

        // Swap the screen buffers
        glfwSwapBuffers(window);
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
