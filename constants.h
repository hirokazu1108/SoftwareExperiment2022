#ifndef _COMMON_H_
#define _COMMON_H_

/* ??????????????��?��???????��?? */
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


#define PORT			(u_short)88888	/* ????????????????????��???????? */
#define MAX_CLIENTS		4				/* ???大�???????�人??? */
#define MAX_DATA		200
#define MAX_BULLET_NUM   25    // ???大弾???
#define BULLET_SPEED   0.5f     // 弾�?????�??
#define DAMAGE         1.0f				
#define MAX_HP 10.0f
#define BARRIER_RADIUS 1.0f
#define MAX_BARRIER 4.0f //hp
#define MAX_DISABLE_TIME 3.0f
#define MAX_LINES_TIME 20.0f //lines
#define MAX_DAMAGEAREA_TIME 5.0f //damage area time
#define DAMEGEAREA_RADIUS 2.0f
#define MAX_BEAM_TIME 20.0f // beam time
#define MAX_SPEED 2.0f
#define MAX_ATTACK 2.0f
#define BULLET_RADIUS 0.3f       //radius of bullet collider
#define BIGBULLET_RADIUS 1.5f
#define MAX_MP 100.0f
#define NOTARGET 10

#define PLAYERDATA_COMMAND 'p'
#define END_COMMAND 'e'
#define BULLETDATA_COMMAND 'b'
#define RANKING_DATA 'r'
#define SCOREBALL_COMMAND 's'
#define TIMER_COMMAND 't'
#define PLAYERINFO_COMMAND 'i'

#define PARAMATER_MAX 5         //max value of status paramaters
#define PARAMATER_NUM 5         //num of status paramaters
#define PARAMATER_SUM_MAX 10    //max of sum of status paramaters
#define NAME_MAX_LENGTH 10      //max length of client name
#define SKILL_NUM 3             //num of skills
#define SPECIAL_NUM 6            //num of specials
#define FILENAME_GAMEDATA "../data/gamedata.bin"
#define FILENAME_RANKINGDATA "../data/ranking.txt"
#define GAMETIME 100

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
    SPECIAL_DISABLE,
    SPECIAL_BIGBULLET,
    SPECIAL_LINES,
    SPECIAL_DAMAGEAREA,
    SPECIAL_BEAM,
    SPECIAL_CHASE,
    SPECIAL_TRANSFORM,
    SPECIAL_GAMBLE,
    SPECIAL_NONE,
}SPECIAL;

typedef enum{
    Move_Stop,
    Move_player,
    Move_henntai,
    Move_atack,
    Move_aho,
    Move_dami,
    Move_tossinn,

}Move_Pattern;

/* ??????�??????????��?? */
class Sphere{
    public:
    float radius;
    glm::vec3 pos;
    Sphere(float r = 1,glm::vec3 m = glm::vec3(0,0,0)){radius = r; pos = m;}
};

class Player{
public:
    bool enabled;   //true????????��???????��??
    bool isspecial;
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
    int isBigbullet;
    int isChase;
    float isBarrier; //0< : barrier mode
    float isSpecial; //0< ::special mode
    float score;
    int kill_player;
    int death;
    int kill_enemy;
    int kill_boss;
    Sphere collider;
    Ability_Type ability;
    SKILL skill;
    SPECIAL special;
    int parm[PARAMATER_NUM];
    float anim; //animation of death
};

class Game{
    public:
        char	clientName[MAX_CLIENTS][NAME_MAX_LENGTH+1];
        unsigned int time; //seconds
        GameState state;
};

/* 弾�??????????? */
class BULLET{
    public:
        int shooter_id;
        SPECIAL type;           // 弾�??�???�???
        glm::vec3 pos;      // 弾�??座�??
        glm::vec3 dir;      // 弾�????��??????????????
        int lifetime;       // 弾�??�???示�?????
        int target_id;
        BULLET(){
            shooter_id = 10;
            type = SPECIAL_NONE;
            pos = glm::vec3(0.0f, 0.0f, 0.0f);
            dir = glm::vec3(0.0f, 0.0f, 0.0f);
            lifetime = 0;
            target_id = NOTARGET;
        }
        BULLET(const BULLET& b){
            shooter_id = b.shooter_id;
            type = b.type;
            pos = b.pos;
            dir = b.dir;
            lifetime = b.lifetime;
            target_id = b.target_id;
        }
};

/* the class of scoreBall */
class ScoreBall{
    public:
        Move_Pattern howMove;
        glm::vec3 pos;
        float hp;
        Sphere collider;
        ScoreBall(){pos = glm::vec3(0,0,0); hp = 1; howMove = Move_Stop; collider = Sphere(1.0,glm::vec3(0,0,0));}
        ScoreBall(const ScoreBall& s){
            howMove = s.howMove;
            pos = s.pos;
            hp = s.hp;
            collider = s.collider;
        }
};

/* the class to save game data */
class SaveData{
    public:
    char clientName[NAME_MAX_LENGTH+1];
    SKILL skill;
    SPECIAL special;
    int parm[PARAMATER_NUM];
};

/* the class to ranking data */
class RankingData{
    public:
    char clientName[MAX_CLIENTS][NAME_MAX_LENGTH+2];
    float score[MAX_CLIENTS];
    int kill_player[MAX_CLIENTS];
    int death[MAX_CLIENTS];
    int kill_enemy[MAX_CLIENTS];
    int kill_boss[MAX_CLIENTS];
    int kill_sum[MAX_CLIENTS];
};

#endif 
