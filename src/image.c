#include "image.h"

#include <GL/glew.h>
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

uint32_t get_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return (a << 24) | (b << 16) | (g << 8) | r;
}

uint32_t get_color_image(int32_t width, int32_t height, int32_t r, int32_t g, int32_t b, int32_t a) {
	uint32_t* image = malloc(width * height * sizeof(uint32_t));
	for (int32_t i = 0; i < width * height; i++) {
		image[i] = get_color(r, g, b, a);
	}
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image == NULL) {
		printf("Failed to create image with color #%d%d%d%d\n", r, g, b, a);
		return -1;
	}

	GLenum format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	stbi_image_free(image);
	return texture;
}

uint32_t get_image(const char* filename, int32_t* width, int32_t* height){
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	int32_t w, h, channels;
	stbi_set_flip_vertically_on_load(1);
	uint8_t* image = stbi_load(filename, &w, &h, &channels, 0);
	if (image == NULL) {
		printf("Failed to load image: %s\n", filename);
		return -1;
	}

	GLenum format;
	switch (channels) {
		case 1:
			format = GL_RED;
		break;
		case 2:
			format = GL_RG;
		break;
		case 3:
			format = GL_RGB;
		break;
		case 4:
			format = GL_RGBA;
		break;
		default:
			printf("Unsupported number of channels: %d\n", channels);
			return -1;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, image);
	*width = w;
	*height = h;
	stbi_image_free(image);
	return texture;
}
