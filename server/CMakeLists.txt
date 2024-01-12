cmake_minimum_required(VERSION 3.27)
project(Server)

set(CMAKE_CXX_STANDARD 17)

add_executable(Server main.cpp
        Server.h
        Server.cpp
        Piece.cpp
        Piece.h
        Pawn.cpp
        Pawn.h
        Knight.cpp
        Knight.h
        Rook.cpp
        Rook.h
        Bishop.cpp
        Bishop.h
        Queen.cpp
        Queen.h
        King.cpp
        King.h
)
