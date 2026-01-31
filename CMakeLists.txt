cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(FluidSimulator)

# Define o padrão C++ (Recomendo C++17 para projetos modernos)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Encontra os pacotes do SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Lista todos os seus arquivos .cpp (Baseado no seu comando tree)
set(SOURCES
    main.cpp
    commands.cpp
    engine/engine.cpp
    entities/particle/particle.cpp
    entities/body/Circle/Circle.cpp
)

# Cria o executável com o nome "sfml-app"
add_executable(sfml-app ${SOURCES})

# Adiciona o diretório raiz aos caminhos de include
# Isso permite que você faça includes baseados na raiz (ex: #include "engine/engine.hpp")
target_include_directories(sfml-app PRIVATE ${CMAKE_SOURCE_DIR})

# Linka as bibliotecas do SFML ao seu executável
target_link_libraries(sfml-app PRIVATE sfml-graphics sfml-window sfml-system)