#pragma once
#include "_defines.h"
#include "../libs/stb_image.h"

class TextureHandler {
	Int32  textureWidth  = 0;
	Int32  textureHeight = 0;
	Int32  bitsPerPixel  = 0;
	GLuint TEXTURE_ID_   = 0;
public:
	int textureUniformLocation;

	TextureHandler(std::string* sceneLocation, ShaderHandler* shader) {
		stbi_uc* textureBuffer = stbi_load((*sceneLocation + "logo.png").c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);

		glGenTextures(1, &TEXTURE_ID_);
		glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (textureBuffer) {
			stbi_image_free(textureBuffer);
		}
		textureUniformLocation = glGetUniformLocation(shader->GetShaderId(), "u_uvs") GL_ERROR
		if (!textureUniformLocation != -1) {
			glUniform1i(textureUniformLocation, 0);
		}
	}

	void Bind() const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_);
		if (!textureUniformLocation != -1) {
			glUniform1i(textureUniformLocation, 0);
		}
	}

	void Use() const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_);
		if (!textureUniformLocation != -1) {
			glUniform1i(textureUniformLocation, 0);
		}
	}

	static void Unbind() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};
