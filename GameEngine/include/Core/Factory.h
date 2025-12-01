#pragma once
#include "CreateStrategy.h"
#include <stdexcept>
#include "Tools/Typelist.h"
#include <map>
#include <memory>

namespace KT
{
    // Compile time Factory with typeList that contains all types
    // need a creation strategy to create and cast types 
    template<typename BaseType, template<typename> class CreateStrategy, typename List> requires is_typeList_v<List>
        struct CT_Factory_Cast
    {
        // create with types, check if DerivedTypes is in typeLists and create it
        template<typename DerivedType, typename ...Args> requires contains_v<DerivedType,List>
        static typename CreateStrategy<DerivedType>::returnTypes Create(Args&&... args);
        // create with index, check il index fit into the typeLists and create it 
        template<size_t index, typename ...Args> requires (size_v<List> > index)
            static typename CreateStrategy<at_t<index,List>>::returnTypes Create(Args&&... args);
    };

    template <typename BaseType, template <typename> class CreateStrategy, typename List> requires is_typeList_v<List>
    template <typename DerivedType, typename ... Args> requires contains_v<DerivedType, List>
    typename CreateStrategy<DerivedType>::returnTypes CT_Factory_Cast<BaseType, CreateStrategy, List>::Create(
	    Args&&... args)
    {
	    return CreateStrategy<DerivedType>::template Create<DerivedType>(std::forward<Args>(args)...);
    }

    template <typename BaseType, template <typename> class CreateStrategy, typename List> requires is_typeList_v<List>
    template <size_t index, typename ... Args> requires (size_v<List> > index)
    typename CreateStrategy<at_t<index, List>>::returnTypes CT_Factory_Cast<BaseType, CreateStrategy, List>::Create(
	    Args&&... args)
    {
	    return CreateStrategy<at_t<index, List>>::template Create<at_t<index, List>>(std::forward<Args>(args)...);
    }

    //Predefined CT_Factory_Cast with creation strategy
    template< typename BaseType, typename list = typelist<>>
    using CT_Static_Factory_Cast = CT_Factory_Cast< BaseType, Static, list>;

    template< typename BaseType, typename list = typelist<>>
    using CT_Pointer_Factory_Cast = CT_Factory_Cast< BaseType, Pointer, list>;

    template<typename BaseType, typename list = typelist<>>
    using CT_UniquePointer_Factory_Cast = CT_Factory_Cast< BaseType, UniquePointer, list>;

    template< typename BaseType, typename list = typelist<>>
    using CT_SharedPointer_Factory_Cast = CT_Factory_Cast< BaseType, SharedPointer, list>;

    // Compile time Factory with typeList that contains all types
    // need a creation strategy to create types
    // works like the CT_Factory_cast
    template<typename BaseType, template<typename> class CreateStrategy, typename List>  requires is_typeList_v<List>
        struct CT_Factory
    {
        using returnTypes = typename CreateStrategy<BaseType>::returnTypes;
        template<typename DerivedType, typename ...Args> requires contains_v<DerivedType, List>
        static returnTypes Create(Args&&... args);

        template<size_t index, typename ...Args> requires (size_v<List> > index)
            static returnTypes Create(Args&&... args);
    };

    template <typename BaseType, template <typename> class CreateStrategy, typename List> requires is_typeList_v<List>
    template <typename DerivedType, typename ... Args> requires contains_v<DerivedType, List>
    typename CT_Factory<BaseType, CreateStrategy, List>::returnTypes CT_Factory<BaseType, CreateStrategy, List>::
    Create(Args&&... args)
    {
	    return CreateStrategy<BaseType>::template Create<DerivedType>(std::forward<Args>(args)...);
    }

    template <typename BaseType, template <typename> class CreateStrategy, typename List> requires is_typeList_v<List>
    template <size_t index, typename ... Args> requires (size_v<List> > index)
    typename CT_Factory<BaseType, CreateStrategy, List>::returnTypes CT_Factory<BaseType, CreateStrategy, List>::
    Create(Args&&... args)
    {
	    return CreateStrategy<BaseType>::template Create<at_t<index, List>>(std::forward<Args>(args)...);
    }

    //Predefined CT_Factory with creation strategy
    template< typename BaseType, typename list = typelist<>>
    using CT_Pointer_Factory = CT_Factory< BaseType, Pointer, list>;

    template< typename BaseType, typename list = typelist<>>
    using CT_UniquePointer_Factory = CT_Factory< BaseType, UniquePointer, list>;

    template<typename KeyType, typename BaseType, typename list = typelist<>>
    using CT_SharedPointer_Factory = CT_Factory< BaseType, SharedPointer, list>;

    //Interface that encapsulate creation 
    template< typename BaseType, template<typename> class CreateStrategy, typename ...CtorArgs> requires ValidCreator < BaseType, CreateStrategy>
    class ICreator
    {
    public:
        using returnTypes = typename CreateStrategy<BaseType>::returnTypes;
        virtual~ICreator() = default;
        virtual returnTypes Create(CtorArgs&&... args) = 0;
    };

    //Implementation for a derivedTypes
    template< typename BaseType, template<typename> class CreateStrategy, typename DerivedType, typename ...CtorArgs> requires ValidCreator < BaseType, CreateStrategy>
    class ConcreteCreator : public ICreator<BaseType, CreateStrategy, CtorArgs...>
    {
    public:
        using returnTypes = typename ICreator<BaseType, CreateStrategy, CtorArgs...>::returnTypes;
        returnTypes Create(CtorArgs&&... args) override;
    };

    template <typename BaseType, template <typename> class CreateStrategy, typename DerivedType, typename ... CtorArgs>
	    requires ValidCreator<BaseType, CreateStrategy>
    typename ConcreteCreator<BaseType, CreateStrategy, DerivedType, CtorArgs...>::returnTypes ConcreteCreator<BaseType,
    CreateStrategy, DerivedType, CtorArgs...>::Create(CtorArgs&&... args)
    {
	    return CreateStrategy<BaseType>::template Create<DerivedType>(std::forward<CtorArgs>(args)...);
    }

    //Real time factory with a register types member function 
    template<typename KeyType, typename BaseType, template<typename> class CreateStrategy, typename list = typelist<>> requires ValidCreator < BaseType, CreateStrategy>
    class RT_Factory;

    //Factory that stack constructor types in typeLists and creation strategy 
    template<typename KeyType, typename BaseType, template<typename> class CreateStrategy, typename ...CtorArgs>
    class RT_Factory<KeyType, BaseType, CreateStrategy, typelist<CtorArgs...>>
    {
    public:
        // alias for creation strategy
        using returnTypes = typename CreateStrategy<BaseType>::returnTypes;
        ~RT_Factory();
        //register as classic factory
        template<typename Derived>
        void Register(const KeyType& key) requires std::is_base_of_v<BaseType, Derived>;

        returnTypes Create(const KeyType& key, CtorArgs&&... args);

    private:
        std::map<KeyType, std::unique_ptr< ICreator<BaseType, CreateStrategy, CtorArgs...>>> m_map;
    };

    template <typename KeyType, typename BaseType, template <typename> class CreateStrategy, typename ... CtorArgs>
    RT_Factory<KeyType, BaseType, CreateStrategy, typelist<CtorArgs...>>::~RT_Factory() = default;

    template <typename KeyType, typename BaseType, template <typename> class CreateStrategy, typename ... CtorArgs>
    template <typename Derived>
    void RT_Factory<KeyType, BaseType, CreateStrategy, typelist<CtorArgs...>>::Register(const KeyType& key) requires std
	    ::is_base_of_v<BaseType, Derived>
    {
	    if (m_map.find(key) != m_map.end())
		    throw std::runtime_error("key Already Register");
	    m_map[key] = std::make_unique<ConcreteCreator<BaseType, CreateStrategy, Derived, CtorArgs...>>();
    }

    template <typename KeyType, typename BaseType, template <typename> class CreateStrategy, typename ... CtorArgs>
    typename RT_Factory<KeyType, BaseType, CreateStrategy, typelist<CtorArgs...>>::returnTypes RT_Factory<KeyType,
    BaseType, CreateStrategy, typelist<CtorArgs...>>::Create(const KeyType& key, CtorArgs&&... args)
    {
	    if (m_map.find(key) == m_map.end())
		    throw std::runtime_error("key not Register");
	    return m_map[key]->Create(std::forward<CtorArgs>(args)...);
    }

    //Predefined RT_Factory with creation strategy
    template<typename KeyType, typename BaseType, typename list = typelist<>>
    using RT_Pointer_Factory = RT_Factory<KeyType, BaseType, Pointer, list>;

    template<typename KeyType, typename BaseType, typename list = typelist<>>
    using RT_UniquePointer_Factory = RT_Factory<KeyType, BaseType, UniquePointer, list>;

    template<typename KeyType, typename BaseType, typename list = typelist<>>
    using RT_SharedPointer_Factory = RT_Factory<KeyType, BaseType, SharedPointer, list>;

}