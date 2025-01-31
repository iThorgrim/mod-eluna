target_include_directories(game
    PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/mod-eluna/src/LuaEngine
)

target_link_libraries(game
    PRIVATE
        lualib
)