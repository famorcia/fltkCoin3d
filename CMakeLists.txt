# fltkCoin3D Porting on fltk of Coin3D (a.k.a Open Inventor) examples
# Copyright (C) 2025  Fabrizio Morciano

# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
# USA

cmake_minimum_required(VERSION 3.15)
project(fltkCoin3D)

include(FindPkgConfig)
include(FindOpenGL)

if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "GL is required")
endif()
if(NOT OPENGL_GLU_FOUND)
    message(FATAL_ERROR "GLU is required")
endif()


pkg_check_modules (COIN3D Coin REQUIRED)

include(FetchContent)

FetchContent_Declare(FLTK
        GIT_REPOSITORY  https://github.com/fltk/fltk
        GIT_TAG         release-1.4.4
        GIT_SHALLOW     TRUE
)

message(STATUS "Download and build FLTK if necessary, please wait...")
FetchContent_MakeAvailable(FLTK)
message(STATUS "Download and build FLTK - done.")


include_directories( ${COIN3D_INCLUDE_DIRS})

add_subdirectory(minimal)
#add_subdirectory(glutDuck)
#add_subdirectory(fltkIvViewer)


