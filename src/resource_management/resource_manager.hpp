#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "resource_loader.hpp"

template <class T>
class ResourceManager {
public:
    std::shared_ptr<T> load_resource(std::string path);
private:
    ResourceLoader<T>& const m_loader;
    std::unordered_map<std::string, std::weak_ptr<T>> m_cache;

    ResourceManager(const ResourceLoader&) = delete;
    ResourceManager& operator=(const ResourceLoader&) = delete;
};

template <class T>
std::shared_ptr<T> ResourceManager<T>::load_resource(std::string path)
{
    if (m_cache.contains(path))
    {
        if (std::shared_ptr<T> cached = m_cache[path].lock())
            return cached;
    }

    std::shared_ptr<T> res = m_loader.load_resource(path);
    m_cache.insert_or_assign(path, res);
    return res;
}