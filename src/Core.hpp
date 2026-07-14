#pragma once

#include <concepts>
#include <expected>
#include <filesystem>
#include <fstream>
#include <string>
#include <type_traits>
#include <cstdint>
#include <vector>

namespace srl
{
	template<typename T>
	concept TrivialType = std::is_trivially_copyable_v<T> && std::is_trivially_constructible_v<T> && !std::is_pointer_v<T>;

	class BinarySerialization
	{
	public:
		std::vector<std::byte> buffer;

	public:
		BinarySerialization()
		{
			//Usar construtor pra inicializar buffer cm metadados como endiannes e versao
		}

		template<TrivialType T>
		auto Convert(const T& obj) -> BinarySerialization&
		{
			const auto* bytePtr = reinterpret_cast<const std::byte*>(&obj);
			buffer.insert(buffer.end(), bytePtr, bytePtr + sizeof(T));
			return *this;
		}

		//Funções para converter outros tipos


		//Funções para escrever em um arquivos e etc...

	};
}

