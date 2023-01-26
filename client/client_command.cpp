#include "client.h"
#include <netinet/in.h>

static void SetCharData2DataBlock(void *data,char charData,int *dataSize);

bool isRanking = false;

// ExecuteCommand()
int ExecuteCommand(char command)
{
    int	endFlag = 1;

    switch(command){
        case PLAYERDATA_COMMAND:{
            Player *p = (Player*)malloc(sizeof(Player)*gClientNum);
            RecvData(p,sizeof(Player)*gClientNum);
            for(int i = 0; i <gClientNum;i++)
            {
                if(i != clientID)
                    player[i] = p[i];
            }
            firstRecvPlayerInfo = true;
            free(p);
        }
            break;
		
        case BULLETDATA_COMMAND:{
            BULLET b;
            RecvData(&b, sizeof(BULLET));
            array_bullet.push_back(BULLET(b));
            bullet_Num++;   
        }
        break;
        case RANKING_DATA:
            {
                // ????????ゃ?ゃ?笺???????版?????
                Player *p = (Player*)malloc(sizeof(Player)*gClientNum);
                RecvData(p,sizeof(Player)*gClientNum);
                //??肩??????????????
                for(int i = 0; i <gClientNum;i++)
                {
                    player[i] = p[i];
                }
                free(p);

                RankingData data;
                for(int i=0; i<gClientNum; i++){
                    sprintf(data.clientName[i],"%s",game.clientName[i]);
                    data.score[i] = player[i].score;
                    data.kill_player[i] = player[i].kill_player;
                    data.death[i] = player[i].death;
                    data.kill_enemy[i] = player[i].kill_enemy;
                    data.kill_boss[i] = player[i].kill_boss;
                }

                WriteRankingFile(gClientNum,&data);
                isRanking = true;
                SendEndCommand();
                endFlag = 0;
            }
            break;
        case SCOREBALL_COMMAND:
            {
                ScoreBall s;
                RecvData(&s, sizeof(ScoreBall));
                ary_scoreBall.push_back(ScoreBall(s));
                scoreBallNum++;
                printf("scoreBall Data was recieved.\n");
            }
            break;
        case PLAYERINFO_COMMAND:
            RecvData(&player[clientID],sizeof(Player));
            break;
        case END_COMMAND:
            if(isRanking){
                endFlag = 0;
                printf("move ranking.\n");
            }
            else{
                endFlag = -1;
                printf("END selected.\n");
            }
			break;
        default:
            break;
    }

    return endFlag;
}

// SendEndCommand()
void SendEndCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data,END_COMMAND,&dataSize);
    SendData(data,dataSize);
}

void SendPlayerDataCommand(void){

    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data,PLAYERDATA_COMMAND,&dataSize);
    SendData(data,dataSize);

    SendData(&(player[clientID]),sizeof(Player));
}

// SendBulletDataCommand
void SendBulletDataCommand(int num){

    char com = BULLETDATA_COMMAND;

    SendData(&com,sizeof(char));
    SendData(&array_bullet[num],sizeof(BULLET));
}

void SendScoreBallDataCommand(void){
    char com = SCOREBALL_COMMAND;
    SendData(&com,sizeof(char));
    SendData(&ary_scoreBall[scoreBallNum-1],sizeof(ScoreBall));
}


//SendPlayerInfoData()
void SendPlayerInfoData(int clientIndex){
    char com = PLAYERINFO_COMMAND;
    SendData(&com,sizeof(char));
    SendData(&clientIndex,sizeof(int));
}

// SetCharData2DataBlock()
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    assert(data!=NULL);
    assert(0<=(*dataSize));
    *(char *)(data + (*dataSize)) = charData;
    (*dataSize) += sizeof(char);
}

