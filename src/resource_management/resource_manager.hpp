#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>

#include "resource_loader.hpp"

/// @brief Class that loads a filesystem resource, caches it and creates resource pointers to it.
/// @tparam T Resource to load
template <class T>
class ResourceManager {
public:
    std::shared_ptr<T> load_resource(const std::string& path);
    ResourceManager<T>(const ResourceLoader<T>& loader);
private:
    const ResourceLoader<T>& m_loader;
    std::unordered_map<std::string, std::weak_ptr<T>> m_cache; // We need some kind of cleanup

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
};

/// @brief Loads a resource
/// @tparam T Type of resource to load
/// @param path Path of the resource to load
/// @return Shared pointer to the loaded resource.
template <class T>
std::shared_ptr<T> ResourceManager<T>::load_resource(const std::string& path)
{
    if (m_cache.find(path) != m_cache.end())
    {
        std::cout << "Checking resource cache" << std::endl;
        if (std::shared_ptr<T> cached = m_cache[path].lock())
            return cached;

        std::cout << "Cache is expired" << std::endl;
    }

    std::cout << "Resource not found in cache" << std::endl;
    std::shared_ptr<T> res = m_loader.load_resource(path);
    m_cache.insert_or_assign(path, res);
    return res;
}

template <class T>
ResourceManager<T>::ResourceManager(const ResourceLoader<T>& loader) : m_loader(loader) {}