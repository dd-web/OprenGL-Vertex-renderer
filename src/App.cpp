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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// imgui
//#include <GLFW/glfw3.h>
//#include <GL/glew.h>
#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif



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
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK)
    {
        Print("Error instantiating glew");
        return -1;
    }

    Print(glGetString(GL_VERSION));

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    glm::vec3 translation(200, 200, 0);


    ImVec4 clear_color = ImVec4(0.45f, 0.45f, 0.5f, 1.00f);

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

        glm::mat4 proj = glm::ortho(0.0f, w_Width, 0.0f, w_Height, -1.0f, 1.0f);        // projection matrix - 4/3
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));        // view matrix

    
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
      
        Texture texture("res/textures/cool.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float frameTime = 0.0f;
        float frameValue = 0.8f;
        float frameAlpha = 0.1f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();
            
            ImGui_ImplGlfwGL3_NewFrame();
     
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 mvp = proj * view * model;

            shader.Bind();
            //shader.SetUniform4f("u_Color", 0.2f, 0.6f, 0.5f, 0.8f);
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);


            if (frameTime > 100.0f)
            {
                frameValue = -0.75f;
                frameAlpha = -0.1f;
            }
            else if (frameTime < 0.0f)
            {
                frameValue = 0.60f;
                frameAlpha = 0.1f;
            }

            frameTime += frameValue;



            // debug menu context
            {
                static float f = 0.0f;
                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, w_Width);

                static int counter = 0;
                ImGui::ColorEdit3("clear color", (float*)&clear_color); 

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }


            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwPollEvents();
            glfwSwapBuffers(window);
        };

    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}