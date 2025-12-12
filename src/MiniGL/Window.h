//
// Created by Bellaedris on 11/12/2025.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace mgl
{
    class Window
    {
    private:
        GLFWwindow *m_window;

        static void framebuffer_size_callback(GLFWwindow *w, int width, int height)
        { glViewport(0, 0, width, height); }

    public:
        Window(int width, int height, int major, int minor);

        bool ShouldClose();
        void PollEvents();
        void SwapBuffers();
    };
}