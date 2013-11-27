#ifndef _INPUT_HANDLER_H_
#define _INPUT_HANDLER_H_

#include <thread>
#include <stdint.h>

#include <glm/glm.hpp>
#include "SDL.h"

class FissionRender;

class FissionInputHandler {

public:
	FissionInputHandler(FissionRender *render);
	~FissionInputHandler();

	/// start the even handler thread
	bool init();

private:

	void handleKeyboardEvent(SDL_Event &event);
	void handleMouseEvent(SDL_Event &event);

	void handlerThread();

	std::thread inputThread;
	FissionRender *renderer;

};

#endif
