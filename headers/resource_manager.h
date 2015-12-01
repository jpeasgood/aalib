#ifndef AA_RESOURCE_MANAGER_H
#define AA_RESOURCE_MANAGER_H

#include "../headers/singleton.h"
#include "../headers/resource_factory.h"
#include "../headers/shader_factory.h"

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <string>

namespace aa
{
	class resource_manager : public singleton<resource_manager>
	{
		friend class singleton<resource_manager>;
		friend class constructor;

		private:
			struct constructor
			{
				constructor();
			};

			static constructor cons;

			template <typename T, typename U>
			class pair_hash
			{
				public:
					size_t operator()(const std::pair<T, U> &pair) const;
			};

			std::unordered_map<std::type_index, std::unique_ptr<resource_factory>> factories;
			std::unordered_map<std::pair<std::type_index, std::string>, std::shared_ptr<resource>, pair_hash<std::type_index, std::string>> resources;

			resource_manager() = default;
			resource_manager(const resource_manager &other) = delete;
			resource_manager(resource_manager &&other) = delete;

			resource_manager &operator=(const resource_manager &other) = delete;
			resource_manager &operator=(resource_manager &&other) = delete;

		public:
			template<class T>
			void add_factory(std::unique_ptr<resource_factory> factory);

			template<class T>
			std::weak_ptr<T> load(const std::string &path);
	};

	template<typename T>
	void resource_manager::add_factory(std::unique_ptr<resource_factory> factory)
	{
		factories[typeid(T)] = std::move(factory);
	}
#include <iostream>
	template<typename T>
	std::weak_ptr<T> resource_manager::load(const std::string &path)
	{
		std::type_index type(typeid(T));
		std::pair<std::type_index, std::string> pair = {type, path};
		auto seach_result = resources.find(pair);
		if(seach_result == resources.end())
		{
			std::shared_ptr<T> res = std::dynamic_pointer_cast<T>(factories[type]->load(path));
			resources[pair] = res;
			std::cout << "found" << std::endl;
			return std::weak_ptr<T>(std::dynamic_pointer_cast<T>(res));
		}
		else
		{
			return std::weak_ptr<T>(std::dynamic_pointer_cast<T>(seach_result->second));
		}
	}
}

template <typename T, typename U>
size_t aa::resource_manager::pair_hash<T, U>::operator()(const std::pair<T, U> &pair) const
{
	std::hash<T> ht;
	std::hash<U> hu;
	size_t h = ht(pair.first);
	h ^= hu(pair.second) + 0x9e3779b9 + (h << 6) + (h >> 2);
	return h;
}

#endif
