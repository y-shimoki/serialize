#pragma once

#include <fstream>
#include <filesystem>
#include <expected>
#include <type_traits>  

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
    concept TrivialType = std::is_trivially_copyable_v<T> && std::is_trivially_constructible_v<T>;
    
    template<TrivialType T>
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

    template<TrivialType T>
    auto SaveAsBinary(const T& object, const std::filesystem::path& filePath) -> IoError
    {
        std::ofstream file(filePath, std::ios::binary);

        if (!file) {
            return IoError::OpenError; 
        }

        file.write(reinterpret_cast<const char*>(&object), sizeof(object));

        if (!file) {
            return IoError::WriteError;
        }

        return IoError::None;
    }

}

