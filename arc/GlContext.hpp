#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL_opengl.h>

#include <memory>

#include "GlError.hpp"

namespace arc {

class Context 
{
public:
	[[nodiscard]]
	static std::unique_ptr<Context> create(const std::string name,
										   const uint32_t width,
										   const uint32_t height);

	Context(const uint32_t width,
			const uint32_t height,
			SDL_Window* window,
			SDL_GLContext context
			);
	~Context();

	SDL_Window* window();

private:
	uint32_t m_width;
	uint32_t m_height;
	SDL_Window* m_window;
	SDL_GLContext m_context;
};	
}
