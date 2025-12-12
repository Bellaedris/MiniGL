//
// Created by Bellaedris on 12/12/2025.
//

#pragma once

#include "Window.h"
#include "glm/glm.hpp"

namespace mgl
{
    class App
    {
    private:
        Window m_window;

        float vertices[12] = {
                0.5f,  0.5f, 0.0f,  // top right
                0.5f, -0.5f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f,  // bottom left
                -0.5f,  0.5f, 0.0f   // top left
        };

        unsigned int indices[6] = {  // note that we start from 0!
                0, 1, 3,   // first triangle
                1, 2, 3    // second triangle
        };

        /**
         * \brief Initializes the application
         */
        void Init();

        /**
         * \brief Called each frame before rendering
         */
        void PreRender();

        /**
         * \brief Called each frame, renders
         */
        void Render();

    public:
        App(int width, int height, int majorVersionHint = 4, int minorVersionHint = 6);

        /**
         * \brief Runs the application until it is quited
         */
        void Run();
    };
}
