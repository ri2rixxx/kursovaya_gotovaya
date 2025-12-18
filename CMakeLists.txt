cmake_minimum_required(VERSION 3.10)
project(memory_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Find SFML
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

# Find PostgreSQL с явным указанием путей
set(PostgreSQL_ADDITIONAL_VERSIONS 15 14 13 12)
find_package(PostgreSQL REQUIRED)

# Если не находит, попробуем вручную
if(NOT PostgreSQL_FOUND)
    message(WARNING "PostgreSQL not found via find_package, trying manual...")
    set(PostgreSQL_INCLUDE_DIRS "/usr/include/postgresql")
    set(PostgreSQL_LIBRARIES "pq")
endif()

# Сообщаем о найденных библиотеках
message(STATUS "PostgreSQL found: ${PostgreSQL_LIBRARIES}")
message(STATUS "PostgreSQL include: ${PostgreSQL_INCLUDE_DIRS}")

add_executable(memory_game
    src/main.cpp
    src/Game.cpp
    src/Card.cpp
    src/Player.cpp
    src/Database.cpp
    src/Achievement.cpp
    src/UserManager.cpp
    src/GUI/Button.cpp
    src/GUI/CardSprite.cpp
    src/GUI/Menu.cpp
    src/Audio/SoundManager.cpp
    src/Audio/MusicPlayer.cpp
    src/ContactForm.cpp
    src/EmailSender.cpp
)

target_include_directories(memory_game PRIVATE 
    include
    ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(memory_game
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    ${PostgreSQL_LIBRARIES}
    pthread
)
