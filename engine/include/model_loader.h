#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

class Renderable;

class ModelLoader {

 public:
    // load the model and return a renderable
	virtual Renderable *loadModel(char *fileName) = 0;
};

#endif
