//
// Created by Bellaedris on 12/12/2025.
//

#pragma once

#include <memory>
#include "Window.h"
#include "Camera.h"
#include "GPU/Shader.h"
#include "Mesh.h"
#include "Timer.h"
#include "GPU/Texture.h"

namespace mgl
{
    class App
    {
    private:
        Window m_window;

        gpu::Shader s;
        std::unique_ptr<gpu::Texture> t;
        std::vector<Mesh> m_meshes;
        std::unique_ptr<Camera> m_camera {nullptr};

        Timer<std::chrono::seconds> m_deltaTimer;

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
