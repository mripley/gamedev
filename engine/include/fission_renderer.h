#ifndef FISSION_RENDERER_H
#define FISSION_RENDERER_H

#include "SDL.h"
#include <GL/glew.h>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <stdint.h>

enum RenderState{
    STOPPED,
    RUNNING,
    EXIT
};

// A wrapper around a GL context
class FissionRender
{
public:
    // instantiate a copy of the render class and init data we need
	FissionRender(uint32_t width, uint32_t height);
    virtual ~FissionRender();

    // control the running state of the render thread.
    void toggleRenderState(RenderState state);

	// Create a window and set the render thread running
	void init();

	// prepare a scene for rendering. Init all the buffers and shaders
	void initScene();

private:
	void SDLDie(const char* msg);
	void checkSDLError(int line = -1);

    // init the openGL rendering thread and bind the context to a SDL window
    int initRenderer(int width, int heigh);
    void render();

	char *loadShader(const char *fileName);

    std::thread renderThread;

    // the actual render context
    SDL_GLContext glContext;
	SDL_Window *mainWindow;

	uint32_t window_width;
	uint32_t window_height;

	std::atomic<bool> quit;
	volatile bool running;
	std::condition_variable renderCv;
	std::mutex renderMutex;
	GLint gVertexPos2DLocation;
	GLuint program_id;
    GLuint gVBO;
    GLuint gIBO;

};

#endif // FISSION_RENDERER_H
