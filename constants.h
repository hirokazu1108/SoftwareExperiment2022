#ifndef _COMMON_H_
#define _COMMON_H_

/* \���\���\���\���\���\���\��� */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<assert.h>
#include "glm/glm.hpp"

#define PORT			(u_short)8988	/* ������?��������?������?������?������?���?������? */
#define MAX_CLIENTS		4				/* ������?������?������?���\���������?������?������?������?�����ď�����?������?���돫����?������?������?������?������? */
#define MAX_NAME_SIZE	10 				/* ������?������?������?������?������?������?���?������?���돫����?������?������?������?������?*/

#define MAX_DATA		200				/* ������?������?������?������?������?������?������?������?������?������?����������?������?������?������?���돫����?������?������?������?������? */

#define PLAYERDATA_COMMAND 's'
#define END_COMMAND 'e'


class Player{
public:
    int type;
    float spead;
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 upVec;
    int mp;
    int hp;
    int reloadTime;
    void SetDir(float turn_xz);
};

class Bullet{
public:
    int type;
    float spead;
    glm::vec3 pos;
    glm::vec3 dir;
};


#endif 