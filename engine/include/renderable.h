#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

class Renderable {

public:

	virtual void prep() = 0;
	virtual void render() = 0;

};

#endif
