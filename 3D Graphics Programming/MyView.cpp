#include "MyView.hpp"
#include <scene/scene.hpp>
#include <tygra/FileHelper.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cassert>

MyView::MyView()
{
}

MyView::~MyView() {
}

void MyView::setScene(const scene::Context * scene)
{
    scene_ = scene;
}

void MyView::windowViewWillStart(tygra::Window * window)
{
    assert(scene_ != nullptr);

	GLint compile_status = GL_FALSE;

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertex_shader_string
		= tygra::createStringFromFile("resource:///sponza_vs.glsl");
	const char * vertex_shader_code = vertex_shader_string.c_str();
	glShaderSource(vertex_shader, 1,
		(const GLchar **) &vertex_shader_code, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) 
	{
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetShaderInfoLog(vertex_shader, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragment_shader_string
		= tygra::createStringFromFile("resource:///sponza_fs.glsl");
	const char * fragment_shader_code = fragment_shader_string.c_str();
	glShaderSource(fragment_shader, 1,
		(const GLchar **)&fragment_shader_code, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) 
	{
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetShaderInfoLog(fragment_shader, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glBindAttribLocation(shader_program, kVertexPosition, "vertex_position");
	glBindAttribLocation(shader_program, kVertexNorm, "vertex_normal");
	glBindAttribLocation(shader_program, kvertexCoord, "vertex_texcoord");
	glDeleteShader(vertex_shader);

	glAttachShader(shader_program, fragment_shader);
	glBindFragDataLocation(shader_program, kFragmentColour, "fragment_colour");
	glDeleteShader(fragment_shader);
	glLinkProgram(shader_program);

	GLint link_status = GL_FALSE;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &link_status);
	if (link_status != GL_TRUE)
	{
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetProgramInfoLog(shader_program, string_length, NULL, log);
		std::cerr << log << std::endl;
	}

	scene::GeometryBuilder builder;

	// Rename to scene_meshes
	const auto& scene_mesh = builder.getAllMeshes();
	std::vector<Vertex> verts;
	std::vector<unsigned int> elems;
	
	// scene_mesh : scene_meshes
	for (const auto& scene_meshes : scene_mesh)
	{
		MeshGL& n_mesh = meshes_[scene_meshes.getId()];

		const auto& source_mesh = builder.getMeshById(scene_meshes.getId());
		const auto& positions = source_mesh.getPositionArray();
		const auto& elements = source_mesh.getElementArray();
		const auto& normals = source_mesh.getNormalArray();
		const auto& texcoords = source_mesh.getTextureCoordinateArray();
		const auto& instances = scene_->getInstancesByMeshId(scene_meshes.getId());

		// Checking to see how many instances are there.
		n_mesh.instance_count = instances.size();

		n_mesh.first_vertex_index = verts.size();
		
		for (int i = 0; i < source_mesh.getPositionArray().size(); i++)
		{
			Vertex vert;
			vert.position = positions[i];
			vert.normal = normals[i];

			// Checks to see if there are texture co-ordinates within the instance.
			if (texcoords.size() > 0)
			{
				vert.texcoord = texcoords[i];
			}

			verts.push_back(vert);
		}

		// Setting the elements in the index.
		n_mesh.first_element_index = elems.size();
		elems.insert(elems.end(), elements.begin(), elements.end());
		n_mesh.element_count = elements.size();
	}
	
	glGenBuffers(1, &vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER,
		verts.size() * sizeof(Vertex),
		verts.data(),
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, kNullId);

	glGenBuffers(1, &element_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		elems.size() * sizeof(unsigned int),
		elems.data(),
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, kNullId);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glEnableVertexAttribArray(kVertexPosition);
	glVertexAttribPointer(kVertexPosition, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), TGL_BUFFER_OFFSET_OF(Vertex, position));

	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glEnableVertexAttribArray(kVertexNorm);
	glVertexAttribPointer(kVertexNorm, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), TGL_BUFFER_OFFSET_OF(Vertex, normal));

	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glEnableVertexAttribArray(kvertexCoord);
	glVertexAttribPointer(kvertexCoord, 2, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), TGL_BUFFER_OFFSET_OF(Vertex, texcoord));

	glBindBuffer(GL_ARRAY_BUFFER, kNullId);
	glBindVertexArray(kNullId);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	tygra::Image hex_texture = tygra::createImageFromPngFile("content:///hex.png");
	if (hex_texture.doesContainData())
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		GLenum pixel_formats[] = { 0, GL_RED, GL_RG, GL_RGB, GL_RGBA };
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, hex_texture.width(), hex_texture.height(), 0,
			pixel_formats[hex_texture.componentsPerPixel()], hex_texture.bytesPerComponent() == 1 ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT, hex_texture.pixelData());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, kNullId);

		glGenSamplers(1, &sampler);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

void MyView::windowViewDidReset(tygra::Window * window,
                                int width,
                                int height)
{
    glViewport(0, 0, width, height);
}

void MyView::windowViewDidStop(tygra::Window * window)
{
	for (auto& m_mesh : meshes_)
	{
		auto mesh_id = m_mesh.first;
		auto& mesh = m_mesh.second;

		glDeleteBuffers(1, &vertex_vbo);
		glDeleteBuffers(1, &element_vbo);
		glDeleteVertexArrays(1, &vao);
	}

	glDeleteProgram(shader_program);
	glDeleteTextures(1, &texture);
}

void MyView::windowViewRender(tygra::Window * window)
{
    assert(scene_ != nullptr);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

    glClearColor(0.f, 0.f, 0.25f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLint viewport_size[4];
	glGetIntegerv(GL_VIEWPORT, viewport_size);
	const float aspect_ratio = viewport_size[2] / (float)viewport_size[3];

	const auto& cam = scene_->getCamera();
	glm::mat4 projection = glm::perspective(glm::radians(cam.getVerticalFieldOfViewInDegrees()), aspect_ratio, cam.getNearPlaneDistance(), cam.getFarPlaneDistance());
	glm::mat4 view = glm::lookAt(cam.getPosition(),
		cam.getPosition() + cam.getDirection(),
		scene_->getUpDirection());

	glUseProgram(shader_program);
	

	glActiveTexture(GL_TEXTURE0 + kTexture);
	glBindTexture(GL_TEXTURE_2D, texture);
	GLuint sampler_id = glGetUniformLocation(shader_program, "hex_texture");
	glUniform1i(sampler_id, kTexture);


	for (const auto& mesh : meshes_)
	{
		const auto& instances = scene_->getInstancesByMeshId(mesh.first);

		for (int i = 0; i < instances.size(); i++)
		{
			auto transform_string = "combined_transform[" + std::to_string(i) + "]";
			GLuint transform_id = glGetUniformLocation(shader_program, transform_string.c_str());

			glm::mat4 model = glm::mat4(scene_->getInstanceById(instances[i]).getTransformationMatrix());
			glm::mat4 combined_transform = projection * view * model;

			glUniformMatrix4fv(transform_id, 1, GL_FALSE, glm::value_ptr(combined_transform));
		}
		
		glBindVertexArray(vao);
		// Casting first_element_index to a void pointer.
		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, mesh.second.element_count, GL_UNSIGNED_INT, (void*)(mesh.second.first_element_index * sizeof(int)), mesh.second.instance_count, mesh.second.first_vertex_index);
	}

}
