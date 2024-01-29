#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "shader.h"

const int g_width = 600;
const int g_height = 400;

// GLfloat vertices[] = {
//     0.5f,  0.5f,  0.0f,  // 右上
//     0.5f,  -0.5f, 0.0f,  // 右下
//     -0.5f, 0.5f,  0.0f,  // 左上
//     -0.5f, -0.5f, 0.0f,  // 左下
// };

// 顶点坐标
GLfloat vertices[] = {
    // 位置                            // 颜色
    0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 右下
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // 左下
    0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f   // 顶部
};

// 纹理坐标
GLfloat texCoords[] = {
    0.0f, 0.0f,  // 左下角
    1.0f, 0.0f,  // 右下角
    0.5f, 1.0f   // 上中
};

const int stride = 6;

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

  // // 读取着色器代码
  // std::ifstream ifs;
  // ifs.open("shader/vertex_shader.glsl", std::ios::in);
  // std::string vertexShaderSource{std::istreambuf_iterator<char>(ifs),
  //                                std::istreambuf_iterator<char>()};
  // ifs.close();
  // ifs.open("shader/fragment_shader.glsl", std::ios::in);
  // std::string fragmentShaderSource{std::istreambuf_iterator<char>(ifs),
  //                                  std::istreambuf_iterator<char>()};
  // ifs.close();

  // const char* vert_source = vertexShaderSource.c_str();
  // const char* frag_source = fragmentShaderSource.c_str();

  // // 3. 创建Shader，指定着色器代码，编译着色器，链接着色器
  // GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // glShaderSource(vertexShader, 1, &vert_source, NULL);
  // glCompileShader(vertexShader);
  // int success = 0;
  // char infoLog[512] = {0};
  // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  // if (!success) {
  //   glad_glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
  //   std::cerr << "顶点着色器编译错误: " << infoLog << std::endl;
  // }
  // GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  // glShaderSource(fragmentShader, 1, &frag_source, NULL);
  // glCompileShader(fragmentShader);
  // std::memset(infoLog, 0, sizeof(infoLog));
  // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  // if (!success) {
  //   glad_glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
  //   std::cerr << "片段着色器编译错误: " << infoLog << std::endl;
  // }
  // GLuint shaderProgram = glCreateProgram();
  // glAttachShader(shaderProgram, vertexShader);
  // glAttachShader(shaderProgram, fragmentShader);
  // glLinkProgram(shaderProgram);
  // glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  // if (!success) {
  //   glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
  //   std::cerr << "着色器链接错误: " << infoLog << std::endl;
  // }
  // glDeleteShader(vertexShader);
  // glDeleteShader(fragmentShader);
  Shader* shader = new Shader("shader/vertex_shader.glsl",
                              "shader/fragment_shader.glsl");

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

  // 8. 启用顶点属性
  glEnableVertexAttribArray(0);  // 启用位置属性
  glEnableVertexAttribArray(1);  // 启用颜色属性

  // 之后我们不需要用到VBO了，因此可以解绑
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // 9. 渲染循环
  while (!glfwWindowShouldClose(win)) {
    // 处理输入
    processInput(win);

    // 清屏
    glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制图元
    // glUseProgram(shaderProgram);  //
    // 进行绘图操作和更改Uniform需要先启用着色器【这句话也可以移到循环外，因为这里只有一个shader】
    shader->useProgram();

    static int cnt = 2;
    if (cnt & 1) {
      static GLfloat offset = 0;
      std::cout << offset << std::endl;
      offset += 0.2;
      shader->setFloat("offset",
                       std::sin(offset));  // 用sin来将值映射在[-1,1]内
    }
    cnt++;

    // GLdouble rgbGreenValue = (std::sin(timeValue) / 2.0f) + 0.5f;
    // int vertexColorLocation = glGetUniformLocation(shaderProgram,
    // "ourColor"); if (vertexColorLocation == -1) {
    //   std::cerr << "渲染管线中没有找到指定的Uniform" << std::endl;
    //   goto exit;
    // }
    // glUniform4f(vertexColorLocation, 0.0f, rgbGreenValue, 0.0f, 1.0f);

    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 使用顶点索引

    // 缓冲区翻页并处理窗口事件
    glfwSwapBuffers(win);
    glfwPollEvents();
  }

exit:
  // 10. 回收资源
  glfwDestroyWindow(win);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &VBO);
  delete shader;  // 显式调用析构函数，否则glfwTerminate会先一步调用，导致段错误
  // glDeleteProgram(shaderProgram);
  glfwTerminate();

  return 0;
}