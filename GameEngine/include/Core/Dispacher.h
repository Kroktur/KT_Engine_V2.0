#pragma once
#include "Tools/Typelist.h"
#include <map>
#include <string>
#include <functional>
#include <stdexcept>

namespace KT
{



	// Forward declaration of Dispatcher
	template<typename LhsType, typename RhsType = LhsType, typename ReturnType = void, typename Args = typelist<>>
	class Dispatcher;

	// Dispatcher class that maps pairs of types name to functions
	// use typeList to add multiple arguments types for the function
	template<typename LhsType, typename RhsType, typename ReturnType, typename... Args>
	class Dispatcher<LhsType, RhsType, ReturnType, typelist<Args...>>
	{
	public:
		// Type aliases for key and value types in the map
		// key is a pair of type names : typeid().name()
		using key_type = std::pair<std::string, std::string>;
		// value is a function taking LhsType&, RhsType&, arguments type and returning ReturnType
		using value_type = std::function<ReturnType(LhsType&, RhsType&, Args&&...)>;
		// Bind 2 types with their interaction function
		template<typename DerivedLhsType, typename DerivedRhsType>
		void Add(const value_type& value)
		{
			key_type key = key_type(typeid(DerivedLhsType).name(), typeid(DerivedRhsType).name());
			auto it = m_map.find(key);
			if (it != m_map.end())
				throw std::runtime_error("Already Register");
			m_map[key] = value;

		}
		// Return the right function By the Types 
		ReturnType operator()(LhsType& lhs, RhsType& rhs, Args&&... args)
		{
			key_type key = key_type(typeid(lhs).name(), typeid(rhs).name());
			auto it = m_map.find(key);
			if (it == m_map.end())
				throw std::runtime_error("not Register");
			return it->second(lhs, rhs,std::forward<Args>(args)...);

		}
	private:
		std::map<key_type, value_type> m_map;
	};

	// ForwardDeclaration of FNDispatcher
	template<typename LhsType, typename RhsType = LhsType, typename ReturnType = void, typename Args = typelist<>>
	class FNDispatcher;

	// the FNDispatcher use a simple Dispatcher and Wrap it to run function
	template<typename LhsType, typename RhsType, typename ReturnType, typename... Args>
	class FNDispatcher<LhsType, RhsType, ReturnType, typelist<Args...>>
	{
	public:
		//arguments list 
		using CtorList = typelist<Args...>;
		// Add cast DerivedTypes through function and add it to dispatcher
		// Pointer Function to register 
		template<typename DerivedLhsType, typename DerivedRhsType, ReturnType(*FN)(DerivedLhsType&, DerivedRhsType&, Args...), bool Mirror = false>
		void Add()
		{
			// Due to inheritance and virtual Destructor cast types to register Derived
			auto fn = [&](LhsType& lhs, RhsType& rhs, Args&&... args)->ReturnType
				{
					return FN(static_cast<DerivedLhsType&>(lhs), static_cast<DerivedRhsType&>(rhs),std::forward<Args>( args)...);
				};
			// add to dispatcher
			m_dispatcher.template Add<DerivedLhsType, DerivedRhsType>(fn);
			// due to Compile time bool if constexpr for optimisation
			if constexpr (!Mirror)
				return;
			auto fn2 = [&](RhsType& rhs, LhsType& lhs, Args&&... args) ->ReturnType
				{
					return fn(lhs, rhs, std::forward<Args>(args)...);
				};
			// add to dispatcher
			m_dispatcher.template Add<DerivedRhsType, DerivedLhsType>(fn2);
		}
		//call dispatcher
		ReturnType operator()(LhsType& lhs, RhsType& rhs, Args... args)
		{
			return m_dispatcher(lhs, rhs, std::forward<Args>(args)...);
		}
	private:
		Dispatcher<LhsType, RhsType, ReturnType, CtorList> m_dispatcher;
	};

}
