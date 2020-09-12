#pragma once
#include <vector>
#include <memory>
#include "gl_vertex.h"
class Geometry;

typedef std::shared_ptr<Geometry> GeometryPointer;

class Geometry: public std::enable_shared_from_this<Geometry> {
private:
	static void caculateTangent(Vertex &p0, Vertex &p1, Vertex &p2);
public:
	

	static GeometryPointer GeneratePlaneGeometry(float size = 1.0f, float texcoordSize = 1.0f);
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	void PushVertex(Vertex &v);

	void PushIndex(const unsigned int &index);

	GeometryPointer Clone();

	GeometryPointer  AsPtr();
};

