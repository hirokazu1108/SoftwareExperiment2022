#include "server.h"

int	gClientNum; //????????¤ã?¢ã?³ã?????
Player *player;
Game game;

uiInfo gUi;
static Uint32 PlayTimer(Uint32 interval, void* param);

int main(int argc,char *argv[])
{

	int	endFlag = 1;
  u_short port = PORT;

	/* å¼??????°ã????§ã????? */
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


  /** ??????????????? **/
    /* SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        return PrintError(SDL_GetError());
    }
    /** UI????????? **/
    if (InitWindow() < 0) {
        PrintError("failed to initialize Windows");
        EndWindow();
        return 0;
    }

  //??¿ã?¤ã????¼èµ·???
  SDL_TimerID ptimer = SDL_AddTimer(1000, PlayTimer, NULL);
  if (ptimer == 0) {
    PrintError(SDL_GetError());
    EndWindow();
    return 0;
  }

	/* ????????¤ã?¢ã?³ã?????????Ž¥ç¶? */
	if(SetUpServer(gClientNum,port) == -1){
		fprintf(stderr,"Cannot setup server\n");
		exit(-1);
	}
  EndWindow();

  PlayerInit();

  /* ??²ã?¼ã???????±ã??????????? */
  game.state = State_Play;

	/* ??¡ã?¤ã?³ã?¤ã????³ã???????¼ã?? */
	while(endFlag){
		endFlag = SendRecvManager();
    printf("ed:%d\n",endFlag);
	};

	/* çµ?äº??????? */
	Ending();

	return 0;
}

int PrintError(const char* str)
{
    fprintf(stderr, "%s\n", str);
    return -1;
}

void EndWindow(void){
    /** çµ?äº??????? **/

    //SDL_RemoveTimer(atimer);
    DestroyWindow();
    SDL_Quit();

}


//??¿ã?¤ã????¼å?????(??¢ã????¡ã?¼ã?·ã?§ã?³ã????´æ??)
Uint32 PlayTimer(Uint32 interval, void* param)
{
  gUi.time_sec ++;
  if(gUi.time_sec >= 1000){
    gUi.time_sec = 999;
  }
  return interval;
}
