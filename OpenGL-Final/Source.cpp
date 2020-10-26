#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "stb_image.h"
#include "Ball.h"
#include <ctime>
#include "Header.h"
#include<iostream>

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "tennis-WYX", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, 800, 600);

	//注册4个回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glEnable(GL_DEPTH_TEST);	//深度测试

	Shader ourShader("shader.vs", "shader.fs");		//绘制正方体着色器
	Shader ballShader("ball.vs", "ball.fs");		//绘制球体着色器
	Shader clockShader("clock.vs", "clock.fs");		//绘制正方形着色器（时钟板、姓名板、操作说明板）


	//绘制网球阴影着色器
	int bsvertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(bsvertexShader, 1, &bsvertexShaderSource, NULL);
	glCompileShader(bsvertexShader);
	int success;
	glGetShaderiv(bsvertexShader, GL_COMPILE_STATUS, &success);
	int bsfragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(bsfragmentShader, 1, &bsfragmentShaderSource, NULL);
	glCompileShader(bsfragmentShader);
	glGetShaderiv(bsfragmentShader, GL_COMPILE_STATUS, &success);
	int bsshaderProgram = glCreateProgram();
	glAttachShader(bsshaderProgram, bsvertexShader);
	glAttachShader(bsshaderProgram, bsfragmentShader);
	glLinkProgram(bsshaderProgram);
	glGetProgramiv(bsshaderProgram, GL_LINK_STATUS, &success);
	glDeleteShader(bsvertexShader);
	glDeleteShader(bsfragmentShader);

	//绘制正方体VB0,VAO
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//绘制球体VB0,VAO,VEO
	unsigned int bVBO, bVAO, bEBO;
	glGenVertexArrays(1, &bVAO);
	glGenBuffers(1, &bVBO);
	glGenBuffers(1, &bEBO);
	glBindVertexArray(bVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bvertices), bvertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bindices), bindices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//绘制正方形（时钟板、姓名板、操作说明板）VB0,VAO
	unsigned int cVBO, cVAO;
	glGenVertexArrays(1, &cVAO);
	glGenBuffers(1, &cVBO);
	glBindVertexArray(cVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cvertices), cvertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//绘制网球阴影VBO,VAO
	unsigned int bsVBO, bsVAO;
	glGenVertexArrays(1, &bsVAO);
	glGenBuffers(1, &bsVBO);
	glBindVertexArray(bsVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bsvertices), bsvertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//加载纹理
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *cdata = stbi_load("clock.jpg", &width, &height, &nrChannels, 0);			//时钟板纹理
	unsigned char *data_label = stbi_load("label.jpg", &width, &height, &nrChannels, 0);	//操作说明纹理
	unsigned char *data_name = stbi_load("name.jpg", &width, &height, &nrChannels, 0);		//姓名板纹理
	unsigned char *data_empty = stbi_load("empty.jpg", &width, &height, &nrChannels, 0);	//全白色纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture"), 0);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		ballhead.move();

		processInput(window);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();
		ourShader.setVec3("light.position", lightPos);
		ourShader.setVec3("viewPos", cameraPos);
		ourShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		ourShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		ourShader.setFloat("material.shininess", 32.0f);

		glm::mat4 projection = glm::perspective(fov, (float)800 / (float)600, 0.1f, 1000.0f);
		ourShader.setMat4("projection", projection);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);

		if (1) {			//网球阴影
			Ball*p = ballhead.next;
			//光源位置(x0,y0,z0)
			float x0 = lightPos.x, y0 = lightPos.y, z0 = lightPos.z;
			//为加快运行速度，手动算好12个三角函数值存放在数组中供后面使用
			float cosvalue[12] = { 1,0.866,0.5,0,-0.5,-0.866,-1,-0.866,-0.5,0,0.5,0.866 };
			float sinvalue[12] = { 0,0.5,0.866,1,0.866,0.5,0,-0.5,-0.866,-1,-0.866,-0.5 };
			while (p != NULL) {
				//实体位置(x1,y1,z1)
				float x1 = p->pos.x, y1 = p->pos.y, z1 = p->pos.z;
				//r1和r2是相互垂直的两个向量，且垂直于光源与球心的连线
				//网球阴影实际上就是网球的一个截面的阴影，这个截面垂直于光线
				glm::vec3 r1(z0 - z1, 0, x1 - x0);
				r1 = r1 / (sqrtf(r1.x*r1.x + r1.z*r1.z))*0.0335f*1.2f;		//为了让阴影更加明显，人为使阴影扩大为1.2倍
				glm::vec3 r2(x0 - x1, -((x0 - x1)*(x0 - x1) + (z0 - z1)*(z0 - z1)) / (y0 - y1), z0 - z1);
				r2 = r2 / (sqrtf(r2.x*r2.x + r2.y*r2.y + r2.z*r2.z))*0.0335f*1.2f;

				glm::vec3 pos = p->pos;			//p是当前球体的指针
				glm::vec3 bspos = bsprojection(pos);
				bsvertices[0] = bspos.x;
				bsvertices[1] = bspos.y;
				bsvertices[2] = bspos.z;
				for (int i = 0; i < 12; i++) {
					//r1,r2标准化之后，乘上相应的三角函数值然后与球心相加，可以得到截面上的各边界点，使用网球投影函数计算得到阴影的坐标
					bspos = bsprojection(pos + r1*cosvalue[i] + r2*sinvalue[i]);
					bsvertices[3 + 3 * i] = bspos.x;
					bsvertices[4 + 3 * i] = bspos.y;
					bsvertices[5 + 3 * i] = bspos.z;
					if (i == 0) {
						bsvertices[39] = bspos.x;
						bsvertices[40] = bspos.y;
						bsvertices[41] = bspos.z;
					}
				}

				glUseProgram(bsshaderProgram);
				int projectionLocation = glGetUniformLocation(bsshaderProgram, "projection");
				glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
				int viewLocation = glGetUniformLocation(bsshaderProgram, "view");
				glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

				glBindBuffer(GL_ARRAY_BUFFER, bsVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(bsvertices), bsvertices, GL_STATIC_DRAW);
				glBindVertexArray(bsVAO);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 14);

				p = p->next;
			}
		}
		if (1) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glBindVertexArray(cVAO);
			clockShader.use();
			if (1) {		//提示板
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_label);
				glm::mat4 model;
				clockShader.setMat4("model", model);
				clockShader.setMat4("projection", projection);
				clockShader.setMat4("view", view);
				glDrawArrays(GL_TRIANGLES, 18, 6);
			}
			if (1) {		//姓名板
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_name);
				glm::mat4 model;
				model = glm::rotate(model, 180.0f, glm::vec3(0.0, 1.0, 0.0));
				clockShader.setMat4("model", model);
				clockShader.setMat4("projection", projection);
				clockShader.setMat4("view", view);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			//时间板
			if (1) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, cdata);
				glm::mat4 model;
				clockShader.setMat4("model", model);
				clockShader.setMat4("projection", projection);
				clockShader.setMat4("view", view);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			if (1) {
				GetLocalTime(&st);		//得到系统时间
				int time = st.wSecond + st.wMinute * 100 + st.wHour * 10000;		//将系统时间用6位数表示
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_empty);

				for (int i = 0; i < 3; i++) {	//i代表当前的数字块（时、分、秒）
					for (int j = 0; j < 2; j++) {//j代表当前数字在数字块的左侧或右侧
						int k = 2 * i + j;		//k代表当前绘制的数字位数，高位0，低位5
						int n = time;
						for (int l = 0; l < k; l++) n = n / 10;	//得到当前位数的数字
						n = n % 10;
						//根据不同数字绘制不同的七段数字显示
						if (n == 1 || n == 2 || n == 3 || n == 7) {
							glm::mat4 model;
							model = glm::translate(model, glm::vec3(0.0, 0, -2.5*(2 - i) - 1.05*(1 - j)));
							clockShader.setMat4("model", model);
							glDrawArrays(GL_TRIANGLES, 6, 6);
						}
						if (n == 1 || n == 3 || n == 4 || n == 5 || n == 7 || n == 9) {
							glm::mat4 model;
							model = glm::translate(model, glm::vec3(0.0, 0, -2.5*(2 - i) - 1.05*(1 - j)));
							model = glm::translate(model, glm::vec3(0.0, -0.85, 0.0));
							clockShader.setMat4("model", model);
							glDrawArrays(GL_TRIANGLES, 6, 6);
						}
						if (n == 5 || n == 6) {
							glm::mat4 model;
							model = glm::translate(model, glm::vec3(0.0, 0, -2.5*(2 - i) - 1.05*(1 - j)));
							model = glm::translate(model, glm::vec3(0.0, 0.0, -0.85));
							clockShader.setMat4("model", model);
							glDrawArrays(GL_TRIANGLES, 6, 6);
						}
						if (n == 2) {
							glm::mat4 model;
							model = glm::translate(model, glm::vec3(0.0, 0, -2.5*(2 - i) - 1.05*(1 - j)));
							model = glm::translate(model, glm::vec3(0.0, -0.85, -0.85));
							clockShader.setMat4("model", model);
							glDrawArrays(GL_TRIANGLES, 6, 6);
						}
						if (n == 1 || n == 4) {
							glm::mat4 model;
							model = glm::translate(model, glm::vec3(0.0, 0, -2.5*(2 - i) - 1.05*(1 - j)));
							clockShader.setMat4("model", model);
							glDrawArrays(GL_TRIANGLES, 12, 6);
						}
						if (n == 1 || n == 7 || n == 0) {
							glm::mat4 model;
							model = glm::translate(model, glm::vec3(0.0, 0, -2.5*(2 - i) - 1.05*(1 - j)));
							model = glm::translate(model, glm::vec3(0.0, -0.85, 0.0));
							clockShader.setMat4("model", model);
							glDrawArrays(GL_TRIANGLES, 12, 6);
						}
						if (n == 1 || n == 4 || n == 7) {
							glm::mat4 model;
							model = glm::translate(model, glm::vec3(0.0, 0, -2.5*(2 - i) - 1.05*(1 - j)));
							model = glm::translate(model, glm::vec3(0.0, -1.7, 0.0));
							clockShader.setMat4("model", model);
							glDrawArrays(GL_TRIANGLES, 12, 6);
						}
					}
				}
			}
		}

		glBindVertexArray(bVAO);
		ballShader.use();
		ballShader.setMat4("projection", projection);
		ballShader.setMat4("view", view);
		if (1) {		//球
			Ball*p = ballhead.next;
			while (p != NULL) {
				glm::mat4 model;
				model = glm::translate(model, p->pos);
				model = glm::translate(model, glm::vec3(0.0, 0.5535, 0.0));
				model = glm::scale(model, glm::vec3(0.335));
				ballShader.setMat4("model", model);
				//这里的绘制顺序在头文件中有详细解释
				glDrawElements(GL_TRIANGLE_FAN, 14, GL_UNSIGNED_INT, 0);
				glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_INT, (void*)56);
				glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_INT, (void*)160);
				glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_INT, (void*)264);
				glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_INT, (void*)368);
				glDrawElements(GL_TRIANGLE_FAN, 14, GL_UNSIGNED_INT, (void*)472);
				//直径0.067m  模型直径0.2m
				p = p->next;
			}
		}

		ourShader.use();

		if (1) {		//陆地
			glBindVertexArray(VAO);
			glm::mat4 model;
			model = glm::scale(model, glm::vec3(1000.0, 0.99, 1000.0));
			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
			ourShader.setVec3("material.diffuse", 0.6f, 0.6f, 0.6f);
			ourShader.setVec3("material.specular", 0.0f, 0.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		if (1) {		//蓝天
			glBindVertexArray(VAO);
			ourShader.setVec3("material.ambient", 0.2f, 0.9f, 0.9f);
			ourShader.setVec3("material.diffuse", 0.2f, 0.9f, 0.9f);
			ourShader.setVec3("material.specular", 0.0f, 0.0f, 0.0f);
			for (int i = 0; i < 4; i++) {
				glm::mat4 model;
				model = glm::rotate(model, 90.0f * i, glm::vec3(0.0, 1.0, 0.0));
				model = glm::translate(model, glm::vec3(0, 50.0, 200.0));
				model = glm::scale(model, glm::vec3(400.0, 100.0, 1.0));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0, 70.0, 0));
			model = glm::scale(model, glm::vec3(500.0, 1.0, 500.0));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		if (1) {		//网球场绿地
			glBindVertexArray(VAO);
			glm::mat4 model;
			model = glm::scale(model, glm::vec3(18.270, 1.0, 36.570));
			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
			ourShader.setVec3("material.diffuse", 0.239f, 0.568f, 0.251f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		if (1) {		//网球场白线
			//白线的绘制：先绘制一个白色区域，然后在上面覆盖上蓝色区域，未覆盖到的区域构成白线
			glBindVertexArray(VAO);
			glm::mat4 model;
			model = glm::scale(model, glm::vec3(11.020, 1.01, 23.920));
			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
			ourShader.setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		if (1) {			//蓝色格子
			for (int i = -1; i < 2; i += 2) {		//网球场边上格子
				for (int j = -1; j < 2; j += 2) {
					glBindVertexArray(VAO);
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(i*4.8, 0, j*5.9425));
					model = glm::scale(model, glm::vec3(1.320, 1.02, 11.835));
					ourShader.setMat4("model", model);
					ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
					ourShader.setVec3("material.diffuse", 0.254f, 0.411f, 0.882f);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
			
			for (int i = -1; i < 2; i += 2) {		//网球场中间格子
				for (int j = -1; j < 2; j += 2) {
					glBindVertexArray(VAO);
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(i*2.0575, 0, j*3.200));
					model = glm::scale(model, glm::vec3(4.065, 1.02, 6.350));
					ourShader.setMat4("model", model);
					ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
					ourShader.setVec3("material.diffuse", 0.254f, 0.411f, 0.882f);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}

			for (int i = -1; i < 2; i += 2) {		//网球场底线格子
				glBindVertexArray(VAO);
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(0, 0, i*9.1425));
				model = glm::scale(model, glm::vec3(8.180, 1.02, 5.435));
				ourShader.setMat4("model", model);
				ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
				ourShader.setVec3("material.diffuse", 0.254f, 0.411f, 0.882f);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		for (int i = -1; i < 2; i += 2) {		//拦网柱子
			glBindVertexArray(VAO);
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(i*6.398, 0.957, 0));
			model = glm::scale(model, glm::vec3(0.100, 0.914, 0.10));
			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
			ourShader.setVec3("material.diffuse", 0.180f, 0.845f, 0.341f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		for (int i = 0; i < 10; i++) {		//水平网线
			glBindVertexArray(VAO);
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0, 1.414 - (0.0957*i), 0));
			model = glm::scale(model, glm::vec3(12.696, 0.02, 0.02));
			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", 0.8f, 0.8f, 0.8f);
			ourShader.setVec3("material.diffuse", 0.8f, 0.8f, 0.8f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		for (int i = 1; i < 80; i++) {		//竖直网线
			glBindVertexArray(VAO);
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(-6.348 + (0.2116*i*0.75), 0.98335, 0));
			model = glm::scale(model, glm::vec3(0.02, 0.8613, 0.02));
			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", 0.8f, 0.8f, 0.8f);
			ourShader.setVec3("material.diffuse", 0.8f, 0.8f, 0.8f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		if (1) {		//运动员休息椅
			glm::mat4 shear;	//shear用作切变变换矩阵
			ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
			ourShader.setVec3("material.diffuse", 0.180f, 0.845f, 0.341f);
			ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			glBindVertexArray(VAO);
			for (int k = -1; k < 2; k += 2) {
				for (int i = -1; i < 2; i += 2) {		//椅脚
					for (int j = -1; j < 2; j += 2) {
						glm::mat4 model;
						model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
						model = glm::translate(model, glm::vec3(i*0.335, 0.275, j*0.8775));
						model = glm::scale(model, glm::vec3(0.05, 0.55, 0.05));
						ourShader.setMat4("model", model);
						glDrawArrays(GL_TRIANGLES, 0, 36);
					}
				}
				for (int i = -1; i < 2; i += 2) {		//扶手
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
					model = glm::translate(model, glm::vec3(0, 0.525, i*0.8775));
					model = glm::scale(model, glm::vec3(0.62, 0.05, 0.05));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				if (1) {			//前方横杠
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
					model = glm::translate(model, glm::vec3(-0.335, 0.285, 0));
					model = glm::scale(model, glm::vec3(0.05, 0.05, 1.705));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				if (1) {			//后方横杠
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
					model = glm::translate(model, glm::vec3(0.335, 0.525, 0));
					model = glm::scale(model, glm::vec3(0.05, 0.05, 1.705));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				for (int i = -1; i < 2; i++) {			//下方支架
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
					model = glm::translate(model, glm::vec3(-0.06, 0.32, i*0.8025));
					model = glm::scale(model, glm::vec3(0.6, 0.02, 0.1));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				for (int i = -1; i < 2; i++) {			//后方支架
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
					model = glm::translate(model, glm::vec3(0, 0.31, 0));
					shear[1] = glm::vec4(0.2, 1, 0, 0);
					model = model*shear;
					model = glm::translate(model, glm::vec3(0, -0.31, 0));
					model = glm::translate(model, glm::vec3(0.25, 0.6, i*0.8025));
					model = glm::scale(model, glm::vec3(0.02, 0.58, 0.1));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				for (int i = -2; i < 3; i++) {			//座板
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
					model = glm::translate(model, glm::vec3(-0.06 + 0.12*i, 0.34, 0));
					model = glm::scale(model, glm::vec3(0.1, 0.02, 1.705));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				for (int i = -1; i < 3; i++) {			//靠板
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
					model = glm::translate(model, glm::vec3(0, 0.31, 0));
					shear[1] = glm::vec4(0.2, 1, 0, 0);
					model = model*shear;
					model = glm::translate(model, glm::vec3(0, -0.31, 0));
					model = glm::translate(model, glm::vec3(0.23, 0.59 + 0.12*i, 0));
					model = glm::scale(model, glm::vec3(0.02, 0.1, 1.705));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		}

		if (1) {		//裁判椅
			glm::mat4 shear1;	//切变矩阵
			glm::mat4 shear2;	//切变矩阵
			ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
			ourShader.setVec3("material.diffuse", 0.180f, 0.845f, 0.341f);
			ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			glBindVertexArray(VAO);
			if (1) {		//座板
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(7.765, 2.5, 0));
				model = glm::scale(model, glm::vec3(0.5, 0.05, 0.5));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			for (int i = -1; i < 2; i += 2) {
				for (int j = -1; j < 2; j += 2) {	//四个椅脚
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.765, 2.5, 0));
					shear1[1] = glm::vec4(-i*0.15, 1.0, 0, 0);
					shear2[1] = glm::vec4(0, 1.0, -j*0.15, 0);
					model = model*shear1 *shear2;
					model = glm::translate(model, glm::vec3(i*0.225, -1.0, j*0.275));
					model = glm::scale(model, glm::vec3(0.05, 2.0, 0.05));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
			for (int i = 1; i < 6; i++) {		//5个横杠
				for (int j = -1; j < 2; j++) {
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.765, 2.5, 0));
					shear1[1] = glm::vec4(0.15, 1.0, 0, 0);
					shear2[1] = glm::vec4(0, 1.0, j*0.15, 0);
					model = model*shear1 *shear2;
					model = glm::translate(model, glm::vec3(-0.225, i / 3.0 - 2.0, 0));
					model = glm::scale(model, glm::vec3(0.05, 0.05, 0.5));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
			for (int i = -1; i < 2; i += 2) {		//2个侧面横杠
				for (int j = -1; j < 2; j++) {
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.765, 2.5, 0));
					shear1[1] = glm::vec4(j*0.15, 1.0, 0, 0);
					shear2[1] = glm::vec4(0, 1.0, -i*0.15, 0);
					model = model*shear1 *shear2;
					model = glm::translate(model, glm::vec3(0, 2.0 / 3.0 - 2.0, i*0.275));
					model = glm::scale(model, glm::vec3(0.4, 0.05, 0.05));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
			for (int i = -2; i < 4; i++) {		//下方置物板
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(7.765, 2.5, 0));
				model = glm::translate(model, glm::vec3(i*0.13 - 0.065, 2.0 / 3.0 - 2.0, 0));
				model = glm::scale(model, glm::vec3(0.1, 0.02, 0.9));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			for (int i = 2; i < 6; i += 3) {		//2个后面横杠
				for (int j = -1; j < 2; j++) {
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.765, 2.5, 0));
					shear1[1] = glm::vec4(-0.15, 1.0, 0, 0);
					shear2[1] = glm::vec4(0, 1.0, -j*0.15, 0);
					model = model*shear1 *shear2;
					model = glm::translate(model, glm::vec3(0.225, i / 3.0 - 2.0, 0));
					model = glm::scale(model, glm::vec3(0.05, 0.05, 0.5));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
			for (int i = -1; i < 2; i += 2) {
				for (int j = -1; j < 2; j += 2) {	//四个扶手支架
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.765, 2.5, 0));
					shear1[1] = glm::vec4(0.15, 1.0, 0, 0);
					model = model*shear1;
					model = glm::translate(model, glm::vec3(i*0.225, 0.1, j*0.275));
					model = glm::scale(model, glm::vec3(0.05, 0.2, 0.05));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
			if (1) {	//椅背
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(7.765, 2.5, 0));
				shear1[1] = glm::vec4(0.15, 1.0, 0, 0);
				model = model*shear1;
				model = glm::translate(model, glm::vec3(0.225, 0.35, 0.0));
				model = glm::scale(model, glm::vec3(0.05, 0.3, 0.6));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			for (int i = -1; i < 2; i += 2) {		//扶手
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(7.765, 2.5, 0));
				shear1[1] = glm::vec4(0.15, 1.0, 0, 0);
				model = model*shear1;
				model = glm::translate(model, glm::vec3(-0.025, 0.225, i*0.275));
				model = glm::scale(model, glm::vec3(0.45, 0.05, 0.05));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
		ourShader.setVec3("material.diffuse", 0.180f, 0.845f, 0.341f);
		glBindVertexArray(VAO);
		for (int i = -1; i < 2; i += 2) {		//围网柱子
			for (int j = -1; j < 2; j += 2) {
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(i*9.135, 3.5, j*18.285));
				model = glm::scale(model, glm::vec3(0.200, 6.0, 0.200));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			for (int j = -1; j < 2; j += 2) {
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(i*3.045, 3.5, j*18.285));
				model = glm::scale(model, glm::vec3(0.200, 6.0, 0.200));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			for (int j = -1; j < 2; j += 2) {
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(i*9.135, 3.5, j*10.971));
				model = glm::scale(model, glm::vec3(0.20, 6.0, 0.20));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			for (int j = -1; j < 2; j += 2) {
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(i*9.135, 3.5, j*3.657));
				model = glm::scale(model, glm::vec3(0.200, 6.0, 0.200));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			if (1) {
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(0, 6.5, i*18.285));
				model = glm::scale(model, glm::vec3(18.270, 0.2, 0.200));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			if (1) {
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(i*9.135, 6.5, 0));
				model = glm::scale(model, glm::vec3(0.2, 0.2, 36.570));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		if (1) {		//围网铁丝
			glm::mat4 shear;	//切变矩阵
			ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
			ourShader.setVec3("material.diffuse", 0.180f, 0.845f, 0.341f);
			ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			glBindVertexArray(VAO);
			for (int s = -1; s < 2; s += 2) {		//左右围网
				shear[1] = glm::vec4(0, 1.0, s*1.0, 0);
				for (int i = -1; i < 2; i += 2) {
					for (int j = 0; j < 62; j++) {
						glm::mat4 model;
						model = glm::translate(model, glm::vec3(i*9.135, 3.5, 15.25 - j*0.5));
						model = model*shear;
						model = glm::scale(model, glm::vec3(0.04, 6.0, 0.04));
						ourShader.setMat4("model", model);
						glDrawArrays(GL_TRIANGLES, 0, 36);
					}
					for (int j = 1; j < 12; j++) {
						for (int k = -1; k < 2; k += 2) {
							glm::mat4 model;
							model = glm::translate(model, glm::vec3(i*9.135, 3.5 + s*k*0.25*j, k*(15.25 + 0.25*j)));
							shear[1] = glm::vec4(0, 1.0, -s*1.0, 0);
							model = model*shear;
							model = glm::scale(model, glm::vec3(0.04, 6.0 - 0.5*j, 0.04));
							ourShader.setMat4("model", model);
							glDrawArrays(GL_TRIANGLES, 0, 36);
						}
					}
				}
			}
			for (int s = -1; s < 2; s += 2) {		//前后围网
				for (int i = -1; i < 2; i += 2) {
					for (int j = 0; j < 25; j++) {
						glm::mat4 model;
						model = glm::translate(model, glm::vec3(6 - j*0.5, 3.5, i*18.285));
						shear[1] = glm::vec4(s*1.0, 1.0, 0, 0);
						model = model*shear;
						model = glm::scale(model, glm::vec3(0.04, 6.0, 0.04));
						ourShader.setMat4("model", model);
						glDrawArrays(GL_TRIANGLES, 0, 36);
					}
					for (int j = 1; j < 12; j++) {
						for (int k = -1; k < 2; k += 2) {
							glm::mat4 model;
							model = glm::translate(model, glm::vec3(k*(6 + 0.25*j), 3.5 + s*k*0.25*j, i*18.285));
							shear[1] = glm::vec4(-s*1.0, 1.0, 0, 0);
							model = model*shear;
							model = glm::scale(model, glm::vec3(0.04, 6.0 - 0.5*j, 0.04));
							ourShader.setMat4("model", model);
							glDrawArrays(GL_TRIANGLES, 0, 36);
						}
					}
				}
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}