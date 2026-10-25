A minimalist OpenGL engine to quickly iterate on rendering concepts. it was created to help Lyon 2 University Gamagora students learn openGL, without having to suffer the initialization and biolerplate work.

## Installation and build
Building on different platforms might not work sometimes. It has been tested on:
- Ubuntu and derived (ZorinOS/KDE Neon)

### Windows
It has only been tested on Visual Studio 2026 so far, open the folder, wait for CMake to run, you should be able to launch the project right away

### Linux
You will need to install the following dependencies (for Debian-derived packages, unfortunately i don't know the names in other package managers): `build-essential cmake libglfw3-dev libomp-dev`
Just `cmake .` at the project root, then build depending on you setup... Probably something lie `make` or `ninja`. 

If it compiles on a platform not present in this list, feel free to tell me.

Feel free to either open a ticket or contact me in case of issue

## Functionalities
Every class should be documented enough and provide enough information on what it does and why it does it.
- The main entry point is the App class, that creates a window and an OpenGL context
- OpenGL wrappers: Buffer, VAO, Shader, Framebuffers, Textures. These classes will help with basic openGL concepts. They are written in a C++ RAII style. It should be noted that copy construction are not allowed for these classes, see [this](https://wikis.khronos.org/opengl/Common_Mistakes#RAII_and_hidden_destructor_calls).
- Mesh creation: read mesh datas using TinyOBJReader. Creates the buffers and everything needed. Can call draw() to render an object
- Simple Camera: can be moved with ZQSD, camera moves with the mouse when ALT is held. Can give transformation matrices. 
- Simple UI with [ImGui](https://github.com/ocornut/imgui) (if you need code examples, there are plenty [right here](https://pthom.github.io/imgui_explorer/?lib=imgui)). UI code should go in the DrawUI function of your App-derived class.
