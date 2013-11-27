#version 130

in vec2 vertexPos2D;

void main() {
    gl_Position = vec4(vertexPos2D, 0.0, 1.0);
}
