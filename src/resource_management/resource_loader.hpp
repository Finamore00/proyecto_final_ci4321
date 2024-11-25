#pragma once

#include <memory>
#include <string>

/// @brief Base class. Loads a resource type.
/// @tparam T Type of resource to load
template <class T>
class ResourceLoader {
public:
    virtual std::shared_ptr<T> load_resource(std::string path) const = 0;
};