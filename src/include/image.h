#pragma once

#include <stdint.h>

uint32_t get_color_image(int32_t width, int32_t height, int32_t r, int32_t g, int32_t b, int32_t a);
uint32_t get_image(const char* filename, int32_t* width, int32_t* height);
