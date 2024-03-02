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

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

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

	while( running ) {

		SDL_Event event;

		while( SDL_PollEvent( &event ) != 0 ) {
			if( event.type == SDL_QUIT )
				running = false;
		}

		glClearColor( 0.0F, 0.0F, 0.6F, 0.0F );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		SDL_RenderPresent( renderer );
	}

	return 0;
}

int main( int argc, char *argv[] )
{
	return DemoApp( argc, argv ).run();
}