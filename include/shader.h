#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class Shader {
 public:
  Shader(const char* vert_shader_path, const char* frag_shader_path);
  ~Shader();

  // 启用程序
  void useProgram() const;
  // uniform工具函数
  void setBool(const std::string& name, GLboolean value) const;
  void setInt(const std::string& name, GLint value) const;
  void setFloat(const std::string& name, GLfloat value) const;

 private:
  void checkCompileErrors(unsigned int shader,
                          const std::string_view type) const;

 private:
  GLuint shaderId;
};

#endif  // SHADER_H