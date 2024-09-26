#include <arc/GlContext.hpp>
#include <arc/GlBasicVertexBuffer.hpp>
#include <arc/GlIndexBuffer.hpp>
#include <arc/GlShader.hpp>
#include <arc/FileSlurp.hpp>
#include <arc/RenderDoc.hpp>

#include <iostream>


//https://www.geeks3d.com/20140704/gpu-buffers-introduction-to-opengl-3-1-uniform-buffers-objects/


#define USE_UNIFORM_BUFFER_OBJECTS false


#define WIDTH 1200
#define HEIGHT 900


struct CameraUniform {
	glm::mat4 view;
	glm::mat4 proj;
};

struct ObjUniform {
	glm::mat4 model;
	glm::vec3 color;
};

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

	
GLint64 glGetInt64(const GLenum name) 
{
	GLint64 ret{0};
	glGetInteger64v(name, &ret);
	return ret;
}

int main(int argc, char** argv) 
{
    (void)argc; (void)argv;
	try {
	 //const std::string renderdoc_path = "./librenderdoc.so";
	 //auto renderdoc = arc::RenderDoc::load(renderdoc_path);
	 //if (!renderdoc)
	 //	throw ARC_GENERIC_ERROR("Could not load renderdoc from path: " + renderdoc_path);

		auto context = arc::Context::create("uniforms", WIDTH, HEIGHT);

		std::cout << "GL_MAX_UNIFORM_BUFFER_BINDINGS "   
				  << glGetInt64(GL_MAX_UNIFORM_BUFFER_BINDINGS)
				  << "\nGL_MAX_UNIFORM_BLOCK_SIZE "      
				  << glGetInt64(GL_MAX_UNIFORM_BLOCK_SIZE)
				  << "\nGL_MAX_VERTEX_UNIFORM_BLOCKS "
				  << glGetInt64(GL_MAX_VERTEX_UNIFORM_BLOCKS)
				  << "\nGL_MAX_FRAGMENT_UNIFORM_BLOCKS " 
				  << glGetInt64(GL_MAX_FRAGMENT_UNIFORM_BLOCKS)
				  << "\nGL_MAX_GEOMETRY_UNIFORM_BLOCKS "
				  << glGetInt64(GL_MAX_GEOMETRY_UNIFORM_BLOCKS)
				  << std::endl;
		
		vertex_buffer_PosTex square_vertices(vertices);
		arc::index_buffer square_indices(indices);
		constexpr auto lineseperator = "======================";
		

#if USE_UNIFORM_BUFFER_OBJECTS
		std::cout << "USING UNIFORM BUFFER OBJECTS" << std::endl;
#else
		std::cout << "USING NORMAL UNIFORMS" << std::endl;
#endif

		
#if USE_UNIFORM_BUFFER_OBJECTS
		const auto vert = arc::file_slurp("../ubo_solid.vert");
#else
		const auto vert = arc::file_slurp("../solid.vert");
#endif
		if (!vert)
			throw ARC_GENERIC_ERROR("could not load vertex source");
		std::cout << lineseperator << "\nVertex shader:\n" << vert.value() 
				  << lineseperator << std::endl;
		
		const auto frag = arc::file_slurp("../solid.frag");
		if (!frag)
			throw ARC_GENERIC_ERROR("could not load fragment source");
		std::cout << lineseperator << "\nFragment shader:\n" << frag.value() 
				  << lineseperator << std::endl;
		
		auto shader = arc::shader::create(vert.value().c_str(),
										  frag.value().c_str()); 
		
		if (!shader)
			throw ARC_GENERIC_ERROR("could not create shader!");
		
		CameraUniform cam_uniform;
		cam_uniform.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)); 
		cam_uniform.proj = glm::perspective(glm::radians(45.0f),
											800.0f / 600.0f,
											0.1f,
											100.0f);
		
		ObjUniform obj_uniform;
		obj_uniform.color = glm::vec3(0.0f, 0.0f, 1.0f);
		obj_uniform.model = glm::rotate(glm::mat4(1.0f),
										glm::radians(45.0f),
										glm::vec3(1.0f, 1.0f, 0.0f));
		
#if USE_UNIFORM_BUFFER_OBJECTS
		/** **************************************************************************
		 * Get the binding point of Camera Uniform
		 */
		const auto cam_uniform_name = "Camera";
	 	const auto cam_binding_block_index = glGetUniformBlockIndex(shader->get_program(),
																	cam_uniform_name);
		std::cout << "Uniform block index [" << cam_uniform_name << "] = "
				  << cam_binding_block_index << std::endl;
		
		GLuint cam_uniform_buffer = 0;
		glGenBuffers(1, &cam_uniform_buffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, cam_binding_block_index, cam_uniform_buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraUniform), &cam_uniform, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
		/** **************************************************************************
		 * Get the binding point of Object Uniform
		 */
		const auto obj_uniform_name = "Obj";
	 	const auto obj_binding_block_index = glGetUniformBlockIndex(shader->get_program(),
																	obj_uniform_name);
		std::cout << "Uniform block index [" << obj_uniform_name << "] = "
				  << obj_binding_block_index << std::endl;


		GLuint obj_uniform_buffer = 0;
		glGenBuffers(1, &obj_uniform_buffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, obj_binding_block_index, obj_uniform_buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(ObjUniform), &obj_uniform, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
#endif
		
		auto startTime = std::chrono::high_resolution_clock::now();
		
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
			
			const auto currentTime = std::chrono::high_resolution_clock::now();
			const float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

			obj_uniform.model = glm::rotate(glm::mat4(1.0f),
											time * glm::radians(90.0f),
											glm::vec3(0.0f, 1.0f, 1.0f));
			
#if USE_UNIFORM_BUFFER_OBJECTS
			/** ******************************************************************
			 * Update Camera Uniform
			 */
			glBindBuffer(GL_UNIFORM_BUFFER, cam_uniform_buffer);
			GLvoid* cam_map = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
			memcpy(cam_map, &cam_uniform, sizeof(CameraUniform));
			glUnmapBuffer(GL_UNIFORM_BUFFER);

			/** ******************************************************************
			 * Update Object Uniform
			 */
			glBindBuffer(GL_UNIFORM_BUFFER, obj_uniform_buffer);
			GLvoid* obj_map = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
			memcpy(obj_map, &obj_uniform, sizeof(ObjUniform));
			glUnmapBuffer(GL_UNIFORM_BUFFER);
			
#else
			shader->set_mat4("view", cam_uniform.view);
			shader->set_mat4("proj", cam_uniform.proj);
			shader->set_mat4("model", obj_uniform.model);
			shader->set_vec3("color", obj_uniform.color);
#endif
	
			decltype(square_vertices)::bind_layout();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_vertices.get_buffer());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_indices.get_buffer());

			glDrawElements(GL_TRIANGLES, square_indices.length(), GL_UNSIGNED_INT, 0);
			
			SDL_GL_SwapWindow(context->window());
		}
		
	} catch (arc::generic_error& err) {
		std::cout << "Caught Fatal error:\n\t" << err.what() << "\n\t"
				  << "[Line: " << err.m_where.line << " -> File: " << err.m_where.file
				  << "]" << std::endl;
	}
}
