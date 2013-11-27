#ifndef _OBJ_LOADER_H
#define _OBJ_LOADER_H

#include "model_loader.h"
#include "gl_types.h"
#include <vector>

class ObjLoader : public ModelLoader {
public:
	ObjLoader();
	~ObjLoader();

	Renderable *loadModel(char *fileName) = 0;

private:
	std::vector<GLVec3> verts;
	std::vector<GLVec3> normals;
	std::vector<GLVec2> texCoords;
};

#endif
