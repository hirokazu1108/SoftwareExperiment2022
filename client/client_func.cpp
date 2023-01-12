#include "client.h"

void PlayerInit(void){
     /* Player???????????? */
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
        player[i].hp = (float)MAX_HP;
	    player[i].ability = UP_ATTACK;
        player[i].skill = SKILL_ATTACK;
        player[i].special = SPECIAL_BIGBULLET;
        for(int j=0; j<PARAMATER_NUM; j++)
            player[i].parm[j] = 0;
        player[i].reloadTime= 0;
        player[i].collider.radius = 1.0;
        player[i].collider.pos = player[i].pos;
    }

    /* ???????????��?��???????��?��????��?��?????読�?�込?????��????��?��????????????????? */
    ReadDataFile(&data);
    player[clientID].skill = data.skill;
    player[clientID].special = data.special;
    for(int i=0; i<PARAMATER_NUM; i++){
        player[clientID].parm[i] = data.parm[i];
    }
    player[clientID].attack += (float)player[clientID].parm[PARM_ATTACK] / 5.0f;
    player[clientID].speed += (float)player[clientID].parm[PARM_SPEED];
    player[clientID].size -= player[clientID].parm[PARM_SIZE];

}

void AudioInit(int *argc, char **argv){
    alutInit(argc,argv);

    alGenBuffers( 1, &buffer );
    alGenSources( 1, &source );

    alGenBuffers( 2, &buffer2 );
    alGenSources( 2, &source2 );

    // ����?�����������������?"hello world"������??�?������������??���?����
    buffer = alutCreateBufferHelloWorld ();
    buffer2 = alutCreateBufferFromFile( "BGM.wav" );

    alSourcei( source, AL_BUFFER, buffer );
    alSourcei( source2, AL_BUFFER, buffer2 );
    alSourcei(source2, AL_LOOPING, AL_TRUE );
    alSourcePlay( source2 );
}

/* ??????�????�?????????��?? */
bool OnColliderSphere(Sphere a, Sphere b){
  
  bool result = false;
  glm::vec3 diff = a.pos - b.pos;

  if(a.radius + b.radius > glm::length(diff)){
    result = true;
  }
  return result;
}

/* player???�?????????��??????????? */
void drawPlayerCollider(void){
    for(int i = 0; i < gClientNum; i++){
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(player[i].collider.pos.x, player[i].collider.pos.y, player[i].collider.pos.z);
        glutSolidSphere(player[i].collider.radius, 200, 200);
        glPopMatrix();
    }
}

/* ???????????��??�???? */
void deleteBullet(int index){
    array_bullet.erase(array_bullet.begin() + index);       //  i??????????????????????????
    bullet_Num--;
}


/* �?�????�?????????��??????????? */
void Collider(void){
    for(int i=0; i<gClientNum; i++){
        for(int j=0; j<bullet_Num;j++){
            if(OnColliderSphere(Sphere(BULLET_RADIUS,array_bullet[j].pos),player[i].collider)){
                Ability(array_bullet[j].shooter_id);
                printf("speed:%f\n" ,player[array_bullet[j].shooter_id].speed);
                player[i].hp -= player[array_bullet[j].shooter_id].attack * player[array_bullet[j].shooter_id].rate_attack / player[i].parm[PARM_HP];
                deleteBullet(j);
                printf("hirokazu: hit client[%d]\n",i);
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

/* ?????��?��?????�????????????????�???? */
bool retExists(const char *f){
    bool isExist;
    FILE *fp;
    if ( (fp = fopen(f,"r")) != NULL ){
        // ?????��?��?????�??????????
        isExist = true;
        fclose(fp);
    }
    else{
        // ?????��?��?????�?????????????
        isExist = false;
    }
    return isExist;
}
/* ??��?��???????��?��????��?��???????��??込�?? */
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
/* ??��?��???????��?��????��?��?????読�?�込??? */
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

/* ???????????��?��??使�???????? */
// 0:???????????��?�中
// 1:???????????��?��??�?
// -1:???????????��?��??�?
void WriteMatchFile(int value){
    FILE *fp; // FILE???�????�?
	char fname[] = "../data/judgeMatch.txt";
 
	fp = fopen(fname, "w"); // ?????��?��??????????????失�???????????NULL???�???????
	if(fp == NULL) {
		printf("%s file not open!\n", fname);
		exit (-1);
	} else {
		fprintf(fp,"%d",value);
	}
 
	fclose(fp); // ?????��?��??????????????
}

/* ?????��?��?��?��?��?????使�???????? */
// �???????�???��??
void WriteRankingFile(void){
    FILE *fp; // FILE???�????�?
	char fname[] = "../data/ranking.txt";
 
	fp = fopen(fname, "w"); // ?????��?��??????????????失�???????????NULL???�???????
	if(fp == NULL) {
		printf("%s file not open!\n", fname);
		exit (-1);
	} else {
        for(int i=0; i<gClientNum; i++)
		    fprintf(fp,"%s\n",game.clientName[game.ranking[i]]);
	}
 
	fclose(fp); // ?????��?��??????????????
}

/* ????????��?��?��???????��?��????????�?�????????????��?��?��?��?��?��??�???��???????? */
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
