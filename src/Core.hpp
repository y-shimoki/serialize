#pragma once

#include <type_traits>
#include <filesystem>
#include <concepts>
#include <expected>
#include <iostream>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector> 
#include <bitset>

namespace srl
{
	//Concepts
	template<typename T>
	concept TrivialType = std::is_trivially_copyable_v<T> && std::is_trivially_constructible_v<T> && !std::is_pointer_v<T>;

	//Enums
	enum class PrintFormat { Decimal, Hex, Binary };

	//Class
	class BinarySerialization
	{
	private:
		std::vector<std::byte> buffer;    
	
	public:
		static constexpr std::uint8_t CURRENT_VERSION = 1;

	public:
		BinarySerialization()
		{
			// Magic Number (""""Number"""")
			buffer.push_back(static_cast<std::byte>('S'));
			buffer.push_back(static_cast<std::byte>('R'));
			buffer.push_back(static_cast<std::byte>('L'));

			// Versão do arquivo
			buffer.push_back(static_cast<std::byte>(CURRENT_VERSION));
		}

		// Serialize Functions
		template<TrivialType T>
		auto Serialize(const T& obj) -> BinarySerialization&
		{
			const auto* bytePtr = reinterpret_cast<const std::byte*>(&obj);
			buffer.insert(buffer.end(), bytePtr, bytePtr + sizeof(T));
			return *this;
		}

		auto Serialize(const std::string& string) -> BinarySerialization&
		{
			auto stringSize = static_cast<std::uint16_t>(string.size());

			const auto* sizeBytePtr = reinterpret_cast<const std::byte*>(&stringSize);
			buffer.insert(buffer.end(), sizeBytePtr, sizeBytePtr + sizeof(stringSize));

			const auto* dataBytePtr = reinterpret_cast<const std::byte*>(string.data());
			buffer.insert(buffer.end(), dataBytePtr, dataBytePtr + stringSize);

			return *this;
		}

		//Auxiliary Functions
		auto GetBuffer() const -> const std::vector<std::byte>&
		{
			return buffer;
		}	

		auto Print(PrintFormat format = PrintFormat::Hex) const -> void
		{
			for (const auto& b : buffer)
			{
				int val = std::to_integer<int>(b);

				switch (format)
				{
				case PrintFormat::Decimal:
					std::cout << val << " ";
					break;
				case PrintFormat::Hex:
					std::cout << std::hex << std::setw(2) << std::setfill('0') << val << " ";
					std::cout << std::dec;
					break;
				case PrintFormat::Binary:
					std::cout << std::bitset<8>(val) << " ";
					break;
				}
			}			
		}

		//Write to File Functions
	};
}

