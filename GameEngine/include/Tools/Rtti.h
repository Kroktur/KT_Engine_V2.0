#pragma once
namespace KT
{
	// Simple RTTI system need improvement
	class RTTI
	{
	public:
		template<typename type>
		static int GetTypeId();

		template <typename type>
		static int GetInstanceId();

	private:
		static int m_id;
	};

	template <typename type>
	int RTTI::GetTypeId()
	{
		// each type will have its own static id
		static int id = m_id++;
		return id;
	}

	template <typename type>
	int RTTI::GetInstanceId()
	{
		// each instance of a type will have its own static id 
		static int id = 0;
		return id++;
	}

	// initialize static member
	inline int RTTI::m_id = 0;
}
