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

#include "scene.h"

class DemoApp
{
public:
	DemoApp( int argc, char **argv );
	~DemoApp();

	DemoApp( DemoApp & ) = delete;
	DemoApp( DemoApp && ) = delete;
	DemoApp &operator=( DemoApp & ) = delete;
	DemoApp &operator=( DemoApp && ) = delete;

	void run();

private:
	ALLEGRO_DISPLAY *window = nullptr;
	ALLEGRO_EVENT_QUEUE *queue = nullptr;
	DemoScene scene;
	int width = 640;
	int height = 480;
};

DemoApp::DemoApp( int /*argc*/, char ** /*argv*/ )
{
	al_install_system( ALLEGRO_VERSION_INT, atexit );
	al_install_keyboard();

	al_set_new_display_flags( ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0 );
	al_set_new_window_title( "Hello from Allegro" );

	/* trunk-ignore(clang-tidy/cppcoreguidelines-prefer-member-initializer) */
	window = al_create_display( width, height );

	/* trunk-ignore(clang-tidy/cppcoreguidelines-prefer-member-initializer) */
	queue = al_create_event_queue();

	al_register_event_source( queue, al_get_display_event_source( window ) );
	al_register_event_source( queue, al_get_keyboard_event_source() );

	if( glewInit() != GLEW_OK )
		throw std::runtime_error( "Cannot load GLEW" );

	scene.make_scene();
}

DemoApp::~DemoApp()
{
	al_destroy_display( window );
	al_destroy_event_queue( queue );

	al_uninstall_system();
}

void DemoApp::run()
{
	bool running = true;

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
				width = event.display.width;
				height = event.display.height;
				al_acknowledge_resize( window );
				break;
			}
		}

		scene.render_scene( width, height );

		al_flip_display();
	}
}

int main( int argc, char *argv[] )
{
	DemoApp( argc, argv ).run();
}