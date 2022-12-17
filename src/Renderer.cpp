#include "Renderer.h"
#include <iostream>


/* Clears errors from the previous frame */
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

/* Sets a breakpoint on exception and logs details about it */
bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}