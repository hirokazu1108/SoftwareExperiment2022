#include "server.h"

void PlayerInit(void){
    /* キャラ情報の初期化 */
	player = (Player*)malloc(sizeof(Player)*gClientNum);
	for(int i = 0; i< gClientNum;i++)
    {
        player[i].enabled = true;
        player[i].isspecial = false;
        player[i].speed = 1.0;
        player[i].dir = glm::vec3(0,0,0);
        player[i].pos = glm::vec3(0,0,0);
        player[i].upVec = glm::vec3(0,0,0);
	    player[i].rate_attack = 1.0;
        player[i].turn1 = 0;
        player[i].turn2 = 0;
        player[i].turn3 = 0;
        player[i].type = 0;
        player[i].attack = DAMAGE;
        player[i].size = 0.0f;
        player[i].mp = 0.0f;
        player[i].hp = (float)MAX_HP;
        player[i].score = 0.0f;
        player[i].kill_player = 0;
        player[i].death = 0;
        player[i].kill_enemy = 0;
        player[i].kill_boss = 0;
        player[i].isBigbullet = 0;
        player[i].isChase = 0;
        player[i].isBarrier = 0.0f;
        player[i].isSpecial = 0.0f;
        player[i].transformIndex = 0;
        player[i].skill = SKILL_ATTACK;
        player[i].special = SPECIAL_BIGBULLET;
        for(int j=0; j<PARAMATER_NUM; j++)
            player[i].parm[j] = 0;
        player[i].reloadTime= 0;
        player[i].collider.radius = 1.0;
        player[i].collider.pos = player[i].pos;
        player[i].anim = 0.0f;
    }
}

