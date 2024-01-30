#version 330 core

layout (location = 0) in vec3 aPos;     // 输入变量的属性位置值为 0
layout (location = 1) in vec3 aColor;   // 颜色变量的属性位置值为 1
layout (location = 2) in vec2 aTexCoord;// 纹理坐标的属性位置值为 2

out vec3 vexColor;
out vec2 texCoord;

void main() {
	gl_Position = vec4(aPos, 1.0);
	vexColor = aColor;
	texCoord = aTexCoord;
}