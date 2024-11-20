#pragma once

#include <functional>
#include <vector>
#include "../../scene_graph/component.hpp"

using OnDestroyFn = void (*)();

class DestroyableComponent : public Component {
protected:
    void __on_collision(ColliderComponent& collider, Transform& transform) override;
public:
    using Component::Component;
};