#ifndef _COMMON_H_
#define _COMMON_H_

/* ヘッダのインクルード */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "glm/glm.hpp"
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <math.h>
#include<assert.h>
#include <cassert>
#include <vector>
#include <algorithm>
#include <fstream>
#include<sys/types.h>


#define PORT			(u_short)88888	/* デフォルトポート番号 */
#define MAX_CLIENTS		4				/* 最大プレイ人数 */
#define MAX_DATA		200
#define MAX_BULLET_NUM   25    // 最大弾数
#define BULLET_SPEED   0.5f     // 弾の速度
#define DAMAGE         1.0f				
#define MAX_HP 3.0f
#define MAX_BARRIER 3.0f
#define MAX_SPEED 2.0f
#define MAX_ATTACK 2.0f
#define BULLET_RADIUS 0.3       //radius of bullet collider
#define MAX_MP 100.0f


#define PLAYERDATA_COMMAND 's'
#define END_COMMAND 'e'
#define BULLETDATA_COMMAND 'b'
#define RANKING_DATA 'r'

#define PARAMATER_MAX 5         //max value of status paramaters
#define PARAMATER_NUM 5         //num of status paramaters
#define PARAMATER_SUM_MAX 10    //max of sum of status paramaters
#define NAME_MAX_LENGTH 20      //max length of client name
#define SKILL_NUM 3             //num of skills
#define SPECIAL_NUM 4           //num of specials
#define FILENAME_GAMEDATA "../data/gamedata.bin"

typedef enum {
    State_Wait,
    State_Play,
    State_Result,
}GameState;

typedef enum{
    UP_ATTACK,
    HEAL,
    UP_SPEED,
}Ability_Type;

typedef enum{
    PARM_ATTACK,
    PARM_HP,
    PARM_SPEED,
    PARM_SIZE,
    PARM_MP,
}PARAMATER;

typedef enum{
    SKILL_ATTACK,
    SKILL_HP,
    SKILL_SPEED,
}SKILL;

typedef enum{
    SPECIAL_BARRIER,
    SPECIAL_LASER,
    SPECIAL_BIGBULLET,
    SPECIAL_POWERUP,
}SPECIAL;

/* 球の当たり判定 */
class Sphere{
    public:
    float radius;
    glm::vec3 pos;
    Sphere(float r = 1,glm::vec3 m = glm::vec3(0,0,0)){radius = r; pos = m;}
};

class Player{
public:
    bool enabled;   //trueならみえる状態
    int type;
    float speed;
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 upVec;
    float rate_attack;
    float turn1;
    float turn2;
    float turn3;
    float attack;
    float mp;
    float hp;
    float size;
    int reloadTime;
    float isBarrier; //true : barrier mode
    Sphere collider;
    Ability_Type ability;
    SKILL skill;
    SPECIAL special;
    int parm[PARAMATER_NUM];
};

class Game{
    public:
        char	clientName[MAX_CLIENTS][NAME_MAX_LENGTH+1];
        GameState state;
        std::vector<int> ranking; //indexにnを入れると第n位のクライアント番号を返す
};

/* 弾のクラス */
class BULLET{
    public:
        int shooter_id;
        //int type;           // 弾�??�??�??
        glm::vec3 pos;      // 弾�??座�??
        //glm::vec3 speed;    // 弾�?????�??
        glm::vec3 dir;      // 弾�????��??????????????
        int lifetime;       // 弾�??�??示�?????
        BULLET(){pos.x = pos.y = pos.z = dir.x = dir.y = dir.z = 0;};
        BULLET(const BULLET& b){
            pos = b.pos;
            dir = b.dir;
            lifetime = b.lifetime;
            shooter_id = b.shooter_id;
        }
};

/* the class to save game data */
typedef class{
    public:
    char clientName[NAME_MAX_LENGTH+1];
    SKILL skill;
    SPECIAL special;
    int parm[PARAMATER_NUM];
}SaveData;

#endif 

