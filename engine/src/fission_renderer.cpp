#include "fission_renderer.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


void printShaderLog( GLuint shader )
{
	//Make sure name is shader
	if( glIsShader( shader ) )
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

		//Allocate string
		char* infoLog = new char[ maxLength ];

		//Get info log
		glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf( "Name %d is not a shader\n", shader );
	}
}

void printProgramLog( GLuint program )
{
	//Make sure name is shader
	if( glIsProgram( program ) )
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

		//Allocate string
		char* infoLog = new char[ maxLength ];

		//Get info log
		glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			printf( "%s\n", infoLog );
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf( "Name %d is not a program\n", program );
	}
}


FissionRender::FissionRender(uint32_t width, uint32_t height) : window_width(width), window_height(height) {
    quit = false;
	running = false;
	renderMutex.lock(); // start un a locked state
}

FissionRender::~FissionRender() {
	quit = true;
	running = false;
	renderMutex.unlock();
    renderThread.join();

    /* Delete our opengl context, destroy our window, and shutdown SDL */
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(mainWindow);
	SDL_Quit();
}

void FissionRender::init(){

    auto threadStart = [this](){
		if (initRenderer(window_width, window_height)) {
			printf("Failed to init renderer!\n");
			return;
		}
		render();
        glDeleteProgram(program_id);
    };

    renderThread = std::thread(threadStart);
}

void FissionRender::SDLDie(const char* msg) {
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
}

void FissionRender::checkSDLError(int line) {
#ifndef NDEBUG
    const char *error = SDL_GetError();
    if (*error != '\0')
    {
        printf("SDL Error: %s\n", error);
        if (line != -1)
            printf(" + line: %i\n", line);
        SDL_ClearError();
    }
#endif
}

// init the openGL subsystem and bind the context to a SDL window
int FissionRender::initRenderer(int width, int height){

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDLDie("Unable to init SDL");
		return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, GL_CONTEXT_COMPATIBILITY_PROFILE_BIT );

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    checkSDLError(__LINE__);
	mainWindow = SDL_CreateWindow("sdltest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    checkSDLError(__LINE__);

    if (!mainWindow) {
        SDLDie("Unable to create window");
		return 1;
    }

    glContext = SDL_GL_CreateContext(mainWindow);
    if (glContext == NULL)
    {
        printf("Gl context could not be created!\n");
        return 1;
    }


    glewExperimental = GL_TRUE;
    GLenum err;
    if ((err = glewInit()) != GLEW_OK)
    {
	 	/* Problem: glewInit failed, something is seriously wrong. */
	 	fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    if (SDL_GL_SetSwapInterval(1) < 0) {
        printf("Unalbe to set VSync!\n");
        return 1;
    }

	initScene();

	return 0;
}

void FissionRender::toggleRenderState(RenderState state)
{
    switch(state)
    {
        case RUNNING:
            running = true;
			renderMutex.unlock();
            break;
        case STOPPED:
            running = false;
            break;
        case EXIT:
			running = false;
			renderMutex.unlock();
            quit = true;
            break;
    }
}

void FissionRender::render(){

    while (!quit.load())
    {
		// go to sleep before we start rendering. Have the calling thread wake us up to get rendering
        renderMutex.lock();

		while (running) {
			//Clear color buffer
			glClear( GL_COLOR_BUFFER_BIT );

            //Bind program
            glUseProgram( program_id );

            //Enable vertex position
            glEnableVertexAttribArray( gVertexPos2DLocation );

            //Set vertex data
            glBindBuffer( GL_ARRAY_BUFFER, gVBO );
            glVertexAttribPointer( gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL );

            //Set index data and render
            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
            glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );

            //Disable vertex position
            glDisableVertexAttribArray( gVertexPos2DLocation );

            //Unbind program
            glUseProgram( 0 );

            SDL_GL_SwapWindow(mainWindow);
		}
    }
}

// temp for the time being but lets just test for now
void FissionRender::initScene() {
    program_id = -1;
	program_id = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char *vertexSource = loadShader("shaders/2d_simple.vert");
    const char *fragmentSource = loadShader("shaders/2d_simple.frag");

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//Check vertex shader for errors
	GLint shaderCompStatus = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompStatus );
	if(shaderCompStatus == GL_FALSE )
	{
		printf( "Unable to compile vertex shader %d!\n", vertexShader );
		printShaderLog(vertexShader);
		delete vertexSource;
        return;
	}

    shaderCompStatus = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompStatus);
	if (shaderCompStatus == GL_FALSE) {
		printf(" Unable to compile fragment shader %d!\n", fragmentShader);
		printShaderLog(fragmentShader);

        delete vertexSource;
        delete fragmentSource;
		return;
	}

	glAttachShader(program_id, vertexShader);
	glAttachShader(program_id, fragmentShader);

	glLinkProgram(program_id);

	GLint programSuccess = GL_FALSE;
	glGetProgramiv(program_id, GL_LINK_STATUS, &programSuccess);
	if (programSuccess == GL_FALSE) {
		printf("Unable to link shader program!\n");
		printProgramLog( program_id );
		delete vertexSource;
		delete fragmentSource;
		return;
	}

	// get the uniform location
	this->gVertexPos2DLocation = glGetAttribLocation(program_id, "vertexPos2D");
	if (this->gVertexPos2DLocation == -1) {
		printf("Couldn't bind 2D vertex location in the shader!\n");
		delete vertexSource;
		delete fragmentSource;
		return;
	}

    printf("attribute location is %d\n", this->gVertexPos2DLocation);
    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

	//VBO data
	GLfloat vertexData[] =
	{
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

	//IBO data
	GLuint indexData[] = { 0, 1, 2, 3 };

	//Create VBO
	glGenBuffers( 1, &gVBO );
	glBindBuffer( GL_ARRAY_BUFFER, gVBO );
	glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );

	//Create IBO
	glGenBuffers( 1, &gIBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );

    delete vertexSource;
    delete fragmentSource;
}

// load a shader from a given file and return its data in a dynamically allocated char *. Free the return value!
char *FissionRender::loadShader(const char *fileName) {
	struct stat stat_buf;
	char *shader_text;

	// stat the file to get the info we need
	int retval =  stat(fileName, &stat_buf);

	if (retval) {
		printf("Unable to open file %s\n", fileName);
		return NULL;
	}

	shader_text = new char[stat_buf.st_size];

	FILE* fp = fopen(fileName, "r");

	if (!fp) {
		printf(" Unable to open file %s for reading!\n", fileName);
		return NULL;
	}

	// inhale the file in its entirety
	fread(shader_text, stat_buf.st_size, 1, fp);

	fclose(fp);

	return shader_text;
}
