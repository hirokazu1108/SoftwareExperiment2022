#include "server.h"

int	gClientNum; //クライアント数
Player *player;
Game game;

uiInfo gUi;
static Uint32 PlayTimer(Uint32 interval, void* param);

int main(int argc,char *argv[])
{

	int	endFlag = 1;
  u_short port = PORT;

	/* 引き数チェック */
  switch (argc) {
  case 1:
	fprintf(stderr,"Usage: number of clients\n");
	return 1;
    break;
  case 2:
    if((gClientNum = atoi(argv[1])) < 0 ||  gClientNum > MAX_CLIENTS){
		fprintf(stderr,"clients limit = %d \n",MAX_CLIENTS);
		exit(-1);
	}
	break;
  case 3:
    if((gClientNum = atoi(argv[1])) < 0 ||  gClientNum > MAX_CLIENTS){
		fprintf(stderr,"clients limit = %d \n",MAX_CLIENTS);
		exit(-1);
	}
    port = atoi(argv[2]);
    strcpy(gUi.port,argv[2]);
    break;
  default:
    fprintf(stderr, "Usage: %s [number of clients] [port number]\n", argv[0]);
    return 1;
  }


  /** 初期化処理 **/
    /* SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        return PrintError(SDL_GetError());
    }
    /** UI初期化 **/
    if (InitWindow() < 0) {
        PrintError("failed to initialize Windows");
        EndWindow();
        return 0;
    }

  //タイマー起動
  SDL_TimerID ptimer = SDL_AddTimer(1000, PlayTimer, NULL);
  if (ptimer == 0) {
    PrintError(SDL_GetError());
    EndWindow();
    return 0;
  }

	/* クライアントとの接続 */
	if(SetUpServer(gClientNum,port) == -1){
		fprintf(stderr,"Cannot setup server\n");
		exit(-1);
	}
  EndWindow();

  PlayerInit();

  /* ゲーム情報の初期化 */
  game.state = State_Play;

	/* メインイベントループ */
	while(endFlag){
		endFlag = SendRecvManager();
	};

	/* 終了処理 */
	Ending();

	return 0;
}

int PrintError(const char* str)
{
    fprintf(stderr, "%s\n", str);
    return -1;
}

void EndWindow(void){
    /** 終了処理 **/

    //SDL_RemoveTimer(atimer);
    DestroyWindow();
    SDL_Quit();

}


//タイマー処理(アニメーションの更新)
Uint32 PlayTimer(Uint32 interval, void* param)
{
  gUi.time_sec ++;
  if(gUi.time_sec >= 1000){
    gUi.time_sec = 999;
  }
  return interval;
}



	/* 終了処理 */
	Ending();

	return 0;
}
