cmake_minimum_required(VERSION 3.10)

project(main)

add_executable(${PROJECT_NAME} main.cpp fft.cpp complex.cpp sender.cpp sdlLayer.cpp BitFrequencyConverter.cpp ByteArrayParser.cpp BufferSoundTask.cpp RecordTask.cpp)

find_package(SDL2)
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})


#add_subdirectory(SDL2)
#target_include_directories(${PROJECT_NAME} PUBLIC SDL2/include)
# link_directories(${PROJECT_NAME} PUBLIC build/SDL2)
# target_link_libraries(${PROJECT_NAME} SDL2)

