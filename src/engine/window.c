#include "window.h"

static App appState = {0};

int init_window(u16 width, u16 height, char* name) {
	appState.width = width;
	appState.height = height;
	
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_LogCritical(
			SDL_LOG_CATEGORY_APPLICATION,
			"Failed to initialize SDL: %s\n",
			SDL_GetError()
		);
		return 0;
	}

	// state openGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	appState.pWindow = SDL_CreateWindow(
		name,
		width,
		height,
		SDL_WINDOW_OPENGL
	);

	if (!appState.pWindow)
	{
		SDL_LogCritical(
			SDL_LOG_CATEGORY_APPLICATION,
			"Failed to create OpenGL window: %s\n",
			SDL_GetError()
		);
		SDL_Quit();
		return 0;
	}

	appState.context = SDL_GL_CreateContext(appState.pWindow);
	if (appState.context == NULL)
	{
		SDL_LogCritical(
			SDL_LOG_CATEGORY_APPLICATION,
			"Failed to create OpenGL context: %s\n",
			SDL_GetError()
		);
		SDL_DestroyWindow(appState.pWindow);
		SDL_Quit();
		return 0;
	}

	// v-sync enable
	SDL_GL_SetSwapInterval(1);

	SDL_GL_MakeCurrent(appState.pWindow, appState.context);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		SDL_Log("Failed to initialize GLAD\n");
		SDL_GL_DestroyContext(appState.context);
		SDL_DestroyWindow(appState.pWindow);
		SDL_Quit();
		return 0;
	}

	printf("- Shading Language = %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("- Version = %s\n", glGetString(GL_VERSION));
	printf("- Vendor = %s\n", glGetString(GL_VENDOR));
	printf("- Renderer = %s\n\n", glGetString(GL_RENDERER));

	// enable back-face culling and set viewport to window height/width
	// eventually, this should be in some kind of callback to handle the change on
	// window resize. Gotta figure out how to do that with SDL (event poll probably)
	glViewport(0, 0, width, height);
	glEnable(GL_CULL_FACE);

	// enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 1;
}

void clean_window(void) {
	SDL_GL_DestroyContext(appState.context);
	SDL_DestroyWindow(appState.pWindow);
	SDL_Quit();
}

void swap_buffer(void) {
	SDL_GL_SwapWindow(appState.pWindow);
}
