#pragma once

#include <fstream>
#include <filesystem>
#include <expected>

namespace srl
{
    enum class IoError
    {
        OpenError,
        WriteError,
        ReadError,
    };

    template<typename T>
    auto LoadFromBinary(const std::filesystem::path& file) -> std::expected<T, IoError>
    {
        
    }
}