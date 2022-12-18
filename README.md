# OpenGL Vertex Buffer Renderer

![image](https://user-images.githubusercontent.com/63762367/208301983-565a4faf-e8e3-42c5-bfcb-21fdbea11d05.png)

it's at a pretty nice spot for starting new projects without having to do a ton of work, without any real application specific operations

## About

```
This project was created for learning and improvement purposes, please do not mistake this for a valid option of OpenGL renderers.
```

While I created this for my own sake, feel free to make any improvement suggestions or even pull requests if you like. Feel free
to open up an issue for anything else. it's not very complex or anything, it renders tris defined by a vertex buffer of verticies.
color can be controlled through shader uniforms


for more OpenGL resources check out these [opengl docs](https://docs.gl/) or the [glew docs](https://www.glfw.org/) which also has dlls available
if you'd rather include it that way.



## GPU calls & frame states

constructors & state manipulation should be called _outside_ of the frame render loop.
ocne state is set, bind to the vertex array buffer directly after the `GLCall(glClear())` call.
binding vertex buffers to the gl context queues a frame from the gpu.

### Vertex buffer collections

you will probable want to implement your own abstracted classes for larger collections of vertex array buffers.
to create and bind an array of vertex buffers create the VertexBuffer with the data[0] and the byte size[1] of the
data collection. ex:

```c++
 float verts[] = {
            -0.5f,  -0.5f,  
             0.5f,  -0.5f,  
             0.5f,   0.5f,  
            -0.5f,   0.5f
        };

VertexArray varr;
VertexBuffer vbuff(positions, 4 * 2 * sizeof(float));
```

you'll notice in `verts` we have two columns of four floats. we want to reference the indexes of these floats for each _triangle_ we want to create,
we have two triangles to print, but we can reuse verticies that overlap.


`vbuff` takes in the array of verticies, and the size of memory those verts take up.


then you'll need a vertex buffer layout which is used for constructing the verticies before binding to the GL buffer.

```c++
VertexBufferLayout layout;

layout.Push<float>(2, GL_FLOAT, false);
varr.AddBuffer(vbuff, layout);
```

`layout.Push<T>(a, b, c)`
    - `T` needs you to tell it what type of data the verts are in, in our case - floats.
    - `a` is the count of each element we have verts for, we have 4 verts for each element and we have two elements.
    - `b` this takes in the type of data in the actual program this time, so we know what size the memory is for each element etc.
    - `c` is the vertex normalized already or not?


varr has a method called `AddBuffer` which takes the set of verticie data and the _map_ of that data, or _layout_ as it's called here.


Next, we need an index buffer which will tell us where each vertex of each element exissts in the memory. 


```c++
uint32_t indicies[] = {
      0, 1, 2,
      2, 3, 0
}
IndexBuffer idxbuff(indicies, 3);
```

we can save a bit of space by using the same memory for when the verticie is in the exact same spot.

the `idxbuff` takes the _map_ of indexes and the size each element uses.

The last thing we need before we can start rendering frames is a shader


```c++
     Shader shader("res/shaders/Basic.shader");
     shader.Bind();
     shader.SetUniform4f("u_Color", 0.9f, 0.3f, 0.4f, 0.7f);
```


the `Shader` constructor takes in a relative path to a `Basic.shader`. I don't know much about them but, there's an extremely simple one
I've provided here that will at least get things up and running. After we create the shader we need to bind it to the current context which
is done through it's member function.

last of all we have `SetUniform4f` which lengthened is `set uniform 4f`. In the shader file we can take advantage of variables, except they're
called uniforms, and the one we want to manipulate is of type vec4 (red, green, blue, alpha) which are 0-255 values _normalized_ to between 0 and 1.
in the `Shader.basic` file the variable name is `u_Color`, so this is how we reference it.

Now we can finally render something to the screen. we're going to start by making sure our buffers and verticies are *unbound* before entering our frame loop.

```c++
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();
```

Now, inside the while loop we can turn on the shaders, bind them, and then bind the buffers we have. that should
be enough to draw on the screen, so we'll call the `glDrawElements` function with mode `GL_TRIANGLES`, count 6,
and the type of data using the GL definition, `GL_UNSIGNED_INT` - this is just so it knows how much memory to loop over before
it's enroached on the next elements data or not. Since we've already bound the buffer data in the vertex buffer constructor 
we can pass a nullptr`.

```c++
 GLCall(glClear(GL_COLOR_BUFFER_BIT));
 
 shader.Bind();
 shader.SetUniform4f("u_Color", 0.5f, 0.5f, 0.5f, 1.0f);

 varr.Bind();
 idxbuff.Bind();
 
 GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

 glfwSwapBuffers(window);
```

This should draw two triangles in the shape of a rectangle or square. if somethings not right, consult the documentation
of OpenGL. 

## Shaders

### `Todo. will update readme later with more info.`




place your shader program in the `res/shaders/` directory and name it `Basic.shader`.

each shader program should contain a `position` shader and `fragment` shader. these are defined
with the `#shader <type>` token to parse and compile them so they can be bound to the vertex buffer.




