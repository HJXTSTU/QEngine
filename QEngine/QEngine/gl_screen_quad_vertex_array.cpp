#include "gl_screen_quad_vertex_array.h"

ScreenQuad::ScreenQuad():GLBaseVertexArray()
{
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
	};

	this->Use();
	m_vbo.BindBuffer();
	m_vbo.BufferData(sizeof(quadVertices), quadVertices);
	this->EnableVertexAttrib(0, GL_FLOAT, 2, GL_FALSE, 2 * sizeof(float), 0);
	this->EnableVertexAttrib(1, GL_FLOAT, 2, GL_FALSE, 2 * sizeof(float), 2*sizeof(float));
	this->UnUse();
	m_vbo.UnBindBuffer();
}

void ScreenQuad::DrawScreenQuad()
{
	this->Use();
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
	this->UnUse();
}
