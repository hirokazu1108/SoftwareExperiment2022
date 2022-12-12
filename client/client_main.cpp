/* --------------------------------------------------------------------
 * gls04.c
 * 2個の立方体を別々に動かす
 * -------------------------------------------------------------------- */

/* ヘッダファイル */
#include <stdarg.h>
#include "client.h"
#include "trackball.h"
#include "glm.h"
int		gClientNum;
int		clientID;
Player *player;
int sendDataFlag = 0;

#define RAD (M_PI / 180.0)
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
std::vector<BULLET> array_bullet;

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
bool key1 = false;
bool key2 = false;
bool key3 = false;
bool key4 = false;
bool key5 = false;
bool key6 = false;
bool key7 = false;
bool startflag = true;
int bullet_Num = 0;   // 発射された弾の個数
bool can_attack = true;     // 弾の発射が可能ならtrue

int bf;
int jnk; //ジョイコンの入力コマンド
int r=0;
int rb;
int af;
joyconlib_t jc;

GLuint     model_list = 0;		/* display list for object */
char*      model_file = NULL;		/* name of the obect file */
GLboolean  facet_normal = GL_FALSE;	/* draw with facet normal? */
GLMmodel*  model;
GLfloat    smoothing_angle = 90.0;	/* smoothing angle */
GLfloat    scale;			/* scaling factor */
GLboolean  bounding_box = GL_FALSE;
GLboolean  performance = GL_FALSE;
GLboolean  stats = GL_FALSE;
GLfloat    weld_distance = 0.00001;
GLuint     material_mode = 1;
/* 関数のプロトタイプ宣言 */

void display(void);
void timer(int timerID);
void keyboard(unsigned char key, int x, int y);
void resize(int w, int h);
void myInit(char *windowTitle);
void drawString3D(const char *str, float charSize, float lineWidth);
void text(GLuint x, GLuint y, GLfloat scale, char* format, ...);
void lists(void);
void init(void);
int joyconev();
void move(void);
void create_bullet(int num);    // 弾の生成
void draw_bullet(int num);  // 弾の描画
void move_bullet(int num);    // 弾丸の動き
void add_lifetime(int add_lifetimeID);  // 弾のライフタイムを加算
void count_time(int count_timeID);      // ゲーム開始からの経過時間（秒）をカウントする
void del_bullet(void);  // 弾の消去
void interval_attack(int interval_attackID);    // 弾の発射間隔を設ける関数
#define TEX_HEIGHT 32
#define TEX_WIDTH 32
static GLubyte image[TEX_HEIGHT][TEX_WIDTH][4];
char *modelname = "sentouki.obj" ;

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
        //player[i].collider.radius = 1.0;
        //player[i].collider.pos = player[i].pos;
    }
    

    
    /* 初期化 */
    glutInit(&argc, argv); /* OpenGL の初期化 */
    myInit(argv[0]);       /* ウインドウ表示と描画設定の初期化 */
    

    model_file = modelname;
    if (!model_file) {
    fprintf(stderr, "usage: smooth model_file.obj\n");
    exit(1);
    }
    init();

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

   move();
        if(flag ==0){
        CameraX = CameraX;
        CameraY = CameraY;
        CameraZ = CameraZ;
       //CameraX = BoxX[0] +sin(turn*0.1f)*5 ;
        //CameraY = BoxY[0] + sin(turn2*0.1f)*5;
        //CameraZ = BoxZ[0]+cos(turn*0.1f)*5;
        CameraX = player[clientID].pos.x /*+sin(turn*0.1f)*5*/  +sin(player[clientID].turn1)*5 *cos(player[clientID].turn3);
        CameraY = player[clientID].pos.y + sin(player[clientID].turn2)*5;
        CameraZ = player[clientID].pos.z /*+cos(turn*0.1f)*5*/ +cos(player[clientID].turn1)*5 *cos(player[clientID].turn3);
        
        
        }
        else{
        CameraX = CameraX;
        CameraY = CameraY;
        CameraZ = CameraZ;

        CameraX = player[clientID].pos.x /*+sin(turn*0.1f)*5 */ + sin(player[clientID].turn1)*5 *cos(player[clientID].turn3);
        CameraY = player[clientID].pos.y + sin(player[clientID].turn2)*5;
        CameraZ = player[clientID].pos.z /*+cos(turn*0.1f)*5 */+cos(player[clientID].turn1)*5 *cos(player[clientID].turn3);
        
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
        glPushMatrix();           /* ??????????¸ */
        glColor3f(1.0, 1.0, 1.0); /* ???????????? */
        glScalef(1.0, 1.0, 1.0);
         
        if (i < gClientNum) {
            /* ????????(BoxX, i, 0)???? */
            // glutWireCube (0.5);

            glTranslatef(player[i].pos.x, player[i].pos.y,player[i].pos.z);
           if(flag == 2){
                //glRotatef(turn*5.75, 0, 0, 1);
                    //flag = 0;
            }
                //glRotatef(turn2*-1*57.5, 1, 0, 0); 
               // glRotatef(turn3*57.5, 0, 1, 0);
             
               glRotatef(player[i].turn1*57.5, 0, 1, 0);
               glRotatef(player[i].turn2*57.5*-1, 1, 0, 0);
            

            glCallList(model_list);
             
            //glutSolidCube(1.0);
           // glutWireSphere(0.4, 20.0, 10.0);
        } else if (i == gClientNum) {
            glTranslatef(0, 0, -2);
            // glutWireCube (0.5);
            glRotatef(f, 0, 0, 1.0);

            glutWireSphere(0.4, 20.0, 10.0);
        } else {
            if (i % 2 == 0) {

                glTranslatef(-1 * i * 10, 0, 5);
                glColor3f(1.0, 0.0, 1.0); /* ???????????? */
                glScalef(2.0, 5.0, 0.5);
                glutSolidCube(1.0);
            } else {
                glTranslatef(-1 * i * 10, 0, -5);
                glColor3f(0.0, 1.0, 1.0); /* ???????????? */
                glScalef(2.0, 5.0, 0.5);
                glutSolidCube(1.0);
            }
            glPopMatrix(); /* ?????????? */
        }
    }

    //???????
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.8, 0.0);
    glScalef(1000.0, 0.1, 1.1);
    glutSolidCube(1.0);
    glPopMatrix();

    //Ũ????
        glColor3f(1.0, 1.0, 1.0); /* ???????????? */
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(0.0, -0.8, 1.5);
    glScalef(1000.0, 0.1, 1.8);
    glutSolidCube(1.0);
    glPopMatrix();

    //Ũ????
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(0.0, -0.8, -1.5);
    glScalef(1000.0, 0.1, 1.8);
    glutSolidCube(1.0);
    glPopMatrix();

    //?????????
   

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

    // ???
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

    

    //printf("%d   %f %f %f\n", jnk, BoxX[0], BoxX[1], BoxX[2]);
    if (flag < 100) {
      //  Goal();
    }

    glPushMatrix();                    /* ???????????¸ */
    glColor3f(1.0, 1.0, 0);            /* ????????(1.0,1.0,1.0)????? */
    glRotatef(90, 0, 1.0, 0);          /* Y??????BoxRotate(??)??ž */
    glTranslatef(-0.18, 0.5, player[clientID].pos.x); /* ????????? */
    
   

    glPushMatrix();
    {
        glTranslatef(-2.0, 0.0, -20.0);
        glRotatef(90, 0.0, 1.0, 0.0);
        glColor3f(1.0, 1.0, 1.0);
    }
    glPopMatrix();

    glFlush();
    /* ?嵭?????褵??CG???????????? */
    glutSwapBuffers();
    

    move_bullet(bullet_Num);    // 弾の移動
    draw_bullet(bullet_Num);    // 弾の描画

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

void
init(void)
{
  tbInit(GLUT_MIDDLE_BUTTON);
  
  /* read in the model */
  model = glmReadOBJ(model_file);
  scale = glmUnitize(model);
  glmFacetNormals(model);
  glmVertexNormals(model, smoothing_angle);

  /* create new display lists */
  lists();

  //glEnable(GL_LIGHTING);
  //glEnable(GL_LIGHT0);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
}


void lists(void)
{
  GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
  GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat shininess = 65.0;

  if (model_list)
    glDeleteLists(model_list, 1);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

 
  if (material_mode == 0) { 
    if (facet_normal)
      model_list = glmList(model, GLM_FLAT);
    else
      model_list = glmList(model, GLM_SMOOTH);
  } else if (material_mode == 1) {
    if (facet_normal)
      model_list = glmList(model, GLM_FLAT | GLM_COLOR);
    else
      model_list = glmList(model, GLM_SMOOTH | GLM_COLOR);
  } else if (material_mode == 2) {
    if (facet_normal)
      model_list = glmList(model, GLM_FLAT | GLM_MATERIAL);
    else
      model_list = glmList(model, GLM_SMOOTH | GLM_MATERIAL);
  }
}

void resize(int w, int h)
{
  
    float aspect = (float)w / (float)h;
    
    glViewport(0, 0, w, h);

   
    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                       
    gluPerspective(45.0, aspect, 1.0, 20.0); */
    
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
       // BoxX[i] += BoxVx[i];
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

    //??ž???
     
        key6 = true;
     
     int xMove = x - xBegin;
     int yMove = y - yBegin;
   
      if(key == 'q'){
      exit(0);
        
       }
    if(key == 'l'){
        j++;
    }

    if(key == 'd'){
        //flag = 0;
        key1 = true;
       /*if(turn>0){
            double a;
            a = -2 * M_PI + turn;
            turn = a;
        }
        turn = turn - (M_PI / 180);

        printf("zahyouhane~%lf\n",turn);
        if(turn == -2 * M_PI){
            turn = 0;
        }*/
       }

    if(key == 's'){
         flag = 2;
        //player[clientID].turn2 = player[clientID].turn2-1;
        key2 = true;
        /*if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",player[clientID].turn2);
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }*/
       }
    
    if(key == 'a'){
        flag = 0;
        key3 = true;
        /*if(turn<0){
            double a;
            a = 2 * M_PI + turn;
            turn = a;
        }
        turn = turn + (M_PI / 180);
        printf("zahyouhane~%lf\n",turn);
        if(turn == 2 * M_PI){
            turn = 0;
        }*/
       }


    if(key == 'w'){
        flag = 2;
        key4 = true;
        //player[clientID].turn2 = player[clientID].turn2-1;
        
        /*if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",player[clientID].turn2);
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }*/
        
       }
        if(key == 'b'){
          //  key5 = true;
         player[clientID].pos.x = player[clientID].pos.x-sin(player[clientID].turn1)*cos(player[clientID].turn2);
         player[clientID].pos.z =player[clientID].pos.z-cos(player[clientID].turn1)*cos(player[clientID].turn2);
         player[clientID].pos.y = player[clientID].pos.y - sin(player[clientID].turn2);
      
        printf("%f\n",player[clientID].pos.x);
       }

       if(key == ' '){
        key7 = true;
    }   
      
    glutPostRedisplay();
    x = y = 0;
   // key6=false;
}

void keyboard2(unsigned char key, int x, int y)
{
    key6 = false;
    printf("koregayobareteirunokawakaranasugitekomatteioriokjnlfjahgljarhgttlijjfgljadflhgbleurhbgladbflajbfgflqihbflajhflejrhglijrndfljadnflgjbwqelrhigb;ajdnflh");
   /*key1 = false;
         key2 = false;
         key3 = false;
         key4 = false;
         key5 = false;
         key6 = false;*/
     switch (key) {
    case 'q':
      //  exit(0);
        break;
    
    case 'd':
        key = false;
        break;
    case 's':
        key2 = false;
        break;
    case 'a':
        key3 = false;
        break;
    case 'w':
        key4 = false;
        break;
    case 'b':
        key5 = false;
        break;
    case ' ':
        key7 = false;
        break;
    default:
        break;
        
    }
    x = y = 0;
}

void move(){
    if(key1 == true && key2 == true){
            printf("nanamekaitenn\n");
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
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",player[clientID].turn2);
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        }
    else if(key2 == true && key3 == true){
        printf("hidarinanamekaitenn");
        // flag = 2;
        //player[clientID].turn2 = player[clientID].turn2-1;
       // key2 = true;
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",player[clientID].turn2);
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }

        if(player[clientID].turn1<0){
            double a;
            a = 2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 + (M_PI / 180);
        if(player[clientID].turn1 == 2 * M_PI){
            player[clientID].turn1 = 0;
        }
    }
    else if(key1 == true && key4 == true){
         if(player[clientID].turn1>0){
            double a;
            a = -2 * M_PI + player[clientID].turn1;
            player[clientID].turn1= a;
        }
        player[clientID].turn1= player[clientID].turn1 - (M_PI / 180);
        if(player[clientID].turn1 == -2 * M_PI){
            player[clientID].turn1 = 0;
        }

        if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",player[clientID].turn2);
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
    }
    else if(key3 == true && key4 == true){
        if(player[clientID].turn1<0){
            double a;
            a = 2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 + (M_PI / 180);
        printf("zahyouhane~%lf\n",player[clientID].turn1);
        if(player[clientID].turn1 == 2 * M_PI){
            player[clientID].turn1= 0;
        }
         if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",player[clientID].turn2);
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
    }


    else if(key1 == true){
        flag = 0;
        //key = true;
        if(player[clientID].turn1>0){
            double a;
            a = -2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1= player[clientID].turn1 - (M_PI / 180);
        if(player[clientID].turn1 == -2 * M_PI){
            player[clientID].turn1 = 0;
        }
       }


   else if(key2 == true){
         flag = 2;
        //player[clientID].turn2 = player[clientID].turn2-1;
        key2 = true;
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",player[clientID].turn2);
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
       }
    
    else if(key3 == true){
        flag = 0;
        key3 = true;
        if(player[clientID].turn1<0){
            double a;
            a = 2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 + (M_PI / 180);
        if(player[clientID].turn1 == 2 * M_PI){
            player[clientID].turn1 = 0;
        }
       }


    else if(key4 == true){
        flag = 2;
        key4 = true;
        //player[clientID].turn2 = player[clientID].turn2-1;
        
        if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",player[clientID].turn2);
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        
       }
    if(key7 == true){
        if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
            printf("ok\n");
        }
    }
       
       if(key6==false){
         key1 = false;
         key2 = false;
         key3 = false;
         key4 = false;
         key5 = false;
         key6 = false;
         key7 = false;
          printf("yobareteorimasutaishousann\n");
       }
        if(key6 == true){
       // key6 = false;
       
       }
       
}

/***********************************************************
|  関数：myInit()
|  説明：ウインドウ表示と描画設定の初期化
|  引数：char *windowTitle      ウインドウのタイトルバーに表示する文字列
|  戻値：なし
***********************************************************/
void myInit(char *windowTitle)
{
    /* ウインドウのサイズ */
    int winWidth  = WINDOW_WIDTH;
    int winHeight = WINDOW_HEIGHT;
    /* ウインドウの縦横の比を計算 */
    float aspect = (float)winWidth / (float)winHeight;
    // window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

    /*以下がｇｌｓ０３のmain*/

    /* OpenGLウインドウ作成までの初期化 */
    glutInitWindowPosition(0, 0);            /* ウインドウ表示位置 */
    glutInitWindowSize(winWidth, winHeight); /* ウインドウサイズ */
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);   /* 描画モード */

    glutCreateWindow(windowTitle);                      /* ウインドウの表示 */
    glClearColor(0.0, 0.0, 0.0, 1.0);                   /* 画面消去色の設定 */
    

    /* イベント発生時に呼び出す関数の登録 */
    glutKeyboardUpFunc(keyboard2);
    glutKeyboardFunc(keyboard);  /* キーボードを押した時 */
    glutReshapeFunc(resize);
    glutDisplayFunc(display);    /* 画面表示 */
    glutTimerFunc(15, timer, 0); /* タイマーを15ミリ秒後に設定 */

    /* CG描画設定 */
    glMatrixMode(GL_PROJECTION);             /* 透視投影(遠近投影法)設定モードに切り替え */
    glLoadIdentity();                        /* 透視投影行列を初期化 */
    gluPerspective(45.0, aspect, 1.0, 100.0); /* 透視投影行列の設定 */
                                             /* 視野角45度, 縦横比 aspect，描画前面までの奥行 1.0，描画背面までの奥行 20.0 */
    glEnable(GL_DEPTH_TEST);                 /* 隠面消去を有効にする */
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
   // initTexture();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_WIDTH, TEX_HEIGHT,
        0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    /* 箱の座標と速度の初期値設定 */

    
}

void create_bullet(int num){
    BULLET b;
    b.pos = player[clientID].pos;
    b.dir = {(- sin(player[clientID].turn1)), - sin(player[clientID].turn2), - cos(player[clientID].turn1)};
    b.lifetime = 0;
    array_bullet.push_back(BULLET(b));
    printf("send:%f, %f, %f\n", array_bullet[bullet_Num].pos.x, array_bullet[bullet_Num].pos.y, array_bullet[bullet_Num].pos.z);
    bullet_Num++;
    SendBulletDataCommand(num);
}

void draw_bullet(int num){
    for(int i = 0; i < num; i++){
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(array_bullet[i].pos.x, array_bullet[i].pos.y, array_bullet[i].pos.z);
        glutSolidSphere(0.3, 200, 200);
        glPopMatrix();
    }
}

void move_bullet(int num){
    for(int i = 0; i < num; i++){
        array_bullet[i].pos += array_bullet[i].dir * 0.5f;
    }
}

void del_bullet(){
    for(int i = 0; i < bullet_Num; i++){
        if(array_bullet[i].lifetime >= 5){
            array_bullet.erase(array_bullet.begin() + i);       //  i番目の要素を削除
            bullet_Num--;
        }
    } 
}

void interval_attack(int interval_attackID){
    can_attack = true;
}

void add_lifetime(int add_lifetimeID){
    for(int i = 0; i < bullet_Num; i++){
        array_bullet[i].lifetime++;
    }  
    del_bullet();               // 弾の削除
    printf("bullet_Num:%d\n", bullet_Num);
}