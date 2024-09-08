/*
 * allegro_demo.cc Copyright 2024 Alwin Leerling dna.leerling@gmail.com
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

#include <allegro5/allegro.h>
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
	ALLEGRO_DISPLAY *window = nullptr;
	ALLEGRO_EVENT_QUEUE *queue = nullptr;

	unsigned int vao = -1;

	void scene_setup();
	void scene_render() const;
};

DemoApp::DemoApp( int /*argc*/, char ** /*argv*/ )
{
	al_install_system( ALLEGRO_VERSION_INT, atexit );
	al_install_keyboard();

	al_set_new_display_flags( ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0 );
	al_set_new_window_title( "Hello from Allegro" );

	/* trunk-ignore(clang-tidy/cppcoreguidelines-prefer-member-initializer) */
	window = al_create_display( 640, 480 );

	/* trunk-ignore(clang-tidy/cppcoreguidelines-prefer-member-initializer) */
	queue = al_create_event_queue();

	al_register_event_source( queue, al_get_display_event_source( window ) );
	al_register_event_source( queue, al_get_keyboard_event_source() );

	if( glewInit() != GLEW_OK )
		throw std::runtime_error( "Cannot load GLEW" );
}

DemoApp::~DemoApp()
{
	al_destroy_display( window );
	al_destroy_event_queue( queue );

	al_uninstall_system();
}

int DemoApp::run()
{
	bool running = true;

	scene_setup();

	while( running ) {

		ALLEGRO_EVENT event;

		while( al_get_next_event( queue, &event ) ) {

			switch( event.type ) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE: running = false; break;

			case ALLEGRO_EVENT_KEY_DOWN:
				switch( event.keyboard.keycode ) {
				case ALLEGRO_KEY_ESCAPE: running = false; break;
				}
				break;

			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				glViewport( 0, 0, event.display.width, event.display.height );
				al_acknowledge_resize( window );
				break;
			}
		}

		glClearColor( 0.0F, 0.0F, 0.6F, 0.0F );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		scene_render();

		al_flip_display();
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