#pragma once

#include <type_traits>
#include <filesystem>
#include <concepts>
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <string>
#include <vector> 
#include <bitset>
#include <array>
#include <span>

namespace srl
{
	//Concepts
	template<typename T>
	concept TrivialType = std::is_trivially_copyable_v<T> && std::is_trivially_constructible_v<T> && !std::is_pointer_v<T>;

	//Enums
	enum class PrintFormat { Decimal, Hex, Binary };

	//Classes
	class BinarySerialization
	{
	private:
		std::vector<std::byte> buffer;    
	
	public:
		static constexpr std::uint8_t CURRENT_VERSION = 1;
		static constexpr std::array<std::byte, 4> header = {
				static_cast<std::byte>('S'),
				static_cast<std::byte>('R'),
				static_cast<std::byte>('L'),
				static_cast<std::byte>(CURRENT_VERSION)
		};

	public:
		BinarySerialization(std::uint16_t initial_capacity = 256)
		{
			Reserve(initial_capacity);
			buffer.insert(buffer.end(), std::begin(header), std::end(header));
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

			Reserve(buffer.size() + stringSize + sizeof(stringSize));

			const auto* sizeBytePtr = reinterpret_cast<const std::byte*>(&stringSize);
			buffer.insert(buffer.end(), sizeBytePtr, sizeBytePtr + sizeof(stringSize));

			const auto* dataBytePtr = reinterpret_cast<const std::byte*>(string.data());
			buffer.insert(buffer.end(), dataBytePtr, dataBytePtr + stringSize);

			return *this;
		}

		//Auxiliary Functions
		//& -> Can only be called on a lvalue
		//std::span -> Prevent Copies
		auto GetBuffer() const & -> std::span<const std::byte>
		{
			return buffer;
		}	

		auto Clear() -> void
		{
			buffer.clear();
			buffer.insert(buffer.end(), std::begin(header), std::end(header));
		}

		auto Reserve(std::size_t sizeBytes) -> void
		{
			buffer.reserve(sizeBytes);
		}

		//Note for later: maybe can overload << operator
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
					break;
				case PrintFormat::Binary:
					std::cout << std::bitset<8>(val) << " ";
					break;
				}
			}	
			//Restore the stream to dec if the print format was hexadecimal
			std::cout << std::dec;
		}

		//Write to File Functions
	};
}

