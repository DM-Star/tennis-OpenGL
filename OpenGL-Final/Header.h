#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Ball.h"
#include <ctime>

glm::vec3 cameraPos = glm::vec3(8.284f, 3.0f, 18.15f);
glm::vec3 cameraFront = glm::vec3(-0.406f, -0.045f, -0.912f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -114.0f;
float pitch = -2.6f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
char mousedir = 'c';	// 鼠标所在屏幕区域	c:中央center		l:左侧left	r:右侧right	u:上侧up		d:下侧down	e:中右right	;:中左left
char eyeview = 'l';		//l:low			h:high
bool press_space = false;	//是否按下空格键
SYSTEMTIME st = { 0 };		//用于存放系统时间
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

Ball ballhead;

float vertices[] = {	//正方体顶点数组  
	//顶点位置				纹理					法向量
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
};

float bvertices[] = {		//球体顶点数组
	//北极点
	0.00f,  0.00f, 0.1f,

	//北极圈取12个点
	0.05f, 0.00f, 0.085f,
	0.0425f, 0.025f, 0.085f,
	0.025f, 0.0425f, 0.085f,
	0.00f, 0.05f, 0.085f,
	-0.025f, 0.0425f, 0.085f,
	-0.0425f, 0.025f, 0.085f,
	-0.05f, 0.00f, 0.085f,
	-0.0425f, -0.025f, 0.085f,
	-0.025f, -0.0425f, 0.085f,
	0.00f, -0.05f, 0.085f,
	0.025f, -0.0425f, 0.085f,
	0.0425f, -0.025f, 0.085f,

	//北回归线取12个点
	0.0821f, 0.0220f, 0.05f,
	0.0601f, 0.0601f, 0.05f,
	0.0220f, 0.0821f, 0.05f,
	-0.0220f, 0.0821f, 0.05f,
	-0.0601f, 0.0601f, 0.05f,
	-0.0821f, 0.0220f, 0.05f,
	-0.0821f, -0.0220f, 0.05f,
	-0.0601f, -0.0601f, 0.05f,
	-0.0220f, -0.0821f, 0.05f,
	0.0220f, -0.0821f, 0.05f,
	0.0601f, -0.0601f, 0.05f,
	0.0821f, -0.0220f, 0.05f,

	//赤道上取12个点
	0.1f, 0.0f, 0.0f,
	0.085f, 0.05f, 0.0f,
	0.05f, 0.085f, 0.0f,
	0.0f, 0.1f, 0.0f,
	-0.05f, 0.085f, 0.0f,
	-0.085f, 0.05f, 0.0f,
	-0.1f, 0.0f, 0.0f,
	-0.085f, -0.05f, 0.0f,
	-0.05f, -0.085f, 0.0f,
	0.0f, -0.1f, 0.0f,
	0.05f, -0.085f, 0.0f,
	0.085f, -0.05f, 0.0f,

	//南回归线取12个点
	0.0821f, 0.0220f, -0.05f,
	0.0601f, 0.0601f, -0.05f,
	0.0220f, 0.0821f, -0.05f,
	-0.0220f, 0.0821f, -0.05f,
	-0.0601f, 0.0601f, -0.05f,
	-0.0821f, 0.0220f, -0.05f,
	-0.0821f, -0.0220f, -0.05f,
	-0.0601f, -0.0601f, -0.05f,
	-0.0220f, -0.0821f, -0.05f,
	0.0220f, -0.0821f, -0.05f,
	0.0601f, -0.0601f, -0.05f,
	0.0821f, -0.0220f, -0.05f,

	//南极圈取12个点
	0.05f, 0.00f, -0.085f,
	0.0425f, 0.025f, -0.085f,
	0.025f, 0.0425f, -0.085f,
	0.00f, 0.05f, -0.085f,
	-0.025f, 0.0425f, -0.085f,
	-0.0425f, 0.025f, -0.085f,
	-0.05f, 0.00f, -0.085f,
	-0.0425f, -0.025f, -0.085f,
	-0.025f, -0.0425f, -0.085f,
	0.00f, -0.05f, -0.085f,
	0.025f, -0.0425f, -0.085f,
	0.0425f, -0.025f, -0.085f,

	//南极点
	0.00f,  0.00f, -0.1f
};

unsigned int bindices[] = {		//球体顶点数组EBO
	//北极圈内区域使用扇形绘制
	0, 1, 2, 3, 4,5,6,7,8,9,10,11,12,1,
	//北极圈与北回归线之间区域使用条带状绘制
	1,13,2,14,3,15,4,16,5,17,6,18,7,19,8,20,9,21,10,22,11,23,12,24,1,13,
	//赤道与北回归线之间区域使用条带状绘制
	25,13,26,14,27,15,28,16,29,17,30,18,31,19,32,20,33,21,34,22,35,23,36,24,25,13,
	//赤道与南回归线之间区域使用条带状绘制
	25,37,26,38,27,39,28,40,29,41,30,42,31,43,32,44,33,45,34,46,35,47,36,48,25,37,
	//南极圈与南回归线之间区域使用条带状绘制
	49,37,50,38,51,39,52,40,53,41,54,42,55,43,56,44,57,45,58,46,59,47,60,48,49,37,
	//南极圈内区域使用扇形绘制
	61,49,50,51,52,53,54,55,56,57,58,59,60
};

float cvertices[] = {		//各种正方形顶点数组
	//顶点位置			纹理

	//时间显示板 同时也是姓名显示板
	-9.034f,6.5f,3.75f, 0.0f, 1.0f,
	-9.034f,6.5f,-3.75f,1.0f, 1.0f,
	-9.034f,4.0f,-3.75f,1.0f, 0.0f,
	-9.034f,6.5f,3.75f,0.0f, 1.0f,
	-9.034f,4.0f,3.75f,0.0f, 0.0f,
	-9.034f,4.0f,-3.75f,1.0f, 0.0f,

	//时间 七段显示竖直线段
	-9.033f,6.06f,3.51f, 0.0f, 1.0f,
	-9.033f,6.06f,3.39f,1.0f, 1.0f,
	-9.033f,5.29f,3.39f,1.0f, 0.0f,
	-9.033f,6.06f,3.51f, 0.0f, 1.0f,
	-9.033f,5.29f,3.51f, 0.0f, 0.0f,
	-9.033f,5.29f,3.39f,1.0f, 0.0f,

	//时间 七段显示水平线段
	-9.033f,6.16f,3.45f, 0.0f, 1.0f,
	-9.033f,6.16f,2.64f,1.0f, 1.0f,
	-9.033f,6.04f,2.64f,1.0f, 0.0f,
	-9.033f,6.16f,3.45f, 0.0f, 1.0f,
	-9.033f,6.04f,3.45f, 0.0f, 0.0f,
	-9.033f,6.04f,2.64f,1.0f, 0.0f,

	//操作说明显示板
	-10.0f,15.1f,-18.3f, 0.0f, 1.0f,
	10.0f,15.1f,-18.3,1.0f, 1.0f,
	10.0f,6.6f,-18.3,1.0f, 0.0f,
	-10.0f,15.1f,-18.3f, 0.0f, 1.0f,
	-10.0f,6.6f,-18.3f, 0.0f, 0.0f,
	10.0f,6.6f,-18.3f,1.0f, 0.0f
};

float bsvertices[] = {		//该数组用于绘制网球阴影，共14个顶点，使用扇形绘制
	//圆心
	0.0f, 0.0f, 0.0f,
	//12个顶点，其中第1个顶点在最后重复一次
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f
};

//网球阴影着色器
const char *bsvertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *bsfragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);\n"
"}\n\0";

//光源位置
glm::vec3 lightPos(-8.0f, 18.5f, -9.0f);

//窗口大小回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//输入函数
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	//按下空格键然后松开 以切换视角
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) press_space = true;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		if (press_space) {
			press_space = false;
			eyeview = 2 * 'j' - eyeview;
		}

	//限制摄像机位置在网球场所在矩形区域
	if (cameraPos.x > 9.0) cameraPos.x = 9.0;
	if (cameraPos.x < -9.0) cameraPos.x = -9.0;
	if (cameraPos.z < -18.15) cameraPos.z = -18.15;
	if (cameraPos.z > 18.15) cameraPos.z = 18.15;

	//高低视角
	if (eyeview == 'l') cameraPos.y = 3.0f;
	if (eyeview == 'h') cameraPos.y = 6.0f;

	//鼠标操作
	if (mousedir == 'c') return;
	if (mousedir == 'r') yaw += 4.0;
	else if (mousedir == 'l')yaw -= 4.0;
	else if (mousedir == 'e')yaw += 2.0;
	else if (mousedir == ';')yaw -= 2.0;
	else if (mousedir == 'd') {
		pitch += 0.3;
		if (pitch > 89.0f) pitch = 89.0f;
	}
	else if (mousedir == 'u') {
		pitch -= 0.3;
		if (pitch < -89.0f) pitch = -89.0f;
	}
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

}

//鼠标按键回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT: {
		//左键产生一个网球
		Ball *p = ballhead.next;
		ballhead.next = new Ball(cameraPos, cameraFront);
		ballhead.next->next = p;
		break;
	}
	case GLFW_MOUSE_BUTTON_RIGHT:
		//右键删除所有网球并释放内存
		ballhead.clear();
		break;
	default:
		return;
	}
	return;
}

//鼠标移动回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	//记录鼠标所在区域
	if (xpos > 650) mousedir = 'r';
	else if (xpos > 550) mousedir = 'e';
	else if (xpos < 150) mousedir = 'l';
	else if (xpos < 250) mousedir = ';';
	else if (ypos > 500) mousedir = 'u';
	else if (ypos < 100) mousedir = 'd';
	else mousedir = 'c';

	float sensitivity = 0.1f; 
	yoffset *= sensitivity;

	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

//滚轮回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

//投影函数，用于计算网球阴影位置
//给定光源位置，计算空间中任意一点在网球场地面上的投影
glm::vec3 bsprojection(glm::vec3 ball) {
	//对球心位置进行修正，加上一个半径长度
	float x0 = lightPos.x, y0 = lightPos.y + 0.0335, z0 = lightPos.z;
	float x1 = ball.x, y1 = ball.y + 0.0335, z1 = ball.z;
	float yg = 0.0;
	//使用相似三角形计算公式
	float x2 = x0 - (x0 - x1)*(y0 - yg) / (y0 - y1);
	float z2 = z0 - (z0 - z1)*(y0 - yg) / (y0 - y1);
	//y2=0.5  但为了使阴影正常显示，不与地面重合，做出0.02的修正
	glm::vec3 bs(x2, yg + 0.52, z2);
	return bs;
}
