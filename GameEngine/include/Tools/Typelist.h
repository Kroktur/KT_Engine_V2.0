#pragma once
#include "TypeTrait.h"

namespace KT
{
	

template<typename ... Args>
struct typelist
{
};

//Front
template <typename List>
struct front;
template<typename Head, typename... Tail>
struct front<typelist<Head, Tail...>> : type_container<Head> {};
template<typename List>
using front_t = typename front<List>::type;

//PushFront
template<typename type, typename List>
struct push_front;
template<typename type, typename... List>
struct push_front<type, typelist<List...>> : type_container<typelist<type, List...>> {};
template<typename type, typename list>
using push_front_t = typename push_front<type, list>::type;

//PopFront
template<typename List>
struct pop_front;
template<typename first, typename... List>
struct pop_front< typelist<first, List...>> : type_container<typelist<List...>> {};
template<typename list>
using pop_front_t = typename pop_front<list>::type;

//Back
template<typename list>
struct back;
template<typename last>
struct back<typelist<last>> : type_container<last> {};
template<typename first, typename... Arg>
struct back<typelist<first, Arg...>> : back < typelist < Arg... >> {};
template<typename list>
using back_t = typename back<list>::type;

//PushBack
template<typename type, typename List>
struct push_back;
template<typename type, typename... List>
struct push_back<type, typelist<List...>> : type_container<typelist<List..., type>> {};
template<typename type, typename List>
using push_back_t = typename push_back<type, List>::type;

//PopBack
template<typename List>
struct pop_back;
template<typename last>
struct pop_back<typelist<last>> :type_container<typelist<>> {};
template<typename first, typename... Rest>
struct pop_back<typelist<first, Rest...>> : type_container<push_front_t<first, typename pop_back<typelist<Rest...>>::type>> {};
template<typename list>
using pop_back_t = typename pop_back<list>::type;

//reverse
template<typename list>
struct reverse;
template<>
struct reverse<typelist<>> : type_container<typelist<>> {};
template<typename first, typename... Rest>
struct reverse<typelist<first, Rest...>> : type_container<push_back_t<first, typename reverse<typelist<Rest...>>::type>> {};
template<typename list>
using reverse_t = typename  reverse<list>::type;

//Size
template<typename List>
struct size;
template<typename... Args>
struct size<typelist<Args...>> :data_container<size_t, sizeof...(Args)> {};
template<typename list>
static constexpr size_t size_v = size<list>::value;

//At
template<size_t index, typename List>
struct at;
template<size_t index>
struct at<index, typelist<>> { static_assert(index != index, "at: index out of bounds in typelist"); };
template<typename... Args>
struct at<0, typelist<Args...>> : type_container<front_t<typelist<Args...>>> {};
template<size_t index, typename... Args>
struct at<index, typelist<Args...>> : at<index - 1, pop_front_t<typelist<Args...>>> {};
template<size_t index, typename list> requires(index < size_v<list>)
	using at_t = typename  at<index, list>::type;

//IsEmpty
template<typename list>
struct is_empty;
template<>
struct is_empty<typelist<>> : true_type {};
template<typename... Arg>
struct is_empty<typelist<Arg...>> : false_type {};
template<typename list>
static constexpr bool is_empty_v = is_empty<list>::value;

//Contains
template<typename type, typename list>
struct contains;
template<typename type>
struct contains<type, typelist<>> : false_type {};
template<typename type, typename...Arg>
struct contains<type, typelist<type, Arg...>> : true_type {};
template<typename type, typename... Arg>
struct contains<type, typelist<Arg...>> : contains<type, pop_front_t<typelist<Arg...>>> {};
template<typename type, typename list>
static constexpr bool contains_v = contains < type, list >::value;

//GetIndex
template<size_t index, typename type, typename list>
struct get_index;
template<size_t index, typename type>
struct get_index<index, type, typelist<>> { static_assert(index != index, "get_index: type not found in typelist"); };
template<size_t index, typename type, typename... Arg>
struct get_index<index, type, typelist<type, Arg...>> : data_container<size_t, index> {};
template<size_t index, typename type, typename... Arg>
struct get_index<index, type, typelist<Arg...>> : get_index<index + 1, type, pop_front_t<typelist<Arg...>>> {};
template< typename type, typename list>requires contains_v<type, list>
static constexpr size_t get_index_v = get_index<0, type, list>::value;

//GetCount
template<size_t index, typename type, typename list>
struct count;
template<size_t index, typename type>
struct count<index, type, typelist<>> : data_container<size_t, index> {};
template<size_t index, typename type, typename... Arg>
struct count<index, type, typelist<type, Arg...>> : count<index + 1, type, typelist<Arg...>> {};
template<size_t index, typename type, typename... Arg>
struct count<index, type, typelist<Arg...>> : count<index, type, pop_front_t<typelist<Arg...>>> {};
template< typename type, typename list>
static constexpr size_t count_v = count < 0, type, list >::value;

//AllTypeSame
template<typename type, typename list>
struct is_all_same;
template<typename type>
struct is_all_same<type, typelist<type>> :true_type {};
template<typename type, typename... Arg>
struct is_all_same<type, typelist<type, Arg...>> :is_all_same<type, typelist<Arg...>> {};
template<typename type, typename... Arg>
struct is_all_same<type, typelist<Arg...>> :false_type {};
template<typename type, typename list>
static constexpr bool is_all_same_v = is_all_same<type, list>::value;

//SameTypelist
template< typename list, typename list2>
struct is_same_typeList;
template<>
struct is_same_typeList<typelist<>, typelist<>> :true_type {};
template< typename... Arg, typename... Arg2>
struct is_same_typeList<typelist<Arg...>, typelist<Arg2...>> :false_type {};
template< typename... Arg>
struct is_same_typeList<typelist<Arg...>, typelist<Arg...>> :true_type {};


template<typename list, typename list2>
static constexpr bool is_same_typeList_v = is_same_typeList<list, list2>::value;

//HasDuplicateType
template<typename type, typename list>
struct has_duplicate_type;
template<typename type>
struct has_duplicate_type <type, typelist<>> :false_type {};
template<typename type, typename... Arg>
struct has_duplicate_type<type, typelist<type, Arg...>> :contains<type, typelist<Arg...>> {};
template<typename type, typename... Arg>
struct has_duplicate_type<type, typelist<Arg...>> :has_duplicate_type<type, pop_front_t<typelist<Arg...>>> {};
template<typename type, typename list>
static constexpr bool has_duplicate_type_v = has_duplicate_type<type, list>::value;

//HasDuplicate
template<typename list>
struct has_duplicate;
template<>
struct has_duplicate <typelist<>> :false_type {};
template<typename first, typename... Arg>
struct has_duplicate<typelist<first, Arg...>> :conditional_t<contains_v<first, typelist<Arg...>>, true_type, has_duplicate<typelist<Arg...>>> {};
template< typename list>
static constexpr bool has_duplicate_v = has_duplicate<list>::value;

//Erase
template<typename type, typename list >
struct erase;
template<typename type>
struct erase<type, typelist<>> :type_container<typelist<>> {};
template<typename type, typename ...Arg>
struct erase<type, typelist<type, Arg...>> :type_container<typelist<Arg...>> {};
template<typename type, typename first, typename ...Arg >
struct erase<type, typelist<first, Arg...>> :type_container<push_front_t<first, typename erase<type, typelist<Arg...>>::type>> {};
template<typename type, typename list>
using erase_t = typename erase<type, list>::type;

//Unique
template<typename list>
struct unique;
template<>
struct unique<typelist<>> :type_container<typelist<>> {};
template<typename First, typename... Rest>
struct unique<typelist<First, Rest...>> : conditional_t<contains_v<First, typelist<Rest...>>, unique<typelist<Rest...>>, push_front<First, typename unique<typelist<Rest...>>::type>> {};
template<typename List>
using unique_t = typename unique<List>::type;

//Remove
template<typename type, typename list>
struct remove_type;
template<typename type>
struct remove_type<type, typelist<>> : type_container<typelist<>> {};
template<typename type, typename ... Arg>
struct remove_type < type, typelist<type, Arg...>> : type_container<typename remove_type<type, typelist<Arg...>>::type> {};
template<typename type, typename first, typename ... Arg>
struct remove_type < type, typelist<first, Arg...>> : type_container<push_front_t<first, typename remove_type<type, typelist<Arg...>>::type>> {};
template<typename type, typename list>
using remove_type_t = typename remove_type<type, list>::type;

//Args
template<typename list, typename... Arg>
struct is_matching_args;
template<>
struct is_matching_args <typelist<>> : true_type {};
template<typename... ts, typename... Arg >
struct is_matching_args < typelist<ts...>, Arg...> : is_same_typeList<typelist<ts...>, typelist<Arg...>> {};
template<typename list, typename... Arg>
static constexpr bool is_matching_args_v = is_matching_args <list, Arg...>::value;

template<typename type>
struct unpack_args;

template<typename... Args>
struct unpack_args<typelist<Args...>> :  type_container<typelist<Args...>>
{
	static constexpr size_t size = sizeof...(Args);
	template<template <typename...> class To>
	using Apply = To<Args...>;

	template<typename returnType , returnType(fn)(Args...)>
	static returnType Call(Args&&...args)
	{
		return  fn(args...);
	}
	template<typename Fn>
	static auto Call(Fn&& fn,Args&&...args)
	{
		return  fn(args...);
	}
};

template<typename type>
struct is_typeList : false_type {};

template<typename... Args>
struct is_typeList<typelist<Args...>> : true_type {};

template<typename list>
static constexpr bool is_typeList_v = is_typeList<list>::value;

}
