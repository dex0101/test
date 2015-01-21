cmake_minimum_required(VERSION 2.8)

# тип сборки через задание значения предопределенной переменной CMAKE_BUILD_TYPE: DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
set( CMAKE_BUILD_TYPE RELEASE )

#----- проект
project(example)
set( SRC_EXE_DIR ${PROJECT_SOURCE_DIR}/sources/example )
list( APPEND SRC_EXE_LIST ${SRC_EXE_DIR}/main.cpp )

#----- библиотека
set( LIB_NAME dlm-pci-util )
set( LIB_VERSION PROPERTIES VERSION 0.0 SOVERSION 0 )
#set( LIB_TYPE STATIC )
set( LIB_TYPE SHARED )
set( SRC_LIB_DIR ${PROJECT_SOURCE_DIR}/sources/lib )
list( APPEND SRC_LIB_LIST ${SRC_LIB_DIR}/main.cpp )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )

#message( PROJECT_BINARY_DIR = ${PROJECT_BINARY_DIR} )
#message( PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR} )
#message( CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR} )

# пути на заголовочные файлы
#include_directories("headers/" "more_headers/")

# пути на библиотеки, которые следует добавить для линковки проекта
#link_directories( /path/to/lib_1 )

#можно проверить в коде через, например, #ifdef SOME_IMPORTANT_DEFINITION.
#add_definitions(-DSOME_IMPORTANT_DEFINITION)

#Если ваша версия GCC меньше, чем 4.7.0, вместо -std=c++11 нужно использовать -std=c++0x.
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pipe -Wall" ) 

#set( CMAKE_SHARED_LIBRARY_C_FLAGS "-fpic" )

add_library( ${LIB_NAME} ${LIB_TYPE} ${SRC_LIB_LIST} )
set_target_properties( ${LIB_NAME} ${LIB_VERSION} )

add_executable( ${PROJECT_NAME} ${SRC_EXE_LIST} )

target_link_libraries( ${PROJECT_NAME} ${LIB_NAME} )

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${PROJECT_SOURCE_DIR}/sources/dxversion.sh WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/sources )

#--- boost
#хотим динамическую линковку
#set(Boost_USE_STATIC_LIBS OFF)
#разрешает бусту внутри своих реализаций использовать треды для распараллеливания
#set(Boost_USE_MULTITHREADED ON)
#Допустим, нам нужны компоненты буста под названием chrono (библиотека для работы со временем) и filesystem (библиотека для работы с файловой системой)
#будут искаться только нужные библиотеки, и их расположение будет записано в переменную Boost_LIBRARIES
#Опция "REQUIRED" говорит о том, что библиотека необходима проекту
#find_package(Boost COMPONENTS chrono filesystem REQUIRED)
#include_directories(${Boost_INCLUDE_DIRS})
#target_link_libraries(test ${Boost_LIBRARIES})

