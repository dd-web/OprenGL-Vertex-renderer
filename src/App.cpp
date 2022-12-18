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
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

    float w_Width = 960, w_Height = 540;

    GLFWwindow* window;

    /* Initialize GLFW & set version + params */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);              
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "OpenGL Render Window", NULL, NULL);
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

        float positions[] = {
            100.0f,   100.0f,   0.0f,   0.0f,
            200.0f,   100.0f,   1.0f,   0.0f,
            200.0f,   200.0f,   1.0f,   1.0f,
            100.0f,   200.0f,   0.0f,   1.0f
        };

        uint32_t indices[] = {
                0, 1, 2,
                2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
 
        VertexBufferLayout layout;
        layout.Push<float>(2, GL_FLOAT, false);
        layout.Push<float>(2, GL_FLOAT, false);
        va.AddBuffer(vb, layout);
 
        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f, w_Width, 0.0f, w_Height, -1.0f, 1.0f); // 4/3 aspect ratio

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.9f, 0.3f, 0.4f, 0.7f);
        shader.SetUniformMat4f("u_MVP", proj);

        Texture texture("res/textures/cool.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

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