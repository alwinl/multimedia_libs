/*
 * sfml_demo.cc Copyright 2024 Alwin Leerling dna.leerling@gmail.com
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

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class DemoApp
{
public:
	DemoApp( int argc, char **argv );
	~DemoApp() = default;

	DemoApp( DemoApp & ) = delete;
	DemoApp( DemoApp && ) = delete;
	DemoApp &operator=( DemoApp & ) = delete;
	DemoApp &operator=( DemoApp && ) = delete;

	int run();

private:
	std::unique_ptr<sf::RenderWindow> window;
};

DemoApp::DemoApp( int /*argc*/, char ** /*argv*/ )
{
	window = std::make_unique<sf::RenderWindow>( sf::VideoMode( 640, 480 ), "Hello from SFML" );
}

int DemoApp::run()
{
	while( window->isOpen() ) {

		auto event = sf::Event{};

		while( window->pollEvent( event ) ) {
			if( event.type == sf::Event::Closed )
				window->close();
		}

		glClearColor( 0.0F, 0.0F, 0.6F, 0.0F );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		window->display();
	}

	return 0;
}

int main( int argc, char *argv[] )
{
	return DemoApp( argc, argv ).run();
}