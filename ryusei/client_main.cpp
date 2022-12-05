/* --------------------------------------------------------------------
 * gls04.c
 * 2�Ĥ�Ω���Τ��̡���ư����
 * -------------------------------------------------------------------- */

/* �إå��ե����� */
#include <stdarg.h>
#include "client.h"
#include "trackball.h"
#include "glm.h"

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

/* �������Х��ѿ� */
int xBegin            = 0;    /* �ޥ����ɥ�å��λ���X��ɸ */
int yBegin            = 0;    /* �ޥ����ɥ�å��λ���Y��ɸ */
int PressButton       = 0;    /* ���߲�����Ƥ���ޥ����ܥ����1:��,2:��,3:���� */
float CameraAzimuth   = 90.0; /* �����ΰ��֡����̳ѡ� */
float CameraElevation = 0.0;  /* �����ΰ��֡ʶĳѡ� */
float CameraDistance  = 5.0;  /* �����ΰ��֡ʸ�������ε�Υ�� */
float CameraX         = 0;  /* �����ΰ��֡�X��ɸ�� */
float CameraY         = 0.0;  /* �����ΰ��֡�Y��ɸ�� */
float CameraZ         = 0.0;  /* �����ΰ��֡�Z��ɸ�� */
float BoxX[3];                /* Ȣ��X��ɸ */
float BoxVx[3];               /* Ȣ��X������ư®�� */
float BoxY[3];
float BoxZ[3];
float turn = 0;
float turn2 = 0;
float turn3 = 0;
float turn4 = 0;
char left  = 0;
char right = 0;
char buf[256];
int kasoku;
float BoxRotate = 10.0;
int flag        = 0;
int flag2 = 1;
int j           = 0; // 2�ΤϤ䤵
int f           = 0; //���ΤϤ䤵
int ps          = 0;
double junp;
int junpf = 0;
bool key1 = false;
bool key2 = false;
bool key3 = false;
bool key4 = false;
bool key5 = false;
bool key6 = false;




int bf;
int jnk; //���祤��������ϥ��ޥ��
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


/* �ؿ��Υץ��ȥ�������� */

void display(void);
void timer(int timerID);
void keyboard(unsigned char key, int x, int y);
void resize(int w, int h);
void myInit(char *windowTitle);
void drawString3D(const char *str, float charSize, float lineWidth);
void init(void);
void text(GLuint x, GLuint y, GLfloat scale, char* format, ...);
void lists(void);
void move(void);
int joyconev();
#define TEX_HEIGHT 32
#define TEX_WIDTH 32
static GLubyte image[TEX_HEIGHT][TEX_WIDTH][4];

/***********************************************************
|  �ؿ���main()
|  �������ᥤ��ؿ�
|  ������int argc       �¹Ի������ο�
|  ������char** argv    �¹Ի����������ơ�ʸ���������
|  ���͡�int            0:���ｪλ
***********************************************************/
int main(int argc, char **argv)
{
    /* ����� */

    glutInit(&argc, argv); /* OpenGL �ν���� */
    myInit(argv[0]);       /* ������ɥ�ɽ������������ν���� */
    
    model_file = argv[1];
    if (!model_file) {
    fprintf(stderr, "usage: smooth model_file.obj\n");
    exit(1);
    }
    init();

    glutMainLoop();

    // glutReshapeFunc(reshape);
    /* ���٥�Ƚ����롼�� */

    /* �ץ�����ཪλ */
    return (0);
}


/**********************************************************
|  �ؿ���display()
|  �������֣���Ρץ���ե��å����襤�٥�Ƚ���
|  �������ʤ�
|  ���͡��ʤ�
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
        CameraX = BoxX[0] /*+sin(turn*0.1f)*5*/  +sin(turn)*5 *cos(turn3);
        CameraY = BoxY[0] + sin(turn2)*5;
        CameraZ = BoxZ[0] /*+cos(turn*0.1f)*5*/ +cos(turn)*5 *cos(turn3);
        
        
        }
        else{
        CameraX = CameraX;
        CameraY = CameraY;
        CameraZ = CameraZ;

        CameraX = BoxX[0] /*+sin(turn*0.1f)*5 */ + sin(turn)*5 *cos(turn3);
        CameraY = BoxY[0] + sin(turn2)*5;
        CameraZ = BoxZ[0] /*+cos(turn*0.1f)*5 */+cos(turn)*5 *cos(turn3);
        /*CameraX = BoxX[0] +sin(turn*0.1f)*5 *cos(turn3*0.1f);
        CameraY = BoxY[0] + sin(turn2*0.1f)*5;
        CameraZ = BoxZ[0] +cos(turn*0.1f)*5 *sin(turn3*0.1f);*/
        
        }
    /* ����� */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* ���̤�õ� */
    glMatrixMode(GL_MODELVIEW);                         /* ������������֤ʤ����ꤹ��˥⡼�� */
    glLoadIdentity();                                   /* �������������� */

    /* ���������� */
    // glMatrixMode(GL_PROJECTION);
        //gluLookAt(BoxX[0] +sin(turn*0.1f)*5, BoxY[0], BoxZ[0]+cos(turn*0.1f)*5, /* �����ΰ��� */
        //BoxX[0]/*+turn*0.1*/, 0 /*+turn*0.01*/,BoxZ[0]/*+turn*0.01f*//*+sin(turn/180)*/,                         /* �������ΰ��� */
        //0, 0.5/*+turn*0.5*/, 0.0); 

   
        gluLookAt(CameraX, CameraY, CameraZ, /* �����ΰ��� */
        BoxX[0], BoxY[0],BoxZ[0],                         /* �������ΰ��� */
        0, 0.5*cos(turn2), 0);
   
    /* Ω���Τ����� */
    for (i = 0; i < 100; i++) {
        glPushMatrix();           /* ������֤���¸ */
        glColor3f(1.0, 1.0, 1.0); /* ���迧����ˤ��� */
        glScalef(1.0, 1.0, 1.0);
         
        if (i < 1) {
            /* ������֤�(BoxX, i, 0)�˰�ư */
            // glutWireCube (0.5);
            if (i == 0) {
                glTranslatef(BoxX[0], BoxY[0], BoxZ[0]);
                if(flag == 2){
                    //glRotatef(turn*5.75, 0, 0, 1);
                    //flag = 0;
                }
                //glRotatef(turn2*-1*57.5, 1, 0, 0); 
               // glRotatef(turn3*57.5, 0, 1, 0);
             
               glRotatef(turn*57.5, 0, 1, 0);
               glRotatef(turn2*57.5*-1, 1, 0, 0);
            
               
            } else {
                glTranslatef(BoxX[1], 0, 2);
                glRotatef(j, 0, 0, 1.0);
            }
            
            glCallList(model_list);
             
            //glutSolidCube(1.0);
           // glutWireSphere(0.4, 20.0, 10.0);
        } else if (i == 2) {
            glTranslatef(BoxX[i], 0, -2);
            // glutWireCube (0.5);
            glRotatef(f, 0, 0, 1.0);

            glutWireSphere(0.4, 20.0, 10.0);
        } else {
            if (i % 2 == 0) {

                glTranslatef(-1 * i * 10, 0, 5);
                glColor3f(1.0, 0.0, 1.0); /* ���迧����ˤ��� */
                glScalef(2.0, 5.0, 0.5);
                glutSolidCube(1.0);
            } else {
                glTranslatef(-1 * i * 10, 0, -5);
                glColor3f(0.0, 1.0, 1.0); /* ���迧����ˤ��� */
                glScalef(2.0, 5.0, 0.5);
                glutSolidCube(1.0);
            }
            glPopMatrix(); /* ������֤��᤹ */
        }
    }

    //����ɽ��
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.8, 0.0);
    glScalef(1000.0, 0.1, 1.1);
    glutSolidCube(1.0);
    glPopMatrix();

    //Ũ����
        glColor3f(1.0, 1.0, 1.0); /* ���迧����ˤ��� */
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(0.0, -0.8, 1.5);
    glScalef(1000.0, 0.1, 1.8);
    glutSolidCube(1.0);
    glPopMatrix();

    //Ũ����
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(0.0, -0.8, -1.5);
    glScalef(1000.0, 0.1, 1.8);
    glutSolidCube(1.0);
    glPopMatrix();

    //�����뾲ɽ��
   

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

    // �㳲ʪ
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

    glPushMatrix();                    /* �������֤���¸ */
    glColor3f(1.0, 1.0, 0);            /* ���迧����(1.0,1.0,1.0)�ˤ��� */
    glRotatef(90, 0, 1.0, 0);          /* Y���濴��BoxRotate(��)��ž */
    glTranslatef(-0.18, 0.5, BoxX[0]); /* ʸ��ɽ����ɸ */
    
   

    glPushMatrix();
    {
        glTranslatef(-2.0, 0.0, -20.0);
        glRotatef(90, 0.0, 1.0, 0.0);
        glColor3f(1.0, 1.0, 1.0);
    }
    glPopMatrix();

    glFlush();
    /* �嵭�����褵�줿CG���˥����˽��� */
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

void 
text(GLuint x, GLuint y, GLfloat scale, char* format, ...)
{
  va_list args;
  char buffer[255], *p;
  GLfloat font_scale = 119.05 + 33.33;

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glTranslatef(x, y, 0.0);

  glScalef(scale/font_scale, scale/font_scale, scale/font_scale);

  for(p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  
  glPopAttrib();

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}


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

  /* generate a list */
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
    /* ������ɥ��νĲ������׻� */
    float aspect = (float)w / (float)h;
    /* ������ɥ����Τ�ӥ塼�ݡ��Ȥ��� */
    glViewport(0, 0, w, h);

    /* CG�������� */
    glMatrixMode(GL_PROJECTION);             /* Ʃ�����(������ˡ)����⡼�ɤ��ڤ��ؤ� */
    glLoadIdentity();                        /* Ʃ����ƹ�������� */
    gluPerspective(45.0, aspect, 1.0, 20.0); /* Ʃ����ƹ�������� */
                                             /* �����45��, �Ĳ��� aspect���������̤ޤǤα��� 1.0���������̤ޤǤα��� 20.0 */
    /* ��ǥ�ӥ塼�Ѵ���������� */
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
|  �����������ޡ���������ַв�˥��٥�Ƚ���
|  ������int timerID    ���٥�Ȥ�ȯ�����������ޡ��μ���ID
|  ���͡��ʤ�
***********************************************************/
void timer(int timerID)
{
    int i;

    /* ���Υ����ޡ���15�ߥ��ø������ */
    glutTimerFunc(15, timer, 0);

    for (i = 0; i < 3; i++) {
        /* Ȣ��X�����˰�ư */
        BoxX[i] += BoxVx[i];
        /* Ȣ��ü����ã�������ư������ȿž */
        // if( BoxX[i] > 22.0 || BoxX[i] < -22.0) BoxVx[i] = -BoxVx[i];
    }
    printf("  %lf  ", BoxVx[0]);
   

    /* �����׵��ľ��� display() �ؿ����ƤФ��� */
    glutPostRedisplay();
    // SDL_Delay(10);
}

void move(){
    if(key1 == true && key2 == true){
            printf("nanamekaitenn\n");
            if(turn>0){
            double a;
            a = -2 * M_PI + turn;
            turn = a;
        }
        turn = turn - (M_PI / 180);

        printf("zahyouhane~%lf\n",turn);
        if(turn == -2 * M_PI){
            turn = 0;
        }
        if(turn2<0){
            double a;
            a = 2 * M_PI + turn2;
            turn2 = a;
        }
        turn2 = turn2 + (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",turn2);
        if(turn2 == 2 * M_PI){
            turn2= 0;
        }
          turn3 = turn3 - (M_PI / 180);
        if(turn3 == 2 * M_PI){
            turn3= 0;
        }
        }
    else if(key2 == true && key3 == true){
        printf("hidarinanamekaitenn");
        // flag = 2;
        //turn2 = turn2-1;
       // key2 = true;
        if(turn2<0){
            double a;
            a = 2 * M_PI + turn2;
            turn2 = a;
        }
        turn2 = turn2 + (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",turn2);
        if(turn2 == 2 * M_PI){
            turn2= 0;
        }
          turn3 = turn3 - (M_PI / 180);
        if(turn3 == 2 * M_PI){
            turn3= 0;
        }

        if(turn<0){
            double a;
            a = 2 * M_PI + turn;
            turn = a;
        }
        turn = turn + (M_PI / 180);
        printf("zahyouhane~%lf\n",turn);
        if(turn == 2 * M_PI){
            turn = 0;
        }
    }
    else if(key1 == true && key4 == true){
         if(turn>0){
            double a;
            a = -2 * M_PI + turn;
            turn = a;
        }
        turn = turn - (M_PI / 180);

        printf("zahyouhane~%lf\n",turn);
        if(turn == -2 * M_PI){
            turn = 0;
        }

        if(turn2>0){
            double a;
            a = -2 * M_PI + turn2;
            turn2 = a;
        }
        turn2 = turn2 - (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",turn2);
        if(turn2 == -2 * M_PI){
            turn2= 0;
        }
          turn3 = turn3 + (M_PI / 180);
        if(turn3 == 2 * M_PI){
            turn3= 0;
        }
    }
    else if(key3 == true && key4 == true){
        if(turn<0){
            double a;
            a = 2 * M_PI + turn;
            turn = a;
        }
        turn = turn + (M_PI / 180);
        printf("zahyouhane~%lf\n",turn);
        if(turn == 2 * M_PI){
            turn = 0;
        }
         if(turn2>0){
            double a;
            a = -2 * M_PI + turn2;
            turn2 = a;
        }
        turn2 = turn2 - (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",turn2);
        if(turn2 == -2 * M_PI){
            turn2= 0;
        }
          turn3 = turn3 + (M_PI / 180);
        if(turn3 == 2 * M_PI){
            turn3= 0;
        }
    }


    else if(key1 == true){
        flag = 0;
        //key = true;
        if(turn>0){
            double a;
            a = -2 * M_PI + turn;
            turn = a;
        }
        turn = turn - (M_PI / 180);

        printf("zahyouhane~%lf\n",turn);
        if(turn == -2 * M_PI){
            turn = 0;
        }
       }


   else if(key2 == true){
         flag = 2;
        //turn2 = turn2-1;
        key2 = true;
        if(turn2<0){
            double a;
            a = 2 * M_PI + turn2;
            turn2 = a;
        }
        turn2 = turn2 + (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",turn2);
        if(turn2 == 2 * M_PI){
            turn2= 0;
        }
          turn3 = turn3 - (M_PI / 180);
        if(turn3 == 2 * M_PI){
            turn3= 0;
        }
       }
    
    else if(key3 == true){
        flag = 0;
        key3 = true;
        if(turn<0){
            double a;
            a = 2 * M_PI + turn;
            turn = a;
        }
        turn = turn + (M_PI / 180);
        printf("zahyouhane~%lf\n",turn);
        if(turn == 2 * M_PI){
            turn = 0;
        }
       }


    else if(key4 == true){
        flag = 2;
        key4 = true;
        //turn2 = turn2-1;
        
        if(turn2>0){
            double a;
            a = -2 * M_PI + turn2;
            turn2 = a;
        }
        turn2 = turn2 - (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",turn2);
        if(turn2 == -2 * M_PI){
            turn2= 0;
        }
          turn3 = turn3 + (M_PI / 180);
        if(turn3 == 2 * M_PI){
            turn3= 0;
        }
        
       }
       
       if(key6==false){
         key1 = false;
         key2 = false;
         key3 = false;
         key4 = false;
         key5 = false;
         key6 = false;
          printf("yobareteorimasutaishousann\n");
       }
        if(key6 == true){
       // key6 = false;
       
       }
       
}

/***********************************************************
|  �ؿ���keyboard()
|  �����������ܡ��ɤ������줿���Υ��٥�Ƚ���
|  ������unsigned char key  �����줿������ʸ��������
|  ������int x              �����������줿�Ȥ��Υޥ����ݥ��󥿤�X��ɸ
|  ������int y              �����������줿�Ȥ��Υޥ����ݥ��󥿤�Y��ɸ
|  ���͡��ʤ�
***********************************************************/


void keyboard(unsigned char key, int x, int y)
{

    //��ž��ɸ
     
        key6 = true;
     
     int xMove = x - xBegin;
     int yMove = y - yBegin;
   
  /*  if(key == NULL){
        bool key1 = false;
        bool key2 = false;
        bool key3 = false;
        bool key4 = false;
        bool key5 = false;
        bool key6 = false;
    }*/
    // int glfwGetKey('d');
    /* �����ܡ��ɽ��� */
    /*switch (key) {
    case 'q':
        exit(0);
        break;
    
    case 'd':
        flag = 0;
        if(turn>0){
            double a;
            a = -2 * M_PI + turn;
            turn = a;
        }
        turn = turn - (M_PI / 180);

        printf("zahyouhane~%lf\n",turn);
        if(turn == -2 * M_PI){
            turn = 0;
        }
        break;
    case 's':
        flag = 2;
        //turn2 = turn2-1;
        
        if(turn2<0){
            double a;
            a = 2 * M_PI + turn2;
            turn2 = a;
        }
        turn2 = turn2 + (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",turn2);
        if(turn2 == 2 * M_PI){
            turn2= 0;
        }
          turn3 = turn3 - (M_PI / 180);
        if(turn3 == 2 * M_PI){
            turn3= 0;
        }
       
        break;
    case 'a':
    flag = 0;
        if(turn<0){
            double a;
            a = 2 * M_PI + turn;
            turn = a;
        }
        turn = turn + (M_PI / 180);
        printf("zahyouhane~%lf\n",turn);
        if(turn == 2 * M_PI){
            turn = 0;
        }
      
        break;
    case 'w':
        flag = 2;
        //turn2 = turn2-1;
        
        if(turn2>0){
            double a;
            a = -2 * M_PI + turn2;
            turn2 = a;
        }
        turn2 = turn2 - (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",turn2);
        if(turn2 == -2 * M_PI){
            turn2= 0;
        }
          turn3 = turn3 + (M_PI / 180);
        if(turn3 == 2 * M_PI){
            turn3= 0;
        }
        
        break;
    case 'j':
        if(turn<0){
            double a;
            a = 2 * M_PI + turn2;
            turn2 = a;
        }
        turn2 = turn2 + (M_PI / 180);
        printf("zahyouhane~%lf\n",turn2);
        if(turn2 == 2 * M_PI){
            turn2 = 0;
        }
        break;
    default:
        junp  = 0;
        junpf = 0;
        //turn = 0;
        break;

        
    }*/
    if(key == 'q'){
      exit(0);
        
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
        //turn2 = turn2-1;
        key2 = true;
        /*if(turn2<0){
            double a;
            a = 2 * M_PI + turn2;
            turn2 = a;
        }
        turn2 = turn2 + (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",turn2);
        if(turn2 == 2 * M_PI){
            turn2= 0;
        }
          turn3 = turn3 - (M_PI / 180);
        if(turn3 == 2 * M_PI){
            turn3= 0;
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
        //turn2 = turn2-1;
        
        /*if(turn2>0){
            double a;
            a = -2 * M_PI + turn2;
            turn2 = a;
        }
        turn2 = turn2 - (M_PI / 180);
        //turn = turn -1;
        printf("zahyouhane~%lf\n",turn2);
        if(turn2 == -2 * M_PI){
            turn2= 0;
        }
          turn3 = turn3 + (M_PI / 180);
        if(turn3 == 2 * M_PI){
            turn3= 0;
        }*/
        
       }
        if(key == 'b'){
          //  key5 = true;
         BoxX[0] = BoxX[0]-sin(turn)*cos(turn2);
         BoxZ[0] =BoxZ[0]-cos(turn)*cos(turn2);
         BoxY[0] = BoxY[0] - sin(turn2);
      
        printf("%f\n",BoxX[0]);
       }
      
    glutPostRedisplay();
    x = y = 0;
   // key6=false;
}

//���祤����ν���
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
    default:
        break;
        
    }
    x = y = 0;
}

/***********************************************************
|  �ؿ���myInit()
|  ������������ɥ�ɽ������������ν����
|  ������char *windowTitle      ������ɥ��Υ����ȥ�С���ɽ������ʸ����
|  ���͡��ʤ�
***********************************************************/
void myInit(char *windowTitle)
{
    /* ������ɥ��Υ����� */
    int winWidth  = WINDOW_WIDTH;
    int winHeight = WINDOW_HEIGHT;
    /* ������ɥ��νĲ������׻� */
    float aspect = (float)winWidth / (float)winHeight;
    // window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

    /*�ʲ������󣰣���main*/

    /* OpenGL������ɥ������ޤǤν���� */
    glutInitWindowPosition(0, 0);            /* ������ɥ�ɽ������ */
    glutInitWindowSize(winWidth, winHeight); /* ������ɥ������� */
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);   /* ����⡼�� */

    glutCreateWindow(windowTitle);                      /* ������ɥ���ɽ�� */
    glClearColor(0.0, 0.0, 0.0, 1.0);                   /* ���̾õ������ */
    

    /* ���٥��ȯ�����˸ƤӽФ��ؿ�����Ͽ */
    glutKeyboardUpFunc(keyboard2);
    glutKeyboardFunc(keyboard);  /* �����ܡ��ɤ򲡤����� */
    //glutKeyboardUpFunc(keyboard2);
   // glutKeyboardFunc(keyboard2); 
    glutReshapeFunc(resize);
    glutDisplayFunc(display);    /* ����ɽ�� */
    glutTimerFunc(15, timer, 0); /* �����ޡ���15�ߥ��ø������ */

    /* CG�������� */
    glMatrixMode(GL_PROJECTION);             /* Ʃ�����(������ˡ)����⡼�ɤ��ڤ��ؤ� */
    glLoadIdentity();                        /* Ʃ����ƹ�������� */
    gluPerspective(90.0, aspect, 1.0, 20.0); /* Ʃ����ƹ�������� */
                                             /* �����45��, �Ĳ��� aspect���������̤ޤǤα��� 1.0���������̤ޤǤα��� 20.0 */
    glEnable(GL_DEPTH_TEST);                 /* ���̾õ��ͭ���ˤ��� */
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

    /* Ȣ�κ�ɸ��®�٤ν�������� */

    
}