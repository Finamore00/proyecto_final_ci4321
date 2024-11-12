#pragma once

#include "../../scene_graph/component.hpp"

class DestroyableComponent : public Component {
public:
    using Component::Component;
protected:
    void __on_collision(ColliderComponent& collider, Transform& transform) override;
};