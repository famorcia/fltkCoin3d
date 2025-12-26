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

ADD_EXECUTABLE(Duck_Coin3D_Window Duck_Coin3D_Window.cpp main.cpp)

TARGET_LINK_LIBRARIES(Duck_Coin3D_Window PRIVATE
        OpenGL::GL
        OpenGL::GLU
        ${COIN3D_LINK_LIBRARIES}
        fltk::fltk fltk::gl)

