//顶点着色器
#version 330 core

layout (location = 0)in vec3 aPos;    // 输入变量的属性位置值为 0
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

uniform float offset;
out vec4 vexColor; // 输出变量

void main() {
	gl_Position = vec4(aPos.x + offset, -aPos.y, aPos.z, 1.0); // 构造一个vec4变量为内置变量gl_Position赋值
	vexColor = vec4(aColor, 1.0); // 为输出变量赋值
}