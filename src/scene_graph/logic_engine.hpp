#pragma once
#include <unordered_map>
#include <vector>

class Component;
class SceneObject;

class LogicEngine {
private:
    std::unordered_map<SceneObject*, std::vector<Component*>> m_comps;

public:
    void register_component(SceneObject* obj, Component* comp);
    void update(float dt) const;
};