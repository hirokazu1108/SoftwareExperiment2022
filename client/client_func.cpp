#include "client.h"

void PlayerInit(void){
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
        player[i].isspecial = false;
        player[i].speed = 1.0;
        player[i].dir = glm::vec3(0,0,0);
        player[i].pos = glm::vec3(0,0,0);//spawnPos[i];
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
        player[i].isBarrier = (float)MAX_BARRIER;
        player[i].isSpecial = 0.0f;
        player[i].transformIndex = 0;
        player[i].skill = SKILL_ATTACK;
        player[i].special = SPECIAL_BIGBULLET;
        for(int j=0; j<PARAMATER_NUM; j++)
            player[i].parm[j] = 0;
        player[i].collider.radius = 1.0;
        player[i].collider.pos = player[i].pos;
        player[i].anim = 0.0f;
    }

    ReadDataFile(&data);
    player[clientID].skill = data.skill;
    player[clientID].special = data.special;
    for(int i=0; i<PARAMATER_NUM; i++){
        player[clientID].parm[i] = data.parm[i];
    }
    player[clientID].attack += ((float)player[clientID].parm[PARM_ATTACK] + 5.0f) * 0.5f;
    player[clientID].speed += 1.0f + (float)player[clientID].parm[PARM_SPEED] * 0.1f;
    player[clientID].parm[PARM_HP] = 6.0f + (player[clientID].parm[PARM_HP] + 5.0f) * 0.5f;
    player[clientID].size = 1.0f - player[clientID].parm[PARM_SIZE] * 0.1f;
    player[clientID].collider.radius = player[clientID].size;

}

void AudioInit(int *argc, char **argv){
    alutInit(argc,argv);

    alGenBuffers( 1, &buffer );
    alGenSources( 1, &source );

    alGenBuffers( 2, &buffer2 );
    alGenSources( 2, &source2 );

    buffer = alutCreateBufferFromFile( "../audio/shot.wav" );
    if(rand()%2 == 0){
    buffer2 = alutCreateBufferFromFile( "../audio/BGM.wav" );
    }
    else{
        buffer2 = alutCreateBufferFromFile("../audio/BGM2.wav");
    }
    
    alSourcei( source, AL_BUFFER, buffer );
    alSourcei( source2, AL_BUFFER, buffer2 );
    alSourcei(source2, AL_LOOPING, AL_TRUE );
    alSourcePlay( source2 );
}

bool OnColliderSphere(Sphere a, Sphere b){
  
  bool result = false;
  glm::vec3 diff = a.pos - b.pos;

  if(a.radius + b.radius > glm::length(diff)){
    result = true;
  }
  return result;
}

bool OnColliderLinesSphere(Sphere a, glm::vec3 sp, glm::vec3 ep){
    glm::vec3 sv = glm::normalize(ep-sp);
    glm::vec3 pv = a.pos - sp;
    float d = glm::dot(sv,pv);

    if(glm::length(ep-a.pos) <= a.radius){
        return true;
    }
    else if(glm::length(sp-a.pos) <= a.radius){
        return true;
    }
    else if(0<d && d<glm::length(ep-sp)){
        pv -= d*sv;
        if(glm::length(pv) < a.radius)
        {
            return true;
        }
    }
    return false;
}

void drawPlayerCollider(void){
    for(int i = 0; i < gClientNum; i++){
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(player[i].collider.pos.x, player[i].collider.pos.y, player[i].collider.pos.z);
        glutSolidSphere(player[i].collider.radius, 200, 200);
        glPopMatrix();
    }
}

void deleteBullet(int index){
    array_bullet.erase(array_bullet.begin() + index);       //  i??????????????????????????
    bullet_Num--;
}


void Collider(void){
    // player loop (player->bullet)
    for(int i=0; i<gClientNum; i++){
        for(int j=0; j<bullet_Num;j++){
            if(OnColliderSphere(Sphere((array_bullet[j].type == SPECIAL_BIGBULLET) ? (BIGBULLET_RADIUS) : (BULLET_RADIUS), array_bullet[j].pos),((player[i].isBarrier > 0.0f) ? Sphere(BARRIER_RADIUS,player[i].pos): player[i].collider))){

                float big_rate = 1.0f;
                if(array_bullet[j].type == SPECIAL_BIGBULLET){big_rate = 5.0f;}
                Ability(array_bullet[j].shooter_id);
                printf("speed:%f\n" ,player[array_bullet[j].shooter_id].speed);

                if(player[i].isBarrier>0.0f){
                    printf("barrier protected me!!\n");
                    player[i].isBarrier -= ((player[array_bullet[j].shooter_id].attack* big_rate - player[i].parm[PARM_HP]) * player[array_bullet[j].shooter_id].rate_attack)/2;
                }
                else{
                    if( i!=clientID && player[i].isSpecial > 0.0f && player[i].special == SPECIAL_TRANSFORM && player[i].transformIndex == clientID){
                        player[clientID].hp -= (player[array_bullet[j].shooter_id].attack* big_rate - player[i].parm[PARM_HP]) * player[array_bullet[j].shooter_id].rate_attack;
                    }
                    else if(i==clientID && player[clientID].isSpecial > 0.0f && player[clientID].special == SPECIAL_TRANSFORM){
                        player[player[clientID].transformIndex].hp -= (player[array_bullet[j].shooter_id].attack* big_rate - player[i].parm[PARM_HP]) * player[array_bullet[j].shooter_id].rate_attack;
                        printf("migawari\n");
                    }
                    else{
                        player[i].hp -= (player[array_bullet[j].shooter_id].attack* big_rate - player[i].parm[PARM_HP]) * player[array_bullet[j].shooter_id].rate_attack;
                    }
                }
                deleteBullet(j);

                //migawari
                if(player[clientID].hp <= 0.0f && player[clientID].enabled && i!=clientID && player[i].isSpecial > 0.0f && player[i].special == SPECIAL_TRANSFORM && player[i].transformIndex == clientID){
                    SendPlayerInfoData(i ,0,+1); //client[i]'s kii_player num ++
                    break;
                }
               else if(player[clientID].hp <= 0.0f && player[clientID].enabled){
                    SendPlayerInfoData(array_bullet[j].shooter_id ,0,+1); //client[i]'s kii_player num ++
                    break;
                }
            }
        }
        
        // lines
        if(player[i].isSpecial > 0.0f && i != clientID && player[i].special == SPECIAL_LINES){
            for(int j=0; j<scoreBallNum; j++){
                if(OnColliderLinesSphere( (player[clientID].isBarrier > 0.0f) ? Sphere(BARRIER_RADIUS,player[clientID].pos): player[clientID].collider, player[i].pos, ary_scoreBall[j].pos)){
                    if(player[clientID].isBarrier>0.0f){
                        player[clientID].isBarrier -= 0.1/2;  //damage to barrier by lines
                        printf("barrier protected me by lines!!\n");
                    }
                    else {
                        player[clientID].hp -= 0.5;
                        printf("hit lines\n");
                    }
                    if(player[clientID].hp <= 0.0f && player[clientID].enabled){
                        SendPlayerInfoData(i,0,+1); //client[i]'s kii_player num ++
                        break;
                    }
                }
            }
        }

        //damageArea  player[me]--enemy[damage] 
        if(player[i].isSpecial >0.0f && i!=clientID && player[i].special == SPECIAL_DAMAGEAREA){
            if(OnColliderSphere(Sphere(DAMEGEAREA_RADIUS,player[i].pos), (player[clientID].isBarrier > 0.0f) ? Sphere(BARRIER_RADIUS,player[clientID].pos): player[clientID].collider)){
                

                if(player[clientID].isBarrier>0.0f){
                    player[clientID].isBarrier -= 0.2/2;  //damage to barrier by damageArea
                    printf("barrier protected me by damageArea!!\n");
                }
                else {
                    player[clientID].hp -= 0.2;  // damage
                    printf("hit damageArea\n");
                }
                if(player[clientID].hp <= 0.0f && player[clientID].enabled){
                    SendPlayerInfoData(i,0,+1); //client[i]'s kii_player num ++
                    break;
                }
            }
        }
	    
	    //beam  player[me]--enemy[beam] 
        if(player[i].isSpecial >0.0f && i!=clientID && player[i].special == SPECIAL_BEAM){
            for(int j=0; j<100; j++){
                glm::vec3 d = glm::vec3(3*player[i].dir.x*(j+1),3*player[i].dir.y*(j+1),3*player[i].dir.z*(j+1));
                if(OnColliderSphere(Sphere(1.5,player[i].pos+d), (player[clientID].isBarrier > 0.0f) ? Sphere(BARRIER_RADIUS,player[clientID].pos): player[clientID].collider)){
                    if(player[clientID].isBarrier>0.0f){
                        printf("barrier protected me by beam!!\n");
                        player[clientID].isBarrier -= 0.1/2;  //damage to barrier by beam
                    }
                    else {
                        player[clientID].hp -= 0.1;  // damage
                        printf("hit beam\n");
                    }
                    if(player[clientID].hp <= 0.0f && player[clientID].enabled){
                        SendPlayerInfoData(i,0,+1); //client[i]'s kii_player num ++
                        break;
                    }
                }
            }
            
        }
    }


    // scoreBall loop  (scoreBall -> player,bullet)
    for(int i=0; i<scoreBallNum; i++){
        //player
        for(int j=0; j<gClientNum; j++){
            if(OnColliderSphere(ary_scoreBall[i].collider,((player[j].isBarrier > 0.0f) ? Sphere(BARRIER_RADIUS,player[j].pos): player[j].collider))){
                //damage
                if(player[j].isBarrier > 0.0f){
                    player[j].isBarrier -= 1.0f/2;
                }
                else{
                    player[j].hp -= 0.01f;
                    printf("damage by scoreBall\n");
                }
            }
        }

        //bullet
        for(int j=0; j<bullet_Num; j++){
            if(OnColliderSphere(ary_scoreBall[i].collider,Sphere(BULLET_RADIUS,glm::vec3(array_bullet[j].pos)))){
                

                //delete
                ary_scoreBall[i].hp -= 1.0f;
                if(ary_scoreBall[i].hp <= 0.0){
                    //score
                    player[array_bullet[j].shooter_id].score += SCORE_KILL_ENEMY;
                    player[array_bullet[j].shooter_id].kill_enemy += 1;
                    deleteScoreBall(i);
                }
            }
        }

        
        for(int j=0; j<gClientNum; j++){
            if(player[j].isSpecial >0.0f && player[j].special == SPECIAL_DAMAGEAREA){
                //damageArea
                if(OnColliderSphere(Sphere(DAMEGEAREA_RADIUS,player[j].pos), ary_scoreBall[i].collider)){            
                    //delete
                    ary_scoreBall[i].hp -= 1.0f; //damage
                    if(ary_scoreBall[i].hp <= 0.0){
                        //score
                        player[j].score += SCORE_KILL_ENEMY;
                        player[j].kill_enemy += 1;
                        deleteScoreBall(i);
                    }
                }
            }  
            //beam  player[me]--enemy[beam] 
            else if(player[j].isSpecial >0.0f && player[j].special == SPECIAL_BEAM){
                for(int k=0; k<100; k++){
                    glm::vec3 d = glm::vec3(3*player[j].dir.x*(k+1),3*player[j].dir.y*(k+1),3*player[j].dir.z*(k+1));
                    if(OnColliderSphere(Sphere(1.5,player[j].pos+d), ary_scoreBall[i].collider)){
                        //delete
                        ary_scoreBall[i].hp -= 2.0f; //damage
                        if(ary_scoreBall[i].hp <= 0.0){
                            //score
                            player[j].score += SCORE_KILL_ENEMY;
                            player[j].kill_enemy += 1;
                            deleteScoreBall(i);
                        }
                    }
                }
            }
        }
    }
}


void Ability(int id){
    switch(player[id].skill){
        
        case SKILL_ATTACK:
        if(player[id].rate_attack < MAX_ATTACK){
            player[id].rate_attack += 0.25;
        }
        break;

        case SKILL_HP:
        if(player[id].hp < 11.0){
            player[id].parm[PARM_HP] += 0.5;
        }
        break;

        case SKILL_SPEED:
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
        player[clientID].mp = 0.0f;
        player[clientID].isspecial = true;
        switch(player[clientID].special){
            case SPECIAL_BARRIER:
		    printf("create barrier\n");
             	player[clientID].isBarrier = (float)MAX_BARRIER;
                break;
            case SPECIAL_DISABLE:
                player[clientID].isSpecial = (float)MAX_DISABLE_TIME;
                break;
            case SPECIAL_BIGBULLET:
                player[clientID].isBigbullet = 4;
                break;
            case SPECIAL_LINES:
                player[clientID].isSpecial = (float)MAX_LINES_TIME;
                break;
            case SPECIAL_DAMAGEAREA:
                player[clientID].isSpecial = (float)MAX_DAMAGEAREA_TIME;
                break;
            case SPECIAL_TRANSFORM:
                do{
                    player[clientID].transformIndex = rand()%gClientNum;
                }while(player[clientID].transformIndex == clientID);
                player[clientID].isSpecial = (float)MAX_TRANSFORM_TIME;
                break;
	        case SPECIAL_GAMBLE:
                break;
            case SPECIAL_BEAM:
                player[clientID].isSpecial = (float)MAX_BEAM_TIME;
                break;
            case SPECIAL_CHASE:
                player[clientID].isChase = 4;
                break;
            default:
                break;
        }
    }
    else{
        printf("can not use special.\n");
    }
    
}


bool retExists(const char *f){
    bool isExist;
    FILE *fp;
    if ( (fp = fopen(f,"r")) != NULL ){
       
        isExist = true;
        fclose(fp);
    }
    else{
       
        isExist = false;
    }
    return isExist;
}

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


void WriteMatchFile(int value){
    FILE *fp; 
	char fname[] = "../data/judgeMatch.txt";
 
	fp = fopen(fname, "w"); 
	if(fp == NULL) {
		printf("%s file not open!\n", fname);
		exit (-1);
	} else {
		fprintf(fp,"%d",value);
	}
 
	fclose(fp); 
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
 
	fclose(fp); 
}

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
    
    for(int i=0; i<10; i++){
        ScoreBall s;
        glm::vec3 spawnPos = glm::vec3(rand()%((int)(WORLDSIZE_X/1.3))*((rand()%2)==1 ? 1: -1),rand()%((int)(WORLDSIZE_Y/1.3))*((rand()%2)==1 ? 1: -1),rand()%((int)(WORLDSIZE_Z/1.3))*((rand()%2)==1 ? 1: -1));
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

void PopEnemy(int PopEnemy_ID){
    if(scoreBallNum <= 20){createScoreBall();}
    glutTimerFunc(15000, PopEnemy, 0);
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
    int num;
    float tamax;
    float tamaz;
    float tamay;
    if(player[clientID].isSpecial <= 0.0f || player[clientID].special != SPECIAL_DISABLE){
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
                         ary_scoreBall[i].howMove = Move_tossinn;
                    }
                    else if(player[i].kill_player > 2){
                        ary_scoreBall[i].howMove = Move_henntai;
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
         glm::vec3 dir;
         glm::vec3 mokuhyou;
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
            
             dir = glm::normalize((cal_vec(player[num].pos , ary_scoreBall[i].pos)));
            ary_scoreBall[i].pos += dir / (1+ 2.0f);  
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
                 mokuhyou.x = x;
                mokuhyou.y = y;
                mokuhyou.z = z; 
            dir = glm::normalize((cal_vec(mokuhyou , ary_scoreBall[i].pos)));
            ary_scoreBall[i].pos += dir / (1+ 6.0f);  
                
                if(ary_scoreBall[i].pos.z+1 >= z&&ary_scoreBall[i].pos.z-1 <= z&& ary_scoreBall[i].pos.y+1 >= y&&ary_scoreBall[i].pos.y-1 <= y
                &&ary_scoreBall[i].pos.x + 1>=x && ary_scoreBall[i].pos.x - 1<=x ){
                    hit = true;
                }
                ary_scoreBall[i].howMove = Move_Stop;
                break;
            default:
                ary_scoreBall[i].howMove = Move_Stop;
                break;
        }
        ary_scoreBall[i].collider.pos = ary_scoreBall[i].pos;
    }
    }
}

void checkDeath(void){
    for(int i=0; i<gClientNum; i++){
        if(player[i].hp <= 0.0f){
	    player[i].hp = 0.0f;
        player[i].isspecial = false;
            player[i].enabled = false;
            if(i==clientID){
                player[clientID].anim = 200.0f;
                player[clientID].death += 1;
                player[clientID].score += SCORE_DEATH;
            }
        }
    }

}


void Respawn(void){
    glm::vec3 respawnPos = glm::vec3(((rand()%2)==1 ? 1: -1)*rand()%(WORLDSIZE_X-10),((rand()%2)==1 ? 1: -1)*rand()%(WORLDSIZE_Y-10),((rand()%2)==1 ? 1: -1)*rand()%(WORLDSIZE_Z-10));
    player[clientID].pos = respawnPos;
    printf("hukkatu\n");
    player[clientID].enabled = true;
    player[clientID].hp = MAX_HP;
    player[clientID].mp /= 5;
    player[clientID].turn1 = 0;
    player[clientID].turn2 = 0;
    player[clientID].turn3 = 0;
}


float cal_angle(glm::vec3 vec1, glm::vec3 vec2){
    vec1 = glm::normalize(vec1);
    vec2 = glm::normalize(vec2);
    float dot = glm::dot( vec1, vec2 ); // ???�????�????�????
    float angle = glm::acos(dot) * 180.0 / M_PI;
    return angle;
}

glm::vec3 cal_vec(glm::vec3 pos1, glm::vec3 pos2){
    glm::vec3 vec = pos1 - pos2;
    return vec;
}


int Target(int shooter){
    int id = NOTARGET;
    float min_value = 0.0f; 
    float angle[gClientNum] = {};
    float len[gClientNum] = {};
    glm::vec3 x[gClientNum] = {};
    glm::vec3 y[gClientNum] = {};

    for(int i = 0; i < gClientNum; i++){
        angle[i] = 90.0f;
        len[i] = 1000.0f;
        x[i] = {glm::vec3(0.0f,0.0f,0.0f)};
        y[i] = {glm::vec3(1000.0f,1000.0f,1000.0f)};
    }

    glm::vec3 dir_vec = {-sin(player[shooter].turn1)*cos(player[shooter].turn2), - sin(player[shooter].turn2), -cos(player[shooter].turn1)*cos(player[shooter].turn2)};
    for(int i = 0; i < gClientNum; i++){
        if(i != shooter || (player[i].isSpecial > 0 && player[i].special == SPECIAL_DISABLE)){
            x[i] = cal_vec(player[i].pos, player[shooter].pos);
            angle[i] = cal_angle(x[i], dir_vec); 
            if(angle[i] >= 0.0f && angle[i] <= 45.0f){
                y[i] = x[i];
                len[i] = glm::length(y[i]);
            }    
        }
    }
    min_value = len[0];
    if(len[0] != 1000.f){
        id = 0;
    }
    for (int j = 0; j < gClientNum; j++) {
        if(j != shooter || (player[j].isSpecial > 0 && player[j].special == SPECIAL_DISABLE)){   
            if (len[j] < min_value) {
                min_value = len[j];
                id = j;
            }
        }
    }

    return id;
}

void Circle2D(float radius,float x,float y)
{
 for (float th1 = 0.0;  th1 <= 720.0;  th1 = th1 + 1.0)
 {
             
  float th2 = th1 + 10.0;
  float th1_rad = th1 / 180.0 * M_PI; 
  float th2_rad = th2 / 180.0 * M_PI;

  float x1 = radius * cos(th1_rad);
  float y1 = radius * sin(th1_rad);
  float x2 = radius * cos(th2_rad);
  float y2 = radius * sin(th2_rad);

 
   glBegin(GL_TRIANGLES); 
   glVertex2f( x, y );
   glVertex2f( x1+x, y1+y );     
   glVertex2f( x2+x, y2+y );
  glEnd();
  
 }
}

void create_bullet(){

    BULLET b;
    b.dir = {-sin(player[clientID].turn1)*cos(player[clientID].turn2), - sin(player[clientID].turn2), -cos(player[clientID].turn1)*cos(player[clientID].turn2)};
    b.pos = player[clientID].pos + b.dir * 1.5f * player[clientID].size; 
    b.shooter_id = clientID;
    if(player[b.shooter_id].isBigbullet > 0){
        b.pos = player[b.shooter_id].pos + b.dir * 1.5f * 5.0f;
        b.type = SPECIAL_BIGBULLET;
        player[b.shooter_id].isBigbullet--;
        if(player[b.shooter_id].isBigbullet <= 0){
            player[b.shooter_id].isspecial = false;
        }
    }
    if(player[b.shooter_id].isChase > 0){
        b.target_id = Target(b.shooter_id);
        b.type = SPECIAL_CHASE;
        player[b.shooter_id].isChase--;
        if(player[b.shooter_id].isChase <= 0){
            player[b.shooter_id].isspecial = false;
        }
    }
    array_bullet.push_back(BULLET(b));
    bullet_Num++;
    SendBulletDataCommand(bullet_Num);
}

void draw_bullet(){
    for(int i = 0; i < bullet_Num; i++){
        
        glPushMatrix();
        if(player[array_bullet[i].shooter_id].rate_attack > 1.0){
            glColor3f(1.0, 2.0 - player[array_bullet[i].shooter_id].rate_attack, 2.0 - player[array_bullet[i].shooter_id].rate_attack);
        }
        else{
            glColor3f(1.0, 1.0, 1.0);
        }
        glTranslatef(array_bullet[i].pos.x, array_bullet[i].pos.y, array_bullet[i].pos.z);
        if(array_bullet[i].type == SPECIAL_BIGBULLET){
            glutSolidSphere(BIGBULLET_RADIUS, 16, 16);
        }
        else{
            glutSolidSphere(BULLET_RADIUS, 16, 16);
        }
        glPopMatrix();
    }
}

void move_bullet(){
    for(int i = 0; i < bullet_Num; i++){
        if(array_bullet[i].type == SPECIAL_CHASE && array_bullet[i].target_id != NOTARGET){
            array_bullet[i].dir = glm::normalize((cal_vec(player[array_bullet[i].target_id].pos , array_bullet[i].pos)));
            array_bullet[i].pos += array_bullet[i].dir/(player[array_bullet[i].target_id].speed + 1.0f);  
        }
        else{
            array_bullet[i].pos += array_bullet[i].dir * player[array_bullet[i].shooter_id].speed;
        }
    }
}

void del_bullet(){
    for(int i = 0; i < bullet_Num; i++){
        if(array_bullet[i].lifetime >= 5){
            array_bullet.erase(array_bullet.begin() + i);      
            bullet_Num--;
        }
    } 
}


    
