#include "client.h"

void PlayerInit(void){
     /* Player???????????? */
    SaveData data;
    srand(time(NULL));
    glm::vec3 spawnPos[gClientNum];
    
    for(int i=0; i<gClientNum; i++){
        spawnPos[i] = glm::vec3(((rand()%2)==1 ? 1: -1)*rand()%(WORLDSIZE_X/5),((rand()%2)==1 ? 1: -1)*rand()%(WORLDSIZE_Y/5),((rand()%2)==1 ? 1: -1)*rand()%(WORLDSIZE_Z/5));
    }
    
    player = (Player*)malloc(sizeof(Player)*gClientNum);
    for(int i = 0; i< gClientNum;i++)
    {
        player[i].enabled = true;
        player[i].speed = 1.0;
        player[i].dir = glm::vec3(0,0,0);
        player[i].pos = glm::vec3(0,0,0);//spawnPos[i];
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
        player[i].isBarrier = (float)MAX_BARRIER;
        player[i].isDisable = 0.0f;
        player[i].isSpecial = 0.0f;
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

    /* ???????????²ã?¼ã???????¼ã?¿ã????¡ã?¤ã?????èª­ã?¿è¾¼?????§ã????¼ã?¿ã????????????????? */
    ReadDataFile(&data);
    player[clientID].skill = data.skill;
    player[clientID].special = data.special;
    for(int i=0; i<PARAMATER_NUM; i++){
        player[clientID].parm[i] = data.parm[i];
    }
    player[clientID].attack += (float)player[clientID].parm[PARM_ATTACK] / 5.0f;
    player[clientID].speed += 0.5f * (float)player[clientID].parm[PARM_SPEED] / 5.0f;
    player[clientID].size -= player[clientID].parm[PARM_SIZE];

}

void AudioInit(int *argc, char **argv){
    alutInit(argc,argv);

    alGenBuffers( 1, &buffer );
    alGenSources( 1, &source );

    alGenBuffers( 2, &buffer2 );
    alGenSources( 2, &source2 );

    // ï¿½ï¿½ï¿½ç?´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?"hello world"ï¿½ï¿½ï¿½ï¿½ï¿½å??ï¼?ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï??ï¿½ï¿½ç¯?ï¿½ï¿½ï¿½ï¿½
    buffer = alutCreateBufferHelloWorld ();
    buffer2 = alutCreateBufferFromFile( "BGM.wav" );

    alSourcei( source, AL_BUFFER, buffer );
    alSourcei( source2, AL_BUFFER, buffer2 );
    alSourcei(source2, AL_LOOPING, AL_TRUE );
    alSourcePlay( source2 );
}

/* ??????å£????å½?????????¤å?? */
bool OnColliderSphere(Sphere a, Sphere b){
  
  bool result = false;
  glm::vec3 diff = a.pos - b.pos;

  if(a.radius + b.radius > glm::length(diff)){
    result = true;
  }
  return result;
}

bool OnColliderLinesSphere(const Sphere *a, glm::vec3 sp, glm::vec3 ep){
    glm::vec3 sv = glm::normalize(ep-sp);
    glm::vec3 pv = a->pos - sp;
    float d = glm::dot(sv,pv);

    if(glm::length(ep-a->pos) <= a->radius){
        return true;
    }
    else if(glm::length(sp-a->pos) <= a->radius){
        return true;
    }
    else if(0<d && d<glm::length(ep-sp)){
        pv -= d*sv;
        if(glm::length(pv) < a->radius)
        {
            return true;
        }
    }
    return false;
}

/* player???å½?????????¤å??????????? */
void drawPlayerCollider(void){
    for(int i = 0; i < gClientNum; i++){
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(player[i].collider.pos.x, player[i].collider.pos.y, player[i].collider.pos.z);
        glutSolidSphere(player[i].collider.radius, 200, 200);
        glPopMatrix();
    }
}

/* ???????????¤ã??è¡???? */
void deleteBullet(int index){
    array_bullet.erase(array_bullet.begin() + index);       //  i??????????????????????????
    bullet_Num--;
}


/* æ©?ä½????å½?????????¤å??????????? */
void Collider(void){
    // player loop (player->bullet)
    for(int i=0; i<gClientNum; i++){
        for(int j=0; j<bullet_Num;j++){
            if(OnColliderSphere(Sphere(BULLET_RADIUS, array_bullet[j].pos),((player[i].isBarrier > 0.0f) ? Sphere(BARRIER_RADIUS*BULLET_RADIUS,player[i].pos): player[i].collider))){

                Ability(array_bullet[j].shooter_id);
                printf("speed:%f\n" ,player[array_bullet[j].shooter_id].speed);

                if(player[clientID].isBarrier>0.0f){
                    printf("barrier protected me!!\n");
                    player[clientID].isBarrier -= player[array_bullet[j].shooter_id].attack * player[array_bullet[j].shooter_id].rate_attack / player[i].parm[PARM_HP];
                }
                else{
                    player[i].hp -= player[array_bullet[j].shooter_id].attack * player[array_bullet[j].shooter_id].rate_attack / player[i].parm[PARM_HP];

                }
                deleteBullet(j);

               if(player[i].hp <= 0.0f){
                    player[array_bullet[j].shooter_id].kill_player += 1; // kill player
                }
            }
        }
        
        if(player[i].isSpecial > 0.0f && i != clientID){
            for(int j=0; j<scoreBallNum; j++){
                if(OnColliderLinesSphere(&player[clientID].collider, player[i].pos, ary_scoreBall[j].pos)){
                    player[clientID].hp -= 0.1;
                    printf("hit lines\n");
                    if(player[clientID].hp <= 0.0f){
                        SendPlayerInfoData(i,0,+1); //client[i]'s kii_player num ++
                    }
                }
            }
        }
    }

    // scoreBall loop  (scoreBall -> player,bullet)
    for(int i=0; i<scoreBallNum; i++){
        //player
        for(int j=0; j<gClientNum; j++){
            if(OnColliderSphere(ary_scoreBall[i].collider,((player[j].isBarrier > 0.0f) ? Sphere(BARRIER_RADIUS*BULLET_RADIUS,player[j].pos): player[j].collider))){
                //damage
                if(player[j].isBarrier > 0.0f){
                    player[j].isBarrier -= 1.0f;
                }
                else{
                    player[j].hp -= 0.01f;
                    printf("dam\n");
                }
            }
        }
        //bullet
        for(int j=0; j<bullet_Num; j++){
            if(OnColliderSphere(ary_scoreBall[i].collider,Sphere(BULLET_RADIUS,glm::vec3(array_bullet[j].pos)))){
                //score
                player[array_bullet[j].shooter_id].score += 1.0f;

                //delete
                ary_scoreBall[i].hp -= 1.0f;
                if(ary_scoreBall[i].hp <= 0.0){
                    player[array_bullet[j].shooter_id].kill_enemy += 1;
                    deleteScoreBall(i);
                }
            }
        }

        
    }
}

void Ability(int id){
    switch(player[id].ability){
        
        case UP_ATTACK:
        if(player[id].rate_attack < MAX_ATTACK){
            player[id].rate_attack += 0.25;
        }
        break;

        case HEAL:
        if(player[id].hp < MAX_HP){
            player[id].hp += 0.5;
        }
        break;

        case UP_SPEED:
        if(player[id].speed < MAX_SPEED){
            player[id].speed += 0.5;
        }
        break;

        default:
        break;
    }
}

void useSpecial(void){
    if(player[clientID].mp >= (float)MAX_MP)
    {
        switch(player[clientID].special){
            case SPECIAL_BARRIER:
		    printf("create barrier\n");
             	player[clientID].isBarrier = (float)MAX_BARRIER;
                break;
            case SPECIAL_DISABLE:
                player[clientID].isDisable = (float)MAX_DISABLE_TIME;
                break;
            case SPECIAL_BIGBULLET:
                break;
            case SPECIAL_LINES:
                player[clientID].isSpecial = (float)MAX_LINES_TIME;
                break;
	    case SPECIAL_GAMBLE:

                break;
            
            default:
                break;
        }
    }
    else{
        printf("can not use special.\n");
    }
    
}

/* ?????¡ã?¤ã?????å­????????????????è¿???? */
bool retExists(const char *f){
    bool isExist;
    FILE *fp;
    if ( (fp = fopen(f,"r")) != NULL ){
        // ?????¡ã?¤ã?????å­??????????
        isExist = true;
        fclose(fp);
    }
    else{
        // ?????¡ã?¤ã?????å­?????????????
        isExist = false;
    }
    return isExist;
}
/* ??²ã?¼ã???????¼ã?¿ã????¡ã?¤ã???????¸ã??è¾¼ã?? */
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
/* ??²ã?¼ã???????¼ã?¿ã????¡ã?¤ã?????èª­ã?¿è¾¼??? */
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

/* ???????????³ã?°ã??ä½¿ç???????? */
// 0:???????????³ã?°ä¸­
// 1:???????????³ã?°å??äº?
// -1:???????????³ã?°ç??äº?
void WriteMatchFile(int value){
    FILE *fp; // FILE???æ§????ä½?
	char fname[] = "../data/judgeMatch.txt";
 
	fp = fopen(fname, "w"); // ?????¡ã?¤ã??????????????å¤±æ???????????NULL???è¿???????
	if(fp == NULL) {
		printf("%s file not open!\n", fname);
		exit (-1);
	} else {
		fprintf(fp,"%d",value);
	}
 
	fclose(fp); // ?????¡ã?¤ã??????????????
}


void WriteRankingFile(int num, RankingData *data){
    FILE *fp; 
 
	fp = fopen(FILENAME_RANKINGDATA, "w"); 
	if(fp == NULL) {
		printf("%s file not open!\n", FILENAME_RANKINGDATA);
		exit (-1);
	} else {
        fprintf(fp,"%d\n", num);
        for(int i=0; i<num; i++){
            fprintf(fp,"%s\n",data->clientName[i]);
        }
        
        for(int i=0; i<num; i++){
            fprintf(fp,"%f,%d,%d,%d,%d\n",data->score[i], data->kill_player[i], data->death[i], data->kill_enemy[i], data->kill_boss[i]);
        }
	}
 
	fclose(fp); // ?????¡ã?¤ã??????????????
}

/* ????????¤ã?¢ã?³ã???????­ã?°ã????????çµ?äº????????????³ã?­ã?³ã?°ç?»é?¢ã??ç«???¡ä???????? */
//mode -1:only exit  0:exit and start result program
void ExitClientProgram(int mode){
    switch(mode)
    {
        case 0:
            char command[256];
            sprintf(command,"gnome-terminal -- bash -c \"echo 'main'; cd ../main; ./main result; bash\"");
            std::cout << command << '\n';
            system(command);
            break;
    }

    printf("here;\n");
    alutExit();

    system("exit");
    exit(0);
}


void createScoreBall(void){
    
    for(int i=0; i<5; i++){
        ScoreBall s;
        glm::vec3 spawnPos = glm::vec3(rand()%((int)(WORLDSIZE_X/5))*((rand()%2)==1 ? 1: -1),rand()%((int)(WORLDSIZE_Y/5))*((rand()%2)==1 ? 1: -1),rand()%((int)(WORLDSIZE_Z/5))*((rand()%2)==1 ? 1: -1));
        s.pos = spawnPos;
        s.collider = Sphere(1.5,s.pos);
        s.hp = 1;
        s.howMove = Move_Stop;
        
        ary_scoreBall.push_back(ScoreBall(s));
        scoreBallNum ++;
        printf("scoreBall was created.\n");
        SendScoreBallDataCommand();
        printf("scoreBall Data was sent.\n");
    }
    
}

void deleteScoreBall(int index){
    ary_scoreBall.erase(ary_scoreBall.begin() + index);
    scoreBallNum --;
    printf("scoreBall was deleted.\n");
}

double move1;
bool hit = true;
float x,y,z;

void moveScoreBall(void){
    //int move;
    int num;
    float tamax;
    float tamaz;
    float tamay;
    
    for(int i=0; i<scoreBallNum; i++){
         for(int j = 0; j < gClientNum; j++){
             if(ary_scoreBall[i].pos.x + 50 > player[j].pos.x && ary_scoreBall[i].pos.x - 50 < player[j].pos.x){
                if(ary_scoreBall[i].pos.y + 50 > player[j].pos.y && ary_scoreBall[i].pos.y - 50 < player[j].pos.y){
                if(ary_scoreBall[i].pos.z + 50 > player[j].pos.z && ary_scoreBall[i].pos.z - 50 < player[j].pos.z){
                
                    if(MAX_HP/2 > player[j].hp){
                        ary_scoreBall[i].howMove = Move_aho;
                        if(MAX_MP/2 > player[j].mp){
                            ary_scoreBall[i].howMove = Move_atack;
                        }
                    }
                    else if(MAX_HP/2 < player[j].hp){
                        ary_scoreBall[i].howMove = Move_henntai;
                        if(MAX_MP/2 > player[j].mp){
                            ary_scoreBall[i].howMove = Move_player;
                        }
                    }
                    if(player[j].kill_enemy < 5){
                         ary_scoreBall[i].howMove = Move_atack;
                    }
                    else if(player[i].kill_player > 2){
                        ary_scoreBall[i].howMove = Move_aho;
                    }

            
                num = j;
                break;
             }
             }
             }
             else {
                ary_scoreBall[i].howMove = Move_aho;
                tamax = ary_scoreBall[i].pos.x;
                tamay = ary_scoreBall[i].pos.y;
                tamaz = ary_scoreBall[i].pos.z;
             }
         }
         
        switch(ary_scoreBall[i].howMove)
        {
            case Move_Stop:
                break;
            case Move_player:
                ary_scoreBall[i].pos.x = rand()%500;
                ary_scoreBall[i].pos.y = rand()%100;
                ary_scoreBall[i].pos.z = rand()%500;
                ary_scoreBall[i].howMove = Move_Stop;
                break;
            case Move_henntai:
                if(ary_scoreBall[i].pos.x  > player[num].pos.x){
                    ary_scoreBall[i].pos.x = (ary_scoreBall[i].pos.x + 1);
                }
                else{
                    ary_scoreBall[i].pos.x = (ary_scoreBall[i].pos.x - 1);
                }
                if(ary_scoreBall[i].pos.y  > player[num].pos.y){
                    ary_scoreBall[i].pos.y = (ary_scoreBall[i].pos.y + 1);
                }
                else{
                    ary_scoreBall[i].pos.y = (ary_scoreBall[i].pos.y - 1);
                }
                if(ary_scoreBall[i].pos.z  > player[num].pos.z){
                    ary_scoreBall[i].pos.z = (ary_scoreBall[i].pos.z + 1);
                }
                else{
                    ary_scoreBall[i].pos.z = (ary_scoreBall[i].pos.z - 1);
                }
                ary_scoreBall[i].howMove = Move_Stop;
                break;
            case Move_atack:
                if(ary_scoreBall[i].pos.x  > player[num].pos.x){
                    ary_scoreBall[i].pos.x = (ary_scoreBall[i].pos.x - 0.1);
                }
                else{
                    ary_scoreBall[i].pos.x = (ary_scoreBall[i].pos.x + 0.1);
                }
                if(ary_scoreBall[i].pos.y  > player[num].pos.y){
                    ary_scoreBall[i].pos.y = (ary_scoreBall[i].pos.y - 0.1);
                }
                else{
                    ary_scoreBall[i].pos.y = (ary_scoreBall[i].pos.y + 0.1);
                }
                if(ary_scoreBall[i].pos.z  > player[num].pos.z){
                    ary_scoreBall[i].pos.z = (ary_scoreBall[i].pos.z - 0.1);
                }
                else{
                    ary_scoreBall[i].pos.z = (ary_scoreBall[i].pos.z + 0.1);
                }
                ary_scoreBall[i].howMove = Move_Stop;
                break;
            case Move_aho:
            if( i%2 == 0){
                ary_scoreBall[i].pos.x = tamax +sin(move1);
            }
            else if(rand()%50 < 20)
                ary_scoreBall[i].pos.y = tamay+cos(move1);
                else{
                ary_scoreBall[i].pos.z = tamaz+cos(move1);
                }
                
                move1 = move1 + 0.001;
                ary_scoreBall[i].howMove = Move_Stop;
                break;
            case Move_tossinn:
                if(hit){
                x = player[num].pos.x;
                y = player[num].pos.y;
                z = player[num].pos.z;
                hit = false;
                }
                if(ary_scoreBall[i].pos.x  > x){
                    ary_scoreBall[i].pos.x = ary_scoreBall[i].pos.x - 0.1;
                }
                else{
                    ary_scoreBall[i].pos.x =ary_scoreBall[i].pos.x + 0.1;
                }
                if(ary_scoreBall[i].pos.y  > y){
                    ary_scoreBall[i].pos.y =ary_scoreBall[i].pos.y - 0.1;
                }
                else{
                    ary_scoreBall[i].pos.y =ary_scoreBall[i].pos.y + 0.1;
                }
                if(ary_scoreBall[i].pos.z  > z){
                    ary_scoreBall[i].pos.z =ary_scoreBall[i].pos.z - 0.1;
                }
                else{
                    ary_scoreBall[i].pos.z =ary_scoreBall[i].pos.z + 0.1;
                }
                /*if(ary_scoreBall[i].pos.z -2 <= z && ary_scoreBall[i].pos.z+2 >= z && ary_scoreBall[i].pos.y-2 <= y && ary_scoreBall[i].pos.y+2 >= y
                && ary_scoreBall[i].pos.x - 2 <= x&& ary_scoreBall[i].pos.x+2 >= x){
                    hit = true;
                }*/
                if(ary_scoreBall[i].pos.z ==z&& ary_scoreBall[i].pos.y == y&&ary_scoreBall[i].pos.x==x){
                    hit = true;
                }
                ary_scoreBall[i].howMove = Move_Stop;
                break;
            default:
                //ary_scoreBall[i].pos.x = ary_scoreBall[i].pos.x + 1;
                
                ary_scoreBall[i].howMove = Move_Stop;
                break;
        }
        ary_scoreBall[i].collider.pos = ary_scoreBall[i].pos;
    }
}

void checkDeath(void){
    for(int i=0; i<gClientNum; i++){
        if(player[i].hp <= 0.0f){
	    player[i].hp = 0.0f;
            player[i].enabled = false;
            if(i==clientID){
                player[clientID].anim = 200.0f;
                player[clientID].death += 1;
            }
        }
    }
}


void Respawn(void){
    glm::vec3 respawnPos = glm::vec3(((rand()%2)==1 ? 1: -1)*rand()%(WORLDSIZE_X/5),((rand()%2)==1 ? 1: -1)*rand()%(WORLDSIZE_Y/5),((rand()%2)==1 ? 1: -1)*rand()%(WORLDSIZE_Z/5));
    player[clientID].pos = respawnPos;
    printf("hukkatu\n");
    player[clientID].enabled = true;
    player[clientID].hp = MAX_HP;
    player[clientID].mp /= 5;
    player[clientID].turn1 = 0;
    player[clientID].turn2 = 0;
    player[clientID].turn3 = 0;
}
