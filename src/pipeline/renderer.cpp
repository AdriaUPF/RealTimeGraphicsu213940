#include "renderer.h"

#include <algorithm> //sort

#include "camera.h"
#include "../gfx/gfx.h"
#include "../gfx/shader.h"
#include "../gfx/mesh.h"
#include "../gfx/texture.h"
#include "../gfx/fbo.h"
#include "../pipeline/prefab.h"
#include "../pipeline/light.h"
#include "../pipeline/material.h"
#include "../pipeline/animation.h"
#include "../utils/utils.h"
#include "../extra/hdre.h"
#include "../core/ui.h"

#include "scene.h"



std::vector<sDrawCommand> draw_command_list; //canviar posicion de esto (no gustan variables globales)
std::vector<SCN::LightEntity*> light_list;
std::vector<Matrix44> view_projectioncam_list;




using namespace SCN;

//some globals
GFX::Mesh sphere;

Renderer::Renderer(const char* shader_atlas_filename)
{
	render_wireframe = false;
	render_boundaries = false;
	scene = nullptr;
	skybox_cubemap = nullptr;

	if (!GFX::Shader::LoadAtlas(shader_atlas_filename))
		exit(1);
	GFX::checkGLErrors();

	//CORE::setupFBO();

	sphere.createSphere(1.0f);
	sphere.uploadToVRAM();
}

void Renderer::setupScene()
{
	//Assigment 3   Wanted to make a fucntion to init, but dont know where or how
	//GFX::Texture *texture = new GFX::Texture(1024,1024);

	if (scene->skybox_filename.size())
		skybox_cubemap = GFX::Texture::Get(std::string(scene->base_folder + "/" + scene->skybox_filename).c_str());
	else
		skybox_cubemap = nullptr;


}

void parseNodes(SCN::Node* node, Camera* cam) {
	if (!node) {
		return;
	}
	if (node->mesh) {
		sDrawCommand draw_com;
		draw_com.mesh = node->mesh;
		draw_com.material = node->material;
		draw_com.model = node->getGlobalMatrix();
		draw_command_list.push_back(draw_com);
	}

	for (SCN::Node* child : node->children) {
		parseNodes(child, cam);
	}
}

float distancefromcamera(sDrawCommand node, Camera* cam) { 
	Vector3f position_of_node = node.model.getTranslation();
	float distance_fr_cam = position_of_node.distance(cam->eye);
	if (node.material->alpha_mode == SCN::NO_ALPHA) {
		return distance_fr_cam;
	}
	else {
		return distance_fr_cam * (-1); //We want to order the transparent objects from last to first
	}
}

void orderNodes(Camera* cam) {
	std::sort(draw_command_list.begin(), draw_command_list.end(), [&cam](sDrawCommand a, sDrawCommand b){ 
		return distancefromcamera(a, cam) > distancefromcamera(b, cam);
		} );

}



void Renderer::parseSceneEntities(SCN::Scene* scene, Camera* cam) {
	// HERE =====================
	// TODO: GENERATE RENDERABLES
	// ==========================
	draw_command_list.clear();
	light_list.clear();
	

	cam->extractFrustum();

	for (int i = 0; i < scene->entities.size(); i++) {
		BaseEntity* entity = scene->entities[i];

		if (!entity->visible) {
			continue;
		}

		if (entity->getType() == eEntityType::PREFAB) { //hay que tener en cuenta los hijos (puede tener extra nodos)

			parseNodes(&((PrefabEntity*)entity)->root, cam);
		}
		else if (entity->getType() == eEntityType::LIGHT) {
			light_list.push_back((LightEntity*)entity);
		}

		orderNodes(cam);

		
		// Store Prefab Entitys
		// ...
		//		Store Children Prefab Entities

		// Store Lights
		// ...
	}
	
}

void Renderer::renderScene(SCN::Scene* scene, Camera* camera)
{
	this->scene = scene;
	setupScene();

	parseSceneEntities(scene, camera);

	Camera* camera_fbo = new Camera();
	//camera_fbo->enable();
	//set the clear color (the background color)
	glClearColor(scene->background_color.x, scene->background_color.y, scene->background_color.z, 1.0);

	// Clear the color and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GFX::checkGLErrors();

	//render skybox
	if(skybox_cubemap)
		renderSkybox(skybox_cubemap);

	// HERE =====================
	// TODO: RENDER RENDERABLES
	// ==========================
	float halfsize = 1; //this values is what works best for my laptop
	camera->extractFrustum();
	Camera* store_camera = Camera::current;
	//Assigment 3
	view_projectioncam_list.clear();
	GFX::FBO::Init();

	int i = 0;
	for (SCN::LightEntity* light : light_list) {
			mat4 light_Model = light->root.getGlobalMatrix();
			vec3 light_pos = light_Model.getTranslation();
			vec3 look_at = light_Model.frontVector();
			vec3 light_dir = light_Model * vec3(0.0f, 0.0f, -1.0f); //D vector
			camera_fbo->lookAt(light_pos, light_dir, vec3(0.0f, 1.0f, 0.0f));
			if (light->light_type == DIRECTIONAL) {
				float half_size = light->area / 2.0f;
				camera_fbo->setOrthographic(-half_size, half_size, -half_size, half_size, light->near_distance, light->max_distance);
			}
			if (light->light_type == SPOT) {
				camera_fbo->setPerspective(light->cone_info.y * 2.0f, 1.0f, 0.1f, 100.0f); //with cone infor doesnt work, dkw
			}
		
			camera_fbo->extractFrustum();
			Camera::current = camera_fbo;
			
			view_projectioncam_list.push_back(camera_fbo->viewprojection_matrix);
			GFX::fbo[i]->bind();
			glColorMask(false, false, false, false);
			glClear(GL_DEPTH_BUFFER_BIT);

			if (use_shadow_front_face_culling) {
				glEnable(GL_CULL_FACE);
				glFrontFace(GL_CW); // Effectively culls front faces
			}
			for (sDrawCommand command : draw_command_list) {
				if (command.material->alpha_mode == SCN::NO_ALPHA) {
					//printf("Alpha mode\n");
					render_fbo(command.model, command.mesh, command.material);
				}
			}
			glFrontFace(GL_CCW);
			glDisable(GL_CULL_FACE);
			glColorMask(true, true, true, true);
			GFX::fbo[i]->unbind();
			i++;
	}


	Camera::current = store_camera;
	
	for (sDrawCommand command : draw_command_list) {


		//camera->testBoxInFrustum(command.model.getTranslation(),vec3(halfsize, halfsize, halfsize));
		//if(camera->testBoxInFrustum(command.model.getTranslation(), vec3(halfsize, halfsize, halfsize)) != CLIP_OUTSIDE){ //trying to implement frustrum
			//printf("%f\n", camera->eye.distance(command.model.getTranslation().length()));
			Renderer::renderMeshWithMaterial(command.model, command.mesh, command.material);
		//}
		
	}
	delete camera_fbo;
}

void Renderer::renderSkybox(GFX::Texture* cubemap)
{
	Camera* camera = Camera::current;

	// Apply skybox necesarry config:
	// No blending, no dpeth test, we are always rendering the skybox
	// Set the culling aproppiately, since we just want the back faces
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if (render_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GFX::Shader* shader = GFX::Shader::Get("skybox");
	if (!shader)
		return;
	shader->enable();

	// Center the skybox at the camera, with a big sphere
	Matrix44 m;
	m.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);
	m.scale(10, 10, 10);
	shader->setUniform("u_model", m);

	// Upload camera uniforms
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_camera_position", camera->eye);

	shader->setUniform("u_texture", cubemap, 0);

	sphere.render(GL_TRIANGLES);

	shader->disable();

	// Return opengl state to default
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
}

// Renders a mesh given its transform and material
void Renderer::renderMeshWithMaterial(const Matrix44 model, GFX::Mesh* mesh, SCN::Material* material)
{
	//in case there is nothing to do
	if (!mesh || !mesh->getNumVertices() || !material )
		return;
    assert(glGetError() == GL_NO_ERROR);
	//define locals to simplify coding
	GFX::Shader* shader = NULL;
	Camera* camera = Camera::current;

	glEnable(GL_DEPTH_TEST);

	//chose a shader
	shader = GFX::Shader::Get("texture");

    assert(glGetError() == GL_NO_ERROR);

	//no shader? then nothing to render
	if (!shader)
		return;

	
	shader->enable();

	material->bind(shader);

	//upload uniforms
	shader->setUniform("u_model", model);

	// Upload camera uniforms
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_camera_position", camera->eye);

	// Upload time, for cool shader effects
	float t = getTime();
	shader->setUniform("u_time", t);
	// ....
	bool multipass = multi_pass;
	if (multipass) {
		//printf("Multipass\n");
		glDepthFunc(GL_LEQUAL);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		int j = 0u;
		bool ambient = true;
		for (SCN::LightEntity* light : light_list) {
			//printf("is %d\n", j);
			if (j == 0) {
				glDisable(GL_BLEND);
				shader->setUniform1("u_ambient1", ambient);
			}
			else {
				ambient = false;
				glEnable(GL_BLEND);
				shader->setUniform1("u_ambient1", ambient);
			}
			shader->setUniform3("u_light_pos1", light->root.getGlobalMatrix().getTranslation());
			shader->setUniform3("u_light_color1", light->color);
			shader->setUniform1("u_light_intensity1", light->intensity);
			shader->setUniform3("u_light_direction1", light->root.model.frontVector());
			shader->setUniform1("u_alpha_max1", light->cone_info.y);
			shader->setUniform1("u_alpha_min1", light->cone_info.x);
			shader->setUniform1("u_multipass1", multipass);
			shader->setUniform1("u_light_type1", (int)light->light_type);
			shader->setUniform1("u_shiny", material->shinyness);


			// Render just the verticies as a wireframe
			if (render_wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			j++;
			mesh->render(GL_TRIANGLES);
		}

		glDisable(GL_BLEND);
		glDepthFunc(GL_LESS);
	}
	else {
		
		//printf("Not multipass\n");
		vec3* light_pos = new vec3[light_list.size()];
		vec3* light_color = new vec3[light_list.size()];
		vec3* light_dir = new vec3[light_list.size()];
		int* light_type = new int[light_list.size()];
		float* light_alpha_max = new float[light_list.size()];
		float* light_alpha_min = new float[light_list.size()];


		float* light_int = new float[light_list.size()];

		int i = 0u;
		for (SCN::LightEntity* light : light_list) {
			light_pos[i] = light->root.getGlobalMatrix().getTranslation();
			light_int[i] = light->intensity;
			light_color[i] = light->color;
			light_dir[i] = light->root.model.frontVector(); //D vector
			light_type[i] = light->light_type;
			if (light->light_type == SCN::eLightType::SPOT) {
				light_alpha_max[i] = light->cone_info.y * DEG2RAD;
				light_alpha_min[i] = light->cone_info.x * DEG2RAD;
			}
			else {
				light_alpha_max[i] = 0;
				light_alpha_min[i] = 0;
			}
			//printf("%s\n", light->name.c_str());
			//printf("%d\n", light->light_type);
			//printf("%d\n", i);
			i++;
			

		}
		shader->setUniform1("u_light_count", i);
		//printf("Lights: %d\n", i);
		
		shader->setUniform3Array("u_light_pos", (float*)light_pos, min(light_list.size(), 10));
		shader->setUniform3Array("u_light_color", (float*)light_color, min(light_list.size(), 10));
		shader->setUniform1Array("u_light_intensity", (float*)light_int, min(light_list.size(), 10));
		shader->setUniform3Array("u_light_direction", (float*)light_dir, min(light_list.size(), 10));
		shader->setUniform1Array("u_light_type", (int*)light_type, min(light_list.size(), 10));
		shader->setUniform1Array("u_alpha_max", (float*)light_alpha_max, min(light_list.size(), 10));
		shader->setUniform1Array("u_alpha_min", (float*)light_alpha_min, min(light_list.size(), 10));
		shader->setUniform1("u_multipass1", multipass);
		shader->setUniform1("u_shiny", material->shinyness);
		//printf("Multipass %d\n", multipass);



		//Assigment3
		shader->setUniform("u_light_viewprojection",view_projectioncam_list);
		//GFX::fbo->depth_texture->texture_id = 2;
		shader->setUniform("u_shadowmap[0]", (GFX::Texture*)GFX::fbo[0]->depth_texture, 2);
		shader->setUniform("u_shadowmap[1]", (GFX::Texture*)GFX::fbo[1]->depth_texture, 3);
		shader->setUniform("u_shadowmap[2]", (GFX::Texture*)GFX::fbo[2]->depth_texture, 4);
		shader->setUniform("u_shadowmap[3]", (GFX::Texture*)GFX::fbo[3]->depth_texture, 5);

		shader->setUniform("u_bias", bias);


		delete[] light_color;
		delete[] light_int;
		delete[] light_pos;
		delete[] light_dir;
		delete[] light_type;
		delete[] light_alpha_max;
		delete[] light_alpha_min;

		// Render just the verticies as a wireframe
		if (render_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//do the draw call that renders the mesh into the screen
		mesh->render(GL_TRIANGLES);
	}
	
	
	
	
	//set the render state as it was before to avoid problems with future renders
	glDisable(GL_BLEND);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	
}

void Renderer::render_fbo(const Matrix44 model, GFX::Mesh* mesh, SCN::Material* material) //to be optimized
{
	//in case there is nothing to do
	if (!mesh || !mesh->getNumVertices() || !material)
		return;
	assert(glGetError() == GL_NO_ERROR);
	//define locals to simplify coding
	GFX::Shader* shader = NULL;
	Camera* camera = Camera::current;

	glEnable(GL_DEPTH_TEST);

	//chose a shader
	shader = GFX::Shader::Get("flat");

	assert(glGetError() == GL_NO_ERROR);

	//no shader? then nothing to render
	if (!shader)
		return;


	shader->enable();

	material->bind(shader);
	//upload uniforms
	shader->setUniform("u_model", model);

	// Upload camera uniforms
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_camera_position", camera->eye);

	// Upload time, for cool shader effects
	float t = getTime();
	shader->setUniform("u_time", t);
	// ....
	bool multipass = multi_pass;
	if (multipass) {
		//printf("Multipass\n");
		glDepthFunc(GL_LEQUAL);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		int j = 0u;
		bool ambient = true;
		for (SCN::LightEntity* light : light_list) {
			printf("is %d\n", j);
			if (j == 0) {
				glDisable(GL_BLEND);
				shader->setUniform1("u_ambient1", ambient);
			}
			else {
				ambient = false;
				glEnable(GL_BLEND);
				shader->setUniform1("u_ambient1", ambient);
			}
			shader->setUniform3("u_light_pos1", light->root.getGlobalMatrix().getTranslation());
			shader->setUniform3("u_light_color1", light->color);
			shader->setUniform1("u_light_intensity1", light->intensity);
			shader->setUniform3("u_light_direction1", light->root.model.frontVector());
			shader->setUniform1("u_alpha_max1", light->cone_info.y);
			shader->setUniform1("u_alpha_min1", light->cone_info.x);
			shader->setUniform1("u_multipass1", multipass);
			shader->setUniform1("u_light_type1", (int)light->light_type);
			shader->setUniform1("u_shiny", material->shinyness);


			// Render just the verticies as a wireframe
			if (render_wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			j++;
			mesh->render(GL_TRIANGLES);
		}

		glDisable(GL_BLEND);
		glDepthFunc(GL_LESS);
	}
	else {
		glDepthFunc(GL_LESS);
		//printf("Not multipass\n");
		vec3* light_pos = new vec3[light_list.size()];
		vec3* light_color = new vec3[light_list.size()];
		vec3* light_dir = new vec3[light_list.size()];
		int* light_type = new int[light_list.size()];
		float* light_alpha_max = new float[light_list.size()];
		float* light_alpha_min = new float[light_list.size()];


		float* light_int = new float[light_list.size()];

		int i = 0u;
		for (SCN::LightEntity* light : light_list) {
			light_pos[i] = light->root.getGlobalMatrix().getTranslation();
			light_int[i] = light->intensity;
			light_color[i] = light->color;
			light_dir[i] = light->root.model.frontVector(); //D vector
			light_type[i] = light->light_type;
			if (light->light_type == SCN::eLightType::SPOT) {
				light_alpha_max[i] = light->cone_info.y * DEG2RAD;
				light_alpha_min[i] = light->cone_info.x * DEG2RAD;
			}
			else {
				light_alpha_max[i] = 0;
				light_alpha_min[i] = 0;
			}
			//printf("%s\n", light->name.c_str());
			//printf("%d\n", light->light_type);
			//printf("%d\n", i);
			i++;

		}
		shader->setUniform1("u_light_count", i);
		//printf("Lights: %d\n", i);

		shader->setUniform3Array("u_light_pos", (float*)light_pos, min(light_list.size(), 10));
		shader->setUniform3Array("u_light_color", (float*)light_color, min(light_list.size(), 10));
		shader->setUniform1Array("u_light_intensity", (float*)light_int, min(light_list.size(), 10));
		shader->setUniform3Array("u_light_direction", (float*)light_dir, min(light_list.size(), 10));
		shader->setUniform1Array("u_light_type", (int*)light_type, min(light_list.size(), 10));
		shader->setUniform1Array("u_alpha_max", (float*)light_alpha_max, min(light_list.size(), 10));
		shader->setUniform1Array("u_alpha_min", (float*)light_alpha_min, min(light_list.size(), 10));
		shader->setUniform1("u_multipass1", multipass);
		shader->setUniform1("u_shiny", material->shinyness);
		//printf("Multipass %d\n", multipass);

		delete[] light_color;
		delete[] light_int;
		delete[] light_pos;
		delete[] light_dir;
		delete[] light_type;
		delete[] light_alpha_max;
		delete[] light_alpha_min;

		// Render just the verticies as a wireframe
		if (render_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//do the draw call that renders the mesh into the screen
		mesh->render(GL_TRIANGLES);
	}

	//set the render state as it was before to avoid problems with future renders
	glDisable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

#ifndef SKIP_IMGUI

void Renderer::showUI()
{
		
	ImGui::Checkbox("Wireframe", &render_wireframe);
	ImGui::Checkbox("Boundaries", &render_boundaries);

	//add here your stuff
	ImGui::Checkbox("Multipass",&multi_pass);

	ImGui::SliderFloat("Bias", &bias, 0.0f, 0.1f);

	ImGui::Checkbox("Use front face culling", &use_shadow_front_face_culling);
}

#else
void Renderer::showUI() {}
#endif