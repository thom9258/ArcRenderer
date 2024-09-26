#include <arc/GlError.hpp>
#include <arc/GlContext.hpp>
#include <arc/GlBasicVertexBuffer.hpp>
#include <arc/GlIndexBuffer.hpp>
#include <arc/GlShader.hpp>

#include <arc/FileSlurp.hpp>
#define BASICGEOMETRY_IMPLEMENTATION
#include <arc/BasicGeometry.hpp>

#define SIMPLE_GEOMETRY_IMPLEMENTATION
#include "simple_geometry.h"

#include <iostream>

#define WIDTH 1200
#define HEIGHT 900

constexpr auto lineseperator = "======================";

struct CameraUniform {
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 cam_pos() {
		const auto cam_pos = inverse(proj * view) * glm::vec4(0,0,-1,0);
		return cam_pos / cam_pos.w;
	}
};

struct ObjUniform {
	glm::mat4 model;
	glm::vec3 color;
};

struct Vertex_PosNorm {
    glm::vec3 pos{};
    glm::vec3 norm{};

	static GLuint create_layout();
};

GLuint Vertex_PosNorm::create_layout()
{
	GLuint layout;
	glGenVertexArrays(1, &layout);  
	glBindVertexArray(layout);
	// position attribute
	constexpr int pos_count = 3;
	glVertexAttribPointer(0, pos_count, GL_FLOAT, GL_FALSE, sizeof(Vertex_PosNorm), 
						  (void*)offsetof(Vertex_PosNorm, pos));
	glEnableVertexAttribArray(0);
	// texture coord attribute
	constexpr int normal_count = 3;
	glVertexAttribPointer(1, normal_count, GL_FLOAT, GL_FALSE, sizeof(Vertex_PosNorm),
						  (void*)offsetof(Vertex_PosNorm, norm));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // unbind
	return layout;
}
	
using vertex_buffer_PosNorm = arc::basic_vertex_buffer<Vertex_PosNorm>;

std::string stringify(const glm::vec3 v)
{
	return "[" + std::to_string(v.x)
         + " " + std::to_string(v.y)
         + " " + std::to_string(v.z)
		 + "]";
}

std::string stringify(const glm::mat4 m)
{
	return "["    + std::to_string(m[0][0])
         + " "    + std::to_string(m[1][0])
         + " "    + std::to_string(m[2][0])
         + " "    + std::to_string(m[3][0])
         + "]\n " + std::to_string(m[0][1])
         + " "    + std::to_string(m[1][1])
         + " "    + std::to_string(m[2][1])
         + " "    + std::to_string(m[3][1])
         + "]\n " + std::to_string(m[0][2])
         + " "    + std::to_string(m[1][2])
         + " "    + std::to_string(m[2][2])
         + " "    + std::to_string(m[3][2])
         + "]\n " + std::to_string(m[0][3])
         + " "    + std::to_string(m[1][3])
         + " "    + std::to_string(m[2][3])
         + " "    + std::to_string(m[3][3])
         + "]\n ";
}

GLint64 glGetInt64(const GLenum name) 
{
	GLint64 ret{0};
	glGetInteger64v(name, &ret);
	return ret;
}

struct Geometry {
	std::unique_ptr<vertex_buffer_PosNorm> vertices{nullptr};
	std::unique_ptr<arc::index_buffer> indices{nullptr};
	glm::mat4 model;
	glm::vec3 color;
};

Geometry create_cube_geometry()
{
	Geometry geometry;

	SG_status status;
	SG_size positions_size{0};
	status = sg_cube_positions(1.0f, 1.0f, 1.0f, nullptr, &positions_size);
	if (!sg_success(status))
		throw ARC_GENERIC_ERROR("Could not get positions size");

	std::vector<sg_position> positions(positions_size);
	status = sg_cube_positions(1.0f, 1.0f, 1.0f, positions.data(), &positions_size);
	if (!sg_success(status))
		throw ARC_GENERIC_ERROR("Could not get positions");

	SG_size normals_size{0};
	status = sg_cube_normals(nullptr, &normals_size);
	if (!sg_success(status))
		throw ARC_GENERIC_ERROR("Could not get normals size");

	std::vector<sg_normal> normals(normals_size);
	status = sg_cube_normals(normals.data(), &normals_size);
	if (!sg_success(status))
		throw ARC_GENERIC_ERROR("Could not get normals");

	if (positions_size != normals_size)
		throw ARC_GENERIC_ERROR("something is wrong with positions and normals of cube!");

	std::vector<Vertex_PosNorm> vertices_data(positions.size());
	for (size_t i = 0; i < vertices_data.size(); i++) {
		Vertex_PosNorm posnorm{};
		posnorm.pos.x = positions[i].x;
		posnorm.pos.y = positions[i].y;
		posnorm.pos.z = positions[i].z;
		posnorm.norm.x = normals[i].x;
		posnorm.norm.y = normals[i].y;
		posnorm.norm.z = normals[i].z;
		vertices_data[i] = posnorm;
	}

	geometry.vertices = std::make_unique<vertex_buffer_PosNorm>(vertices_data);
	geometry.model = glm::mat4(1.0f);
	geometry.color = glm::vec3(1.0f, 0.0f, 0.0f);
	return geometry;
}


void draw_geometry(Geometry& geom)
{
	if (!geom.vertices)
		throw ARC_GENERIC_ERROR("Cannot Draw geometry that does not exist!");

	if (geom.indices) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom.vertices->get_buffer());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom.indices->get_buffer());
		glDrawElements(GL_TRIANGLES, geom.indices->length(), GL_UNSIGNED_INT, 0);
		return;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom.vertices->get_buffer());
	glDrawArrays(GL_TRIANGLES, 0, geom.vertices->length());
}

std::unique_ptr<arc::shader> create_shader(const char* vert_path,
										   const char* frag_path)
{
	const auto vert = arc::file_slurp(vert_path);
	if (!vert) {
		throw ARC_GENERIC_ERROR("could not load vertex source");
		std::cout << lineseperator << "\nVertex shader:\n" << vert.value() 
				  << lineseperator << std::endl;
	}
	
	const auto frag = arc::file_slurp(frag_path);
	if (!frag) {
		throw ARC_GENERIC_ERROR("could not load fragment source");
		std::cout << lineseperator << "\nFragment shader:\n" << frag.value() 
				  << lineseperator << std::endl;
	}
	
	return arc::shader::create(vert.value().c_str(), frag.value().c_str()); 
}

int main(int argc, char** argv) 
{
    (void)argc; (void)argv;
	try {
		auto context = arc::Context::create("phong-lighting", WIDTH, HEIGHT);

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
				  << "\nGL_MAX_VERTEX_ATTRIBS "
				  << glGetInt64(GL_MAX_VERTEX_ATTRIBS)
				  << "\nGL_MAX_FRAGMENT_INPUT_COMPONENTS "
				  << glGetInt64(GL_MAX_FRAGMENT_INPUT_COMPONENTS)
				  << "\nGL_MAX_VERTEX_OUTPUT_COMPONENTS "
				  << glGetInt64(GL_MAX_VERTEX_OUTPUT_COMPONENTS)
				  << "\nGL_MAX_VERTEX_TEXTURE_IMAGE_UNITS "
				  << glGetInt64(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS)
				  << std::endl;
		
	    glEnable(GL_DEPTH_TEST);
		
		auto solid_shader = create_shader("../solid.vert", "../solid.frag");
		if (!solid_shader)
			throw ARC_GENERIC_ERROR("could not create base shader!");

		auto phong_shader = create_shader("../phong.vert", "../phong.frag");
		if (!phong_shader)
			throw ARC_GENERIC_ERROR("could not create phong shader!");

		auto normal_shader = create_shader("../normal.vert", "../normal.frag");
		if (!normal_shader)
			throw ARC_GENERIC_ERROR("could not create normal shader!");
		
		ARC_ON_GL_ERROR(arc::throw_gl_error);
		
		CameraUniform cam_uniform;
		cam_uniform.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)); 
		cam_uniform.proj = glm::perspective(glm::radians(45.0f),
											800.0f / 600.0f,
											0.1f,
											100.0f);
		
		Geometry cube = create_cube_geometry();
		cube.color = glm::vec3(1.0f, 0.0f, 0.0f);
		Geometry light = create_cube_geometry();
		light.color = glm::vec3(1.0f, 1.0f, 1.0f);
		light.model = glm::scale(glm::translate(light.model, glm::vec3(1.0f, 1.0f, 0.0f)),
								 glm::vec3(0.1f));
	
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
			const auto currentTime = std::chrono::high_resolution_clock::now();
			const float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

			cube.model = glm::rotate(glm::mat4(1.0f),
									 time * glm::radians(90.0f),
									 glm::vec3(0.0f, 1.0f, 1.0f));
			cube.model = glm::scale(cube.model, glm::vec3(0.5f));
			
			/** ***************************************************
			 * Draw
			 */
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			solid_shader->activate();
			solid_shader->set_mat4("view", cam_uniform.view);
			solid_shader->set_mat4("proj", cam_uniform.proj);
			solid_shader->set_mat4("model", light.model);
			solid_shader->set_vec3("color", light.color);
			vertex_buffer_PosNorm::bind_layout();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			draw_geometry(light);
			ARC_ON_GL_ERROR(arc::throw_gl_error);
			
			phong_shader->activate();
			phong_shader->set_mat4("view", cam_uniform.view);
			phong_shader->set_mat4("proj", cam_uniform.proj);
			phong_shader->set_mat4("model", cube.model);
			//phong_shader->set_mat4("model", glm::translate(cube.model,
			//glm::vec3(0.5f, 0.0f, 0.0f)));
			phong_shader->set_vec3("view_pos", cam_uniform.cam_pos());
			
			phong_shader->set_vec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
			phong_shader->set_vec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
			phong_shader->set_vec3("material.specular", glm::vec3(0.5f));
			phong_shader->set_float("material.shininess", 32.0f);
			phong_shader->set_vec3("light.position", light.model[3]);
			phong_shader->set_vec3("light.ambient", glm::vec3(0.2f));
			phong_shader->set_vec3("light.diffuse", glm::vec3(0.5f));
			phong_shader->set_vec3("light.specular", glm::vec3(1.0f));

			vertex_buffer_PosNorm::bind_layout();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			draw_geometry(cube);
			ARC_ON_GL_ERROR(arc::throw_gl_error);

			SDL_GL_SwapWindow(context->window());
			ARC_ON_GL_ERROR(arc::throw_gl_error);
		}
		
	} catch (arc::generic_error& err) {
		std::cout << "Caught Fatal error:\n\t" << err.what() << "\n\t"
				  << "[Line: " << err.m_where.line << " -> File: " << err.m_where.file
				  << "]" << std::endl;
	}
}
