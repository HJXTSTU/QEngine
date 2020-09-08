#include "gl_array_buffer.h"

void GLArrayBuffer::resize(GLsizeiptr size) {
	if (this->m_totalSize > 0) {
		GLuint newBuffer;
		glGenBuffers(1, &newBuffer);
		glBindBuffer(m_currentBufferTarget, newBuffer);
		glBufferData(m_currentBufferTarget, size, NULL, m_usage);
		glBindBuffer(m_currentBufferTarget, 0);
		glBindBuffer(GL_COPY_READ_BUFFER, this->id);
		glBindBuffer(GL_COPY_WRITE_BUFFER, newBuffer);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, m_totalSize);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		glDeleteBuffers(1, &this->id);

		this->id = newBuffer;
	}
	else {
		glBufferData(m_currentBufferTarget, size, NULL, m_usage);
	}
	this->m_totalSize = size;
	
}

void GLArrayBuffer::Allocate(GLsizeiptr size) {
	this->resize(size);
}

GLArrayBuffer::GLArrayBuffer(GLenum usage)
	:m_usage(usage), m_totalSize(0), m_oppicupied(0), m_currentBufferTarget(0){
	glGenBuffers(1, &this->id);
}

void GLArrayBuffer::BindTo(GLuint buffer_target)
{
	this->m_currentBufferTarget = buffer_target;
	glBindBuffer(buffer_target, this->id);
}

void GLArrayBuffer::UnBind()
{
	if (this->m_currentBufferTarget == 0)return;
	glBindBuffer(this->m_currentBufferTarget, 0);
	this->m_currentBufferTarget = 0;
}

void GLArrayBuffer::AddArray(GLsizeiptr size, const void* values) {
	if (m_currentBufferTarget == 0)return;
	if (m_oppicupied + size >= m_totalSize) {
		this->resize(m_oppicupied + size);
	}
	glBufferSubData(m_currentBufferTarget, m_oppicupied, size, values);
	m_oppicupied += size;
}

bool GLArrayBuffer::ReplaceArrayData(GLsizeiptr startOffset, GLsizeiptr size, const void* values) {
	if (m_currentBufferTarget == 0)return false;
	if (startOffset + size > this->m_oppicupied)return false;
	glBufferSubData(m_currentBufferTarget, startOffset, size, values);
	return true;
}

void GLArrayBuffer::Clear()
{
	glDeleteBuffers(this->m_totalSize, &this->id);
	this->id = 0;
	glGenBuffers(1, &this->id);
	this->m_totalSize = 0;
	this->m_oppicupied = 0;
	this->m_currentBufferTarget = 0;
}

GLArrayBuffer GLArrayBuffer::Create(GLenum usage)
{
	return GLArrayBuffer(usage);
}

GLArrayBuffer GLArrayBuffer::Create(GLsizeiptr size, GLenum usage)
{
	GLArrayBuffer res = Create(usage);
	res.Allocate(size);
	return res;
}