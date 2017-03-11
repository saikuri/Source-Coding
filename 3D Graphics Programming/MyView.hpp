#pragma once

#include <scene/scene_fwd.hpp>
#include <tygra/WindowViewDelegate.hpp>
#include <tgl/tgl.h>
#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <map>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
};

class MyView : public tygra::WindowViewDelegate
{
public:
    
    MyView();
    
    ~MyView();
    
    void setScene(const scene::Context * scene);

private:

    void windowViewWillStart(tygra::Window * window) override;
    
    void windowViewDidReset(tygra::Window * window,
                            int width,
                            int height) override;

    void windowViewDidStop(tygra::Window * window) override;
    
    void windowViewRender(tygra::Window * window) override;

private:

    const scene::Context * scene_;
	
	GLuint texture{ 0 };
	GLuint shader_program{ 0 };
	GLuint sampler{ 0 };

	GLuint vertex_vbo{ 0 };
	GLuint element_vbo{ 0 };
	GLuint vao{ 0 };

	struct MeshGL
	{		
		GLuint first_element_index;
		GLuint first_vertex_index;
		GLuint element_count;
		GLuint instance_count;
	};

	// Looping over a map, 'first' element of a map, 'second' element of a map.
	std::map<scene::MeshId, MeshGL> meshes_;

	const static GLuint kNullId = 0;
	enum VertexAttribIndexes
	{
		kVertexPosition = 0,
		kVertexNorm = 1,
		kvertexCoord = 2
	};

	enum FragmentDataIndexes 
	{
		kFragmentColour = 0
	};

	enum TextureIndexes
	{
		kTexture = 0
	};

};
