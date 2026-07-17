#pragma once

#include <array>
#include <cstddef>
#include <cstdlib>
#include <type_traits>

namespace srl
{
	template<typename T>
	concept TrivialType = std::is_trivially_copyable_v<T> && std::is_trivially_constructible_v<T> && !std::is_pointer_v<T>;

	template<TrivialType T>
	class BinarySerialization
	{
	public:
		std::array<std::byte, sizeof(T)> mContent;

	private:
		BinarySerialization() = default;

		friend auto SerializeToBinary(const T& object) -> BinarySerialization<T>;
	};

	template<TrivialType T>
	auto SerializeToBinary(const T& object) -> BinarySerialization<T>
	{
		auto serializedObject = BinarySerialization<T>{};

		std::memcpy(binarySerialization.mContent.data(), &object, sizeof(T));

		return serializedObject;
	}
}