#pragma once
namespace KT
{

	//data_container
	template<typename Type, Type data>
	struct data_container
	{
		using type = Type;
		static constexpr type value = data;
		constexpr operator type() const { return value; }
		constexpr type operator()() const { return value; }
	};

	//type_container
	template<typename Type>
	struct  type_container
	{
		using type = Type;
		constexpr operator type() const { return type{}; }
		constexpr type operator()() const { return type{}; }
	};

	//BasicBoolType
	using false_type = data_container<bool, false>;
	using true_type = data_container<bool, true >;

	//ConstTypeModification
	template<typename type>
	struct remove_const : type_container<type> {};
	template<typename type>
	struct remove_const<const type> :type_container<type> {};
	template<typename type>
	using remove_const_t = typename remove_const<type>::type;

	template<typename type>
	struct make_const : type_container<const type> {};
	template<typename type>
	struct make_const<const type> :type_container<const type> {};
	template<typename type>
	using make_const_t = typename make_const<type>::type;

	//VolatileTypeModification
	template<typename type>
	struct remove_volatile : type_container<type> {};
	template<typename type>
	struct remove_volatile<volatile type> :type_container<type> {};
	template<typename type>
	using remove_volatile_t = typename remove_volatile<type>::type;

	template<typename type>
	struct make_volatile : type_container<volatile type> {};
	template<typename type>
	struct make_volatile<volatile type> :type_container<volatile type> {};
	template<typename type>
	using make_volatile_t = typename make_volatile<type>::type;

	//ConstVolatileTypeModification
	template<typename type>
	using remove_cv_t = remove_const_t<remove_volatile_t<type>>;
	template<typename type>
	using make_cv_t = make_const_t<make_volatile_t<type>>;

	//TypeCompare
	template<typename lhstype, typename rhstype>
	struct is_same : false_type {};
	template<typename Type>
	struct is_same<Type, Type> : true_type {};
	template<typename lhstype, typename rhstype>
	static constexpr bool is_same_v = is_same<remove_cv_t<lhstype>, remove_cv_t<rhstype>>::value;
	template<typename lhstype, typename rhstype>
	static constexpr bool is_exactly_same_v = is_same<lhstype, rhstype>::value;

	//Conditional
	template<bool condition, typename lhstype, typename rhstype>
	struct conditional;
	template<typename lhstype, typename rhstype>
	struct conditional<true, lhstype, rhstype> : type_container<lhstype> {};
	template<typename lhstype, typename rhstype>
	struct conditional < false, lhstype, rhstype > : type_container<rhstype> {};
	template<bool condition, typename lhstype, typename rhstype>
	using conditional_t = typename conditional<condition, lhstype, rhstype>::type;

	//is_type_in_pack
	template<typename type, typename first, typename... Args>
	struct is_type_in_pack :is_type_in_pack<type, Args...>
	{
	};
	template<typename type, typename... Args>
	struct is_type_in_pack<type, type, Args...> : true_type
	{
	};
	template<typename type, typename last>
	struct is_type_in_pack<type, last> : false_type
	{
	};

	template<typename type, typename first, typename... Args>
	static constexpr bool  is_type_in_pack_v = is_type_in_pack<type, first, Args...>::value;
}
