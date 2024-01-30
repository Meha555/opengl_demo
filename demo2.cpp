#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <cmath>
#include <cstring>
#include <iostream>
#include <string>

#include "shader.h"

const int g_width = 600;
const int g_height = 400;
int g_tex_wid;
int g_tex_hei;
int nr_channels;

// 顶点坐标
GLfloat vertices[] = {
    // positions          // colors           // texture coords
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
};

const int stride = 8;

// 纹理坐标
GLfloat texCoords[] = {
    0.0f, 0.0f,  // 左下角
    1.0f, 0.0f,  // 右下角
    0.5f, 1.0f   // 上中
};

GLuint indices[] = {
    0, 1, 2,  // 第一个三角形
    2, 3, 1,  // 第二个三角形
};

void frameBufferSizeCallback(GLFWwindow* win, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* win) {
  if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(win, true);
  }
}

int main() {
  // 0. 设置GLFW版本
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 1. 创建窗口
  GLFWwindow* win =
      glfwCreateWindow(g_width, g_height, "OpenGL demo", NULL, NULL);
  if (win == NULL) {
    std::cerr << "无法创建窗口" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(win);
  glfwSetFramebufferSizeCallback(win, &frameBufferSizeCallback);

  // 2. 加载OpenGL函数
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "加载OpenGL函数失败" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  Shader* shader =
      new Shader("shader/demo2/vertex.glsl", "shader/demo2/fragment.glsl");

  // 4. 创建VBO、VAO、EBO
  GLuint VBO, VAO, EBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);  // 需要先绑定VAO，再绑定VBO，最后绑定顶点属性

  // 5. 绑定VBO到GL_ARRAY_BUFFER并填入顶点数据
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 6. 绑定EBO到GL_ELEMENT_ARRAY_BUFFER并填入索引数据
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // 7. 关联顶点属性
  // 7.1 位置属性
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat),
                        (void*)0);
  // 7.2 颜色属性
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat),
                        (void*)(3 * sizeof(GLfloat)));
  // 7.3 纹理属性
  glad_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat),
                             (void*)(6 * sizeof(GLfloat)));

  // 8. 启用顶点属性
  glEnableVertexAttribArray(0);  // 启用位置属性
  glEnableVertexAttribArray(1);  // 启用颜色属性
  glEnableVertexAttribArray(2);  // 启用纹理属性

  // 之后我们不需要用到VBO了，因此可以解绑
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // 9. 加载图片并创建纹理
  // 9.1 创建纹理对象
  GLuint texture;
  glGenTextures(1, &texture);
  // 9.2 将纹理对象绑定到GL_TEXTURE_2D上
  glBindTexture(GL_TEXTURE_2D, texture);
  // 9.3 设置纹理环绕方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  // 9.4 设置纹理过滤方式并生成多级渐远纹理
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // 9.5 加载图像
  unsigned char* data = stbi_load("resource/texture/container.jpg", &g_tex_wid,
                                  &g_tex_hei, &nr_channels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_tex_wid, g_tex_hei, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  // 10. 渲染循环
  while (!glfwWindowShouldClose(win)) {
    // 处理输入
    processInput(win);

    // 清屏
    glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, texture);

    // 绘制图元
    // 进行绘图操作和更改Uniform需要先启用着色器【这句话也可以移到循环外，因为这里只有一个shader】
    shader->useProgram();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 使用顶点索引

    // 缓冲区翻页并处理窗口事件
    glfwSwapBuffers(win);
    glfwPollEvents();
  }

exit:
  // 11. 回收资源
  glfwDestroyWindow(win);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &VBO);
  delete shader;  // 显式调用析构函数，否则glfwTerminate会先一步调用，导致段错误
  // glDeleteProgram(shaderProgram);
  glfwTerminate();

  return 0;
}