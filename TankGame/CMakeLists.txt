cmake_minimum_required(VERSION 3.27)
project(TankGame)

set(CMAKE_CXX_STANDARD 17)

# Find OpenGL and GLEW
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Source files
set(SOURCE_FILES
        main.cpp
        header_files/Shader.cpp
        header_files/Vector.cpp
        header_files/Mesh.cpp
        header_files/Matrix.cpp
        header_files/SphericalCameraManipulator.cpp
        header_files/Texture.cpp
        glad/src/glad.c
)

# Object files
set(OBJECT_FILES
        header_files/main.o
        header_files/Shader.o
        header_files/Vector.o
        header_files/Mesh.o
        header_files/Matrix.o
        header_files/SphericalCameraManipulator.o
        header_files/Texture.o
)

# Header files
set(HEADER_FILES
        header_files/Shader.h
        header_files/Vector.h
        header_files/Mesh.h
        header_files/Matrix.h
        header_files/SphericalCameraManipulator.h
        header_files/Texture.h
        header_files/stb_truetype.h
        VBO.h
        VBO.cpp
        VAO.h
        VAO.cpp
        EBO.h
        EBO.cpp
        glad/glad.h
)

# Compile source files
add_custom_command(
        OUTPUT ${OBJECT_FILES}
        COMMAND ${CMAKE_CXX_COMPILER}
        ARGS -g -std=gnu++1z -fPIC
        $<FLAGS>
        $<fälltousrelative:SOURCE_FILES>
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Compiling source files"
)

# Compile source files
add_executable(TankGame ${SOURCE_FILES} ${HEADER_FILES})

# Include directories
target_include_directories(TankGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/header_files)

include_directories(
        "C:/Program Files/Msys/mingw64/include/stb/stb_image"
)

# Link against the required libraries
target_link_libraries(TankGame PRIVATE
        -lOpenGL32
        -lglew32
        -lfreeglut
)
