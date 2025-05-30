#ifndef FBO_H
#define FBO_H

#include "../core/includes.h"
#include "texture.h"

namespace GFX {

	//FrameBufferObject
	//helps rendering the scene inside a texture

	extern GFX::FBO* fbo[10];


	class FBO {
	public:
		GLuint fbo_id;
		Texture* color_textures[4];
		Texture* depth_texture;
		int num_color_textures;
		GLenum bufs[4];
		int width;
		int height;
		bool owns_textures;

		GLuint renderbuffer_color;
		GLuint renderbuffer_depth;//not used

		FBO();
		~FBO();

		static void Init();
		bool create(int width, int height, int num_textures = 1, int format = GL_RGB, int type = GL_UNSIGNED_BYTE, bool use_depth_texture = true);
		bool setTexture(Texture* texture, int cubemap_face = -1);
		bool setTextures(std::vector<Texture*> textures, Texture* depth = NULL, int cubemap_face = -1);
		bool setDepthOnly(int width, int height); //use this for shadowmaps

		void bind();
		void unbind();

		//to render momentarily to a single buffer
		void enableSingleBuffer(int num);
		void enableBuffers(bool buffer0, bool buffer1, bool buffer2, bool buffer3);
		void enableAllBuffers(); //back to all

		void freeTextures();
	};

};
#endif