#ifndef _COMMON_H_
#define _COMMON_H_

/* \ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ */
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


#define PORT			(u_short)88888	/* «Á¢Ä?ªò¢Â«Á¢Ä?«Á¢Ä?«Á¢Ä?«Á¢Ä?ªÓ?«Á¢Ä? */
#define MAX_CLIENTS		4				/* «Á¢Ä?«Á¢Ä?«Á¢Ä?«±\¢ð«Á¢Ä?«Á¢Ä?«Á¢Ä?«Á¢Ä?ª²¢Ä«Á¢Ä?«Á¢Ä?ªÂ¢ë«Á¢Ä?«Á¢Ä?«Á¢Ä?«Á¢Ä?«Á¢Ä? */
#define MAX_NAME_SIZE	10 				/* «Á¢Ä?©Á¢Â?«Á¢Ä?«Á¢Ä?«Á¢Ä?«Á¢Ä?ªÀ?«Á¢Ä?ªÂ¢ë«Á¢Ä?«Á¢Ä?«Á¢Ä?«Á¢Ä?«Á¢Ä?*/


#define MAX_DATA		200				/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½Âï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½Âï¿½ë«ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½? */

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