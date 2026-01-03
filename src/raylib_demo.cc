/*
 * raylib_demo.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <vector>
#include <array>

#include <raylib.h>
#include "rlgl.h"

#include "../vendor/glad.h"

#include "scene.h"
#include <assert.h>

std::string vs = R"(
#version 330 core

layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 colour;

//out vec3 a_colour;

void main()
{
    gl_Position = vec4( position, 1.0 );
    //a_colour = colour;
}
)";

std::string fs = R"(
#version 330 core

//in vec3 a_colour;

out vec4 color;

void main()
{
  //color = vec4(a_colour, 1.0);
  color = vec4(1.0);
}
)";

struct vertex {
    std::array<float, 3> position;
    std::array<float, 3> colour;
};


std::vector<vertex> vertices = { { { -0.5F, -0.5F, 0.0F }, { 1.0F, 0.0F, 0.0F } },
                                    { { 0.5F, 0.5F, 0.0F }, { 0.0F, 1.0F, 0.0F } },
                                    { { 0.5F, -0.5F, 0.0F }, { 0.0F, 0.0F, 1.0F } } };
	// std::vector<vertex> vertices = { { { -1.0F, -1.0F, 0.0F } },
	// 								 { { 0.0F, 1.0F, 0.0F }   },
	// 								 { { 1.0F, -1.0F, 0.0F }  } };

int main( int argc, char** argv )
{
    SetConfigFlags( FLAG_WINDOW_RESIZABLE );
    InitWindow( 800, 640, "Hello World");  // Initialize window and OpenGL context

    Shader shader = LoadShaderFromMemory( vs.c_str(), fs.c_str() );

    int buffer_size = vertices.size() *  sizeof( vertex );

    assert( buffer_size ==  18 * sizeof( float) );

    GLuint vao = 0;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    {
        GLuint vbo = 0;
        glGenBuffers( 1, &vbo );

        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glBufferData( GL_ARRAY_BUFFER, vertices.size() *  sizeof( vertex ), vertices.data(), GL_STATIC_DRAW );

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0 );    // position is location 0 and is three floats
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( vertex ), (void *)offsetof( vertex, colour ) );    // colour is location 1 and is three floats

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        // glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }
    glBindVertexArray( 0 );

    while( ! WindowShouldClose() ) {

        BeginDrawing();
        EndDrawing();
        {
            // ClearBackground( Color{0x80, 0x80, 0x80, 0x00} );                          // Set background color (framebuffer clear color)

            // rlDrawRenderBatchActive();

	        glViewport( 0, 0, GetScreenWidth(), GetScreenHeight() );

	        glClearColor( 0.0F, 0.0F, 0.6F, 1.0F );
            glClear( GL_COLOR_BUFFER_BIT );

            glUseProgram( shader.id );

            glBindVertexArray( vao );
            glDrawArrays( GL_TRIANGLES, 0, 3 );
            glBindVertexArray( 0 );

            SwapScreenBuffer();

        }
        // EndDrawing();
    }

    CloseWindow();

    return 0;
}