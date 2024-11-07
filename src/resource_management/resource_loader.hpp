#pragma once

#include <memory>
#include <string>

template <class T>
class ResourceLoader {
public:
    virtual std::shared_ptr<T> load_resource(std::string path) = 0;
};