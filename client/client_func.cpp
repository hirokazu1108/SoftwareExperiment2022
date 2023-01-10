#include "client.h"

void PlayerInit(void){
     /* Playerの初期化 */
    SaveData data;

	player = (Player*)malloc(sizeof(Player)*gClientNum);
    for(int i = 0; i< gClientNum;i++)
    {
        player[i].enabled = true;
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
        player[i].hp = 3.0;
	    player[i].ability = UP_ATTACK;
        player[i].skill = SKILL_ATTACK;
        player[i].special = SPECIAL_BIGBULLET;
        for(int j=0; j<PARAMATER_NUM; j++)
            player[i].parm[j] = 0;
        player[i].reloadTime= 0;
        player[i].collider.radius = 1.0;
        player[i].collider.pos = player[i].pos;
    }

    /* 自分のゲームデータファイルを読み込んでデータを反映させる */
    ReadDataFile(&data);
    player[clientID].skill = data.skill;
    player[clientID].special = data.special;
    for(int i=0; i<PARAMATER_NUM; i++){
        player[clientID].parm[i] = data.parm[i];
    }
    player[clientID].attack += (float)player[clientID].parm[PARM_ATTACK];
    player[clientID].hp += (float)player[clientID].parm[PARM_HP];
    player[clientID].speed += (float)player[clientID].parm[PARM_SPEED];
    player[clientID].size -= player[clientID].parm[PARM_SIZE];

}

/* 球同士の当たり判定 */
bool OnColliderSphere(Sphere a, Sphere b){
  
  bool result = false;
  glm::vec3 diff = a.pos - b.pos;

  if(a.radius + b.radius > glm::length(diff)){
    result = true;
  }
  return result;
}

/* playerの当たり判定をだす */
void drawPlayerCollider(void){
    for(int i = 0; i < gClientNum; i++){
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(player[i].collider.pos.x, player[i].collider.pos.y, player[i].collider.pos.z);
        glutSolidSphere(player[i].collider.radius, 200, 200);
        glPopMatrix();
    }
}

/* 球の削除を行う */
void deleteBullet(int index){
    array_bullet.erase(array_bullet.begin() + index);       //  i??????????????????????????
    bullet_Num--;
}


/* 機体の当たり判定を取る */
void Collider(void){
    for(int i=0; i<gClientNum; i++){
        for(int j=0; j<bullet_Num;j++){
            if(OnColliderSphere(Sphere(BULLET_RADIUS,array_bullet[j].pos),player[i].collider)){
                Ability(array_bullet[j].shooter_id);
                printf("speed:%f\n" ,player[array_bullet[j].shooter_id].speed);
                player[i].hp -= player[array_bullet[j].shooter_id].attack * player[array_bullet[j].shooter_id].rate_attack;
                deleteBullet(j);
                printf("hirokazu: hit client[%d]\n",i);
            }
        }
    }
}

void Ability(int id){
    switch(player[id].ability){
        
        case UP_ATTACK:
        if(player[id].rate_attack <= MAX_ATTACK){
            player[id].rate_attack += 0.3;
        }
        break;

        case HEAL:
        if(player[id].hp <= MAX_HP){
            player[id].hp += 1.0;
        }
        break;

        case UP_SPEED:
        if(player[id].speed <= MAX_SPEED){
            player[id].speed += 0.5;
        }
        break;

        default:
        break;
    }
}

/* ファイルが存在するかを返す */
bool retExists(const char *f){
    bool isExist;
    FILE *fp;
    if ( (fp = fopen(f,"r")) != NULL ){
        // ファイルが存在する
        isExist = true;
        fclose(fp);
    }
    else{
        // ファイルは存在しない
        isExist = false;
    }
    return isExist;
}
/* ゲームデータファイルを書き込む */
void WriteDataFile(SaveData *data){
    FILE *fp;
    if ( (fp = fopen(FILENAME_GAMEDATA,"wb")) != NULL ){
        fwrite(data,sizeof(SaveData),1,fp);
        fclose(fp);
    }
    else{
        printf("failed to open writefile.\n");
        exit(1);
    }
}
/* ゲームデータファイルを読み込む */
void ReadDataFile(SaveData *data){
    FILE *fp;
    if ( (fp = fopen(FILENAME_GAMEDATA,"rb")) != NULL ){
        fread(data,sizeof(SaveData),1,fp);
        fclose(fp);
    }
    else{
        printf("failed to open readfile.\n");
        exit(1);
    }
}