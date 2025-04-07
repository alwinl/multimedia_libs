/*
 * shader_prgram.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#pragma once

#include <string>
#include <vector>
#include <filesystem>

class ShaderProgram
{
public:
	ShaderProgram() = default;
	~ShaderProgram();

	void set_source( std::filesystem::path path );

	void bind();
	void unbind() const;

private:
	unsigned int resource_id = -1;
	std::vector<std::string> shaders;

	std::vector<std::string> parse_source( std::istream& shader_stream );
	unsigned int make_shader( unsigned int shader_type, std::string code );
	unsigned int make_program( std::vector<unsigned int> shader_ids );
};
