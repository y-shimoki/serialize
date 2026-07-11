#pragma once

#include <concepts>
#include <expected>
#include <filesystem>
#include <fstream>
#include <string>
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

    template<typename T>
    requires std::same_as<T, std::string>
    auto LoadFromBinary(const std::filesystem::path& filePath) -> std::expected<std::string, IoError>
    {
        auto file = std::ifstream(filePath, std::ios::binary);
        if (!file.is_open())
        {
            return std::unexpected{ IoError::OpenError };
        }

        size_t stringSize = 0;
        file.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));

        auto loadDestination = std::string(stringSize, '\0');
        file.read(loadDestination.data(), stringSize);

        if (file.fail() || file.gcount() != static_cast<std::streamsize>(stringSize))
        {
            return std::unexpected{ IoError::ReadError };
        }

        return loadDestination;
    }

    template<TrivialType T>
    auto SaveAsBinary(const T& object, const std::filesystem::path& filePath) -> IoError
    {
        auto file = std::ofstream(filePath, std::ios::binary);

        if (!file.is_open()) {
            return IoError::OpenError; 
        }

        file.write(reinterpret_cast<const char*>(&object), sizeof(object));

        if (file.fail()) {
            return IoError::WriteError;
        }

        return IoError::None;
    }

    auto SaveAsBinary(const std::string& string, const std::filesystem::path& filePath) -> IoError 
    {
        auto file = std::ofstream(filePath, std::ios::binary);
        
        if (!file.is_open()) {
            return IoError::OpenError; 
        }

        size_t stringSize =  string.size();
        file.write(reinterpret_cast<const char*>(&stringSize), sizeof(stringSize));
        file.write(string.data(), stringSize);

        if (file.fail()) {
            return IoError::WriteError;
        }

        return IoError::None;
    }
}