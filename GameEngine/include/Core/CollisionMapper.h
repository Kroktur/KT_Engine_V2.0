#pragma once 
#include "Math/OBB.h"
#include <map>

namespace KT
{
	template<typename KeyType, typename EntityType, typename OBBType, OBBType(*fn)(EntityType*)>
	class OBBMapper
	{
	public:
		static OBBType GetOrCreateOBB(const KeyType& key, EntityType* entity)
		{
			if (!m_map.contains(key))
			{
				if (!entity)
					throw std::runtime_error("entity must not be nullptr because obb not register");
				OBBType obb = fn(entity);
				Register(key, obb);
			}
			return m_map[key];
		}

		static void Clear()
		{
			m_map.clear();
		}
	private:
		static void Register(const KeyType& key, const OBBType& OBB)
		{
			if (m_map.contains(key))
				throw std::out_of_range("already Register");
			m_map[key] = OBB;
		}

		static std::map<KeyType, OBBType> m_map;
	};

	template<typename KeyType, typename EntityType, typename OBBType, OBBType(*fn)(EntityType*)>
	std::map<KeyType, OBBType> OBBMapper<KeyType, EntityType, OBBType, fn>::m_map{};

	template<typename KeyType, typename EntityType, KT::OBB2DF(*fn)(EntityType*)>
	using OBB2DFMapper = OBBMapper<KeyType, EntityType, KT::OBB2DF, fn>;

	// Pour double (2D)
	template<typename KeyType, typename EntityType, KT::OBB2DD(*fn)(EntityType*)>
	using OBB2DDMapper = OBBMapper<KeyType, EntityType, KT::OBB2DD, fn>;

	// Pour float (3D)
	template<typename KeyType, typename EntityType, KT::OBB3DF(*fn)(EntityType*)>
	using OBB3DFMapper = OBBMapper<KeyType, EntityType, KT::OBB3DF, fn>;

	// Pour double (3D)
	template<typename KeyType, typename EntityType, KT::OBB3DD(*fn)(EntityType*)>
	using OBB3DDMapper = OBBMapper<KeyType, EntityType, KT::OBB3DD, fn>;
}