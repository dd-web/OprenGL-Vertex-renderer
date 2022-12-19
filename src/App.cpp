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

#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif


int main(void)
{

    float w_Width = 960, w_Height = 540;
    const char* w_Title("Window title");

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);              
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, w_Title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error initializing glew" << std::endl;
        return -1;
    }
    

    Print(glGetString(GL_VERSION));

    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();



    
    {/* scope for classes instantiated before frame loop to let their destructors run  */

        float positions[] = {
            -50.0f,  -50.0f,   0.0f,   0.0f,
             50.0f,  -50.0f,   1.0f,   0.0f,
             50.0f,   50.0f,   1.0f,   1.0f,
            -50.0f,   50.0f,   0.0f,   1.0f
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

        layout.Push<float>(2, GL_FLOAT, false); // vertex
        layout.Push<float>(2, GL_FLOAT, false); // texture
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f, w_Width, 0.0f, w_Height, -1.0f, 1.0f);        // projection matrix
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));           // view matrix

    
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.5f, 0.9f, 0.1f, 1.0f);
      
        Texture texture("res/textures/cool.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        
        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 100, 0);

        ImVec4 clear_color = ImVec4(0.45f, 0.45f, 0.5f, 1.00f);

        float c_r = 0.0f;
        float c_step = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();
            
            ImGui_ImplGlfwGL3_NewFrame();
     

            {// A
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            {// B
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }



            //shader.SetUniform4f("u_Color", c_r, 0.2f, 0.5f, 1.0f);


            if (c_r > 1.0f)
                c_step = -0.05f;
            else if (c_r < 0.0f)
                c_step = 0.05f;

            c_r += c_step;

 
            {/* Debug window scope */
                static float f = 0.0f;
                ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, w_Width);
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, w_Width);

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


    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}