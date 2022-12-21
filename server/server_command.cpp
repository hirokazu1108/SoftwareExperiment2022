#include "server.h"
#include <netinet/in.h>
static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);

int readNum = 0; //鐃宿み刻申鐃緒申鐃緒申鐃銃?鐃緒申
/*****************************************************************
????	: ExecuteCommand
???	: ??????????????????????????????
		  ??????????????????????
????	: char	command		: ???????
		  int	pos			: ??????????????????????????
????	: ?????????????????????????????????0???????
		  ?????????1?????
*****************************************************************/
int ExecuteCommand(char command,int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize,intData;
    int			endFlag = 1;


    switch(command){

        case PLAYERDATA_COMMAND:{
            dataSize = 0;
            Player *p = (Player*)malloc(sizeof(Player));
            RecvData(pos, p, sizeof(Player)); //Player鐃塾常申鐃緒申鐃緒申鐃緒申鐃緒申鐃?
            player[pos] = *p; //Player鐃塾常申鐃緒申鐃緒申納
            free(p);
        }
            
            readNum |= (1 << pos); //鐃緒申鐃殉っわ申鐃緒申鐃初イ鐃緒申鐃緒申鐃緒申峭鐃緒申判鐃緒申 1101鐃緒申client[2]鐃淑鰹申鐃緒申鐃殉ってわ申鐃緒申
            if(readNum == (15>>(4-gClientNum))){
			    SendData(ALL_CLIENTS,&command,sizeof(char));
                SendData(ALL_CLIENTS,player,sizeof(Player)*gClientNum); //Player鐃緒申鐃緒申鐃緒申鐃緒申鐃緒申鐃緒申鐃緒申鐃?

                readNum = 0;
            }
            
            break;
        
        case BULLETDATA_COMMAND:{
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

        case RANKING_DATA:
            game.ranking.push_back(pos); //ランキングにクライアント番号を追加
            
            /*ここにデータが集まったらリザルトへの移動をするように送る処理*/
            if(game.ranking.size() == gClientNum-1){
                
                /* 1位のクライアントをここで追加 */
                for(int cnum=0; cnum<gClientNum; cnum++){
                    auto itr = std::find(game.ranking.begin(),game.ranking.end(), cnum);
                    
                    if(itr == game.ranking.end()){
                        //cnumがみつからなかったら
                        game.ranking.push_back(cnum);
                        break;
                    }
                }
                /* ランキング順になるように逆順にする */
                std::reverse(game.ranking.begin(),game.ranking.end());

               

                int *p = (int*) malloc(sizeof(int)*gClientNum);
                for(int i=0; i<gClientNum; i++){
                    p[i] = game.ranking[i];
                }
                
                SendData(ALL_CLIENTS,&command,sizeof(char));
                SendData(ALL_CLIENTS,p,sizeof(int)*gClientNum);

                free(p);
            }
            break;

	    case END_COMMAND:
			dataSize = 0;
			/* ??????????????? */
			SetCharData2DataBlock(data,command,&dataSize);

			/* ???????????????? */
			SendData(ALL_CLIENTS,data,dataSize);

			endFlag = 1;
			break;
	  default:
			/* ?????????????????????? */
			fprintf(stderr,"0x%02x is not command!\n",command);
    }
    return endFlag;
}

/*****
static
*****/
/*****************************************************************
????	: SetIntData2DataBlock
???	: int ?????????????????????????????????????
????	: void		*data		: ??????????
		  int		intData		: ???????????????
		  int		*dataSize	: ???????????????????????
????	: ???
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;

    /* ??????????????? */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp = htonl(intData);

    /* int ???????????????????????????????????? */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* ????????????????? */
    (*dataSize) += sizeof(int);
}

/*****************************************************************
????	: SetCharData2DataBlock
???	: char ?????????????????????????????????????
????	: void		*data		: ??????????
		  int		intData		: ???????????????
		  int		*dataSize	: ???????????????????????
????	: ???
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* ??????????????? */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* int ???????????????????????????????????? */
    *(char *)(data + (*dataSize)) = charData;
    /* ????????????????? */
    (*dataSize) += sizeof(char);
}

