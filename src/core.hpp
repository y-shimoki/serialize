#pragma once

#include <array>
#include <cstddef>
#include <cstdlib>
#include <type_traits>

namespace srl
{
	template<typename T>
	concept TrivialType = std::is_trivially_copyable_v<T> &&
						  std::is_trivially_constructible_v<T> &&
						  !std::is_pointer_v<T>;

	template<TrivialType T>
	class BinarySerialization
	{
	public:
		std::array<std::byte, sizeof(T)> mContent;

	private:
		BinarySerialization() = default;

		friend auto SerializeToBinary(const T& object) -> BinarySerialization<T>;
		friend auto DeserializeFromBinary(const BinarySerialization<T>& serializedObject) -> T;
	};

	template<TrivialType T>
	auto SerializeToBinary(const T& object) -> BinarySerialization<T>
	{
		auto serializedObject = BinarySerialization<T>{};

		std::memcpy(serializedObject.mContent.data(), &object, sizeof(T));

		return serializedObject;
	}

	template<TrivialType T>
	auto DeserializeFromBinary(const BinarySerialization<T>& serializedObject) -> T
	{
		auto deserializedObject = T();

		std::memcpy(&deserializedObject, serializedObject.mContent.data(), sizeof(T));

		return deserializedObject;
	}
}

// Biblioteca Serialize
namespace srl
{
	//template<typename T>
	//struct MemberPointerTraits
	//{
	//};

	//template<typename M, typename C>
	//struct MemberPointerTraits<M C::*>
	//{
	//	using OwnerType = C;
	//	using MemberType = M;
	//};

	//template<auto T>
	//using PointedType = typename MemberPointerTraits<decltype(T)>::MemberType;

	template <typename T>
	constexpr auto kSerializableFields = std::tuple{};

	template<typename T, typename... Ms>
	consteval auto ReflectFields(Ms T::*... memberPointers)
	{
		return std::make_tuple(memberPointers...);
	}

	template<typename ClassType, typename MemberType>
	void SerializeField(const ClassType& object, MemberType ClassType::* memberPointer)
	{
		const auto objectBaseAddress = reinterpret_cast<const std::byte*>(&object);
		const auto objectMemberAddress = reinterpret_cast<const std::byte*>(&(object.*memberPointer));

		const     auto memberOffset = objectMemberAddress - objectBaseAddress;
		constexpr auto memberSize = sizeof(MemberType);

		// [TODO]: Write member serialization logic
	}

	template<typename T>
	auto SerializeToBinary(const T& object) -> BinarySerialization<T>
	{
		// Serializable fields are passed in as member pointers
		const auto memberPointers = kSerializableFields<T>;

		std::apply(
			[&](auto... memberPointers) {
				(SerializeField(object, memberPointers), ...);
			},
			memberPointers
		);
	}
}