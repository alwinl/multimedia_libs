/*
 * sdl_demo.cc Copyright 2024 Alwin Leerling dna.leerling@gmail.com
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

#include <stdexcept>
#include <vector>
#include <array>

#include <SDL2/SDL.h>

#include <GL/glew.h>

#include "load_shaders.h"

class DemoApp
{
public:
	DemoApp( int argc, char **argv );
	~DemoApp();

	DemoApp( DemoApp & ) = delete;
	DemoApp( DemoApp && ) = delete;
	DemoApp &operator=( DemoApp & ) = delete;
	DemoApp &operator=( DemoApp && ) = delete;

	int run();

private:
	SDL_Window *window;
	SDL_Renderer *renderer;

	unsigned int vao = -1;

	void scene_setup();
	void scene_render() const;
};

DemoApp::DemoApp( int /*argc*/, char ** /*argv*/ )
{
	const int rendererFlags = SDL_RENDERER_ACCELERATED;
	const int windowFlags = 0;
	const int width = 640;
	const int height = 480;

	SDL_Init( SDL_INIT_VIDEO );

	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "linear" );

	window = SDL_CreateWindow( "Hello from SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
							   windowFlags );
	renderer = SDL_CreateRenderer( window, -1, rendererFlags );

	if( glewInit() != GLEW_OK )
		throw std::runtime_error( "Cannot load GLEW" );
}

DemoApp::~DemoApp()
{
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );

	SDL_Quit();
}

int DemoApp::run()
{
	bool running = true;

	scene_setup();

	while( running ) {

		SDL_Event event;

		while( SDL_PollEvent( &event ) != 0 ) {
			if( event.type == SDL_QUIT )
				running = false;
		}

		glClearColor( 0.0F, 0.0F, 0.6F, 0.0F );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		scene_render();

		SDL_RenderPresent( renderer );
	}

	return 0;
}

void DemoApp::scene_setup()
{
	constexpr unsigned int position_index = 0;
	constexpr unsigned int colour_index = 1;

	struct vertex {
		std::array<float, 3> position;
		std::array<float, 3> colour;
	};

	struct attribute_description {
		unsigned int index;
		int component_count;
		unsigned int component_type;
		unsigned char is_normalized;
		size_t offset;
	};

	const std::vector<attribute_description> vertex_description = {
		{ position_index, 3, GL_FLOAT, GL_FALSE, offsetof( vertex, position ) }, // position attribute
		{ colour_index, 3, GL_FLOAT, GL_FALSE, offsetof( vertex, colour ) }		 // colour attribute
	};

	std::vector<vertex> vertices = { { { -1.0F, -1.0F, 0.0F }, { 1.0F, 0.0F, 0.0F } },
									 { { 0.0F, 1.0F, 0.0F }, { 0.0F, 1.0F, 0.0F } },
									 { { 1.0F, -1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F } } };

	const unsigned int program_id = load_program( "../res/shaders/simple.glsl" );
	unsigned int vertex_buffer = -1;

	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	glGenBuffers( 1, &vertex_buffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
	glBufferData( GL_ARRAY_BUFFER, static_cast<int64_t>( vertices.size() * sizeof( vertex ) ), vertices.data(),
				  GL_STATIC_DRAW );

	for( const auto &attribute : vertex_description ) {
		glEnableVertexAttribArray( attribute.index );
		glVertexAttribPointer( attribute.index, attribute.component_count, attribute.component_type,
							   attribute.is_normalized, sizeof( vertex ),
							   /* trunk-ignore(clang-tidy/cppcoreguidelines-pro-type-reinterpret-cast) */
							   /* trunk-ignore(clang-tidy/performance-no-int-to-ptr) */
							   reinterpret_cast<const void *>( attribute.offset ) );
	}

	glUseProgram( program_id );

	glBindVertexArray( 0 );
}

void DemoApp::scene_render() const
{
	glBindVertexArray( vao );

	glDrawArrays( GL_TRIANGLES, 0, 3 );
}

int main( int argc, char *argv[] )
{
	return DemoApp( argc, argv ).run();
}