#pragma once

#include <array>
#include <tuple>
#include <cstddef>
#include <cstdlib>
#include <type_traits>

namespace srl
{
	template <typename T>
	concept SafeLeafType = (std::is_arithmetic_v<T> ||
						    std::is_enum_v<T>) &&
					       !std::is_pointer_v<T>;

	template <typename T>
	constexpr auto kBinaryRecordableFields = [] {
		static_assert(
			std::always_false_v<T>,
			R"del(The specified type wasn't specialized.
You can specialize your type through the following code:
namespace srl
{
    template<>
    constexpr auto kBinaryRecordableFields<YourClass> =
		ReflectFields(&YourClass::field1, &YourClass::field2, ...);
})del"
);
		return std::tuple{};
		}();

	template<typename T, typename... Ms>
	consteval auto ReflectFields(Ms T::*... memberPointers)
	{
		return std::make_tuple(memberPointers...);
	}

	template<typename T>
	consteval auto ToSafeLeafTypes()
	{
		constexpr auto memberPointers = kBinaryRecordableFields<T>;

		if constexpr (SafeLeafType<T>)
		{
			return std::tuple{  };
		}
	}

	template<typename T, typename M>
	consteval auto ComputeFieldSize(M T::* memberPointer) -> std::size_t
	{
		return sizeof(M);
	}

	template<typename T>
	consteval auto ComputeBinaryRecordSize() -> std::size_t
	{
		constexpr auto memberPointers = kBinaryRecordableFields<T>;

		return std::apply(
			[](auto... memberPointer)
			{
				return (ComputeFieldSize(memberPointer) + ... + 0);
			},
			memberPointers
		);
	}

	template<typename T>
	struct BinaryRecordTraits
	{
		static constexpr auto size = ComputeBinaryRecordSize<T>();
		static constexpr auto memberPointers = kSerializableFields<T>;
	};

	template<typename T>
	inline constexpr auto kBinaryRecordSize = BinaryRecordTraits<T>::size;

	// [WARNING]: kMemberPointers is too ambiguous, as we can also have named member pointers
	// for JSON serialization.
	template<typename T>
	inline constexpr auto kBinaryRecordMemberPointers = BinaryRecordTraits<T>::memberPointers;

	template<typename T>
	class BinaryRecord
	{
		std::array<std::byte, kBinaryRecordSize<T>> payload;
	};
} // namespace srl