/*
 * vertex_buffer.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "vertex_buffer.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <stdexcept>

VertexBuffer::VertexBuffer ()
{
	glGenBuffers ( 1, &resource_id );

	if ( resource_id == -1 )
		throw std::runtime_error ( "Failed to create vertex buffer" );
}

VertexBuffer::~VertexBuffer ()
{
	glBindBuffer ( GL_ARRAY_BUFFER, 0 );
	glDeleteBuffers ( 1, &resource_id );
}

void VertexBuffer::bind()
{
	glBindBuffer ( GL_ARRAY_BUFFER, resource_id );
}

void VertexBuffer::unbind() const
{
	glBindBuffer ( GL_ARRAY_BUFFER, 0 );
}

void VertexBuffer::set_data(const void * data, unsigned int size)
{
	glBindBuffer ( GL_ARRAY_BUFFER, resource_id );
	glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );
}
