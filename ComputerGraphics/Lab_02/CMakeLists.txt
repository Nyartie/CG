cmake_minimum_required(VERSION 3.10)
project(Lab_02)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем флаг для статической линковки
set(SFML_STATIC_LIBRARIES TRUE)

# Находим SFML
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
find_package(OpenGL REQUIRED)

find_library(GLU_LIBRARIES glu32)

file(GLOB_RECURSE SOURCES src/*.cpp glad/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})


target_include_directories(${PROJECT_NAME} PUBLIC include glad/include)
target_include_directories(${PROJECT_NAME} PUBLIC include glm)
# Линкуем библиотеки
target_link_libraries(Lab_02 
    sfml-system
    sfml-window
    sfml-graphics
    opengl32
    ${GLU_LIBRARIES}
)