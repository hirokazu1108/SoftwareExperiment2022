#include "server.h"

int maxButtonNum[SCENE_NUM]={};    //シーンごとのボタンの最大数,indexはScene列挙体に対応

void PlayerInit(void){
    /* キャラ情報の初期化 */
	player = (Player*)malloc(sizeof(Player)*gClientNum);
	for(int i = 0; i< gClientNum;i++)
    {
        player[i].enabled = true;
        player[i].isspecial = false;
        player[i].speed = 1.0;
        player[i].dir.x = 0;
        player[i].dir.y = 0;
        player[i].dir.z = 0;
        player[i].pos.x = 0;
        player[i].pos.y = 0;
        player[i].pos.z = 0;
        player[i].upVec.x = 0;
        player[i].upVec.y = 0;
        player[i].upVec.z = 0;
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
        player[i].ability = UP_ATTACK;
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

/* 選択しているボタンの番号をずらす.引数に+1で次,-1で前のボタンへ */
void shiftButtonSelect(int shift, int max){

    gUi.selectButton += (shift>=0) ? shift%max : -(-shift)%max;
    if(gUi.selectButton < 0){
        gUi.selectButton += max;
    }
    else if(gUi.selectButton >= max){
        gUi.selectButton -= max;
    }
}
