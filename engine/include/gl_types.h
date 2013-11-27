#ifndef GL_TYPES_H
#define GL_TYPES_H

/* If using gl3.h */
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL/gl.h>

struct GLVec2 {
	GLfloat x, y;
} __attribute__ ((__packed__));

struct GLVec3 {
	GLfloat x, y, z;
} __attribute__ ((__packed__));

struct GLVec4 {
	GLfloat x, y, z, w;
} __attribute__ ((__packed__));

struct GLColor3 {
	GLfloat r, g, b;
} __attribute__ ((__packed__));

struct GLColor4 {
	GLfloat r, g, b, a;
} __attribute__ ((__packed__));


#endif
