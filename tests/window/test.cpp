#include <arc/GlContext.hpp>
#include <arc/GlBasicVertexBuffer.hpp>
#include <arc/GlIndexBuffer.hpp>
#include <arc/GlShader.hpp>
#include <arc/FileSlurp.hpp>

#include <iostream>

#define WIDTH 1200
#define HEIGHT 900

constexpr auto lineseperator = "------------------------------";

struct Vertex_PosTex {
    glm::vec3 pos{};
    glm::vec2 uv{};

	static GLuint create_layout();
};

GLuint Vertex_PosTex::create_layout()
{
	GLuint layout;
	glGenVertexArrays(1, &layout);  
	glBindVertexArray(layout);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_PosTex), 
						  (void*)offsetof(Vertex_PosTex, pos));
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_PosTex),
						  (void*)offsetof(Vertex_PosTex, uv));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // unbind
	return layout;
}
	
using vertex_buffer_PosTex = arc::basic_vertex_buffer<Vertex_PosTex>;

const vertex_buffer_PosTex::vector_type vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f},  {0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f},   {0.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f},  {1.0f, 1.0f}}
};

const arc::index_buffer::vector_type indices = {
    0, 1, 2, 2, 3, 0
};

int main(int argc, char** argv) 
{
    (void)argc;
    (void)argv;
	auto context = arc::Context::create("window", WIDTH, HEIGHT);
	
	vertex_buffer_PosTex square_vertices(vertices);
	arc::index_buffer square_indices(indices);
	
	const auto vert = arc::file_slurp("../solid.vert");
	if (!vert)
		throw std::runtime_error("could not load vertex source"); //std::cout << lineseperator << "\nVertex shader:\n" << vert.value() 
	//<< lineseperator << std::endl;

	const auto frag = arc::file_slurp("../solid.frag");
	if (!frag)
		throw std::runtime_error("could not load fragment source");
	//std::cout << lineseperator << "\nFragment shader:\n" << frag.value() 
	//<< lineseperator << std::endl;
	
	auto shader = arc::shader::create(vert.value().c_str(),
									  frag.value().c_str()); 
	
	if (!shader)
		throw std::runtime_error("could not create shader!");

    bool exit = false;
    SDL_Event event;
    while (!exit) {
        /* =======================================================
         * Input
         */
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                exit = true;
                break;

            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ) {
                case SDLK_ESCAPE:
                    exit = true;
                    break;
                }
                break;

            case SDL_WINDOWEVENT: {
                const SDL_WindowEvent& wev = event.window;
                switch (wev.event) {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    //context.window_resized_event();
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    exit = true;
                    break;
                }
            } break;
            }
        }
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		shader->activate();
		decltype(square_vertices)::bind_layout();
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_vertices.get_buffer());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_indices.get_buffer());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(context->window());
    }
}
