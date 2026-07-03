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

        // Initialize enough space for T without constructing it.
        auto loadDestination = T();
        file.read(&loadDestination, sizeof(T));

        if (auto bytesRead = file.gcount(); bytesRead != sizeof(T))
        {
            return std::unexpected{ IoError::ReadError };
        }

        return loadDestination;
    }
}