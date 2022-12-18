# OpenGL Vertex Buffer Renderer

Created by **David Duncan**

```
an extremely simple, bare-bones OpenGL vertex renderer & shader program compiler 
```


## About

```
This project was created for learning and improvement purposes, please do not mistake this for a valid option of OpenGL renderers.
```

While I created this for my own sake, feel free to make any improvement suggestions or even pull requests if you like. Feel free
to open up an issue for anything else. it's not very complex or anything, it renders tris defined by a vertex buffer of verticies.
color can be controlled through shader uniforms


---
## GPU calls & frame states

constructors & state manipulation should be called **outside** of the frame render loop.
ocne state is set, bind to the vertex array buffer directly after the `GLCall(glClear())` call.
binding vertex buffers to the gl context queues a frame from the gpu.

### Vertex buffer collections

you will probable want to implement your own abstracted classes for larger collections of vertex array buffers.
to create and bind an array of vertex buffers create the VertexBuffer with the data[0] and the byte size[1] of the
data collection. ex:

```c++
float loc[] = {
  0.5f, -0.5f, -0.5f,
 };

VertexArray varr;
VertexBuffer vbuff(loc, 1 * 3 * sizeof(float));     // (g * s * t), g = number of verticies that make up one 'obj'
                                                    // s = number of 'objs' in this collection, and t = byte size of the datatype.
```

then you'll need a vertex buffer layout which is used for constructing the verticies before binding to the GL buffer.

```c++
VertexBufferLayout layout;

layout.Push<float>(1, GL_FLOAT, false);       // <T>(uint32_t C, T type, bool normalize)
varr.AddBuffer(vbuff, layout);                // the vertex array AddBuffer takes in the buffer and it's layout 
```

the last thing we need is an index buffer, theres a lot of ways to do this,
here each number is an index to a

```c++
uint32_t indicies[] = { 0, 1, 2 };

IndexBuffer idxbuff(indicies, 3);              // takes the data and count of references (overall)
```

that's it for the data structures, the last thing to do before you'll be able to see the output is to bind the vertex array under the `glClear` call
but above the `glfwSwapBuffers` or any other destructor methods.

```c++
 GLCall(glClear(GL_COLOR_BUFFER_BIT));
 
 varr.Bind();
 idxbuff.Bind();
 
 glfwSwapBuffers(window);
```
## Shaders

place your shader program in the `res/shaders/` directory and name it `Basic.shader`.

each shader program should contain a `position` shader and `fragment` shader. these are defined
with the `#shader <type>` token to parse and compile them so they can be bound to the vertex buffer.


---

[OpenGL]: https://docs.gl/
[GLFW]: https://www.glfw.org/
