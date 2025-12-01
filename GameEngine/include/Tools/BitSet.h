#pragma once
#include <type_traits>

namespace KT
{
	struct BitSet
	{
		template<typename Type> requires std::is_enum_v<Type>
		static bool HasFlag(int bite, Type state)
		{
			return (bite & static_cast<int>(state)) != 0;
		}
		template<typename... Type> requires (... && std::is_enum_v<Type>)
			static bool HasAllFlag(int bite, Type... state)
		{
			return (... && ((bite & static_cast<int>(state)) != 0));
		}
		template<typename... Type> requires (... && std::is_enum_v<Type>)
			static bool HasAnyFlag(int bite, Type... state)
		{
			return (... || ((bite & static_cast<int>(state)) != 0));
		}
		template<typename... Type> requires (... && std::is_enum_v<Type>)
			static void AddFlag(int& bite, Type... state)
		{
			(..., (bite |= static_cast<int>(state)));
		}
		template<typename... Type> requires (... && std::is_enum_v<Type>)
			static void RemoveFlag(int& bite, Type... state)
		{
			(..., (bite &= ~static_cast<int>(state)));
		}
		template<typename... Type> requires (... && std::is_enum_v<Type>)
			static void ReSetFlag(int& bite, Type... state)
		{
			bite = 0;
			AddFlag(bite, state...);
		}
		template<typename Type>
		static void ReplaceFlag(int& bite, Type startState, Type State)
		{
			RemoveFlag(bite, startState);
			AddFlag(bite, State);
		}
		template<typename... Type> requires (... && std::is_enum_v<Type>)
			static void ToggleFlag(int& bite, Type... state)
		{
			(..., (bite ^= static_cast<int>(state)));
		}
		static void ClearFlag(int& bite)
		{
			bite = 0;
		}
	};
}
