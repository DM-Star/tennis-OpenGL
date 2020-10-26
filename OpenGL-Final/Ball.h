#pragma once
#include <glm/glm.hpp>

//网球类链表
class Ball {
public:
	Ball *next;
	glm::vec3 pos;
	glm::vec3 v;
	glm::vec3 a;
	char state;		//运动模式，包括   飞行fly   滚动roll   静止static
	Ball() {
		next = NULL;
		pos = glm::vec3(0.0, 0.0, 0.0);
		v = glm::vec3(0.0, 0.0, 0.0);
		a = glm::vec3(0.0, 0.0, 0.0);
		state = 's';
	}
	Ball(glm::vec3 position,glm::vec3 dir) {
		next = NULL;
		pos = position;
		v = dir;
		a = glm::vec3(0.0, -0.04, 0.0);
		state = 'f';
	}
	void clear() {
		//释放所有网球的内存
		Ball*p = next;
		if (p == NULL) return;
		p->clear();
		delete p;
		next = NULL;
	}
	void move() {
		//所有网球根据速度、加速度进行移动，并支持地面、边界、拦网的碰撞检测
		//考虑飞行和滚动两个模式
		Ball*p = next;
		while (p != NULL) {
			p->pos += p->v;
			p->v +=p->a;
			//与地面的碰撞检测
			if (p->pos.y < 0) {
				p->pos.y = -p->pos.y;
				p->v.y = -0.4*p->v.y;
				if (p->v.y < 0.08) {
					p->v.y = 0;
					p->a.y = 0;
					p->pos.y = 0;
					state = 'r';
				}
			}
			//与拦网的碰撞检测
			if (p->pos.z <= 0 && (p->pos.z - p->v.z)>=0 && p->pos.y<0.914&&p->pos.x>-6.398&&p->pos.x < 6.938) {
				p->pos.z = -p->pos.z;
				p->v.z = -0.5*p->v.z;
			}
			if (p->pos.z >= 0 && (p->pos.z - p->v.z)<=0 && p->pos.y<0.914&&p->pos.x>-6.398&&p->pos.x < 6.938) {
				p->pos.z = -p->pos.z;
				p->v.z = -0.5*p->v.z;
			}
			//与四个边界的碰撞检测
			if (p->pos.z < -18.285) {
				p->pos.z = -36.570-p->pos.z;
				p->v.z = -0.5*p->v.z;
			}
			if (p->pos.z > 18.285) {
				p->pos.z = 36.570 - p->pos.z;
				p->v.z = -0.5*p->v.z;
			}
			if (p->pos.x < -9.135) {
				p->pos.x = -18.270 - p->pos.x;
				p->v.x = -0.5*p->v.x;
			}
			if (p->pos.x > 9.135) {
				p->pos.x = 18.270 - p->pos.x;
				p->v.x = -0.5*p->v.x;
			}
			//滚动情况下的运动
			if (state == 'r') {
				p->a.x = -0.01*p->v.x;
				p->a.z = -0.01*p->v.z;
			}
			p = p->next;
		}
	}
};