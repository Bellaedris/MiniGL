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

        glm::vec3 vertices[3] =
        {
            {.0f, .5f, .0f},
            {-.5f, -.5f, .0f},
            {.5f, -.5f, .0f}
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
