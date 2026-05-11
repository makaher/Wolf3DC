cmake_minimum_required(VERSION 3.10)
project(Wolf3DC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Source files
set(SOURCES
    src/main.cpp
    src/ui.cpp
    src/project_manager.cpp
    src/compiler_pipeline.cpp
    src/ide_interface.cpp
    src/logger.cpp
    src/simple_c_compiler.cpp
)

# Headers
set(HEADERS
    src/ui.h
    src/project_manager.h
    src/compiler_pipeline.h
    src/ide_interface.h
    src/logger.h
    src/simple_c_compiler.h
)

# Create executable
add_executable(w3d_ide ${SOURCES} ${HEADERS})

# Platform-specific settings
if(WIN32)
    target_compile_options(w3d_ide PRIVATE /W4)
else()
    target_compile_options(w3d_ide PRIVATE -Wall -Wextra)
endif()
