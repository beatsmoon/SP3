#ifndef MESH_H
#define MESH_H
//fragment shader Max texture = 8

#define MAX_TEXTURES 3
#include <string>
#include "Material.h"

class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	virtual ~Mesh();
	virtual void Render();
	void Render(unsigned offset, unsigned count);

	const std::string name;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;

	unsigned textureID; // bcos of text on screen

	Material material;

	//multitexturing
	unsigned textureArray[MAX_TEXTURES];
};

#endif