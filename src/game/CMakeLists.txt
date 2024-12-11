add_library(game STATIC 
    bullet_component.cpp 
    firetruck_component.cpp 
    firetruck_cannon.cpp
    speedo_meter_component.cpp
    components/destroyable_component.cpp 
    components/obstacle_counter_component.cpp
    )
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/components")
target_link_libraries(game PUBLIC scene_graph particles)