cmake_minimum_required(VERSION 3.10)
project(Lab_01)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем флаг для статической линковки
set(SFML_STATIC_LIBRARIES TRUE)



# Находим SFML
find_package(SFML COMPONENTS system window graphics REQUIRED PATHS "C:\\Users\\dima_\\Documents\\libraries\\SFML-2.5.0\\include")

file(GLOB SOURCES ./*cpp ./*hpp)

add_executable(Lab_01 ${SOURCES})

# Линкуем библиотеки
target_link_libraries(Lab_01 
    sfml-system
    sfml-window
    sfml-graphics
    opengl32
)