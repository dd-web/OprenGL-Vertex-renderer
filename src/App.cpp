#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>

#include "App.h"
#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize GLFW & set version + params */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);              
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "OpenGL Render Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK)
    {
        Print("Error instantiating glew");
        return -1;
    }

    Print(glGetString(GL_VERSION)); // if we have a window, log our version

    {
        /* initialize some verticies to use */
        float positions[] = {
            -0.5f,  -0.5f,  
             0.5f,  -0.5f,  
             0.5f,   0.5f,  
            -0.5f,   0.5f
        };

        uint32_t indices[] = {
                0, 1, 2,
                2, 3, 0
        };

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
 
        VertexBufferLayout layout;
        layout.Push<float>(2, GL_FLOAT, false);
        va.AddBuffer(vb, layout);
 
        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.9f, 0.3f, 0.4f, 0.7f);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();


        float r = 0.0f;
        float g = 0.4f;
        float increment = 0.05f;
        float incrementA = 0.01f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            shader.Bind();
            shader.SetUniform4f("u_Color", r, g, 0.4f, 1.0f);

            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (g > 1.0f)
                incrementA = -0.02f;
            else if (g < 0.0f)
                incrementA = 0.03f;

            g += incrementA;

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers && Poll for and process events  */
            glfwSwapBuffers(window);
            glfwPollEvents();
        };

    }
    glfwTerminate();
    return 0;
}