cmake_minimum_required(VERSION 3.16)

project(openg4-core) 
set(CMAKE_CXX_STANDARD 17) 
find_package(Geant4 REQUIRED ui_all vis_all) 

include(${Geant4_USE_FILE}) 
include_directories(include) # 'include' klasörü başlık dosyaları için include path olarak ekleniyor.
include_directories(include external) # 'external' klasörü de include path olarak ekleniyor.

file(GLOB SOURCES src/*.cc) # 'src' klasöründeki tüm .cc kaynak dosyaları SOURCES listesine ekleniyor.

add_executable(main main.cc ${SOURCES}) # 'main.cc' ve diğer kaynaklarla birlikte SMR-G4 adında çalıştırılabilir dosya oluşturuluyor.

target_link_libraries(main ${Geant4_LIBRARIES}) 

# 'macros', 'config', 'output' klasörleri derleme dizinine kopyalanıyor.
file(COPY ${PROJECT_SOURCE_DIR}/macros DESTINATION ${CMAKE_BINARY_DIR}) 
