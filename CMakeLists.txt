cmake_minimum_required(VERSION 3.23)
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
        column_list.c
        column_list.h
        main.c
        test_failed.c
        menu.c
        menu.c
        cdataframe_list.c
        cdataframe_list.h
        cdataframe_list.h
        column_tableau_dynamique.c
        cdataframe_tab_dynamique.c
        cdataframe_LDC.c
        column_LDC.c
        cdataframe_LDC.h
        column_LDC.c)
