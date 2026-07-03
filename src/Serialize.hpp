#pragma once

#include <fstream>
#include <filesystem>
#include <iostream>
#include <concepts>
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
    auto SaveAsBinary(const T& object, const std::filesystem::path& filePath) -> IoError
    {
        std::ofstream file(filePath, std::ios::binary);

        if (!file) {
            std::cerr << "Error: Could not create or open the file!" << std::endl;
            return IoError::OpenError; 
        }

        file.write(reinterpret_cast<const char*>(&object), sizeof(object));

        if (!file) {
            std::cerr << "Error: Failed writing data to file!" << std::endl;
            return IoError::WriteError;
        }

        return IoError::None;
    }

}

