#pragma once
#include <map>
#include <string>
#include <filesystem>
#include "Tools/Typelist.h"
namespace KT
{
	//Resource Manager
	// need Improvement Reload Resource
	template<typename TextureType, typename ConstructArgs, auto FN>
	class ResourceManager;

	// take a function that load the resource and return unique_ptr
	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	class ResourceManager<TextureType, typelist<ConstructArgs...>, FN >
	{
	public:
		using TexturePtr = std::unique_ptr<TextureType>;

		static void SetGlobalFIlePath(const std::filesystem::path& racinePath);
		~ResourceManager();

		static TextureType& Load(const std::string& relativePath, ConstructArgs&& ... args);
		static void Unload(const std::string& relativePath);
		static void Reload(const std::string& relativePath, ConstructArgs&& ... args);
		static void ReloadAll(ConstructArgs&& ... args);
		static void UnloadAll();
		static bool Exists(const std::string& relativePath);
		static bool Exists(TextureType* texture);
		std::string GetRelativePath(TextureType& texture);
	private:
		static std::string GetAbsoluteFilePath(const std::string& relativePath);
		static std::string GetRelativeFilePath(const std::string& AbsolutePath);
		static std::filesystem::path m_absoluteFilePath;
		static std::map<std::string, TexturePtr> m_textureMap;
	};
	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	std::filesystem::path ResourceManager<TextureType, typelist<ConstructArgs...>, FN >::m_absoluteFilePath = std::filesystem::path{};

	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	std::map<std::string, std::unique_ptr<TextureType>> ResourceManager<TextureType, typelist<ConstructArgs...>, FN >::m_textureMap = std::map<std::string, std::unique_ptr<TextureType>>{};


	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	TextureType& ResourceManager<TextureType, typelist<ConstructArgs...>, FN>::Load(const std::string& relativePath,
		ConstructArgs&&... args)
	{
		// save in the map and return if already store
		auto fullPath = GetAbsoluteFilePath(relativePath);
		auto it = m_textureMap.find(fullPath);
		if (it != m_textureMap.end())
			return *it->second;
		// create and save if new 
		m_textureMap[fullPath] = std::move(FN(fullPath, std::forward<ConstructArgs>(args)...));
		return *m_textureMap[fullPath];
	}

	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	void ResourceManager<TextureType, typelist<ConstructArgs...>, FN>::SetGlobalFIlePath(
		const std::filesystem::path& racinePath)
	{
		static bool init = false;
		if (init)
			throw std::runtime_error("can set the FilePhat once");
		m_absoluteFilePath = racinePath;
		init = true;
	}

	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	ResourceManager<TextureType, typelist<ConstructArgs...>, FN>::~ResourceManager()
	{
		m_textureMap.clear();
	}

	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	void ResourceManager<TextureType, typelist<ConstructArgs...>, FN>::UnloadAll()
	{
		m_textureMap.clear();
	}

	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	void ResourceManager<TextureType, typelist<ConstructArgs...>, FN>::Reload(const std::string& relativePath,
		ConstructArgs&&... args)
	{
		Unload(relativePath);
		Load(relativePath, std::forward<ConstructArgs>(args)...);
	}

	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	void ResourceManager<TextureType, typelist<ConstructArgs...>, FN>::ReloadAll(ConstructArgs&&... args)
	{
		for (auto it = m_textureMap.begin(); it != m_textureMap.end(); )
		{
			auto relativePath = GetRelativeFilePath(it->first);
			it = m_textureMap.erase(it);
			Load(relativePath, std::forward<ConstructArgs>(args)...);
		}
	}

	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	bool ResourceManager<TextureType, typelist<ConstructArgs...>, FN>::Exists(const std::string& relativePath)
	{
		return m_textureMap.contains(GetAbsoluteFilePath(relativePath));
	}

	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	bool ResourceManager<TextureType, typelist<ConstructArgs...>, FN>::Exists(TextureType* texture)
	{
		auto it = std::find_if(m_textureMap.begin(),
			m_textureMap.end(),
			[&](const auto& pair) {return pair.second.get() == texture; }
		);
		return it != m_textureMap.end();
	}

	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	std::string ResourceManager<TextureType, typelist<ConstructArgs...>, FN>::GetRelativePath(TextureType& texture)
	{
		auto it = std::find_if(m_textureMap.begin(),
			m_textureMap.end(),
			[&](const auto& pair) {return pair.second.get() == texture; }
		);
		if (it == m_textureMap.end())
			throw std::out_of_range("texture dont exist");
		return GetRelativeFilePath(it->first);
	}

	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	void ResourceManager<TextureType, typelist<ConstructArgs...>, FN>::Unload(const std::string& relativePath)
	{
		if (!Exists(relativePath))
			throw std::out_of_range("ressource is not load ");
		m_textureMap.erase(GetAbsoluteFilePath(relativePath));
	}

	// get the full path
	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	std::string ResourceManager<TextureType, typelist<ConstructArgs...>, FN>::GetAbsoluteFilePath(const std::string& relativePath)
	{
		auto filePath = m_absoluteFilePath / std::filesystem::path(relativePath);
		return filePath.string();
	}

	template<typename TextureType, typename... ConstructArgs, std::unique_ptr<TextureType>(*FN)(const std::string&, ConstructArgs...)>
	std::string ResourceManager<TextureType, typelist<ConstructArgs...>, FN>::GetRelativeFilePath(const std::string& AbsolutePath)
	{
		std::filesystem::path full = AbsolutePath;
		std::filesystem::path local = relative(full, m_absoluteFilePath);
		return local.string();
	}
}