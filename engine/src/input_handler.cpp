#include "input_handler.h"
#include "fission_renderer.h"
#include <assert.h>

FissionInputHandler::FissionInputHandler(FissionRender *render) : renderer(render) {

}

FissionInputHandler::~FissionInputHandler() {
	// before we go join the input handler thread
	inputThread.join();
}

bool FissionInputHandler::init(){
	if (renderer == NULL) {
		printf("Input handler requires a Fission Render context to work against. Refusing to start!\n");
		return false;
	}

 	auto threadStart = [this](){
		handlerThread();
	};


	inputThread = std::thread(threadStart);
	return true;
}

void FissionInputHandler::handlerThread() {

	while(1){
		SDL_Event event;
		while (SDL_PollEvent(&event)) {


			switch(event.type) {
			case SDL_QUIT:
				printf("got the quit event!\n");
				renderer->toggleRenderState(STOPPED);
				renderer->toggleRenderState(EXIT);
				return;

			case SDL_KEYDOWN:
				handleKeyboardEvent(event);
				break;

			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
				handleMouseEvent(event);
				break;

			default:
				printf("Got something else!\n");
				break;
			}
		}
	}
}

void FissionInputHandler::handleKeyboardEvent(SDL_Event &event) {
	assert(renderer != NULL);
}

void FissionInputHandler::handleMouseEvent(SDL_Event &event) {
	assert(renderer != NULL);
}
