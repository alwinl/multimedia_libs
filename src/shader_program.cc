/*
 * shader_program.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "shader_program.h"

#include "fstream"
#include <algorithm>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>


ShaderProgram::~ShaderProgram()
{
	if( resource_id != -1 )
		glDeleteProgram( resource_id );
}

void ShaderProgram::set_source(std::filesystem::path path)
{
	std::ifstream shader_stream( path );

	shaders = parse_source( shader_stream );

	if( shaders.size() != 2 )
		throw std::runtime_error( "Shader source file must contain both vertex and fragment shader" );
}

void ShaderProgram::bind()
{
	if( resource_id == -1 ) {
		if( shaders.empty() )
			throw std::runtime_error( "Shader program not initialized" );

		resource_id = make_program( { make_shader( GL_VERTEX_SHADER, shaders[0] ), make_shader( GL_FRAGMENT_SHADER, shaders[1] ) } );
	}
	if( resource_id == -1 )
		throw std::runtime_error( "Shader program not initialized" );

	glUseProgram( resource_id );
}

void ShaderProgram::unbind() const
{
	glUseProgram( 0 );
}

std::vector<std::string> ShaderProgram::parse_source( std::istream& shader_stream )
{
	enum class eState { NOTHING = -1, VERTEX = 0, FRAGMENT = 1 };

	std::vector<std::string> shaders( 2 );
	eState current_state = eState::NOTHING;
	std::string line;

	while( std::getline( shader_stream, line ) ) {

		if( line.find( ":vertex" ) != std::string::npos ) {
			current_state = eState::VERTEX;
		} else if( line.find( ":fragment" ) != std::string::npos ) {
			current_state = eState::FRAGMENT;
		} else
			switch( current_state ) {
			case eState::VERTEX: shaders[0] += line + "\n"; break;
			case eState::FRAGMENT: shaders[1] += line + "\n"; break;
			default: break;
			}
	}

	return shaders;
}

unsigned int ShaderProgram::make_shader( unsigned int shader_type, std::string code )
{
	const unsigned int shader_id = glCreateShader( shader_type );

	char const *source = code.c_str();
	glShaderSource( shader_id, 1, &source, nullptr );

	glCompileShader( shader_id );

	int shader_compiled = GL_FALSE;
	glGetShaderiv( shader_id, GL_COMPILE_STATUS, &shader_compiled );

	if( shader_compiled == GL_FALSE ) {

		int size = 0;
		glGetShaderiv( shader_id, GL_INFO_LOG_LENGTH, &size );

		if( size == 0 )
			throw std::runtime_error( "Unknown shader compile error" );

		std::vector<char> log( size + 1 );
		glGetShaderInfoLog( shader_id, size, nullptr, log.data() );

		throw std::runtime_error( std::string( log.begin(), log.end() ) );
	}

	return shader_id;
}

unsigned int ShaderProgram::make_program( std::vector<unsigned int> shader_ids )
{
	unsigned int program_id = glCreateProgram();

	auto attach_fun = [&program_id]( unsigned int shader_id ) { glAttachShader( program_id, shader_id ); };
	auto detach_fun = [&program_id]( unsigned int shader_id ) {
		glDetachShader( program_id, shader_id );
		glDeleteShader( shader_id );
	};

	std::for_each( shader_ids.begin(), shader_ids.end(), attach_fun );

	glLinkProgram( program_id );

	std::for_each( shader_ids.begin(), shader_ids.end(), detach_fun );

	int program_linked = GL_FALSE;
	glGetProgramiv( program_id, GL_LINK_STATUS, &program_linked );

	if( program_linked == GL_FALSE ) {

		int size = 0;
		glGetProgramiv( program_id, GL_INFO_LOG_LENGTH, &size );

		if( size == 0 )
			throw std::runtime_error( "Unknown program link error" );

		std::vector<char> log( size + 1 );
		glGetProgramInfoLog( program_id, size, nullptr, log.data() );

		throw std::runtime_error( std::string( log.begin(), log.end() ) );
	}

	return program_id;
}
