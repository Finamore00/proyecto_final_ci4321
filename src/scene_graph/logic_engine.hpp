#pragma once
#include <unordered_map>
#include <vector>

class Component;
class SceneObject;

class LogicEngine {
private:
    static LogicEngine *g_instance;
    std::unordered_map<unsigned int, Component*> m_comps;

public:
    LogicEngine();
    
    static LogicEngine* get_instance();
    void register_component(Component* comp);
    void unregister_component(unsigned int id);
    void update(float dt) const;
};