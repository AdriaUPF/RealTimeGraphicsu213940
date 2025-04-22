#pragma once
#include "scene.h"
#include "prefab.h"

#include "light.h"


struct sDrawCommand {
	GFX::Mesh* mesh;
	SCN::Material* material;
	Matrix44 model;
};
//forward declarations
class Camera;
class Skeleton;
namespace GFX {
	class Shader;
	class Mesh;
	class FBO;
}

namespace SCN {

	class Prefab;
	class Material;

	// This class is in charge of rendering anything in our system.
	// Separating the render from anything else makes the code cleaner
	class Renderer
	{
	public:
		bool render_wireframe;
		bool render_boundaries;

		bool multi_pass = false; //For knowing if to multipass or not

		bool use_shadow_front_face_culling = true;

		float bias = 0.005f; //for shadow bias
		GFX::Texture* skybox_cubemap;

		SCN::Scene* scene;

		//updated every frame
		Renderer(const char* shaders_atlas_filename);

		//just to be sure we have everything ready for the rendering
		void setupScene();

		//add here your functions
		//...
		void render_fbo(const Matrix44 model, GFX::Mesh* mesh, SCN::Material* material);

		void parseSceneEntities(SCN::Scene* scene, Camera* camera);

		float distancefromcamera(sDrawCommand* node, Camera* cam);

		//renders several elements of the scene
		void renderScene(SCN::Scene* scene, Camera* camera);

		//render the skybox
		void renderSkybox(GFX::Texture* cubemap);

		//to render one mesh given its material and transformation matrix
		void renderMeshWithMaterial(const Matrix44 model, GFX::Mesh* mesh, SCN::Material* material);

		void showUI();
	};

};