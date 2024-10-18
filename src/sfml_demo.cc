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

#include <stdexcept>
#include <memory>
#include <vector>
#include <array>

#include <SFML/Graphics.hpp>
#include <GL/glew.h>

#include "scene.h"

class DemoApp
{
public:
	DemoApp( int argc, char **argv );
	~DemoApp() = default;

	DemoApp( DemoApp & ) = delete;
	DemoApp( DemoApp && ) = delete;
	DemoApp &operator=( DemoApp & ) = delete;
	DemoApp &operator=( DemoApp && ) = delete;

	void run();

private:
	std::unique_ptr<sf::RenderWindow> window;
	DemoScene scene;
	int width = 640;
	int height = 480;
};

DemoApp::DemoApp( int /*argc*/, char ** /*argv*/ )
{
	window = std::make_unique<sf::RenderWindow>( sf::VideoMode( width, height ), "Hello from SFML" );

	if( glewInit() != GLEW_OK )
		throw std::runtime_error( "Cannot Load GLEW" );

	scene.make_scene();
}

void DemoApp::run()
{
	while( window->isOpen() ) {

		auto event = sf::Event{};

		while( window->pollEvent( event ) ) {
			switch( event.type ) {
			case sf::Event::Closed: window->close(); break;
			case sf::Event::KeyPressed:
				if( event.key.code == sf::Keyboard::Escape )
					window->close();
				break;
			case sf::Event::Resized:
				width = event.size.width;
				height = event.size.height;
				break;
			}
		}

		scene.render_scene( width, height );

		window->display();
	}
}

int main( int argc, char *argv[] )
{
	DemoApp( argc, argv ).run();
}