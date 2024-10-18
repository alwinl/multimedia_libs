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
	SDL_Window *window;
	DemoScene scene;
	int width = 640;
	int height = 480;
};

DemoApp::DemoApp( int /*argc*/, char ** /*argv*/ )
{
	const int windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	SDL_Init( SDL_INIT_VIDEO );

	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "linear" );

	window = SDL_CreateWindow( "Hello from SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
							   windowFlags );

	SDL_GL_CreateContext( window );

	if( glewInit() != GLEW_OK )
		throw std::runtime_error( "Cannot load GLEW" );

	scene.make_scene();
}

DemoApp::~DemoApp()
{
	SDL_DestroyWindow( window );

	SDL_Quit();
}

void DemoApp::run()
{
	bool running = true;

	while( running ) {

		SDL_Event event;

		while( SDL_PollEvent( &event ) != 0 ) {
			switch( event.type ) {
			case SDL_QUIT: running = false; break;
			case SDL_KEYDOWN:
				if( event.key.keysym.sym == SDLK_ESCAPE )
					running = false;
				break;
			case SDL_WINDOWEVENT:
				if( event.window.event == SDL_WINDOWEVENT_RESIZED ) {
					width = event.window.data1;
					height = event.window.data2;
				}
				break;
			}
		}

		scene.render_scene( width, height );

		SDL_GL_SwapWindow( window );
	}
}

int main( int argc, char *argv[] )
{
	DemoApp( argc, argv ).run();
}