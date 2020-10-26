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

	//ע��4���ص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glEnable(GL_DEPTH_TEST);	//��Ȳ���

	Shader ourShader("shader.vs", "shader.fs");		//������������ɫ��
	Shader ballShader("ball.vs", "ball.fs");		//����������ɫ��
	Shader clockShader("clock.vs", "clock.fs");		//������������ɫ����ʱ�Ӱ塢�����塢����˵���壩


	//����������Ӱ��ɫ��
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

	//����������VB0,VAO
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

	//��������VB0,VAO,VEO
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

	//���������Σ�ʱ�Ӱ塢�����塢����˵���壩VB0,VAO
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

	//����������ӰVBO,VAO
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

	//��������
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *cdata = stbi_load("clock.jpg", &width, &height, &nrChannels, 0);			//ʱ�Ӱ�����
	unsigned char *data_label = stbi_load("label.jpg", &width, &height, &nrChannels, 0);	//����˵������
	unsigned char *data_name = stbi_load("name.jpg", &width, &height, &nrChannels, 0);		//����������
	unsigned char *data_empty = stbi_load("empty.jpg", &width, &height, &nrChannels, 0);	//ȫ��ɫ����
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

		if (1) {			//������Ӱ
			Ball*p = ballhead.next;
			//��Դλ��(x0,y0,z0)
			float x0 = lightPos.x, y0 = lightPos.y, z0 = lightPos.z;
			//Ϊ�ӿ������ٶȣ��ֶ����12�����Ǻ���ֵ����������й�����ʹ��
			float cosvalue[12] = { 1,0.866,0.5,0,-0.5,-0.866,-1,-0.866,-0.5,0,0.5,0.866 };
			float sinvalue[12] = { 0,0.5,0.866,1,0.866,0.5,0,-0.5,-0.866,-1,-0.866,-0.5 };
			while (p != NULL) {
				//ʵ��λ��(x1,y1,z1)
				float x1 = p->pos.x, y1 = p->pos.y, z1 = p->pos.z;
				//r1��r2���໥��ֱ�������������Ҵ�ֱ�ڹ�Դ�����ĵ�����
				//������Ӱʵ���Ͼ��������һ���������Ӱ��������洹ֱ�ڹ���
				glm::vec3 r1(z0 - z1, 0, x1 - x0);
				r1 = r1 / (sqrtf(r1.x*r1.x + r1.z*r1.z))*0.0335f*1.2f;		//Ϊ������Ӱ�������ԣ���Ϊʹ��Ӱ����Ϊ1.2��
				glm::vec3 r2(x0 - x1, -((x0 - x1)*(x0 - x1) + (z0 - z1)*(z0 - z1)) / (y0 - y1), z0 - z1);
				r2 = r2 / (sqrtf(r2.x*r2.x + r2.y*r2.y + r2.z*r2.z))*0.0335f*1.2f;

				glm::vec3 pos = p->pos;			//p�ǵ�ǰ�����ָ��
				glm::vec3 bspos = bsprojection(pos);
				bsvertices[0] = bspos.x;
				bsvertices[1] = bspos.y;
				bsvertices[2] = bspos.z;
				for (int i = 0; i < 12; i++) {
					//r1,r2��׼��֮�󣬳�����Ӧ�����Ǻ���ֵȻ����������ӣ����Եõ������ϵĸ��߽�㣬ʹ������ͶӰ��������õ���Ӱ������
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
			if (1) {		//��ʾ��
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_label);
				glm::mat4 model;
				clockShader.setMat4("model", model);
				clockShader.setMat4("projection", projection);
				clockShader.setMat4("view", view);
				glDrawArrays(GL_TRIANGLES, 18, 6);
			}
			if (1) {		//������
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_name);
				glm::mat4 model;
				model = glm::rotate(model, 180.0f, glm::vec3(0.0, 1.0, 0.0));
				clockShader.setMat4("model", model);
				clockShader.setMat4("projection", projection);
				clockShader.setMat4("view", view);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			//ʱ���
			if (1) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, cdata);
				glm::mat4 model;
				clockShader.setMat4("model", model);
				clockShader.setMat4("projection", projection);
				clockShader.setMat4("view", view);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			if (1) {
				GetLocalTime(&st);		//�õ�ϵͳʱ��
				int time = st.wSecond + st.wMinute * 100 + st.wHour * 10000;		//��ϵͳʱ����6λ����ʾ
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_empty);

				for (int i = 0; i < 3; i++) {	//i����ǰ�����ֿ飨ʱ���֡��룩
					for (int j = 0; j < 2; j++) {//j����ǰ���������ֿ�������Ҳ�
						int k = 2 * i + j;		//k����ǰ���Ƶ�����λ������λ0����λ5
						int n = time;
						for (int l = 0; l < k; l++) n = n / 10;	//�õ���ǰλ��������
						n = n % 10;
						//���ݲ�ͬ���ֻ��Ʋ�ͬ���߶�������ʾ
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
		if (1) {		//��
			Ball*p = ballhead.next;
			while (p != NULL) {
				glm::mat4 model;
				model = glm::translate(model, p->pos);
				model = glm::translate(model, glm::vec3(0.0, 0.5535, 0.0));
				model = glm::scale(model, glm::vec3(0.335));
				ballShader.setMat4("model", model);
				//����Ļ���˳����ͷ�ļ�������ϸ����
				glDrawElements(GL_TRIANGLE_FAN, 14, GL_UNSIGNED_INT, 0);
				glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_INT, (void*)56);
				glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_INT, (void*)160);
				glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_INT, (void*)264);
				glDrawElements(GL_TRIANGLE_STRIP, 26, GL_UNSIGNED_INT, (void*)368);
				glDrawElements(GL_TRIANGLE_FAN, 14, GL_UNSIGNED_INT, (void*)472);
				//ֱ��0.067m  ģ��ֱ��0.2m
				p = p->next;
			}
		}

		ourShader.use();

		if (1) {		//½��
			glBindVertexArray(VAO);
			glm::mat4 model;
			model = glm::scale(model, glm::vec3(1000.0, 0.99, 1000.0));
			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
			ourShader.setVec3("material.diffuse", 0.6f, 0.6f, 0.6f);
			ourShader.setVec3("material.specular", 0.0f, 0.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		if (1) {		//����
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

		if (1) {		//�����̵�
			glBindVertexArray(VAO);
			glm::mat4 model;
			model = glm::scale(model, glm::vec3(18.270, 1.0, 36.570));
			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
			ourShader.setVec3("material.diffuse", 0.239f, 0.568f, 0.251f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		if (1) {		//���򳡰���
			//���ߵĻ��ƣ��Ȼ���һ����ɫ����Ȼ�������渲������ɫ����δ���ǵ������򹹳ɰ���
			glBindVertexArray(VAO);
			glm::mat4 model;
			model = glm::scale(model, glm::vec3(11.020, 1.01, 23.920));
			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
			ourShader.setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		if (1) {			//��ɫ����
			for (int i = -1; i < 2; i += 2) {		//���򳡱��ϸ���
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
			
			for (int i = -1; i < 2; i += 2) {		//�����м����
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

			for (int i = -1; i < 2; i += 2) {		//���򳡵��߸���
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

		for (int i = -1; i < 2; i += 2) {		//��������
			glBindVertexArray(VAO);
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(i*6.398, 0.957, 0));
			model = glm::scale(model, glm::vec3(0.100, 0.914, 0.10));
			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
			ourShader.setVec3("material.diffuse", 0.180f, 0.845f, 0.341f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		for (int i = 0; i < 10; i++) {		//ˮƽ����
			glBindVertexArray(VAO);
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0, 1.414 - (0.0957*i), 0));
			model = glm::scale(model, glm::vec3(12.696, 0.02, 0.02));
			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", 0.8f, 0.8f, 0.8f);
			ourShader.setVec3("material.diffuse", 0.8f, 0.8f, 0.8f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		for (int i = 1; i < 80; i++) {		//��ֱ����
			glBindVertexArray(VAO);
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(-6.348 + (0.2116*i*0.75), 0.98335, 0));
			model = glm::scale(model, glm::vec3(0.02, 0.8613, 0.02));
			ourShader.setMat4("model", model);
			ourShader.setVec3("material.ambient", 0.8f, 0.8f, 0.8f);
			ourShader.setVec3("material.diffuse", 0.8f, 0.8f, 0.8f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		if (1) {		//�˶�Ա��Ϣ��
			glm::mat4 shear;	//shear�����б�任����
			ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
			ourShader.setVec3("material.diffuse", 0.180f, 0.845f, 0.341f);
			ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			glBindVertexArray(VAO);
			for (int k = -1; k < 2; k += 2) {
				for (int i = -1; i < 2; i += 2) {		//�ν�
					for (int j = -1; j < 2; j += 2) {
						glm::mat4 model;
						model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
						model = glm::translate(model, glm::vec3(i*0.335, 0.275, j*0.8775));
						model = glm::scale(model, glm::vec3(0.05, 0.55, 0.05));
						ourShader.setMat4("model", model);
						glDrawArrays(GL_TRIANGLES, 0, 36);
					}
				}
				for (int i = -1; i < 2; i += 2) {		//����
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
					model = glm::translate(model, glm::vec3(0, 0.525, i*0.8775));
					model = glm::scale(model, glm::vec3(0.62, 0.05, 0.05));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				if (1) {			//ǰ�����
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
					model = glm::translate(model, glm::vec3(-0.335, 0.285, 0));
					model = glm::scale(model, glm::vec3(0.05, 0.05, 1.705));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				if (1) {			//�󷽺��
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
					model = glm::translate(model, glm::vec3(0.335, 0.525, 0));
					model = glm::scale(model, glm::vec3(0.05, 0.05, 1.705));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				for (int i = -1; i < 2; i++) {			//�·�֧��
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
					model = glm::translate(model, glm::vec3(-0.06, 0.32, i*0.8025));
					model = glm::scale(model, glm::vec3(0.6, 0.02, 0.1));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				for (int i = -1; i < 2; i++) {			//��֧��
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
				for (int i = -2; i < 3; i++) {			//����
					glm::mat4 model;
					model = glm::translate(model, glm::vec3(7.3225, 0.5, k*5.98));
					model = glm::translate(model, glm::vec3(-0.06 + 0.12*i, 0.34, 0));
					model = glm::scale(model, glm::vec3(0.1, 0.02, 1.705));
					ourShader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				for (int i = -1; i < 3; i++) {			//����
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

		if (1) {		//������
			glm::mat4 shear1;	//�б����
			glm::mat4 shear2;	//�б����
			ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
			ourShader.setVec3("material.diffuse", 0.180f, 0.845f, 0.341f);
			ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			glBindVertexArray(VAO);
			if (1) {		//����
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(7.765, 2.5, 0));
				model = glm::scale(model, glm::vec3(0.5, 0.05, 0.5));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			for (int i = -1; i < 2; i += 2) {
				for (int j = -1; j < 2; j += 2) {	//�ĸ��ν�
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
			for (int i = 1; i < 6; i++) {		//5�����
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
			for (int i = -1; i < 2; i += 2) {		//2��������
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
			for (int i = -2; i < 4; i++) {		//�·������
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(7.765, 2.5, 0));
				model = glm::translate(model, glm::vec3(i*0.13 - 0.065, 2.0 / 3.0 - 2.0, 0));
				model = glm::scale(model, glm::vec3(0.1, 0.02, 0.9));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			for (int i = 2; i < 6; i += 3) {		//2��������
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
				for (int j = -1; j < 2; j += 2) {	//�ĸ�����֧��
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
			if (1) {	//�α�
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(7.765, 2.5, 0));
				shear1[1] = glm::vec4(0.15, 1.0, 0, 0);
				model = model*shear1;
				model = glm::translate(model, glm::vec3(0.225, 0.35, 0.0));
				model = glm::scale(model, glm::vec3(0.05, 0.3, 0.6));
				ourShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			for (int i = -1; i < 2; i += 2) {		//����
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
		for (int i = -1; i < 2; i += 2) {		//Χ������
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
		if (1) {		//Χ����˿
			glm::mat4 shear;	//�б����
			ourShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
			ourShader.setVec3("material.diffuse", 0.180f, 0.845f, 0.341f);
			ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			glBindVertexArray(VAO);
			for (int s = -1; s < 2; s += 2) {		//����Χ��
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
			for (int s = -1; s < 2; s += 2) {		//ǰ��Χ��
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