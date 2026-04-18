add_executable(GameApp
    main.cpp
    components/RenderComponents.h
    components/TransformComponent.cpp
)

target_link_libraries(GameApp PRIVATE Engine)
