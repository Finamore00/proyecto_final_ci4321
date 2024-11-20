#pragma once
#include <unordered_map>
#include <vector>

class Component;
class SceneObject;

class LogicEngine {
private:
    static LogicEngine *g_instance;
    std::unordered_map<SceneObject*, std::vector<Component*>> m_comps;

public:
    LogicEngine();
    
    static LogicEngine* get_instance();
    void register_component(SceneObject* obj, Component* comp);
    void update(float dt) const;
};