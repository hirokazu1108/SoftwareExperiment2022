#ifndef _CLIENT_FUNC_H_
#define _CLIENT_FUNC_H_

#include"../constants.h"
#include <AL/alut.h>
#include <joyconlib.h>

#define WINDOW_PosX 0
#define WINDOW_PosY 0
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

#define WORLDSIZE_X 1000
#define WORLDSIZE_Y 500
#define WORLDSIZE_Z 1000

#define Model_Num 7

/* bar struct */
class Bar{
    public:
    float x1;
    float y1;
    float x2;
    float y2;
    float w;
    Bar(float a,float b, float c, float d){x1=a;y1=b;x2=c;y2=d;w=c-a;}
};

/* client_net.cpp */
extern int SetUpClient(char *hostName,u_short port,int *clientID,int *num,char clientNames[][NAME_MAX_LENGTH+1]);
extern void CloseSoc(void);
extern int RecvIntData(int *intData);
extern void SendData(void *data,int dataSize);
extern int SendRecvManager(void);
extern int RecvData(void *data,int dataSize);


/* client_win.cpp */
extern int InitWindows(int clientID,int num,char name[][NAME_MAX_LENGTH+1]);
extern void DestroyWindow(void);
extern void WindowEvent(int num);
extern void uiSetting(void);
extern void DrawString(std::string str,int x0, int y0, void *font=GLUT_BITMAP_TIMES_ROMAN_24);

/* client_command.cpp */
extern int ExecuteCommand(char command);
extern void SendEndCommand(void);
extern void SendPlayerDataCommand(void);
extern void SendBulletDataCommand(int num);

/* client_func.cpp */
extern void PlayerInit(void);
extern void AudioInit(int *argc, char **argv);
extern bool OnColliderSphere(Sphere a, Sphere b);
extern void drawPlayerCollider(void);
extern void Collider(void);
extern void Ability(int id);
extern void useSpecial(void);
extern bool retExists(const char *file);
extern void WriteDataFile(SaveData *data);
extern void ReadDataFile(SaveData *data);
extern void WriteMatchFile(int value);
extern void WriteRankingFile(void);
extern void ExitClientProgram(int mode);

extern int		gClientNum;
extern int		clientID;
extern int bullet_Num;   // 発射された弾の個数
extern Player *player;
extern Game game;
extern std::vector<BULLET> array_bullet;

extern ALuint buffer;
extern ALuint source;
extern ALuint buffer2;
extern ALuint source2;
#endif

