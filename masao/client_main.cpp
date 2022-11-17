/* --------------------------------------------------------------------
 * gls04.c
 * 2個の立方体を別々に動かす
 * -------------------------------------------------------------------- */

/* ヘッダファイル */
#include "client.h"
int		gClientNum;
int		clientID;
Player *player;
int sendDataFlag = 0;

#define RAD (M_PI / 180.0)
#define MAX_BULLET_NUM   100    // 弾の最大数
#define BULLET_SPEED   0.5f     // 弾の速度
typedef struct {
    float ShiftX;
    float ShiftY;
    float ShiftZ;
    float RotateX;
    float RotateY;
    float RotateZ;
} Geometry;
Geometry Cube;

BULLET bullet;
class BULLET array_bullet[MAX_BULLET_NUM];

/* グローバル変数 */
int xBegin            = 0;    /* マウスドラッグの始点X座標 */
int yBegin            = 0;    /* マウスドラッグの始点Y座標 */
int PressButton       = 0;    /* 現在押されているマウスボタン（1:左,2:中,3:右） */
float CameraAzimuth   = 90.0; /* カメラの位置（方位角） */
float CameraElevation = 0.0;  /* カメラの位置（仰角） */
float CameraDistance  = 5.0;  /* カメラの位置（原点からの距離） */
float CameraX         = 0;  /* カメラの位置（X座標） */
float CameraY         = 1.0;  /* カメラの位置（Y座標） */
float CameraZ         = 0.0;  /* カメラの位置（Z座標） */
float BoxX[3];                /* 箱のX座標 */
float BoxVx[3];               /* 箱のX方向移動速度 */
float BoxY[3];
float BoxZ[3];
char left  = 0;
char right = 0;
char buf[256];
int kasoku;
float BoxRotate = 10.0;
int flag        = 106;
int j           = 0; // 2のはやさ
int f           = 0; //３のはやさ
int ps          = 0;
double junp;
int junpf = 0;
int bullet_Num=0;   // 発射された弾の個数

int bf;
int jnk; //ジョイコンの入力コマンド
int r=0;
int rb;
int af;
joyconlib_t jc;

/* 関数のプロトタイプ宣言 */

void display(void);
void timer(int timerID);
void keyboard(unsigned char key, int x, int y);
void resize(int w, int h);
void myInit(char *windowTitle);
void drawString3D(const char *str, float charSize, float lineWidth);
void init(void);
int joyconev();
void create_bullet(int num);    // 弾の生成
void draw_bullet(int num);  // 弾の描画
void move_bullet(int num);    // 弾丸の動き
void add_lifetime(int add_lifetimeID);  // 弾のライフタイムを加算
void del_bullet(void);  // 弾の消去
#define TEX_HEIGHT 32
#define TEX_WIDTH 32
static GLubyte image[TEX_HEIGHT][TEX_WIDTH][4];

/***********************************************************
|  関数：main()
|  説明：メイン関数
|  引数：int argc       実行時引数の数
|  引数：char** argv    実行時引数の内容（文字列配列）
|  戻値：int            0:正常終了
***********************************************************/
int main(int argc, char **argv)
{
    u_short port = PORT;
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    int		endFlag=1;
    char	serverName[MAX_NAME_SIZE];
    
    sprintf(serverName, "localhost");

    /* 引き数チェック */
  switch (argc) {
  case 1:
    break;
  case 2:
    sprintf(serverName, "%s", argv[1]);
    break;
  case 3:
    sprintf(serverName, "%s", argv[1]);
    port = (u_short)atoi(argv[2]);
    break;
  default:
    fprintf(stderr, "Usage: %s [server name] [port number]\n", argv[0]);
    return 1;
  }

    /* サーバーとの接続 */
    if(SetUpClient(serverName,port,&clientID,&gClientNum,name)==-1){
		fprintf(stderr,"setup failed : SetUpClient\n");
		return -1;
	}

	player = (Player*)malloc(sizeof(Player)*gClientNum);
    for(int i = 0; i< gClientNum;i++)
    {
        player[i].spead = 0.0;
        player[i].dir.x = 0;
        player[i].dir.y = 0;
        player[i].dir.z = 0;
        player[i].pos.x = 0;
        player[i].pos.y = 0;
        player[i].pos.z = 0;
        player[i].upVec.x = 0;
        player[i].upVec.y = 0;
        player[i].upVec.z = 0;
        player[i].turn1 = 0;
        player[i].turn2 = 0;
        player[i].turn3 = 0;
        player[i].type = 0;
        player[i].mp = 0;
        player[i].hp = 0;
        player[i].reloadTime= 0;
    }
    

    
    /* 初期化 */
    glutInit(&argc, argv); /* OpenGL の初期化 */
    myInit(argv[0]);       /* ウインドウ表示と描画設定の初期化 */
    
    glutMainLoop();

    // glutReshapeFunc(reshape);
    /* イベント処理ループ */

    /* プログラム終了 */
    return (0);
}


/**********************************************************
|  関数：display()
|  説明：「１枚の」グラフィック描画イベント処理
|  引数：なし
|  戻値：なし
***********************************************************/
void display(void)
{
    int i;

    if(flag ==0){
        CameraX = CameraX;
        CameraY = CameraY;
        CameraZ = CameraZ;
       //CameraX = player[clientID].pos.x +sin(player[clientID].turn1*0.1f)*5 ;
        //CameraY = player[clientID].pos.y + sin(player[clientID].turn2*0.1f)*5;
        //CameraZ = player[clientID].pos.z+cos(player[clientID].turn1*0.1f)*5;
        CameraX = player[clientID].pos.x /*+sin(player[clientID].turn1*0.1f)*5*/  +sin(player[clientID].turn1)*5 *cos(player[clientID].turn3);
        CameraY = player[clientID].pos.y + sin(player[clientID].turn2)*5;
        CameraZ = player[clientID].pos.z /*+cos(player[clientID].turn1*0.1f)*5*/ +cos(player[clientID].turn1)*5 *cos(player[clientID].turn3);
        
        
        }
        else{
        CameraX = CameraX;
        CameraY = CameraY;
        CameraZ = CameraZ;

        CameraX = player[clientID].pos.x /*+sin(player[clientID].turn1*0.1f)*5 */ + sin(player[clientID].turn1)*5 *cos(player[clientID].turn3);
        CameraY = player[clientID].pos.y + sin(player[clientID].turn2)*5;
        CameraZ = player[clientID].pos.z /*+cos(player[clientID].turn1*0.1f)*5 */+cos(player[clientID].turn1)*5 *cos(player[clientID].turn3);
        /*CameraX = player[clientID].pos.x +sin(player[clientID].turn1*0.1f)*5 *cos(player[clientID].turn3*0.1f);
        CameraY = player[clientID].pos.y + sin(player[clientID].turn2*0.1f)*5;
        CameraZ = player[clientID].pos.z +cos(player[clientID].turn1*0.1f)*5 *sin(player[clientID].turn3*0.1f);*/
    }
    
    /* 初期化 */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* 画面を消去 */
    glMatrixMode(GL_MODELVIEW);                         /* 幾何（描画位置など設定する）モード */
    glLoadIdentity();                                   /* 幾何を初期化する */

    /* 視点の設定 */
    // glMatrixMode(GL_PROJECTION);
    
    gluLookAt(CameraX, CameraY, CameraZ, /* カメラの位置 */
        player[clientID].pos.x, player[clientID].pos.y,player[clientID].pos.z,        /* 注視点の位置 */
        0, 0.5*cos(player[clientID].turn2), 0.0);         /* カメラ上方向のベクトル */

    /* 立方体の描画 */
    for (i = 0; i < 100; i++) {
        glPushMatrix();           /* 描画位置を保存 */
        glColor3f(1.0, 1.0, 1.0); /* 描画色を白にする */
        glScalef(1.0, 1.0, 1.0);
        if (i < gClientNum+1) {
            /* 描画位置を(BoxX, i, 0)に移動 */
            // glutWireCube (0.5);
            if (i >=0 && i <= gClientNum) {
                glTranslatef(player[i].pos.x, player[i].pos.y, player[i].pos.z);
                glRotatef(player[clientID].turn1, 0, 1, 0);
            }
             else {
                glTranslatef(BoxX[1], 0, 2);
                glRotatef(j, 0, 0, 1.0);
            }
            glutSolidCube(1.0);
           // glutWireSphere(0.4, 20.0, 10.0);
        } else if (i == 2) {
            glTranslatef(BoxX[i], 0, -2);
            // glutWireCube (0.5);
            glRotatef(f, 0, 0, 1.0);

            glutWireSphere(0.4, 20.0, 10.0);
        } else {
            if (i % 2 == 0) {

                glTranslatef(-1 * i * 10, 0, 5);
                glColor3f(1.0, 0.0, 1.0); /* 描画色を白にする */
                glScalef(2.0, 5.0, 0.5);
                glutSolidCube(1.0);
            } else {
                glTranslatef(-1 * i * 10, 0, -5);
                glColor3f(0.0, 1.0, 1.0); /* 描画色を白にする */
                glScalef(2.0, 5.0, 0.5);
                glutSolidCube(1.0);
            }
        }
        /* ワイヤーの立方体を描画 */
        glPopMatrix(); /* 描画位置を戻す */
    }

    //床の表示
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.8, 0.0);
    glScalef(1000.0, 0.1, 1.1);
    glutSolidCube(1.0);
    glPopMatrix();

    //敵１床
    glColor3f(1.0, 1.0, 1.0); /* 描画色を白にする */
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(0.0, -0.8, 1.5);
    glScalef(1000.0, 0.1, 1.8);
    glutSolidCube(1.0);
    glPopMatrix();

    //敵２床
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(0.0, -0.8, -1.5);
    glScalef(1000.0, 0.1, 1.8);
    glutSolidCube(1.0);
    glPopMatrix();

    //ゴール床表示
   

    for (i = 0; i < 20; i++) {
        glPushMatrix();
        glColor3f(1.0, 0.0, 1.0);
        glTranslatef(i * -15 - 10, -1.5, 4);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCone(0.5, 3.6, 10, 2);
        glPopMatrix();
    }
    for (i = 0; i < 20; i++) {
        glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(i * -15 - 10, -1.5, -4);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCone(0.5, 3.6, 10, 2);
        glPopMatrix();
    }

    for (i = 0; i < 50; i++) {
        glPushMatrix();

        if (i % 2) {
            glColor3f(1.0, 0.0, 0.0);
            glTranslatef(i * -15 - 10, -1.5, i % 4);
        } else {
            glColor3f(0.0, 1.0, 0.0);
            glTranslatef(i * -15 - 10, 2, (i * -1) % 4 * -1);
        }
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCube(0.1);
        glPopMatrix();
    }

    for (i = 0; i < 50; i++) {
        glPushMatrix();
        glColor3f(1.0, 0.0, 1.0);

        glTranslatef(i * -15 - 10, -1.5, 3);

        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCube(0.1);
        glPopMatrix();
    }

    // 障害物
    for (i = 0; i < 5; i++) {
        glPushMatrix();
        glColor3f(0.0, 0.0, 1.0);
        glTranslatef(i * -100 - 10, -0.4, 0);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCone(0.2, 0.6, 10, 2);
        glPopMatrix();
    }
    for (i = 0; i < 5; i++) {
        glPushMatrix();
        glColor3f(0.0, 1.0, 0.0);
        glTranslatef(i * -100 - 70, -0.4, -1.4);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCone(0.2, 0.6, 10, 2);
        glPopMatrix();
    }
    for (i = 0; i < 5; i++) {
        glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(i * -100 - 50, -0.4, 1.4);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCone(0.2, 0.6, 10, 2);
        glPopMatrix();
    }

    //printf("%d   %f %f %f\n", jnk, player[clientID].pos.x, BoxX[1], BoxX[2]);
    if (flag < 100) {
      //  Goal();
    }


    glPushMatrix();                    /* 視点位置を保存 */
    glColor3f(1.0, 1.0, 0);            /* 描画色を白(1.0,1.0,1.0)にする */
    glRotatef(90, 0, 1.0, 0);          /* Y軸中心にBoxRotate(度)回転 */
    glTranslatef(-0.18, 0.5, player[clientID].pos.x); /* 文字表示座標 */

   

    glPushMatrix();
    {
        glTranslatef(-2.0, 0.0, -20.0);
        glRotatef(90, 0.0, 1.0, 0.0);
        glColor3f(1.0, 1.0, 1.0);
    }
    glPopMatrix();
   
    move_bullet(bullet_Num);    // 弾の移動
    draw_bullet(bullet_Num);    // 弾の描画
    del_bullet();               // 弾の削除
    
    glutSwapBuffers();

    /* ネットワークの処理 */
    SendRecvManager();
    SendPlayerDataCommand(); //PlayerDataの送信
}

/*void reshape(int w, int h)
{
    printf("%d %d\n",w,h);
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-5.0, 5.0,-5.0, 5.0, 5.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
*/

void resize(int w, int h)
{
    /* ウインドウの縦横の比を計算 */
    float aspect = (float)w / (float)h;
    /* ウィンドウ全体をビューポートする */
    glViewport(0, 0, w, h);

    /* CG描画設定 */
    glMatrixMode(GL_PROJECTION);             /* 透視投影(遠近投影法)設定モードに切り替え */
    glLoadIdentity();                        /* 透視投影行列を初期化 */
    gluPerspective(45.0, aspect, 1.0, 20.0); /* 透視投影行列の設定 */
                                             /* 視野角45度, 縦横比 aspect，描画前面までの奥行 1.0，描画背面までの奥行 20.0 */
    /* モデルビュー変換行列の設定 */
    glMatrixMode(GL_MODELVIEW);
}

void drawString3D(const char *str, float charSize, float lineWidth)
{
    glPushMatrix();
    glPushAttrib(GL_LINE_BIT);
    glScaled(0.001 * charSize, 0.001 * charSize, 0.01);
    glLineWidth(lineWidth);
    while (*str) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
        ++str;
    }
    glPopAttrib();
    glPopMatrix();
}
/*
|  説明：タイマー（設定時間経過）イベント処理
|  引数：int timerID    イベントが発生したタイマーの識別ID
|  戻値：なし
***********************************************************/
void timer(int timerID)
{
    int i;

    /* 次のタイマーを15ミリ秒後に設定 */
    glutTimerFunc(15, timer, 0);

    for (i = 0; i < 3; i++) {
        /* 箱をX方向に移動 */
        BoxX[i] += BoxVx[i];
        /* 箱が端に到達したら移動方向を反転 */
        // if( BoxX[i] > 22.0 || BoxX[i] < -22.0) BoxVx[i] = -BoxVx[i];
    }
    //printf("  %lf  ", BoxVx[0]);
   


    /* 描画要求（直後に display() 関数が呼ばれる） */
    glutPostRedisplay();
    // SDL_Delay(10);
    
}

/***********************************************************
|  関数：keyboard()
|  説明：キーボードが押された時のイベント処理
|  引数：unsigned char key  押されたキーの文字コード
|  引数：int x              キーが押されたときのマウスポインタのX座標
|  引数：int y              キーが押されたときのマウスポインタのY座標
|  戻値：なし
***********************************************************/
void keyboard(unsigned char key, int x, int y)
{

    //回転座標
     int xMove = x - xBegin;
     int yMove = y - yBegin;

    /* キーボード処理 */
    switch (key) {
    case 'q':
        exit(0); /* プログラム終了 */
        break;
    case 'b':
       //player[clientID].pos.x=player[clientID].pos.x-1;
        //if(player[clientID].turn1>=0){
         player[clientID].pos.x = player[clientID].pos.x-sin(player[clientID].turn1);
         player[clientID].pos.z =player[clientID].pos.z-cos(player[clientID].turn1);
         player[clientID].pos.y = player[clientID].pos.y - sin(player[clientID].turn2);
       // }
       /* else{
         player[clientID].pos.x = player[clientID].pos.x-cos(player[clientID].turn1*0.1f);
         player[clientID].pos.z =player[clientID].pos.z-sin(player[clientID].turn1*0.1f);
        }*/
         //player[clientID].pos.z * /*cos(player[clientID].turn1/180)*/sin(player[clientID].turn1/180);
        printf("%f\n",player[clientID].pos.x);
         //player[clientID].pos.y= player[clientID].pos.y+0.01*cos(player[clientID].turn1/180.0);
      // player[clientID].pos.x -= 1;
        break;
    case 'd':
        if(bullet_Num > MAX_BULLET_NUM){bullet_Num = 0;}
        create_bullet(bullet_Num);
        glutTimerFunc(1000, add_lifetime, 0);
        bullet_Num++;
        break;
    case 's':
        flag = 0;
        if(player[clientID].turn1>0){
            double a;
            a = -2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 - (M_PI / 180);

        printf("zahyouhane~%lf\n",player[clientID].turn1);
        if(player[clientID].turn1 == -2 * M_PI){
            player[clientID].turn1 = 0;
        }
        /*CameraX = player[clientID].pos.x +sin(player[clientID].turn1*0.1f)*5;
        CameraY = player[clientID].pos.y;
        CameraZ = player[clientID].pos.z+cos(player[clientID].turn1*0.1f)*5;*/
        break;
    case