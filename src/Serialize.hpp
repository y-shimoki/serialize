#pragma once

#include <fstream>
#include <filesystem>
#include <expected>
#include <cstddef>

namespace srl
{
    enum class IoError
    {
        None,
        OpenError,
        WriteError,
        ReadError,
    };

    template<typename T>
    auto LoadFromBinary(const std::filesystem::path& filePath) -> std::expected<T, IoError>
    {
        auto file = std::ifstream(filePath, std::ios::binary);
        if (!file.is_open())
        {
            return std::unexpected{ IoError::OpenError };
        }

        auto loadDestination = T();
        file.read(reinterpret_cast<char*>(&loadDestination), sizeof(T));

        if (file.fail() || file.gcount() != sizeof(T))
        {
            return std::unexpected{ IoError::ReadError };
        }

        return loadDestination;
    }
}