//
// Created by Bellaedris on 12/12/2025.
//

#pragma once

#include <string>
#include <optional>

namespace mgl::utils
{
    class FileUtils
    {
    public:
        static std::optional<std::string> read_file(const char* filename);
    };

} // mgl::utils