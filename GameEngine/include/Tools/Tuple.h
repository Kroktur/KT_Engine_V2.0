#pragma once
#include <vector>
#include <memory>
#include "Typelist.h"
namespace KT

{
	struct IData
	{
		virtual  ~IData() = default;
	};
	template<typename type>
	struct CData : IData
	{
		type data;
	};

template <typename... Types>
	struct Tuple
{
		using type_list = KT::typelist<Types...>;
		Tuple() : m_datas(KT::size_v<type_list>)
		{
			int i = 0;
			(..., (m_datas[i++] = std::make_unique<CData<Types>>()));
		}

		template<size_t index>
		KT::at_t<index,type_list>& Get()
		{
			using DataType = KT::at_t<index, type_list>;
			CData<DataType>* dataPtr = static_cast<CData<DataType>*>(m_datas[index].get());
			return dataPtr->data;
		}

private:
		std::vector<std::unique_ptr<IData>> m_datas;
};



	
} 
