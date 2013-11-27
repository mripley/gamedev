#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include "fission_renderer.h"
#include "input_handler.h"

int main(int argc, char **argv) {

    FissionRender renderer(512, 512);
	FissionInputHandler inputHandler(&renderer);
	renderer.init();
	if (!inputHandler.init()){
		printf("Unable to init event handler!\n");
		return 1;
	}

	renderer.toggleRenderState(RUNNING);

    return 0;
}
