#ifndef _CLIENT_FUNC_H_
#define _CLIENT_FUNC_H_

#include"../constants.h"
#include <AL/alut.h>
#include <joyconlib.h>

#define WINDOW_PosX 0
#define WINDOW_PosY 0
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

#define WORLDSIZE_X 250
#define WORLDSIZE_Y 250
#define WORLDSIZE_Z 250

#define Model_Num 9

/* 10 button Joystick */
#define GLUT_JOYSTICK_BUTTON_E  0x10    /* 5 */
#define GLUT_JOYSTICK_BUTTON_F  0x20    /* 6 */
#define GLUT_JOYSTICK_BUTTON_G  0x40    /* 7 */
#define GLUT_JOYSTICK_BUTTON_H  0x80    /* 8 */
#define GLUT_JOYSTICK_BUTTON_I  0x100   /* 9 */
#define GLUT_JOYSTICK_BUTTON_J  0x200   /* 10 */
/* 10 button Joystick */

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
extern void drawScoreBall(void);

/* client_command.cpp */
extern int ExecuteCommand(char command);
extern void SendEndCommand(void);
extern void SendPlayerDataCommand(void);
extern void SendBulletDataCommand(int num);
extern void SendScoreBallDataCommand(void);
extern void SendPlayerInfoData(int clientIndex);

/* client_func.cpp */
extern void PlayerInit(void);
extern void AudioInit(int *argc, char **argv);
extern bool OnColliderSphere(Sphere a, Sphere b);
extern bool OnColliderLinesSphere(Sphere a, glm::vec3 sp, glm::vec3 ep);
extern void drawPlayerCollider(void);
extern void Collider(void);
extern void Ability(int id);
extern void useSpecial(void);
extern bool retExists(const char *file);
extern void WriteDataFile(SaveData *data);
extern void ReadDataFile(SaveData *data);
extern void WriteMatchFile(int value);
extern void WriteRankingFile(int num, RankingData *data);
extern void ExitClientProgram(int mode);
extern void createScoreBall(void);
void PopEnemy(int PopEnemy_ID);
extern void deleteScoreBall(int index);
extern void moveScoreBall(void);
extern void checkDeath(void);
extern void Respawn(void);
extern float cal_angle(glm::vec3 vec1, glm::vec3 vec2);
extern glm::vec3 cal_vec(glm::vec3 pos1, glm::vec3 pos2);
extern int Target(int shooter);
extern void Circle2D(float radius,float x,float y);
extern void create_bullet(void);    
extern void draw_bullet(void); 
extern void move_bullet(void); 
extern void del_bullet(void);

extern int		gClientNum;
extern int		clientID;
extern Player *player;
extern Game game;
extern std::vector<ScoreBall> ary_scoreBall;
extern int scoreBallNum;
extern bool firstRecvPlayerInfo;
extern std::vector<BULLET> array_bullet;
extern int bullet_Num;   // 発射された弾の個数
extern GLuint  model_list[Model_Num];		/* display list for object */
extern int bullet_Num;
extern bool can_attack; 

extern ALuint buffer;
extern ALuint source;
extern ALuint buffer2;
extern ALuint source2;
#endif

