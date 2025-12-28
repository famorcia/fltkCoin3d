# fltkCoin3D Porting on fltk of Coin3D (a.k.a Open Inventor) examples
# MIT License
#
# Copyright (c) 2025 Fabrizio Morciano
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

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
pkg_check_modules(EGL REQUIRED egl)
find_package(FLTK REQUIRED)

include_directories( ${COIN3D_INCLUDE_DIRS})

add_subdirectory(minimal)
add_subdirectory(Duck_Coin3D_Window)
#add_subdirectory(fltkIvViewer)


