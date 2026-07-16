#pragma once

#include <cstdlib>

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
		BinarySerialization() {}

		template<TrivialType T>
		friend auto SerializeToBinary(const T& object) -> BinarySerialization<T>
	};

	template<TrivialType T>
	auto SerializeToBinary(const T& object) -> BinarySerialization<T>
	{
		auto binarySerialization = BinarySerialization<T>{};

		auto* copyDestination = reinterpret_cast<void*>(&binarySerialization);
		auto* copySource      = reinterpret_cast<void*>(&object);

		auto copySizeBytes = sizeof(T);

		std::memcpy(copyDestination, copySource, copySizeBytes);

		return binarySerialization;
	}
}