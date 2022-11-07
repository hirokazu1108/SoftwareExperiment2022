
/* --------------------------------------------------------------------
 * gls04.c
 * 2個の立方体を別々に動かす
 * -------------------------------------------------------------------- */

/* ヘッダファイル */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <SDL2/SDL.h> // SDL2を用いるために必要なヘッダファイルをインクルード]
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <joyconlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
float turn = 0;
char left  = 0;
char right = 0;
char buf[256];
SDL_Renderer *renderer; //レンダラーの作成
SDL_Window *window;     // ウィンドウのデータを格納する構造体
int kasoku;
float BoxRotate = 10.0;
int flag        = 106;
int j           = 0; // 2のはやさ
int f           = 0; //３のはやさ
int ps          = 0;
double junp;
int junpf = 0;
int bool  = 1;
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
void mouseButton(int button, int state, int x, int y);
void mouseDrag(int x, int y);
void myInit(char *windowTitle);
void drawString3D(const char *str, float charSize, float lineWidth);
void endstart(void);
void displayTexPolygon(void);
void init(void);
void reshape(int w, int h);
void junpk(void);
int joyconev();
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

   
    /* 初期化 */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* 画面を消去 */
    glMatrixMode(GL_MODELVIEW);                         /* 幾何（描画位置など設定する）モード */
    glLoadIdentity();                                   /* 幾何を初期化する */

    /* 視点の設定 */
    // glMatrixMode(GL_PROJECTION);
    
    gluLookAt(BoxX[0] +sin(turn*0.1f)*5, BoxY[0], BoxZ[0]+cos(turn*0.1f)*5, /* カメラの位置 */
        BoxX[0]/*+turn*0.1*/, 0 /*+turn*0.01*/,BoxZ[0]/*+turn*0.01f*//*+sin(turn/180)*/,                         /* 注視点の位置 */
        0, 0.5/*+turn*0.5*/, 0.0);                              /* カメラ上方向のベクトル */
    /* 立方体の描画 */
    for (i = 0; i < 100; i++) {
        glPushMatrix();           /* 描画位置を保存 */
        glColor3f(1.0, 1.0, 1.0); /* 描画色を白にする */
        glScalef(1.0, 1.0, 1.0);
        if (i < 2) {
            /* 描画位置を(BoxX, i, 0)に移動 */
            // glutWireCube (0.5);
            if (i == 0) {
                glTranslatef(BoxX[0], BoxY[0], BoxZ[0]);
                glRotatef(turn*5.75, 0, 1, 0);
            } else {
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

    printf("%d   %f %f %f\n", jnk, BoxX[0], BoxX[1], BoxX[2]);
    if (flag < 100) {
      //  Goal();
    }

    glPushMatrix();                    /* 視点位置を保存 */
    glColor3f(1.0, 1.0, 0);            /* 描画色を白(1.0,1.0,1.0)にする */
    glRotatef(90, 0, 1.0, 0);          /* Y軸中心にBoxRotate(度)回転 */
    glTranslatef(-0.18, 0.5, BoxX[0]); /* 文字表示座標 */

   

    glPushMatrix();
    {
        glTranslatef(-2.0, 0.0, -20.0);
        glRotatef(90, 0.0, 1.0, 0.0);
        glColor3f(1.0, 1.0, 1.0);
        displayTexPolygon();
    }
    glPopMatrix();
    glFlush();
    glutSwapBuffers();

    glFlush();
    /* 上記で描画されたCGをモニターに出力 */
    glutSwapBuffers();
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

void displayTexPolygon(void)
{
    
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
    printf("  %lf  ", BoxVx[0]);
   

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
        //BoxX[0]=BoxX[0]-1;
         BoxX[0] = BoxX[0]-sin(turn*0.1f);
         BoxZ[0] =BoxZ[0]-cos(turn*0.1f);//BoxZ[0] * /*cos(turn/180)*/sin(turn/180);
        printf("%f\n",BoxX[0]);
         //BoxY[0]= BoxY[0]+0.01*cos(turn/180.0);
      // BoxX[0] -= 1;
        break;
    case 'd':
        BoxX[0]=BoxX[0]+1;
        // BoxY[0]= BoxY[0]-0.01*cos(turn/180.0*3.141592);
        break;
    case 's':
        turn = turn + 1;
        printf("zahyouhane~%lf\n",turn);
        if(turn == 64){
            turn = 0;
        }
        
        break;
    case 'a':
        turn =0;
        break;
    case 'n':
        flag -= 1;

        break;
    case 'j':
        junpf++;
        break;
    default:
        junp  = 0;
        junpf = 0;
        //turn = 0;
        break;
    }
    

    // jyoikonnの処理
    //カメラの処理
    /* CameraAzimuth   += (float)xMove / 2.0;
     CameraElevation += (float)yMove / 2.0;
     if (CameraElevation >  90.0){
       CameraElevation =  90.0;
           }
     if (CameraElevation < -90.0){
       CameraElevation = -90.0;
     }
   CameraX += BoxVx[0];// CameraDistance * cos(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
   CameraY = CameraDistance * sin(CameraElevation * RAD);
   CameraZ = CameraDistance * sin(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
   printf("Camera AZ:%.1f, EL:%.1f, dist:%.1f, x,y,z= %.1f, %.1f, %.1f\n",
       CameraAzimuth, CameraElevation, CameraDistance, CameraX, CameraY, CameraZ);*/

   // 現在のマウスポインタの座標を次の始点用に記録する
   xBegin = x;
   yBegin = y;
    /* 描画要求（直後に display() 関数が呼ばれる） */
    glutPostRedisplay();

    /* コンパイル時の警告対策（定義された変数を使わないと警告になるので） */
    x = y = 0;
}

//ジョイコンの処理
/*int joyconev()
{
    joycon_get_state(&jc);
    if (rb) {
        joycon_rumble(&jc, 50);
        rb = 0;
    }
    if (jc.button.btn.Home) {
        joycon_rumble(&jc, 50);
        exit(0);
        return 0;
    }
    
    return 1;
}
*/




/***********************************************************
|  関数：mouseButton()
|  説明：マウスのボタン操作時のイベント処理
|  引数：int button     操作したマウスボタンの番号
|  引数：int state      操作の種類 GLUT_DOWN（押す）かGLUT_UP（離す）
|  引数：int x          キーが押されたときのマウスポインタのX座標
|  引数：int y          キーが押されたときのマウスポインタのY座標
|  戻値：なし
***********************************************************/
void mouseButton(int button, int state, int x, int y)
{
    // マウスが押された時
    /* if (state == GLUT_DOWN)
     {
         switch(button)
         {
         case GLUT_LEFT_BUTTON:  // マウス左ボタンを押した時の処理
             PressButton = button;
             break;

         case GLUT_MIDDLE_BUTTON:// マウス中ボタンを押した時の処理
             break;

         case GLUT_RIGHT_BUTTON: // マウス右ボタンを押した時の処理
             PressButton = button;
             break;
         }

         // マウスボタンを押した瞬間の始点座標を記録する
         xBegin = x;
         yBegin = y;
     }*/
}


/***********************************************************
|  関数：mouseDrag()
|  説明：画面上でマウスがドラッグされた時のイベント処理
|  引数：int x          現在のマウスポインタのX座標
|  引数：int y          現在のマウスポインタのY座標
|  戻値：なし
***********************************************************/
void mouseDrag(int x, int y)
{
    // マウスボタンが押されてからの移動量を計算する
   // int xMove = x - xBegin;
   // int yMove = y - yBegin;

    // switch (PressButton)
    //{
    /* マウス左ボタンドラッグ時の処理 */
    /* 視点を方位角，仰角に対して回転させる */
    /*case GLUT_LEFT_BUTTON:
        CameraAzimuth   += (float)xMove / 2.0;
        CameraElevation += (float)yMove / 2.0;
        if (CameraElevation >  90.0) CameraElevation =  90.0;
        if (CameraElevation < -90.0) CameraElevation = -90.0;
        break;/*

    /* マウス右ボタンドラッグ時の処理 */
    /* 視点を奥行き方向に前後に移動させる */
    /*case GLUT_RIGHT_BUTTON:
        CameraDistance += (float)yMove / 40.0;
        break;
    }*/
    /* CameraAzimuth   += (float)xMove / 2.0;
     CameraElevation += (float)yMove / 2.0;
     if (CameraElevation >  90.0){
       CameraElevation =  90.0;
           }
     if (CameraElevation < -90.0){
       CameraElevation = -90.0;
     }
   CameraX += BoxVx[0];// CameraDistance * cos(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
   CameraY = CameraDistance * sin(CameraElevation * RAD);
   CameraZ = CameraDistance * sin(CameraAzimuth * RAD) * cos(CameraElevation * RAD);
   printf("Camera AZ:%.1f, EL:%.1f, dist:%.1f, x,y,z= %.1f, %.1f, %.1f\n",
       CameraAzimuth, CameraElevation, CameraDistance, CameraX, CameraY, CameraZ);

   // 現在のマウスポインタの座標を次の始点用に記録する
   xBegin = x;
   yBegin = y;

   // 描画要求（直後に display() 関数が呼ばれる）
   glutPostRedisplay();*/
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
    int winWidth  = 2000;
    int winHeight = 2000;
    /* ウインドウの縦横の比を計算 */
    float aspect = (float)winWidth / (float)winHeight;
    // window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

    /*以下がｇｌｓ０３のmain*/

    /* OpenGLウインドウ作成までの初期化 */
    glutInitWindowPosition(0, 0);            /* ウインドウ表示位置 */
    glutInitWindowSize(winWidth, winHeight); /* ウインドウサイズ */
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);   /* 描画モード */
    // renderer = SDL_CreateRenderer(window, -1, 0); // 生成したウィンドウに対してレンダリングコンテキスト（RC）を生成
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); // 生成したRCに描画色として白を設定
    glutCreateWindow(windowTitle);                      /* ウインドウの表示 */
    glClearColor(0.0, 0.0, 0.0, 1.0);                   /* 画面消去色の設定 */
    

    /* イベント発生時に呼び出す関数の登録 */
    glutKeyboardFunc(keyboard);  /* キーボードを押した時 */
    glutMouseFunc(mouseButton);  /* マウスボタンを押した時*/
    glutMotionFunc(mouseDrag);   /* マウスドラッグした時 */
    glutDisplayFunc(display);    /* 画面表示 */
    glutTimerFunc(15, timer, 0); /* タイマーを15ミリ秒後に設定 */

    /* CG描画設定 */
    glMatrixMode(GL_PROJECTION);             /* 透視投影(遠近投影法)設定モードに切り替え */
    glLoadIdentity();                        /* 透視投影行列を初期化 */
    gluPerspective(45.0, aspect, 1.0, 20.0); /* 透視投影行列の設定 */
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