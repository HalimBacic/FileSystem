cmake_minimum_required(VERSION 3.14)
project(FSystem)

set(CMAKE_CXX_STANDARD 14)
link_libraries(-lstdc++fs)
add_executable(FSystem main.cpp Superblok.cpp Inode.cpp Inode.h Memory.cpp Memory.h Filesystem.cpp Filesystem.h Bitmap.cpp Bitmap.h)