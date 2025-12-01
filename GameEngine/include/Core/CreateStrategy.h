#pragma once
#include <memory>
#include "Tools/PrimaryTypeTrait.h"
#include "Tools/TypeTrait.h"

namespace KT
{
    // Static: returns an object
    template<typename Type>
    class Static
    {
    public:
        using returnTypes = Type;
        template<typename SameType, typename ...CtorArgs> requires is_same_v< Type, SameType >
        static returnTypes Create(CtorArgs&&... args);
    };

    template <typename Type>
    template <typename SameType, typename ... CtorArgs> requires is_same_v<Type, SameType>
    typename Static<Type>::returnTypes Static<Type>::Create(CtorArgs&&... args)
    {
	    return SameType{ std::forward<CtorArgs>(args)... };
    }

    // Pointer: returns a new raw pointer to an object
    template<typename BaseType>
    class Pointer
    {
    public:
        using returnTypes = BaseType*;
        template<typename DerivedType, typename ...CtorArgs> requires is_base_of_v<BaseType, DerivedType>
        static returnTypes Create(CtorArgs&&... args);
    };

    template <typename BaseType>
    template <typename DerivedType, typename ... CtorArgs> requires is_base_of_v<BaseType, DerivedType>
    typename Pointer<BaseType>::returnTypes Pointer<BaseType>::Create(CtorArgs&&... args)
    {
	    return new DerivedType(std::forward<CtorArgs>(args)...);
    }

    // Pointer: returns a pointer to an object
    template<typename BaseType>
    class UniquePointer
    {
    public:
        using returnTypes = std::unique_ptr<BaseType>;
        template<typename DerivedType, typename ...CtorArgs> requires is_base_of_v<BaseType, DerivedType>
        static returnTypes Create(CtorArgs&&... args);
    };

    template <typename BaseType>
    template <typename DerivedType, typename ... CtorArgs> requires is_base_of_v<BaseType, DerivedType>
    typename UniquePointer<BaseType>::returnTypes UniquePointer<BaseType>::Create(CtorArgs&&... args)
    {
	    return std::make_unique<DerivedType>(std::forward<CtorArgs>(args)...);
    }

    // Pointer: returns a pointer to an object
    template<typename BaseType>
    class SharedPointer
    {
    public:
        using returnTypes = std::shared_ptr<BaseType>;
        template<typename DerivedType, typename ...CtorArgs> requires is_base_of_v<BaseType, DerivedType>
        static returnTypes Create(CtorArgs&&... args);
    };

    template <typename BaseType>
    template <typename DerivedType, typename ... CtorArgs> requires is_base_of_v<BaseType, DerivedType>
    typename SharedPointer<BaseType>::returnTypes SharedPointer<BaseType>::Create(CtorArgs&&... args)
    {
	    return std::make_shared<DerivedType>(std::forward<CtorArgs>(args)...);
    }

    // Concept to check if a strategy is valid
    // need improvement
    template<typename type, template<typename> class CreateStrategy>
    concept ValidCreator = requires
    {
        typename CreateStrategy<type>::returnTypes;
        { CreateStrategy<type>::template Create<type>() } -> std::same_as<typename CreateStrategy<type>::returnTypes>;
    };
}