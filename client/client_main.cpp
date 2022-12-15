/* --------------------------------------------------------------------
 * gls04.c
 * 2???????????????????????????????????????
 * -------------------------------------------------------------------- */

/* ??????????????ï¿???????? */
#include <stdarg.h>
#include "client.h"
#include "trackball.h"
#include "glm.h"
int		gClientNum;
int		clientID;
Player *player;
Game game; //?????????

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

/* ??????????????????ï¿?????? */
int xBegin            = 0;    /* ????????ï¿?????????????????ï¿??????Xç¶ºï¿½??? */
int yBegin            = 0;    /* ????????ï¿?????????????????ï¿??????Yç¶ºï¿½??? */
int PressButton       = 0;    /* ?????????????????????????????????ï¿??????ï¿???ï¿????1:ï¿???,2:ï¿???,3:?????? */
float CameraAzimuth   = 90.0; /* ?????ï¿???????ï¿?????????????????????????? */
float CameraElevation = 0.0;  /* ?????ï¿???????ï¿???????????ï¿??????????? */
float CameraDistance  = 5.0;  /* ?????ï¿???????ï¿????????????????ï¿????????????????????? */
float CameraX         = 0;  /* ?????ï¿???????ï¿???????????Xç¶ºï¿½??????? */
float CameraY         = 1.0;  /* ?????ï¿???????ï¿???????????Yç¶ºï¿½??????? */
float CameraZ         = 0.0;  /* ?????ï¿???????ï¿???????????Zç¶ºï¿½??????? */
float BoxX[3];                /* ???ï¿????Xç¶ºï¿½??? */
float BoxVx[3];               /* ???ï¿????X?????????ï¿½ç³¸?????ï¿??? */
float BoxY[3];
float BoxZ[3];
char left  = 0;
char right = 0;
char buf[256];
int kasoku;
float BoxRotate = 10.0;
int flag        = 106;
int j           = 0; // 2????????????
int f           = 0; //????????????????
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
int bullet_Num = 0;   // ????????????????ç¶£ï¿½??????????
bool can_attack = true;     // ç¶£ï¿½???????????????????ï¿???????true

int bf;
int jnk; //?????????????ï¿???ï¿?????????????ï¿??????ï¿????
int r=0;
int rb;
int af;
joyconlib_t jc;

GLuint     model_list[Model_Num] = {0};		/* display list for object */
char*      model_file[Model_Num] = {NULL};		/* name of the obect file */
GLboolean  facet_normal = GL_FALSE;	/* draw with facet normal? */
GLMmodel*  model[Model_Num];
GLfloat    smoothing_angle[Model_Num] = {90.0};	/* smoothing angle */
GLfloat    scale[Model_Num];			/* scaling factor */
GLboolean  bounding_box = GL_FALSE;
GLboolean  performance = GL_FALSE;
GLboolean  stats = GL_FALSE;
GLfloat    weld_distance = 0.00001;
GLuint     material_mode = 1;
/* ???????????????????????ï¿????????ï¿??ï¿???? */

void display(void);
void timer(int timerID);
void keyboard(unsigned char key, int x, int y);
void resize(int w, int h);
void myInit(char *windowTitle);
void drawString3D(const char *str, float charSize, float lineWidth);
void init(int CModel);
void text(GLuint x, GLuint y, GLfloat scale, char* format, ...);
void lists(int CModel);
int joyconev();
void move(void);
void create_bullet(int num);    // ç¶£ï¿½??????????
void draw_bullet(int num);  // ç¶£ï¿½??????????
void move_bullet(int num);    // ç¶£ï¿½??ï¿???????????
void add_lifetime(int add_lifetimeID);  // ç¶£ï¿½????????????????ï¿??????????????????
void count_time(int count_timeID);      // ??????????????ï¿????????????????????????????????????????????????ï¿??????????
void del_bullet(void);  // ç¶£ï¿½????ï¿???????
void deleteBullet(int index);
void interval_attack(int interval_attackID);    // ç¶£ï¿½??????????????????????????????????????
#define TEX_HEIGHT 32
#define TEX_WIDTH 32
static GLubyte image[TEX_HEIGHT][TEX_WIDTH][4];
char *modelname[] = {"sentouki.obj","data/castle.obj"} ;

/***********************************************************
|  ???????????main()
|  ?????????????ï¿???????ï¿????????
|  ï¿??????????int argc       ï¿??????????ï¿?????????????
|  ï¿??????????char** argv    ï¿??????????ï¿?????????????ï¿??????????ï¿????????????????
|  ??ï¿????????int            0:ï¿??ï¿??å¹»ï¿½???ï¿???
***********************************************************/
int main(int argc, char **argv)
{
    u_short port = PORT;
    int		endFlag=1;
    char	serverName[MAX_NAME_SIZE];
    
    sprintf(serverName, "localhost");

    /* ï¿?????????????????????? */
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

    /* ??ï¿?????????????????????ï¿½ï¿½??? */
    if(SetUpClient(serverName,port,&clientID,&gClientNum,game.clientName)==-1){
		fprintf(stderr,"setup failed : SetUpClient\n");
		return -1;
	}

    /* ????????? */
	player = (Player*)malloc(sizeof(Player)*gClientNum);
    for(int i = 0; i< gClientNum;i++)
    {
        player[i].enabled = true;
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
        player[i].hp = 3;
        player[i].reloadTime= 0;
        player[i].collider.radius = 1.0;
        player[i].collider.pos = player[i].pos;
    }
    
    /* ????????? */
    game.state = State_Play;


    
    /* ????????? */
    glutInit(&argc, argv); /* OpenGL ???????????? */
    myInit(game.clientName[clientID]);       /* ?????????ï¿??????????????????????????ï¿½ï¿½??ï¿??????????????? */
    

    for (int i = 0; i < Model_Num; i++){
        model_file[i] = modelname[i];
        if (!model_file[i]) {
        fprintf(stderr, "usage: smooth model_file.obj\n");
        exit(1);
        }
        init(i);
    }

    glutMainLoop();

    // glutReshapeFunc(reshape);
    /* ?????????ï¿???????????????????? */

    /* ?????????????????????ï¿??? */
    return (0);
}


/**********************************************************
|  ???????????display()
|  ???????????????????????????????????????ï¿????????????ï¿??????????ï¿??????????
|  ï¿????????????????
|  ??ï¿??????????????
***********************************************************/
void display(void)
{
    /* ???????? */
    if(!player[clientID].enabled)
    {

    }

    int i;
    float color[4] = {1.0};
    //move();
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
    
    /* ????????? */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* ???????????ï¿??????? */
    glMatrixMode(GL_MODELVIEW);                         /* ï¿????????????????ï¿??????????????????????????????????????????? */
    glLoadIdentity();                                   /* ï¿????????????????????????? */

    /* ??????ï¿???????????? */
    // glMatrixMode(GL_PROJECTION);
    
    gluLookAt(CameraX, CameraY, CameraZ, /* ?????ï¿???????ï¿??????? */
        player[clientID].pos.x, player[clientID].pos.y,player[clientID].pos.z,        /* ï¿?????????ï¿????ï¿??????? */
        0, 0.5*cos(player[clientID].turn2), 0.0);         /* ?????ï¿????ï¿????????????????????????? */
    /*if(startflag == true){
        glPushMatrix();
        glColor3f(1.0, 0.0, 0);
         glTranslatef(player[i].pos.x-1, player[i].pos.y,player[i].pos.z);
        drawString3D("Space Battle", 3.0, 2.0);
        glPushMatrix();
        if(j == 3){
            startflag = false;
        }
        }*/
    
    /* ???????????????????? */
     move();

   

    for (i = 0; i < 100; i++) {
        glPushMatrix();           /* ??????????ï¿?? */
        glColor3f(1.0, 1.0, 1.0); /* ???????????? */
        glScalef(1.0, 1.0, 1.0);
         
        if (i < gClientNum) {
            /* ????????(BoxX, i, 0)???? */
            // glutWireCube (0.5);

            glTranslatef(player[i].pos.x, player[i].pos.y,player[i].pos.z);

            //glRotatef(turn2*-1*57.5, 1, 0, 0); 
            // glRotatef(turn3*57.5, 0, 1, 0);
             
            glRotatef(player[i].turn1*57.5, 0, 1, 0);
            glRotatef(player[i].turn2*57.5*-1, 1, 0, 0);

            for (int j = 0; j < Model_Num; j++){
                glCallList(model_list[j]);
            }
             
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
    
    /*glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.8, 0);
    glScalef(1000.0, 0.1, 1.1);
    glutSolidCube(1.0);
    glPopMatrix();
    
    //???????
    
     glColor3f(1.0, 1.0, 1.0); 
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(0.0, -0.8, 1000);
    glScalef(1000.0, 0.1, 1.8);
    glutSolidCube(1.0);
    glPopMatrix();

    //???????
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(0.0, -0.8, -1.5*i);
    glScalef(1000.0, 0.1, 1000);
    glutSolidCube(1.0);
    glPopMatrix();*/
    
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

    if (flag < 100) {
      //  Goal();
    }

    glPushMatrix();                    /* ???????????ï¿?? */
    glColor3f(1.0, 1.0, 0);            /* ????????(1.0,1.0,1.0)????? */
    glRotatef(90, 0, 1.0, 0);          /* Y??????BoxRotate(??)??ï¿?? */
    glTranslatef(-0.18, 0.5, player[clientID].pos.x); /* ????????? */
    glPopMatrix();
   

    glPushMatrix();
    glTranslatef(-2.0, 0.0, -20.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glPopMatrix();

    move_bullet(bullet_Num);    // ç¶£ï¿½??????ï¿½ç³¸??
    draw_bullet(bullet_Num);    // ç¶£ï¿½??????????

    /* Draw ui */
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    uiSetting();
    glColor3f( 1.0f, 1.0f, 1.0f );
    glRectf(2-(1.5f)/3*player[clientID].hp,1.7f,2.0f,2.0f);
    glColor3f( 1.0f, 0.0f, 0.0f );
    glRectf( 0.5f, 1.7f, 2-(1.5f)/3*player[clientID].hp, 2);
    glPopMatrix();


    //drawPlayerCollider();

    glFlush();
    /* ?ï¿??????????????CG???????????? */
    glutSwapBuffers();
    
    
    if(player[clientID].enabled){
        Collider();
    }
    if(player[clientID].enabled && player[clientID].hp <= 0){
        player[clientID].enabled = false;
        /* ??????????????????? */
        char com=RANKING_DATA;
        SendData(&com,sizeof(char));
    } 
    
    
    /* ???????????????????????????? */
    SendRecvManager();
    SendPlayerDataCommand(); //PlayerData??????ï¿???
    
    
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
init(int CModel)
{
  tbInit(GLUT_MIDDLE_BUTTON);
  
  /* read in the model */
  model[CModel] = glmReadOBJ(model_file[CModel]);
  scale[CModel] = glmUnitize(model[CModel]);
  glmFacetNormals(model[CModel]);
  glmVertexNormals(model[CModel], smoothing_angle[CModel]);

  /* create new display lists */
  lists(CModel);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
}


void lists(int CModel)
{
  GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
  GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat shininess = 65.0;

  if (model_list[CModel])
    glDeleteLists(model_list[CModel], 1);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

 
  if (material_mode == 0) { 
    if (facet_normal)
      model_list[CModel] = glmList(model[CModel], GLM_FLAT);
    else
      model_list[CModel] = glmList(model[CModel], GLM_SMOOTH);
  } else if (material_mode == 1) {
    if (facet_normal)
      model_list[CModel] = glmList(model[CModel], GLM_FLAT | GLM_COLOR);
    else
      model_list[CModel] = glmList(model[CModel], GLM_SMOOTH | GLM_COLOR);
  } else if (material_mode == 2) {
    if (facet_normal)
      model_list[CModel] = glmList(model[CModel], GLM_FLAT | GLM_MATERIAL);
    else
      model_list[CModel] = glmList(model[CModel], GLM_SMOOTH | GLM_MATERIAL);
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
|  ?????????????ï¿??????????ï¿??????????????????????????????????????ï¿??????????
|  ï¿??????????int timerID    ?????????ï¿?????????ï¿????????????ï¿??????????????????????ID
|  ??ï¿??????????????
***********************************************************/
void timer(int timerID)
{
    int i;

    /* ï¿??ï¿??????ï¿???????????????15??????????ï¿?????????????? */
    glutTimerFunc(15, timer, 0);

    for (i = 0; i < 3; i++) {
        /* ???ï¿????X????????????ï¿½ç³¸?? */
       // BoxX[i] += BoxVx[i];
        /* ???ï¿?????????????????????????????ï¿½ç³¸??????????????????? */
        // if( BoxX[i] > 22.0 || BoxX[i] < -22.0) BoxVx[i] = -BoxVx[i];
    }
    //printf("  %lf  ", BoxVx[0]);
   


    /* ??????????ï¿?????????ï¿??????? display() ????????????????????????????? */
    glutPostRedisplay();
    // SDL_Delay(10);
    
}

/***********************************************************
|  ???????????keyboard()
|  ????????????????????????????????????????????????????????????ï¿??????????
|  ï¿??????????unsigned char key  ???????????????????????????ï¿?????ï¿????????
|  ï¿??????????int x              ?????????????????????????????????????????ï¿??????????ï¿???ï¿????Xç¶ºï¿½???
|  ï¿??????????int y              ?????????????????????????????????????????ï¿??????????ï¿???ï¿????Yç¶ºï¿½???
|  ??ï¿??????????????
***********************************************************/
void keyboard(unsigned char key, int x, int y)
{

    //??ï¿?????
     
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
         player[clientID].collider.pos = player[clientID].pos;
      
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
    //printf("koregayobareteirunokawakaranasugitekomatteioriokjnlfjahgljarhgttlijjfgljadflhgbleurhbgladbflajbfgflqihbflajhflejrhglijrndfljadnflgjbwqelrhigb;ajdnflh");
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

    if(key1 == true && key2 == true && key7 == true){
            if(player[clientID].turn1>0){
            double a;
            a = -2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 - (M_PI / 180);

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
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
        }
    else if(key2 == true && key3 == true&&key7 == true){
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
        if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
    }
    else if(key1 == true && key4 == true && key7 == true){
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
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
    }
    else if(key3 == true && key4 == true && key7 == true){
        if(player[clientID].turn1<0){
            double a;
            a = 2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 + (M_PI / 180);
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
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
    }
    
    else if(key1 == true && key2 == true){
            if(player[clientID].turn1>0){
            double a;
            a = -2 * M_PI + player[clientID].turn1;
            player[clientID].turn1 = a;
        }
        player[clientID].turn1 = player[clientID].turn1 - (M_PI / 180);

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
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        }
    else if(key2 == true && key3 == true){
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
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
    }
    else if(key1 == true && key7 == true){
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
         if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
       }


   else if(key2 == true&&key7 == true){
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
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
         if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
       }
    
    else if(key3 == true&&key7 == true){
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
         if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
       }


    else if(key4 == true&&key7==true){
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
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
         if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
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
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        
       }
    else if(key7 == true){
        if(can_attack == true){
            if(bullet_Num >= MAX_BULLET_NUM){bullet_Num = 0;}
            create_bullet(bullet_Num);
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
    }

     player[clientID].pos.x = player[clientID].pos.x-sin(player[clientID].turn1)*cos(player[clientID].turn2)*0.1;
         player[clientID].pos.z =player[clientID].pos.z-cos(player[clientID].turn1)*cos(player[clientID].turn2)*0.1;
         player[clientID].pos.y = player[clientID].pos.y - sin(player[clientID].turn2)*0.1;
         player[clientID].collider.pos = player[clientID].pos;
         
      
        //printf("%f\n",player[clientID].pos.x);
        
       if(key6==false){
         key1 = false;
         key2 = false;
         key3 = false;
         key4 = false;
         key5 = false;
         key6 = false;
         key7 = false;
       }
        if(key6 == true){
       // key6 = false;
       
       }

       
       
}

/***********************************************************
|  ???????????myInit()
|  ????????????????????ï¿??????????????????????????ï¿½ï¿½??ï¿???????????????
|  ï¿??????????char *windowTitle      ?????????ï¿????????????ï¿???????????????????????????????????????ï¿??????
|  ??ï¿??????????????
***********************************************************/
void myInit(char *windowTitle)
{
    /* ?????????ï¿????????????ï¿???????? */
    int winWidth  = WINDOW_WIDTH;
    int winHeight = WINDOW_HEIGHT;
    /* ?????????ï¿??????????????ï¿??????ï¿?????????????? */
    float aspect = (float)winWidth / (float)winHeight;
    // window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

    /*ï¿?????????????????????????????????main*/

    /* OpenGL?????????ï¿???????ï¿????????????????????????? */
    glutInitWindowPosition(0, 0);            /* ?????????ï¿??????????????????????? */
    glutInitWindowSize(winWidth, winHeight); /* ?????????ï¿?????????ï¿???????? */
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);   /* ?????ï¿???????????? */

    glutCreateWindow(windowTitle);                      /* ?????????ï¿?????????????????? */
    glClearColor(0.0, 0.0, 0.0, 1.0);                   /* ?????????????????????????????? */
    

    /* ?????????ï¿??????ï¿??????????????????????????????????????????? */
    glutKeyboardUpFunc(keyboard2);
    glutKeyboardFunc(keyboard);  /* ?????????????????????????????????? */
    glutReshapeFunc(resize);
    glutDisplayFunc(display);    /* ????????ï¿½ï¿½?????? */
    glutTimerFunc(15, timer, 0); /* ??ï¿???????????????15??????????ï¿?????????????? */

    /* CG???????ï¿½ï¿½??ï¿??? */
    glMatrixMode(GL_PROJECTION);             /* ?????????ï¿???(??????????ç¶µæŽ©??)??????????????????????????????ï¿???? */
    glLoadIdentity();                        /* ?????????ç¶µæ²¿????????????????? */
    gluPerspective(45.0, aspect, 1.0, 100.0); /* ?????????ç¶µæ²¿???????????????? */
                                             /* ???????????45ï¿???, ????ï¿???ï¿??? aspect?????????ï¿??????????????????ï¿??????? 1.0????????????????????????????ï¿??????? 20.0 */
    glEnable(GL_DEPTH_TEST);                 /* ??????????????????????ï¿?????????? */
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

    /* ???ï¿????ç¶ºï¿½?????????ï¿????????????????????ï¿??? */

    
}

void create_bullet(int num){
    BULLET b;
    b.dir = {-sin(player[clientID].turn1)*cos(player[clientID].turn2), - sin(player[clientID].turn2), -cos(player[clientID].turn1)*cos(player[clientID].turn2)};
    b.pos = player[clientID].pos + b.dir * 1.5f;
    b.lifetime = 0;
    array_bullet.push_back(BULLET(b));
    bullet_Num++;
    SendBulletDataCommand(num);
}

void draw_bullet(int num){
    for(int i = 0; i < num; i++){
        
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(array_bullet[i].pos.x, array_bullet[i].pos.y, array_bullet[i].pos.z);
        glutSolidSphere(BULLET_RADIUS, 200, 200);
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
            array_bullet.erase(array_bullet.begin() + i);       //  i??????????????????????????
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
    del_bullet();               // ç¶£ï¿½??????????
    if(bullet_Num > 0){
        glutTimerFunc(1000, add_lifetime, 0);
    }
    printf("bullet_Num:%d\n", bullet_Num);
}
