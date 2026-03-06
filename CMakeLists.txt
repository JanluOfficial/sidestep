cmake_minimum_required(VERSION 3.10)
project(sidestep VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Code files ---
set(SOURCES 
    main.cpp 
    map.cpp 
    menu.cpp
    sound_resource.cpp
)

# --- Build Options ---
if(EMSCRIPTEN)
    # WebAssembly specific settings
    add_executable(sidestep ${SOURCES})
    
    set_target_properties(sidestep PROPERTIES OUTPUT_NAME "game")
    set_target_properties(sidestep PROPERTIES SUFFIX ".html")

    target_compile_options(sidestep PRIVATE -Os -Wall -DPLATFORM_WEB)
    
    # Linker flags for Emscripten
    target_link_options(sidestep PRIVATE 
        "-sUSE_GLFW=3" 
        "--shell-file" "${CMAKE_CURRENT_SOURCE_DIR}/path-to/shell.html"
    )
    
    # Point to your pre-built web library
    target_link_libraries(sidestep PRIVATE "C:/raylib/raylib/src/web/libraylib.a")
    target_include_directories(sidestep PRIVATE . "C:/raylib/raylib/src/external")

elseif(WIN32)
    # Windows specific settings
    # Add WIN32 to add_executable to hide the console (the -mwindows equivalent)
    add_executable(sidestep WIN32 ${SOURCES})
    
    target_compile_options(sidestep PRIVATE -O1 -Wall -Wno-missing-braces)
    
    # Static linking and standard libraries
    target_link_libraries(sidestep PRIVATE 
        raylib 
        opengl32 
        gdi32 
        winmm
    )
    target_link_options(sidestep PRIVATE -static)

else() # Linux and whatever else
    add_executable(sidestep ${SOURCES})
    target_link_libraries(sidestep PRIVATE raylib)
endif()

# Common include path
target_include_directories(sidestep PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# --- Resource Handling ---

# Desktop (Windows/Linux)
if(NOT EMSCRIPTEN)
    add_custom_command(TARGET sidestep POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:sidestep>/resources"
        COMMENT "Copying resources to build directory..."
    )
endif()

# 2. WebAssembly (Emscripten)
if(EMSCRIPTEN)
    target_link_options(sidestep PRIVATE 
        "--preload-file" "${CMAKE_CURRENT_SOURCE_DIR}/resources@resources"
    )
endif()
