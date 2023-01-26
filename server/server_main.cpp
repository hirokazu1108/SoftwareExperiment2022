#include "server.h"

int	gClientNum;
Player *player;
Game game;
uiInfo gUi;

static Uint32 PlayTimer(Uint32 interval, void* param);
static Uint32 GameTimer(Uint32 interval, void* param);

int main(int argc,char *argv[])
{
	int	endFlag = 1;
  u_short port = PORT;
  game.state = State_Wait;

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

  /* SDL Init */
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    return PrintError(SDL_GetError());
  }
  /* UI Init */
  if (InitWindow() < 0) {
    PrintError("failed to initialize Windows");
    EndWindow();
    return 0;
  }

  /* Timer Init */
  SDL_TimerID ptimer = SDL_AddTimer(1000, PlayTimer, NULL);
  if (ptimer == 0) {
    PrintError(SDL_GetError());
    EndWindow();
    return 0;
  }

	/* Server Init */
	if(SetUpServer(gClientNum,port) == -1){
		fprintf(stderr,"Cannot setup server\n");
		exit(-1);
	}

  EndWindow(); // Quit Window
  SDL_RemoveTimer(ptimer);



  /* GameState Init */
  PlayerInit();
  game.state = State_Play;
  game.time = 0;
  
  /* Timer Init */
  SDL_TimerID qtimer = SDL_AddTimer(1000, GameTimer, NULL);
  if (qtimer == 0) {
    PrintError(SDL_GetError());
    EndWindow();
    return 0;
  }

	/* server loop */
	while(endFlag){
		endFlag = SendRecvManager();
	};

	Ending();
	return 0;
}

int PrintError(const char* str)
{
    fprintf(stderr, "%s\n", str);
    return -1;
}

void EndWindow(void){
  DestroyWindow();
  SDL_Quit();
}

// PlayTimer()
Uint32 PlayTimer(Uint32 interval, void* param)
{
  gUi.time_sec ++;
  if(gUi.time_sec >= 1000){
    gUi.time_sec = 999;
  }
  return interval;
}

// GameTimer()
Uint32 GameTimer(Uint32 interval, void* param)
{
    game.time++;
    if(game.time >= (int)GAMETIME&& game.state==State_Play){
      SendRankingDataCommand();
      game.state = State_Result;
    }
    return interval;
}
