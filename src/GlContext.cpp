#include "../arc/GlContext.hpp"

namespace arc {
	
bool initGL()
{
    bool success = true;
    GLenum error = GL_NO_ERROR;

    //Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        success = false;
    }

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        success = false;
    }
    
    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        success = false;
    }
    
    return success;
}


Context::Context(const uint32_t width,
				 const uint32_t height,
				 SDL_Window* window,
				 SDL_GLContext context)
	: m_width(width)
	, m_height(height)
	, m_window(window)
	, m_context(context)
{
}

std::unique_ptr<Context> Context::create(const std::string name,
										 const uint32_t width,
										 const uint32_t height)
{
	auto sdl_status = SDL_Init(SDL_INIT_VIDEO);
    if (sdl_status < 0)
		throw arc_generic_error("Could not init SDL");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	auto window = SDL_CreateWindow(name.c_str(),
								   SDL_WINDOWPOS_UNDEFINED,
								   SDL_WINDOWPOS_UNDEFINED,
								   width,
								   height,
								   SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!window) {
		const std::string sdl_error = SDL_GetError();
		throw arc_generic_error("Window could not be created! SDL Error: "
								+ sdl_error);
	}
	
	auto context = SDL_GL_CreateContext(window);
	if (!context) {
		const std::string sdl_error = SDL_GetError();
		throw arc_generic_error("OpenGL context could not be created! SDL Error: "
								+ sdl_error);
	}
	
	sdl_status = SDL_GL_SetSwapInterval(1);
	if(sdl_status  < 0) {
		const std::string sdl_error = SDL_GetError();
		throw arc_generic_error("Warning: Unable to set VSync! SDL Error: "
								+ sdl_error);
	}
	
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));

	const auto gl_ok = initGL();
	if (!gl_ok)
		throw arc_generic_error("Unable to initialize OpenGL");
	
	glViewport(0, 0, width, height);

	return std::make_unique<Context>(width, height, window, context);
}
	


Context::~Context()
{

}


SDL_Window* Context::window()
{
	return m_window;
}

}
