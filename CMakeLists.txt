cmake_minimum_required(VERSION 3.16)

project(NRXVertexApp LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(NRX_DIR "C:/Program Files/Nanosoft/nanoCAD x64 25.0/SDK")

add_definitions(-D_AFXDLL)
set(CMAKE_GENERATOR_TOOLSET "v142")
set(CMAKE_MFC_FLAG 2)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.h
    include/json.hpp
)

add_library(NRXVertexApp SHARED ${PROJECT_SOURCES})

# add_library(manip src/Manipulator.cpp src/Manipulator.h)
# target_link_libraries(NRXVertexApp PUBLIC manip)
#target_link_libraries(NRXVertexApp PRIVATE include/json.hpp)

set_target_properties(NRXVertexApp PROPERTIES 
    VS_USER_PROPS ${NRX_DIR}/include/arxgate/rxsdk_debug.props
    VS_GLOBAL_CharacterSet "Unicode"
    VS_GLOBAL_UseOfMfc "Dynamic")

# set_target_properties(NRXVertexApp PROPERTIES VS_GLOBAL_CharacterSet "Unicode")

# set_target_properties(NRXVertexApp PROPERTIES VS_GLOBAL_UseOfMfc "Dynamic")

target_compile_definitions(NRXVertexApp PRIVATE
    UNICODE
    _UNICODE
)
target_compile_options(NRXVertexApp PRIVATE
    "$<$<CONFIG:Debug>:/DUNICODE /D_UNICODE>"
    "$<$<CONFIG:Release>:/DUNICODE /D_UNICODE>"
)

target_include_directories(NRXVertexApp PRIVATE ${NRX_DIR}/include/arxgate)
target_include_directories(NRXVertexApp PRIVATE ${NRX_DIR}/include/nrxdbgate)
target_include_directories(NRXVertexApp PRIVATE ${NRX_DIR}/include/MAPI)
target_include_directories(NRXVertexApp PRIVATE ${NRX_DIR}/include/nrxhostgate)
target_include_directories(NRXVertexApp PRIVATE ${NRX_DIR}/include/nrxuigate)
target_include_directories(NRXVertexApp PRIVATE ${NRX_DIR}/include/nrxgate)
target_include_directories(NRXVertexApp PRIVATE ${NRX_DIR}/include/nrxpropsgate)
target_include_directories(NRXVertexApp PRIVATE ${NRX_DIR}/include)
target_include_directories(NRXVertexApp PRIVATE ${NRX_DIR}/include-x64)
target_include_directories(NRXVertexApp PRIVATE ${NRX_DIR}/include-x64/Drawing/ActiveX)
target_include_directories(NRXVertexApp PRIVATE ${NRX_DIR}/include-x64/Drawing/ActiveX/OdaX)

target_link_directories(NRXVertexApp PRIVATE ${NRX_DIR}/lib-x64)
target_link_directories(NRXVertexApp PRIVATE ${NRX_DIR}/lib-x64/TD)
target_link_directories(NRXVertexApp PRIVATE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)

target_link_libraries(NRXVertexApp
    nrxdbgate.lib
    nrxgatemgd.lib
    nrxhostgate.lib
    mfcs140.lib # Пример для VS2022 (140 = версия CRT)
    mfc140u.lib # Unicode версия MFC (обязательно!)
)

# Переименуем DLL в .nrx
set_target_properties(NRXVertexApp PROPERTIES SUFFIX ".nrx")

# Установи нужный стандарт C++
target_compile_features(NRXVertexApp PRIVATE cxx_std_14)
