cmake_minimum_required(VERSION 3.30)
project(COMP2510Assignment1 C)

set(CMAKE_C_STANDARD 23)

add_executable(COMP2510Assignment1 main.c
        hospital.h
        hospital.c
        patient.h
        patient.c
        doctor.h
        doctor.c
        utils.h
        utils.c
        globals.h
        globals.c)
