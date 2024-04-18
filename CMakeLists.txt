cmake_minimum_required(VERSION 3.24)
project(Projet_C_with_Warren)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(.idea)

add_executable(Projet_C_with_Warren
        .idea/.gitignore
        .idea/modules.xml
        .idea/Projet_C_with_Warren.iml
        .idea/vcs.xml
        .idea/workspace.xml
        column.c
        column.h
        main.c)
