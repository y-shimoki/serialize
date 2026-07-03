#pragma once

#include <fstream>
#include <filesystem>

namespace srl
{
    enum class IoError
    {
        OpenError,
        WriteError,
        ReadError,
    };

}