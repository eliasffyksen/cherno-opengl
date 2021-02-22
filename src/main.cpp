
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "log.hpp"
#include "shader.hpp"
#include "error.hpp"

int main(void)
{
    LOG("Creating window");
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(640, 480, "Cherno-OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        FATAL_ERR("Failed to initialize GLEW");

    LOG("Window created, GLEW initialized");
    LOG("Using OpenGL version: " << glGetString(GL_VERSION));

    float positions[] = {
        -0.5f, 0.5f,  // TR
        0.5f, 0.5f,   // TL
        -0.5f, -0.5f, // BR
        0.5f, -0.5f,  // BL
    };

    uint32_t indices[] = {
        0,
        1,
        2,
        1,
        3,
        2,
    };

    GLuint vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    GLuint buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

    GLuint ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GLuint shader = createShader(ShaderSource("./res/shaders/basic.glsl"));
    GLCall(glUseProgram(shader));

    GLCall(GLint u_Color = glGetUniformLocation(shader, "u_Color"));
    if (u_Color == -1)
        FATAL_ERR("Invalid uniform location");

    float r = 0.0f;
    float r_inc = 0.05f;

    LOG("Running game loop");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (r > 1.0f)
        {
            r_inc = -0.05f;
        }
        else if (r < 0.0f)
        {
            r_inc = 0.05f;
        }
        r += r_inc;

        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUniform4f(u_Color, r, 0.0f, 1.0f, 1.0f));

        GLCall(glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    GLCall(glDeleteProgram(shader));

    LOG("Terminating");

    glfwTerminate();
    return 0;
}