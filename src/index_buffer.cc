/*
 * index_buffer.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "index_buffer.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <stdexcept>

IndexBuffer::IndexBuffer ()
{
	glGenBuffers ( 1, &resource_id );

	if ( resource_id == -1 )
		throw std::runtime_error ( "Failed to create vertex buffer" );
}

IndexBuffer::~IndexBuffer ()
{
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glDeleteBuffers ( 1, &resource_id );
}

void IndexBuffer::bind()
{
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, resource_id );
}

void IndexBuffer::unbind() const
{
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void IndexBuffer::set_data(const unsigned int * data, unsigned int count)
{
	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, resource_id );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW );
}
