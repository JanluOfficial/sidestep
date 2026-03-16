cmake_minimum_required(VERSION 3.10)
project(sidestep VERSION 1.0 LANGUAGES CXX)

find_package(raylib CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Automatically find all source files ---
# This looks in src/ and its subdirectories (like src/scenes/)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# --- Build Options ---
if(EMSCRIPTEN)
    add_executable(sidestep ${SOURCES})
    
    set_target_properties(sidestep PROPERTIES OUTPUT_NAME "index")
    set_target_properties(sidestep PROPERTIES SUFFIX ".html")

    target_compile_options(sidestep PRIVATE -Os -Wall -DPLATFORM_WEB)
    
    # We use -L and -l instead of hardcoded C:/ paths to find raylib
    # You just need to ensure the raylib web library is in your library path
    target_link_options(sidestep PRIVATE 
        "-sUSE_GLFW=3" 
        "--shell-file" "${CMAKE_CURRENT_SOURCE_DIR}/path-to/shell.html"
        "--preload-file" "${CMAKE_CURRENT_SOURCE_DIR}/resources@resources"
    )
    
    # This assumes raylib for web is installed/findable by the emscripten toolchain
    target_link_libraries(sidestep PRIVATE raylib)

elseif(WIN32)
    # Hide the console window on Windows 
    add_executable(sidestep WIN32 ${SOURCES})
    
    target_compile_options(sidestep PRIVATE -O1 -Wall -Wno-missing-braces)
    
    # Standard Windows libraries 
    target_link_libraries(sidestep PRIVATE 
        raylib
	glfw3
        opengl32 
        gdi32 
        winmm
    )
    target_link_options(sidestep PRIVATE -static)

else() # Linux (Arch)
    add_executable(sidestep ${SOURCES})
    target_link_libraries(sidestep PRIVATE raylib m pthread dl rt)
endif()

# --- Common include paths ---
# Now that headers are in /include, we tell CMake where to look
target_include_directories(sidestep PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src  # In case you have headers there
)

# --- Resource Handling for Desktop ---
if(NOT EMSCRIPTEN)
    add_custom_command(TARGET sidestep POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:sidestep>/resources"
        COMMENT "Copying resources to build directory..."
    )
endif()
