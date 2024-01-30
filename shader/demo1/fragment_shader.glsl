//片段着色器
#version 330 core

in vec4 vexColor; // 从顶点着色器输出到片段着色器的输入变量(要求名字和类型都一样)
out vec4 FragColor;

void main() {
	FragColor = vexColor;
}