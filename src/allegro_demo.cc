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

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>

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
};

DemoApp::DemoApp( int /*argc*/, char ** /*argv*/ )
{
	al_install_system( ALLEGRO_VERSION_INT, atexit );

	al_set_new_display_flags( ALLEGRO_WINDOWED | ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0 );
	al_set_new_window_title( "Hello from Allegro" );

	/* trunk-ignore(clang-tidy/cppcoreguidelines-prefer-member-initializer) */
	window = al_create_display( 640, 480 );

	/* trunk-ignore(clang-tidy/cppcoreguidelines-prefer-member-initializer) */
	queue = al_create_event_queue();

	al_register_event_source( queue, al_get_display_event_source( window ) );
}

DemoApp::~DemoApp()
{
	al_destroy_display( window );

	al_uninstall_system();
}

int DemoApp::run()
{
	bool running = true;

	while( running ) {

		ALLEGRO_EVENT event;

		while( al_get_next_event( queue, &event ) ) {
			if( event.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
				running = false;
		}

		glClearColor( 0.0F, 0.0F, 0.6F, 0.0F );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		al_flip_display();
	}

	return 0;
}

int main( int argc, char *argv[] )
{
	return DemoApp( argc, argv ).run();
}