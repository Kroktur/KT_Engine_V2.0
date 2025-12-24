#pragma once
#include "Composite.h"
#include "Tools/PrimaryTypeTrait.h"


namespace KT
{
	template<typename DerivedType, typename BaseType, typename RootType  >
	concept IncludeRootVerifier = is_base_of_v<RootType, DerivedType>&& is_base_of_v<BaseType, DerivedType>;



	template<typename DerivedType, typename BaseType, typename RootType > requires is_base_of_v<BaseType, DerivedType>
	class CompositeCRTP : public IComposite<BaseType, RootType>
	{
	public:
		using composite = IComposite<BaseType, RootType>;
		void ExecuteAction(const std::function<void(BaseType*)>& fn) override final;
		void ExecuteAction(const std::function<void(typename composite::component*)>& fn) override final;
		BaseType* AsBase() override final;
		const BaseType* AsBase() const override final;
	protected:
		CompositeCRTP(composite* owner);
	};



	template<typename DerivedType, typename BaseType, typename RootType > requires is_base_of_v<RootType, DerivedType>
	class ExcludeRootCRTP : public IRoot<BaseType, RootType>
	{
	public:
		using composite = IComposite<BaseType, RootType>;
		using root = IRoot < BaseType, RootType >;

		~ExcludeRootCRTP() override = default;
		void ExecuteAction(const std::function<void(BaseType*)>& fn) override final;
		void ExecuteAction(const std::function<void(typename composite::component*)>& fn) override final;
		BaseType* AsBase() override final;
		const BaseType* AsBase() const override final;

		RootType* AsRoot() override final;
		const RootType* AsRoot() const override final;

	protected:
		void AddFullTree(std::vector<typename composite::component*>& vec) override final;
		ExcludeRootCRTP();
	};

	template<typename DerivedType, typename BaseType, typename RootType > requires  IncludeRootVerifier<DerivedType, BaseType, RootType>
	class IncludeRootCRTP : public IRoot<BaseType, RootType>
	{
	public:
		using composite = IComposite<BaseType, RootType>;
		using root = IRoot < BaseType, RootType >;

		~IncludeRootCRTP() override = default;
		void ExecuteAction(const std::function<void(BaseType*)>& fn) override;
		void ExecuteAction(const std::function<void(typename composite::component*)>& fn) override final;

		BaseType* AsBase() override;
		const BaseType* AsBase() const override final;

		RootType* AsRoot() override;
		const RootType* AsRoot() const override final;

	protected:
		void AddFullTree(std::vector<typename composite::component*>& vec) override final;
		IncludeRootCRTP();
	};


	template<typename DerivedType, typename BaseType, typename RootType > requires is_base_of_v<BaseType, DerivedType>
	class LeafCRTP : public ILeaf<BaseType, RootType>
	{
	public:
		using composite = IComposite<BaseType, RootType>;
		using leaf = ILeaf<BaseType, RootType>;
		~LeafCRTP() override = default;
		void ExecuteAction(const std::function<void(BaseType*)>& fn) override;
		void ExecuteAction(const std::function<void(typename composite::component*)>& fn) override final;
		BaseType* AsBase() override;
		const BaseType* AsBase() const override;
	protected:
		LeafCRTP(composite* owner);
	};
	//CompositeCRTP Implementation
	template <typename DerivedType, typename BaseType, typename RootType> requires is_base_of_v<BaseType, DerivedType>
	void CompositeCRTP<DerivedType, BaseType, RootType>::ExecuteAction(const std::function<void(BaseType*)>& fn)
	{
		fn(static_cast<BaseType*>(static_cast<DerivedType*>(this)));
		for (auto& child : this->m_child)
			child->ExecuteAction(fn);
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires is_base_of_v<BaseType, DerivedType>
	void CompositeCRTP<DerivedType, BaseType, RootType>::ExecuteAction(
		const std::function<void(typename composite::component*)>& fn)
	{
		fn(this);
		for (auto& child : this->m_child)
			child->ExecuteAction(fn);
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires is_base_of_v<BaseType, DerivedType>
	BaseType* CompositeCRTP<DerivedType, BaseType, RootType>::AsBase()
	{
		return static_cast<BaseType*>(static_cast<DerivedType*>(this));
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires is_base_of_v<BaseType, DerivedType>
	const BaseType* CompositeCRTP<DerivedType, BaseType, RootType>::AsBase() const
	{
		return static_cast<const BaseType*>(static_cast<const DerivedType*>(this));
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires is_base_of_v<BaseType, DerivedType>
	CompositeCRTP<DerivedType, BaseType, RootType>::CompositeCRTP(composite* owner) : composite(owner)
	{
	}

	// ExcludeRootCRTP Implementation 

	template <typename DerivedType, typename BaseType, typename RootType>  requires is_base_of_v<RootType, DerivedType>
	ExcludeRootCRTP<DerivedType, BaseType, RootType>::ExcludeRootCRTP() : root()
	{
	}

	template <typename DerivedType, typename BaseType, typename RootType>  requires is_base_of_v<RootType, DerivedType>
	void ExcludeRootCRTP<DerivedType, BaseType, RootType>::ExecuteAction(const std::function<void(BaseType*)>& fn)
	{
		for (auto& child : this->m_child)
			child->ExecuteAction(fn);
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires is_base_of_v<RootType, DerivedType>
	void ExcludeRootCRTP<DerivedType, BaseType, RootType>::ExecuteAction(
		const std::function<void(typename composite::component*)>& fn)
	{
		for (auto& child : this->m_child)
			child->ExecuteAction(fn);
	}

	template <typename DerivedType, typename BaseType, typename RootType>  requires is_base_of_v<RootType, DerivedType>
	BaseType* ExcludeRootCRTP<DerivedType, BaseType, RootType>::AsBase()
	{
		return nullptr;
	}

	template <typename DerivedType, typename BaseType, typename RootType>  requires is_base_of_v<RootType, DerivedType>
	const BaseType* ExcludeRootCRTP<DerivedType, BaseType, RootType>::AsBase() const
	{
		return nullptr;
	}

	template <typename DerivedType, typename BaseType, typename RootType>  requires is_base_of_v<RootType, DerivedType>
	RootType* ExcludeRootCRTP<DerivedType, BaseType, RootType>::AsRoot()
	{
		return  static_cast<RootType*>(static_cast<DerivedType*>(this));
	}

	template <typename DerivedType, typename BaseType, typename RootType>  requires is_base_of_v<RootType, DerivedType>
	const RootType* ExcludeRootCRTP<DerivedType, BaseType, RootType>::AsRoot() const
	{
		return  static_cast<const RootType*>(static_cast<const DerivedType*>(this));
	}

	template <typename DerivedType, typename BaseType, typename RootType>  requires is_base_of_v<RootType, DerivedType>
	void ExcludeRootCRTP<DerivedType, BaseType, RootType>::AddFullTree(std::vector<typename composite::component*>& vec)
	{
		IRoot<BaseType, RootType>::AddFullTree(vec);
		vec.erase(vec.begin());
	}

	// IncludeRootCRTP Implementation 

	template <typename DerivedType, typename BaseType, typename RootType> requires IncludeRootVerifier<DerivedType, BaseType, RootType>
	IncludeRootCRTP<DerivedType, BaseType, RootType>::IncludeRootCRTP() : root()
	{

	}

	template <typename DerivedType, typename BaseType, typename RootType> requires  IncludeRootVerifier<DerivedType, BaseType, RootType>
	void IncludeRootCRTP<DerivedType, BaseType, RootType>::ExecuteAction(const std::function<void(BaseType*)>& fn)
	{
		fn(static_cast<BaseType*>(static_cast<DerivedType*>(this)));
		for (auto& child : this->m_child)
			child->ExecuteAction(fn);
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires IncludeRootVerifier<DerivedType,
		BaseType, RootType>
	void IncludeRootCRTP<DerivedType, BaseType, RootType>::ExecuteAction(
		const std::function<void(typename composite::component*)>& fn)
	{
		fn(this);
		for (auto& child : this->m_child)
			child->ExecuteAction(fn);
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires  IncludeRootVerifier<DerivedType, BaseType, RootType>
	BaseType* IncludeRootCRTP<DerivedType, BaseType, RootType>::AsBase()
	{
		return static_cast<BaseType*>(static_cast<DerivedType*>(this));
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires  IncludeRootVerifier<DerivedType, BaseType, RootType>
	const BaseType* IncludeRootCRTP<DerivedType, BaseType, RootType>::AsBase() const
	{
		return static_cast<const BaseType*>(static_cast<const DerivedType*>(this));
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires  IncludeRootVerifier<DerivedType, BaseType, RootType>
	RootType* IncludeRootCRTP<DerivedType, BaseType, RootType>::AsRoot()
	{
		return  static_cast<RootType*>(static_cast<DerivedType*>(this));
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires  IncludeRootVerifier<DerivedType, BaseType, RootType>
	const RootType* IncludeRootCRTP<DerivedType, BaseType, RootType>::AsRoot() const
	{
		return  static_cast<const RootType*>(static_cast<const DerivedType*>(this));
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires  IncludeRootVerifier<DerivedType, BaseType, RootType>
	void IncludeRootCRTP<DerivedType, BaseType, RootType>::AddFullTree(std::vector<typename composite::component*>& vec)
	{
		IRoot<BaseType, RootType>::AddFullTree(vec);
	}


	// LeafCRTP Implementation 



	template <typename DerivedType, typename BaseType, typename RootType> requires is_base_of_v<BaseType, DerivedType>
	void LeafCRTP<DerivedType, BaseType, RootType>::ExecuteAction(const std::function<void(BaseType*)>& fn)
	{
		fn(static_cast<BaseType*>(static_cast<DerivedType*>(this)));
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires is_base_of_v<BaseType, DerivedType>
	void LeafCRTP<DerivedType, BaseType, RootType>::ExecuteAction(
		const std::function<void(typename composite::component*)>& fn)
	{
		fn(this);
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires is_base_of_v<BaseType, DerivedType>
	BaseType* LeafCRTP<DerivedType, BaseType, RootType>::AsBase()
	{
		return static_cast<BaseType*>(static_cast<DerivedType*>(this));
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires is_base_of_v<BaseType, DerivedType>
	const BaseType* LeafCRTP<DerivedType, BaseType, RootType>::AsBase() const
	{
		return static_cast<const BaseType*>(static_cast<const DerivedType*>(this));
	}

	template <typename DerivedType, typename BaseType, typename RootType> requires is_base_of_v<BaseType, DerivedType>
	LeafCRTP<DerivedType, BaseType, RootType>::LeafCRTP(composite* owner) : leaf(owner)
	{
	}

}
