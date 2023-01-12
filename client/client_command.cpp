#include "client.h"
#include <netinet/in.h>
/*****************************************************************
ファイル名	: client_command.c
機能		: クライアントのコマンド処理
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);

/*****************************************************************
関数名	: ExecuteCommand
機能	: サーバーから送られてきたコマンドを元に，
		  引き数を受信し，実行する
引数	: char	command		: コマンド
出力	: プログラム終了コマンドがおくられてきた時には0を返す．
		  それ以外は1を返す
*****************************************************************/
int ExecuteCommand(char command)
{
    int	endFlag = 1;

    switch(command){
        case PLAYERDATA_COMMAND:{
            Player *p = (Player*)malloc(sizeof(Player)*gClientNum);
            RecvData(p,sizeof(Player)*gClientNum);
            //格納する処理
            for(int i = 0; i <gClientNum;i++)
            {
                if(i != clientID)
                    player[i] = p[i];
            }
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
        case RANKING_DATA:{
            int *p = (int*) malloc(sizeof(int)*gClientNum);
            RecvData(p,sizeof(int)*gClientNum);
            for(int i=0; i<gClientNum; i++){
                game.ranking.push_back(p[i]);
            }
            free(p);
            
            for(int i=0;i<gClientNum;i++)
                printf("No.%d:%s\n",i+1,game.clientName[game.ranking[i]]);
            }
	    //ゲームをリザルトに切り替える
            WriteRankingFile();
            endFlag = 0;
            break;
        case END_COMMAND:
            endFlag = -1;
            printf("END SELECTED.\n");
			break;
        default:
            break;
    }

    return endFlag;
}

/*****************************************************************
関数名	: SendEndCommand
機能	: プログラムの終了を知らせるために，
		  サーバーにデータを送る
引数	: なし
出力	: なし
*****************************************************************/
void SendEndCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data,END_COMMAND,&dataSize);

    /* データの送信 */
    SendData(data,dataSize);
}

void SendPlayerDataCommand(void){

    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data,PLAYERDATA_COMMAND,&dataSize);
    /* データの送信 */
    SendData(data,dataSize);

    //playerDataの送信

    SendData(&(player[clientID]),sizeof(Player));

}

void SendBulletDataCommand(int num){

    char com = BULLETDATA_COMMAND;

    /* データの送信 */
    SendData(&com,sizeof(char));

    //clientDataの送信

    
    SendData(&array_bullet[num],sizeof(BULLET));

}

/*****
static
*****/
/*****************************************************************
関数名	: SetIntData2DataBlock
機能	: int 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;

    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp = htonl(intData);

    /* int 型のデータを送信用データの最後にコピーする */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* データサイズを増やす */
    (*dataSize) += sizeof(int);
}

/*****************************************************************
関数名	: SetCharData2DataBlock
機能	: char 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* char 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}

