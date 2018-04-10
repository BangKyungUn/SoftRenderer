#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Vertex
{
public:
	Vertex() {}
	Vector3 position;
	ULONG color;
};

struct Triangle
{
public:
	Triangle() {}
	Vertex vertices[3];

	bool IsInTriangle() { return false; }
};

