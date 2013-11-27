#ifndef _MESH_RENDERABLE_H_
#define _MESH_RENDERABLE_H_

#include <stdint.h>
#include <GL/glew.h>
#include "renderable.h"

class MeshRenderable : public Renderable {

public:
    MeshRenderable();
    ~MeshRenderable();

    void prep();
    void render();
private:
    GLint programId;
    GLuint VBO;
    GLuint IBO;

    uint32_t VBOSize;
    uint32_t IBOSize;
};


#endif
