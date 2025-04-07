#
# CMakeLists.txt Copyright 2024 Alwin Leerling dna.leerling@gmail.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.
#

add_executable( allegro_demo allegro_demo.cc scene.cc shader_program.cc )
add_executable( sdl_demo sdl_demo.cc scene.cc shader_program.cc )
add_executable( sfml_demo sfml_demo.cc scene.cc shader_program.cc )

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED allegro-5)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SFML REQUIRED sfml-all)
pkg_check_modules( GLEW REQUIRED glew)

target_link_libraries(allegro_demo PRIVATE ${ALLEGRO_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries( sdl_demo PRIVATE ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries( sfml_demo PRIVATE ${SFML_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
