#ifndef _CLIENT_FUNC_H_
#define _CLIENT_FUNC_H_

#include"../constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define ALL_CLIENTS	-1
#define WD_Height 1000
#define WD_Width 1000
#define IMG_NUM 2
#define TEXT_NUM 12
#define SCENE_NUM 1

typedef enum{
    uname_title_sky,
    uname_title_sky2,
}uiName;


typedef enum{
    tname_WaitClient,
	tname_Passcode,
    tname_0,
    tname_1,
    tname_2,
    tname_3,
    tname_4,
    tname_5,
    tname_6,
    tname_7,
    tname_8,
    tname_9,
}textName;


typedef struct {
    public:
    int w;
    int h;         
    SDL_Texture* texture;
    void drawTexture(int x, int y, int width=0,int height=0);
    
}ImgInfo;

class uiInfo{
	public:
	SDL_Window *window;
    SDL_Renderer *render;
    int selectButton;
	int currentNum;
	char port[6];
	int time_sec;
};

typedef struct{
	int		fd;
	char	name[NAME_MAX_LENGTH+1];
}CLIENT;

/* server_main.cpp */
extern int	gClientNum;
extern Player *player;
extern Game game;
extern uiInfo gUi;
extern int PrintError(const char* str);
extern void EndWindow(void);

/* window.cpp */
extern int InitWindow(void);
extern void DestroyWindow(void);
extern void RenderWaitClientWindow(void);
extern std::vector<SDL_Rect> buttonPos[SCENE_NUM];

/* server_func.cpp */
extern int maxButtonNum[SCENE_NUM];

/* server_net.cpp */
extern int SetUpServer(int num, u_short port);
extern void Ending(void);
extern void SendData(int pos,void *data,int dataSize);
extern int RecvData(int pos,void *data,int dataSize);
extern int SendRecvManager(void);

extern CLIENT gClients[MAX_CLIENTS];

/* server_command.cpp */
extern int ExecuteCommand(char command,int pos);
extern void SendTimeCommand(void);
extern void SendRankingDataCommand(void);

/* server_func.cpp */
extern void PlayerInit(void);

#endif
