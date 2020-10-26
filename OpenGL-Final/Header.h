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
char mousedir = 'c';	// ���������Ļ����	c:����center		l:���left	r:�Ҳ�right	u:�ϲ�up		d:�²�down	e:����right	;:����left
char eyeview = 'l';		//l:low			h:high
bool press_space = false;	//�Ƿ��¿ո��
SYSTEMTIME st = { 0 };		//���ڴ��ϵͳʱ��
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

Ball ballhead;

float vertices[] = {	//�����嶥������  
	//����λ��				����					������
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

float bvertices[] = {		//���嶥������
	//������
	0.00f,  0.00f, 0.1f,

	//����Ȧȡ12����
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

	//���ع���ȡ12����
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

	//�����ȡ12����
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

	//�ϻع���ȡ12����
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

	//�ϼ�Ȧȡ12����
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

	//�ϼ���
	0.00f,  0.00f, -0.1f
};

unsigned int bindices[] = {		//���嶥������EBO
	//����Ȧ������ʹ�����λ���
	0, 1, 2, 3, 4,5,6,7,8,9,10,11,12,1,
	//����Ȧ�뱱�ع���֮������ʹ������״����
	1,13,2,14,3,15,4,16,5,17,6,18,7,19,8,20,9,21,10,22,11,23,12,24,1,13,
	//����뱱�ع���֮������ʹ������״����
	25,13,26,14,27,15,28,16,29,17,30,18,31,19,32,20,33,21,34,22,35,23,36,24,25,13,
	//������ϻع���֮������ʹ������״����
	25,37,26,38,27,39,28,40,29,41,30,42,31,43,32,44,33,45,34,46,35,47,36,48,25,37,
	//�ϼ�Ȧ���ϻع���֮������ʹ������״����
	49,37,50,38,51,39,52,40,53,41,54,42,55,43,56,44,57,45,58,46,59,47,60,48,49,37,
	//�ϼ�Ȧ������ʹ�����λ���
	61,49,50,51,52,53,54,55,56,57,58,59,60
};

float cvertices[] = {		//���������ζ�������
	//����λ��			����

	//ʱ����ʾ�� ͬʱҲ��������ʾ��
	-9.034f,6.5f,3.75f, 0.0f, 1.0f,
	-9.034f,6.5f,-3.75f,1.0f, 1.0f,
	-9.034f,4.0f,-3.75f,1.0f, 0.0f,
	-9.034f,6.5f,3.75f,0.0f, 1.0f,
	-9.034f,4.0f,3.75f,0.0f, 0.0f,
	-9.034f,4.0f,-3.75f,1.0f, 0.0f,

	//ʱ�� �߶���ʾ��ֱ�߶�
	-9.033f,6.06f,3.51f, 0.0f, 1.0f,
	-9.033f,6.06f,3.39f,1.0f, 1.0f,
	-9.033f,5.29f,3.39f,1.0f, 0.0f,
	-9.033f,6.06f,3.51f, 0.0f, 1.0f,
	-9.033f,5.29f,3.51f, 0.0f, 0.0f,
	-9.033f,5.29f,3.39f,1.0f, 0.0f,

	//ʱ�� �߶���ʾˮƽ�߶�
	-9.033f,6.16f,3.45f, 0.0f, 1.0f,
	-9.033f,6.16f,2.64f,1.0f, 1.0f,
	-9.033f,6.04f,2.64f,1.0f, 0.0f,
	-9.033f,6.16f,3.45f, 0.0f, 1.0f,
	-9.033f,6.04f,3.45f, 0.0f, 0.0f,
	-9.033f,6.04f,2.64f,1.0f, 0.0f,

	//����˵����ʾ��
	-10.0f,15.1f,-18.3f, 0.0f, 1.0f,
	10.0f,15.1f,-18.3,1.0f, 1.0f,
	10.0f,6.6f,-18.3,1.0f, 0.0f,
	-10.0f,15.1f,-18.3f, 0.0f, 1.0f,
	-10.0f,6.6f,-18.3f, 0.0f, 0.0f,
	10.0f,6.6f,-18.3f,1.0f, 0.0f
};

float bsvertices[] = {		//���������ڻ���������Ӱ����14�����㣬ʹ�����λ���
	//Բ��
	0.0f, 0.0f, 0.0f,
	//12�����㣬���е�1������������ظ�һ��
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

//������Ӱ��ɫ��
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

//��Դλ��
glm::vec3 lightPos(-8.0f, 18.5f, -9.0f);

//���ڴ�С�ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//���뺯��
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
	//���¿ո��Ȼ���ɿ� ���л��ӽ�
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) press_space = true;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		if (press_space) {
			press_space = false;
			eyeview = 2 * 'j' - eyeview;
		}

	//���������λ�����������ھ�������
	if (cameraPos.x > 9.0) cameraPos.x = 9.0;
	if (cameraPos.x < -9.0) cameraPos.x = -9.0;
	if (cameraPos.z < -18.15) cameraPos.z = -18.15;
	if (cameraPos.z > 18.15) cameraPos.z = 18.15;

	//�ߵ��ӽ�
	if (eyeview == 'l') cameraPos.y = 3.0f;
	if (eyeview == 'h') cameraPos.y = 6.0f;

	//������
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

//��갴���ص�����
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT: {
		//�������һ������
		Ball *p = ballhead.next;
		ballhead.next = new Ball(cameraPos, cameraFront);
		ballhead.next->next = p;
		break;
	}
	case GLFW_MOUSE_BUTTON_RIGHT:
		//�Ҽ�ɾ�����������ͷ��ڴ�
		ballhead.clear();
		break;
	default:
		return;
	}
	return;
}

//����ƶ��ص�����
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

	//��¼�����������
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

//���ֻص�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

//ͶӰ���������ڼ���������Ӱλ��
//������Դλ�ã�����ռ�������һ�������򳡵����ϵ�ͶӰ
glm::vec3 bsprojection(glm::vec3 ball) {
	//������λ�ý�������������һ���뾶����
	float x0 = lightPos.x, y0 = lightPos.y + 0.0335, z0 = lightPos.z;
	float x1 = ball.x, y1 = ball.y + 0.0335, z1 = ball.z;
	float yg = 0.0;
	//ʹ�����������μ��㹫ʽ
	float x2 = x0 - (x0 - x1)*(y0 - yg) / (y0 - y1);
	float z2 = z0 - (z0 - z1)*(y0 - yg) / (y0 - y1);
	//y2=0.5  ��Ϊ��ʹ��Ӱ������ʾ����������غϣ�����0.02������
	glm::vec3 bs(x2, yg + 0.52, z2);
	return bs;
}
