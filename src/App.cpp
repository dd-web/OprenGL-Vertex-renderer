#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>

#include "App.h"
#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

void UpdateRGB(float& R, float& G, float& IG, float& IR)
{
    if (G > 1.0f)
        IG = -0.02f;
    else if (G < 0.0f)
        IG = 0.03f;

    G += IG;

    if (R > 1.0f)
        IR = -0.05f;
    else if (R < 0.0f)
        IR = 0.05f;

    R += IR;
}


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

    Print(glGetString(GL_VERSION));

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

        Renderer renderer;


        float r = 0.0f;
        float i_r = 0.05f;
        float g = 0.4f;
        float i_g = 0.01f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();
            
            shader.Bind();
            shader.SetUniform4f("u_Color", r, g, 0.4f, 1.0f);

            renderer.Draw(va, ib, shader);


            UpdateRGB(r, g, i_g, i_r); // add some color to make it more interesting

            /* Swap front and back buffers && Poll for and process events  */
            glfwSwapBuffers(window);
            glfwPollEvents();
        };

    }
    glfwTerminate();
    return 0;
}