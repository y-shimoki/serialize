#pragma once

#include <array>
#include <tuple>
#include <cstddef>
#include <cstdlib>
#include <type_traits>

namespace srl
{
	template<typename T, typename M>
	consteval auto ComputeFieldSize(Ms T::* memberPointer) -> std::size_t
	{
		return sizeof(M);
	}

	template<typename T>
	consteval auto ComputePackSize() -> std::size_t
	{
		constexpr auto memberPointers = kSerializableFields<T>;

		return std::apply(
			[](auto... memberPointer)
			{
				return (ComputeFieldSize(memberPointer) + ... + 0);
			},
			memberPointers
		);
	}

	template<typename T>
	struct PackMetadata
	{
		static constexpr auto packSize = ComputePackSize<T>();
		static constexpr auto memberPointers = kSerializableFields<T>;
	};


} // namespace srl