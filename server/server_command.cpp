#include "server.h"
#include <netinet/in.h>
static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);

int readNum = 0; 

// ExecuteCommand()
int ExecuteCommand(char command,int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize,intData;
    int			endFlag = 1;


    switch(command){

        case PLAYERDATA_COMMAND:
            {
                Player *p = (Player*)malloc(sizeof(Player));
                RecvData(pos, p, sizeof(Player));
                player[pos] = *p;
                free(p);
            }
            // through until all player's data collected perfectly.
            readNum |= (1 << pos);
            if(readNum == (15>>(4-gClientNum))){
			    SendData(ALL_CLIENTS,&command,sizeof(char));
                SendData(ALL_CLIENTS,player,sizeof(Player)*gClientNum);

                readNum = 0;
            }
            break;
        case BULLETDATA_COMMAND:
            {
                BULLET b;
                RecvData(pos, &b, sizeof(BULLET));
                for(int i = 0; i < gClientNum; i++){
                    if(i != pos){
                        SendData(i,&command,sizeof(char));
                    }
                } 
                for(int i = 0; i < gClientNum; i++){
                    if(i != pos){
                        SendData(i, &b, sizeof(BULLET));
                    }
                }
            }
            break;
        case SCOREBALL_COMMAND:
            {
                ScoreBall s;
                RecvData(pos, &s, sizeof(ScoreBall));
                for(int i = 0; i < gClientNum; i++){
                    if(i != pos){
                        SendData(i,&command,sizeof(char));
                    }
                }
                for(int i = 0; i < gClientNum; i++){
                    if(i != pos){
                        SendData(i, &s, sizeof(ScoreBall));
                    }
                }
            }
            break;
	case PLAYERINFO_COMMAND:
            {
                int mode, cindex;
                RecvData(pos, &mode, sizeof(int));
                RecvData(pos, &cindex,sizeof(int));
                switch(mode){
                    case 0: //kill_player
                        player[cindex].kill_player += 1;
                        player[cindex].score += SCORE_KILL_PLAYER;
                    break;
                }
                SendData(cindex, &command,sizeof(char));
                SendData(cindex, &player[cindex],sizeof(Player));
            }
            break;
	 case END_COMMAND:
			dataSize = 0;
			SetCharData2DataBlock(data,command,&dataSize); //command set
			SendData(ALL_CLIENTS,data,dataSize);
			endFlag = 0;
			break;
	  default:
			fprintf(stderr,"0x%02x is not command!\n",command);
    }
    return endFlag;
}

// SendTimeCommand()
void SendTimeCommand(void){
    char com = TIMER_COMMAND;
    SendData(ALL_CLIENTS,&com,sizeof(char));
    SendData(ALL_CLIENTS,&game.time,sizeof(unsigned int));
}

// SendRankingDataCommand()
void SendRankingDataCommand(void){
    char com = RANKING_DATA;
    SendData(ALL_CLIENTS,&com,sizeof(char));
    SendData(ALL_CLIENTS,player,sizeof(Player)*gClientNum);
}

// SetIntData2DataBlock()
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;

    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp = htonl(intData);

    memcpy((int*)data + (*dataSize),&tmp,sizeof(int));
    (*dataSize) += sizeof(int);
}

// SetCharData2DataBlock()
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    assert(data!=NULL);
    assert(0<=(*dataSize));

    *(char *)((char*)data + (*dataSize)) = charData;
    (*dataSize) += sizeof(char);
}
