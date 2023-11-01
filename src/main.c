#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#ifdef __WIN64
#include <windows.h>
#endif

#include "shader.h"
#include "image.h"

#define FPS 60
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

float vertices[20] = {
    // Position		    //Tex coords
     1.0f,  1.0f, -1.0f,    1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,    1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,    0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,    0.0f, 1.0f
};

unsigned int indices[6] = {
    0, 3, 1,
    1, 3, 2
};

void glfw_resize_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
    const char* filename;
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    } else {
        filename = argv[1];
    }

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello GL", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    #if (defined(__WIN64) || defined(__WIN32)) && defined(_DEBUG)
    FreeConsole();
    #endif

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    uint32_t vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uint32_t ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    uint32_t shader_program = get_shader();
    glUseProgram(shader_program);

    int32_t pos_attrib = glGetAttribLocation(shader_program, "vertex");
    glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    glEnableVertexAttribArray(pos_attrib);

    int32_t tex_attrib = glGetAttribLocation(shader_program, "texCoord");
    glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(tex_attrib);

    int w, h;
    int32_t texture = get_image(filename, &w, &h);
    if (texture == -1) {
        w = 1;
        h = 1;
        uint8_t r = 100;
        uint8_t g = 0;
        uint8_t b = 100;
        uint8_t a = 255;
        texture = get_color_image(w, h, 100, 0, 100, 255);
        printf("Loaded default color #%02x%02x%02x%02x\n", r, g, b, a);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    int32_t tex_uniform = glGetUniformLocation(shader_program, "image");
    glUniform1i(tex_uniform, 0);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glfwSetFramebufferSizeCallback(window, glfw_resize_callback);

    float time_start = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        time_start = glfwGetTime();
        glfwSwapBuffers(window);
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        float time_end = glfwGetTime();
        float time_diff = time_end - time_start;
        if (time_diff < 1.0f / FPS) {
            usleep((1.0f / FPS - time_diff) * 1000000);
        }
    }

    glfwTerminate();
    return 0;
}
