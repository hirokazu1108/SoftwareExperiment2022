#include <stdarg.h>
#include "client.h"
#include "trackball.h"
#include "glm.h"
int		gClientNum;
int		clientID;
Player *player;
Game game;
std::vector<ScoreBall> ary_scoreBall;
int scoreBallNum = 0;
bool firstRecvPlayerInfo = false;
int endFlag = 1; 
static float nameColor[MAX_CLIENTS][3] = {{1.0,0.0,0.0}, {0.0,0.0,1.0}, {1.0 , 0.8 , 0.0}, {0.0 , 0.4 , 0.0}};

ALuint buffer;
ALuint source;
ALuint buffer2;
ALuint source2;
/* use for network */
u_short port = PORT;
char	serverName[NAME_MAX_LENGTH+1];

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

int xBegin            = 0;    
int yBegin            = 0;    
float CameraX         = 0;  
float CameraY         = 1.0;  
float CameraZ         = 0.0;  
char buf[256];


int flag        = 106;
int j           = 0;
bool key1 = false;
bool key2 = false;
bool key3 = false;
bool key4 = false;
bool key5 = false;
bool key6 = false;
bool key7 = false;
bool key8 = false;
int bullet_Num = 0;  
bool can_attack = true;    
bool cameraflag = false;
double turn4;
double turn5;
float CameraX2         = 0; 
float CameraY2         = 1.0;  
float CameraZ2         = 0.0; 


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
void add_lifetime(int add_lifetimeID); 
void count_time(int count_timeID);        
void deleteBullet(int index);
void interval_attack(int interval_attackID);    
void Oval2D(float radius,int x,int y,float ovalx,float ovaly);
float calmini(int f,int pnum,float ppx, float epx,float ppy, float epy);
float calmap(int i);
#define TEX_HEIGHT 32
#define TEX_WIDTH 32
static GLubyte image[TEX_HEIGHT][TEX_WIDTH][4];
char *modelname[] = {"../object/player_red.obj","../object/player_blue.obj","../object/player_yellow.obj","../object/player_green.obj","../object/cloud.obj" ,"../object/under_cloud.obj" ,"../object/castle.obj","../object/laser.obj","../object/enemy.obj"} ;
int cloud_flag = 0;


int main(int argc, char **argv)
{
    int		endFlag=1;
	
    sprintf(serverName, "localhost");
    
    
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

    if(SetUpClient(serverName,port,&clientID,&gClientNum,game.clientName)==-1){
        fprintf(stderr,"setup failed : SetUpClient\n");
        WriteMatchFile(-1);
        exit(1);
    }
    game.state = State_Play;
    PlayerInit(); 
    AudioInit(&argc,argv);
	
    
    glutInit(&argc, argv); /* OpenGL init */
    myInit(game.clientName[clientID]);       

    for (int i = 0; i < Model_Num; i++){
        model_file[i] = modelname[i];
        if (!model_file[i]) {
        fprintf(stderr, "usage: smooth model_file.obj\n");
        exit(1);
        }
        init(i);
    }

    glutMainLoop();

    printf("exit in client_main.cpp/main().\n");
    return (0);
}


void display(void)
{

    /* netwaork*/
    if((endFlag = SendRecvManager()) != 1)
    {
        ExitClientProgram(endFlag);
    }
    SendPlayerDataCommand(); //PlayerData

    if(player[clientID].mp <= MAX_MP && player[clientID].isspecial == false){
        player[clientID].mp += 0.1f +player[clientID].parm[PARM_MP]*0.001;
    }
    if(player[clientID].isBarrier > 0.0f){
        player[clientID].isBarrier -= 0.01f;
        if(player[clientID].isBarrier <= 0.0f){
            player[clientID].isspecial = false;
        }
    }
    else{
        player[clientID].isBarrier = 0.0f;
    }

    if(player[clientID].isSpecial > 0.0f){
        player[clientID].isSpecial -= 0.01f;
        if(player[clientID].isSpecial <= 0.0f){
            player[clientID].isspecial = false;
        }
    }
    else{
        player[clientID].isSpecial = 0.0f;
    }
    
    // death
    if(player[clientID].anim > 0.0f){
        player[clientID].anim -= 1.0f;
    }
    else if(!player[clientID].enabled){
        player[clientID].anim = 0.0f;
        Respawn();
    }
	
    int i;
    float color[4] = {1.0};
    //move();
    if(flag ==0){
        CameraX = player[clientID].pos.x +sin(player[clientID].turn1)*5 *cos(player[clientID].turn3);
        CameraY = player[clientID].pos.y + sin(player[clientID].turn2)*5;
        CameraZ = player[clientID].pos.z +cos(player[clientID].turn1)*5 *cos(player[clientID].turn3);
    }
    else{

        CameraX = player[clientID].pos.x /*+sin(turn*0.1f)*5 */ + sin(player[clientID].turn1)*5 *cos(player[clientID].turn3);
        CameraY = player[clientID].pos.y + sin(player[clientID].turn2)*5;
        CameraZ = player[clientID].pos.z /*+cos(turn*0.1f)*5 */+cos(player[clientID].turn1)*5 *cos(player[clientID].turn3); 
    }
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);                        
    glLoadIdentity();                                   

    
    // glMatrixMode(GL_PROJECTION);
    if(cameraflag){
        
       gluLookAt(CameraX2, CameraY2, CameraZ2, 
        player[clientID].pos.x , player[clientID].pos.y,player[clientID].pos.z ,        
        0, 0.5*cos(player[clientID].turn2), 0.0);       
    }
    else{
       
    gluLookAt(CameraX, CameraY, CameraZ, 
        player[clientID].pos.x, player[clientID].pos.y,player[clientID].pos.z,        
        0, 0.5*cos(player[clientID].turn2), 0.0);        
    }
    
     move();

    if (firstRecvPlayerInfo == true && cloud_flag == 0){
        for(int z=0; z<gClientNum; z++){
          
        }
                glmScale(model[4], 150.00);
                lists(4);
                glmScale(model[5], 60.00);
                lists(5);
                glmScale(model[6], 36.00);
                lists(6);
		        glmScale(model[7], 1000.00);
                lists(7);
                for (int p = 0; p< gClientNum; p++) {
                    glmScale(model[p],player[p].size);
                    lists(p);
                }
                cloud_flag = 1;
    }
   
    
    for (i = 0; i < gClientNum; i++) {
        if(player[i].enabled){
            glPushMatrix();           
            glColor3f(1.0, 1.0, 1.0);
            glScalef(1.0, 1.0, 1.0);
            glTranslatef(player[i].pos.x, player[i].pos.y,player[i].pos.z);

            glRotatef(player[i].turn1*57.5, 0, 1, 0);
            glRotatef(player[i].turn2*57.5*-1, 1, 0, 0);

            if(player[i].isSpecial > 0.0f && player[i].special == SPECIAL_DISABLE)
            {
                glEnable(GL_BLEND);
                glColor4f(1.0f,1.0f,1.0f,0.95f);
                glCallList(model_list[1]);
                glDisable(GL_BLEND);
            }
            else if(player[i].isSpecial > 0.0f && player[i].special == SPECIAL_TRANSFORM){
                glCallList(model_list[player[i].transformIndex]);
            }
            else{
                glCallList(model_list[i]);
            }
            
            glPopMatrix();

            
        }
        
    }
    
    for (int i= 0 ; i < 2; i++){
        for(int j=0; j < 2; j++){
            for (int k = 0; k < 2; k++){
                glPushMatrix(); 
                glTranslatef(i*350 - 175,j*350 - 175 ,k*350 - 175);
                glCallList(model_list[4]);
                glPopMatrix();
            }
        }
        
    }


    glPushMatrix(); 
    glTranslatef(0,0,0);
    glCallList(model_list[4]);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(0, -35,0);
    glCallList(model_list[5]);
    glPopMatrix();

    glPushMatrix(); 
    glCallList(model_list[6]);
    glPopMatrix();
	
    move_bullet();    
    draw_bullet();   
	
    moveScoreBall();
    drawScoreBall();

    for(int i=0; i<gClientNum; i++){
        if(player[i].isBarrier > 0.0f){
                glPushMatrix();           
                glEnable(GL_BLEND);
                glColor4f(0.8 , 0.0 , 0.8, 0.4); 
                glScalef(1.0, 1.0, 1.0);
                glTranslatef(player[i].pos.x, player[i].pos.y,player[i].pos.z);
                glutSolidSphere(BARRIER_RADIUS,12,12);
                glDisable(GL_BLEND);
                glPopMatrix();
        }

        //toumei
        if(player[i].isSpecial > 0.0f && player[i].special == SPECIAL_LINES){
            for(int j=0; j<scoreBallNum; j++){
                glPushMatrix(); 
                glDisable(GL_LIGHTING);
                glColor3f(1,0,0);
                glLineWidth(5);
                glBegin(GL_LINES);
                glVertex3f( player[i].pos.x, player[i].pos.y, player[i].pos.z );
                glVertex3f( ary_scoreBall[j].pos.x, ary_scoreBall[j].pos.y, ary_scoreBall[j].pos.z );
                glEnd();
                glEnable(GL_LIGHTING);
                glPopMatrix();
            }
        }

        //damageArea
        if(player[i].isSpecial > 0.0f && player[i].special == SPECIAL_DAMAGEAREA){
            glPushMatrix();           
                glEnable(GL_BLEND);
                glColor4f(1.0 , 0.0 , 0.0, 0.9); 
                glTranslatef(player[i].pos.x, player[i].pos.y,player[i].pos.z);
                glutSolidSphere(DAMEGEAREA_RADIUS,12,12);
                glDisable(GL_BLEND);
            glPopMatrix();
        }
	
        if(player[i].isSpecial >0.0f && player[i].special == SPECIAL_BEAM){
            for(int j=0; j<100; j++){
                glm::vec3 d = glm::vec3(3*player[i].dir.x*(j+1),3*player[i].dir.y*(j+1),3*player[i].dir.z*(j+1));
                glPushMatrix();           
                glEnable(GL_BLEND);
                glColor4f(1.0 , 0.0 , 0.0, 0.9); 
                glTranslatef(player[i].pos.x+d.x, player[i].pos.y+d.y, player[i].pos.z+d.z);
                glutSolidSphere(1.5,12,12);
                glDisable(GL_BLEND);
                glPopMatrix();
            }
        }
        
    }

    

    /* Draw ui(right up) */
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    uiSetting();
    // annmaku
    if(!player[clientID].enabled){
        float r=(200-player[clientID].anim)/100*3.0f;
        glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        Circle2D(r,0,0);
        glPopMatrix();
    }
    glPushMatrix();
        glColor3f(nameColor[clientID][0],nameColor[clientID][1],nameColor[clientID][2]);
        glTranslatef(0.70,1.8,0);
        DrawString(game.clientName[clientID], 0, 0);
    glPopMatrix();
    // HPbar
    Bar bar_hp(0.7f,1.65f,2.0f,1.75f);
    glPushMatrix();
        glColor3f(0.0,0.0,0.0);
        glTranslatef(0.50,1.68,0);
        DrawString("HP",0,0);
    glPopMatrix();
    glColor3f( 0.2f, 1.0f, 0.2f );
    glRectf( bar_hp.x1, bar_hp.y1, bar_hp.x1+bar_hp.w*player[clientID].hp/(float)MAX_HP, bar_hp.y2);
    glColor3f( 0.8f, 0.0f, 0.0f );
    glRectf(bar_hp.x1+bar_hp.w*player[clientID].hp/MAX_HP,bar_hp.y1,bar_hp.x2,bar_hp.y2);
    // MPbar
    Bar bar_mp(0.7f,1.5f,2.0f,1.6f);
    glPushMatrix();
        glColor3f(0.0,0.0,0.0);
        glTranslatef(0.50,1.53,0);
        DrawString("MP",0,0);
    glPopMatrix();
    glColor3f( 0.8 , 0.2 , 1.0);
    glRectf( bar_mp.x1, bar_mp.y1, bar_mp.x1+bar_mp.w*player[clientID].mp/(float)MAX_MP, bar_mp.y2);
    glColor3f( 0.0f, 0.0f, 0.0f );
    glRectf(bar_mp.x1+bar_mp.w*player[clientID].mp/MAX_MP,bar_mp.y1,bar_mp.x2,bar_mp.y2);
    //special text
    if(!player[clientID].isspecial && player[clientID].mp >=100){
        glPushMatrix();
            glColor3f(0.0,0.0,0.0);
            glTranslatef(0.65,1.30,0);
            DrawString("Special charged. Push 7 or 8!",0,0);
        glPopMatrix();
    }
    //  special mater
    if(player[clientID].isBarrier > 0.0f)
    {
        glPushMatrix();
            glColor3f(0.0,0.0,0.0);
            glTranslatef(0.70,1.20,0);
            DrawString("BARRIER GAGE",0,0);
        glPopMatrix();

        Bar bar_barrier(0.7f,1.05f,2.0f,1.15f);
        glColor3f( 0.4f, 0.4f, 1.0f );
        glRectf( bar_barrier.x1, bar_barrier.y1, bar_barrier.x1+bar_barrier.w*player[clientID].isBarrier/(float)MAX_BARRIER, bar_barrier.y2);
        glColor3f( 0.0f, 0.0f, 0.0f );
        glRectf(bar_barrier.x1+bar_barrier.w*player[clientID].isBarrier/(float)MAX_BARRIER,bar_barrier.y1,bar_barrier.x2,bar_barrier.y2);
    }
    else if(player[clientID].isSpecial > 0.0f){
        glPushMatrix();
            glColor3f(0.0,0.0,0.0);
            glTranslatef(0.70,1.20,0);
            DrawString("SPECIAL GAGE",0,0);
        glPopMatrix();

        Bar bar_special(0.7f,1.05f,2.0f,1.15f);
        float bar_pos;
        glColor3f( 0.4f, 0.4f, 1.0f );
        switch(player[clientID].special){
            case SPECIAL_DISABLE:
                bar_pos = bar_special.w*player[clientID].isSpecial/(float)MAX_DISABLE_TIME;
                break;
            case SPECIAL_LINES:
                bar_pos = bar_special.w*player[clientID].isSpecial/(float)MAX_LINES_TIME;
                break;
            case SPECIAL_DAMAGEAREA:
                bar_pos = bar_special.w*player[clientID].isSpecial/(float)MAX_DAMAGEAREA_TIME;
                break; 
            case SPECIAL_TRANSFORM:
                bar_pos = bar_special.w*player[clientID].isSpecial/(float)MAX_TRANSFORM_TIME;
                break;
            case SPECIAL_BEAM:
                bar_pos = bar_special.w*player[clientID].isSpecial/(float)MAX_BEAM_TIME;
                break;
        }
        glRectf( bar_special.x1, bar_special.y1, bar_special.x1+bar_pos, bar_special.y2);
        glColor3f( 0.0f, 0.0f, 0.0f );
        glRectf(bar_special.x1+bar_pos,bar_special.y1,bar_special.x2,bar_special.y2);
    }
    else if(player[clientID].isBigbullet>0){
        glPushMatrix();
    //uiSetting();
    glColor3f(0.0,0.0,0.0);
            glTranslatef(0.70,1.20,0);
            DrawString("REMAINING BULLETS",0,0);
        glPopMatrix();
    glPushMatrix();
    glColor3f(0.0,0.0,0.0);
            glTranslatef(0.70,1.20,0);
            DrawString("REMAINING BULLETS",0,0);
        glPopMatrix(); 
    //uiSetting();
    glColor3f(1.0f, 0.0f, 0.0f);
    for(int i=0; i<player[clientID].isBigbullet; i++){
        
        Circle2D(0.06f, 0.435f+0.425*i+0.3, 1.02f);
      
    }
  glPopMatrix();
    }
    else if(player[clientID].isChase>0){
        glPushMatrix();
    //uiSetting();
    glColor3f(0.0,0.0,0.0);
            glTranslatef(0.70,1.20,0);
            DrawString("REMAINING BULLETS",0,0);
        glPopMatrix();
    glPushMatrix();
    glColor3f(0.0,0.0,0.0);
            glTranslatef(0.70,1.20,0);
            DrawString("REMAINING BULLETS",0,0);
        glPopMatrix(); 
    //uiSetting();
    glColor3f(1.0f, 0.0f, 0.0f);
    for(int i=0; i<player[clientID].isChase; i++){
        
        Circle2D(0.06f, 0.435f+0.425*i+0.3, 1.02f);
      
    }
  glPopMatrix();
    }
    
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();

    /* Draw ui(down) */
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    uiSetting();
    int draw_hpPos =0;
    for(int i=0; i<gClientNum; i++){
        if(i == clientID){
            continue;
        }
        glPushMatrix();
        glColor3f(nameColor[i][0],nameColor[i][1],nameColor[i][2]);
        glTranslatef(-2.12, 0.75f-draw_hpPos*0.34,0);
        DrawString(game.clientName[i], 0, 0);
        glPopMatrix();
        // HP bar
        Bar bar_hp(-2.15f,0.60f-draw_hpPos*0.34f,-1.3f,0.70f-draw_hpPos*0.34f);
        glPushMatrix();
            glColor3f(0.0,0.0,0.0);
            glTranslatef(-2.3,0.61f-draw_hpPos*0.34f,0);
            glScalef(0.1f,0.1f,0.1f);
            DrawString("HP",0,0);
        glPopMatrix();
        glPushMatrix();
            glColor3f( 0.2f, 1.0f, 0.2f );
            glRectf( bar_hp.x1, bar_hp.y1, bar_hp.x1+bar_hp.w*player[i].hp/(float)MAX_HP, bar_hp.y2);
            glColor3f( 0.8f, 0.0f, 0.0f );
            glRectf(bar_hp.x1+bar_hp.w*player[i].hp/MAX_HP,bar_hp.y1,bar_hp.x2,bar_hp.y2);
        glPopMatrix();
        draw_hpPos+=1;
    }
    
    char uiText[32];

    glPushMatrix();
        glColor3f( 1.0f, 1.0f, 1.0f );
        glRectf( -2.3,-1.4-(gClientNum-1)*0.2,-1.2,-0.9);
    glPopMatrix();
    glPushMatrix();
        glColor3f( 0.0f, 0.0f, 0.0f );
        glTranslatef(-1.9,-1.05,0.0);
        DrawString("SCORE", 0, 0);
    glPopMatrix();
    for(int i=0; i<gClientNum; i++){
        int digit = 0;
        int nn = player[i].score;
        do{
            nn /= 10;
            digit++;
        }while(nn != 0);
        glPushMatrix();
            glColor3f(nameColor[i][0],nameColor[i][1],nameColor[i][2]);
            glTranslatef(-2.15, -1.3f-i*0.19,0);
            DrawString(game.clientName[i], 0, 0);
        glPopMatrix();
        glPushMatrix();
            sprintf(uiText,(player[i].score>=0) ? "  %.0f" : "%.0f",player[i].score);
            glColor3f(nameColor[i][0],nameColor[i][1],nameColor[i][2]);
            glTranslatef(-1.45-(digit-1)*0.04f, -1.3f-i*0.19, 0);
            DrawString(uiText, 0, 0);
        glPopMatrix();
    }
    
    

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();

 
    if(key8){
        glPushMatrix();
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        uiSetting();
        glColor3f(1.0f,1.0f,1.0f);
        glRectf(-1,-1,1,1);
        glColor3f(0.0f,0.0f,0.0f);
        Circle2D(0.03,0,0);
        for(int i = 0; i<gClientNum; i++){
            glColor3f(nameColor[i][0],nameColor[i][1],nameColor[i][2]);
            if(player[i].isSpecial > 0.0f && player[i].special == SPECIAL_DISABLE){
            }
            else {
                Circle2D(0.03,player[i].pos.x/250,player[i].pos.z*-1/250);
            }
        }
	for(int i=0; i<scoreBallNum; i++){
            glColor3f(0.6 , 0.0 , 1.0);
            Circle2D(0.02,ary_scoreBall[i].pos.x/250,ary_scoreBall[i].pos.z*-1/250);
        }
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
    }

    
      glPushMatrix();
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        uiSetting();
        glColor3f(1.0f,1.0f,1.0f);
        glRectf(-2.3,1.0,-1.3,2.0);
        
        glColor3f(0.0f,0.0f,0.0f);

        //glLineWidth(10);
        glLineWidth(1);
        glBegin(GL_LINES);
		glVertex2f(-1.8,1.5);
		//glVertex2f(-1.8*sin(player[clientID].turn1),2.0);
        
        glVertex2f(-1.8 - sin(player[clientID].turn1)/2,1.5 + cos(player[clientID].turn1)/2);
        
		//glVertex2f(0.9 , 0.9);
	    glEnd();

        glBegin(GL_TRIANGLES);
                glColor3f(1.0, 0.0, 0.0);
                glVertex2f(-1.8,1.5); 
                glVertex2f(-0.4f, 0.4f); 
                glColor3f(1.0, 0.0, 1.0);
                glVertex2f(-0.2f, 0.0f); 
                glEnd();
                
        glPopMatrix();

       // glRectf(-1.805,1.5,-1.795,2.0);
        for(int i = 0; i<gClientNum; i++){
            glPushMatrix();
            uiSetting();
            glColor3f(nameColor[i][0],nameColor[i][1],nameColor[i][2]);
            if(i == clientID){
                Circle2D(0.03,-1.8f,1.5f);
                glPopMatrix();
            }
            else{
                if(player[i].isSpecial > 0.0f && player[i].special == SPECIAL_DISABLE){
                }
                else{
                    if(player[clientID].pos.y<player[i].pos.y){
                        Circle2D(0.03,calmini(1,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z),calmini(2,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z));
                        glPopMatrix();
                        glPushMatrix();
                        uiSetting();
                        glBegin(GL_TRIANGLES);
                        glColor3f(nameColor[i][0],nameColor[i][1],nameColor[i][2]);
                        glVertex2f(calmini(1,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z),calmini(2,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z) + 0.09); // (1.0, 0.0, 0.0) ???
                        glVertex2f(calmini(1,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z) - 0.06,calmini(2,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z) + 0.06);
                        glVertex2f(calmini(1,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z) + 0.06,calmini(2,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z) + 0.06); // (1.0, 0.0, 0.0) ???
                        glEnd();
                       
                    }
                    else{
                        Circle2D(0.03,calmini(1,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z),calmini(2,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z));
                        glPopMatrix();
                        glPushMatrix();
                        uiSetting();
                        glBegin(GL_TRIANGLES);
                        glColor3f(nameColor[i][0],nameColor[i][1],nameColor[i][2]);
                        glVertex2f(calmini(1,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z) - 0.06,calmini(2,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z) - 0.06);
                        glVertex2f(calmini(1,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z),calmini(2,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z) - 0.09); 
                        glVertex2f(calmini(1,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z) + 0.06,calmini(2,i,player[clientID].pos.x,player[i].pos.x,player[clientID].pos.z,player[i].pos.z) - 0.06);
                        glEnd();
                        
                    }
                }
                 glPopMatrix();
            }
        }

        
        // height
        glPushMatrix();
        uiSetting();
            char height[10];
            sprintf(height,"%.2f",player[clientID].pos.y);
            glTranslatef(-1.7, 1.1 ,0.0);
            DrawString(height,0,0);
        glPopMatrix();

        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();



    

    
    glFlush();
    
    glutSwapBuffers();
    
    
    if(player[clientID].enabled){
        Collider();
        checkDeath();
    }    
}


float calmap(int i){
    float map;
    if(i == 1){
        map = -1.8;
        if(player[clientID].turn1>45&&player[clientID].turn1<135){
            map = -2.3;
        }
        else if(player[clientID].turn1>225&&player[clientID].turn1<315){
            map = -2.3;
        }
        else if(player[clientID].turn1<-45&&player[clientID].turn1>-135){
            map = -2.3;
        }
        else if(player[clientID].turn1<-225&&player[clientID].turn1>-315){
            map = -2.3;
        }
        else{
            if(key3){
                map = map - 0.0111;
            }
            else if(key1){
                map = map + 0.0111;
            }
        }
    }
    else{
        map = 1.5;
        if(player[clientID].turn1>0&&player[clientID].turn1<45){
            map = 2.0;
        }
        else if(player[clientID].turn1>135&&player[clientID].turn1<180){
            map = 1.0;
        }
        else if(player[clientID].turn1<0&&player[clientID].turn1>-45){
            map = 2.0;
        }
        else if(player[clientID].turn1<-135&&player[clientID].turn1>-180){
            map = 1.0;
        }
        else{
            if(key3){
                map = map - 0.0111;
            }
            else if(key1){
                map = map + 0.0111;
            }
        }
    }
    return map;
}




float calmini(int f,int pnum,float ppx, float epx,float ppy, float epy){
    float point;
    float pointx;
    float pointy;
    float cosin;
    float sin;
    float numtiten;

    pointx = epx - ppx;
    pointy = epy - ppy;
    point = sqrt(pointx*pointx + pointy*pointy);
   

        if(f == 1){
            cosin = pointx/point;
            if(point > 0){
                numtiten =-1.8 + point*cosin/200;
            }
            else{
                numtiten =-1.8 + point*cosin/200;
            }
            if(numtiten<-2.35||numtiten>-1.28){
                if(numtiten<-2.35){
                    numtiten = -2.3;
                }
                else if(numtiten>-1.28){
                    numtiten = -1.3;
                }
            }
        }
        else{
            sin = pointy/point;
            if(point > 0){
                numtiten =1.5 - point*sin/200;
            }
            else{
                numtiten =1.5 + point*sin/200;
            }
            if(numtiten>2.0||numtiten<1.0){
                if(numtiten>2.0){
                    numtiten = 2.0;
                }
                else if(numtiten<1.0){
                    numtiten = 1.0;
                }
        }
        }
    
   
    return numtiten;
}


void Oval2D(float radius,int x,int y,float ovalx,float ovaly)
{
 for (float th1 = 0.0;  th1 <= 360.0;  th1 = th1 + 1.0)
 {             
  float th2 = th1 + 10.0;
  float th1_rad = th1 / 180.0 * M_PI; 
  float th2_rad = th2 / 180.0 * M_PI;

  float x1 = radius * cos(th1_rad)*(ovalx/100.0f);
  float y1 = radius * sin(th1_rad)*(ovaly/100.0f);
  float x2 = radius * cos(th2_rad)*(ovalx/100.0f);
  float y2 = radius * sin(th2_rad)*(ovaly/100.0f);
  glLineWidth(1);
  glBegin(GL_LINES);   
   glVertex2f( x1+x, y1+y );     
   glVertex2f( x2+x, y2+y );
  glEnd();
 }
}

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


void timer(int timerID)
{
    int i;
    glutTimerFunc(15, timer, 0);

    for (i = 0; i < 3; i++) {
    }
    glutPostRedisplay();   
}

void keyboard(unsigned char key, int x, int y)
{

        key6 = true;
     
     int xMove = x - xBegin;
     int yMove = y - yBegin;
   
    //esc
    if(key == '\033'){
        SendEndCommand();
    }
    if(key == 'l'){
        j++;
    }

    if(key == 'd'){
        key1 = true;
        if(cos(player[clientID].turn2)<0){
            key1 = false;
            key3 = true;
        }
       }

    if(key == 's'){
         flag = 2;
        key2 = true;
       }
    
    if(key == 'a'){
        flag = 0;
        key3 = true;
        if(cos(player[clientID].turn2)<0){
            key1 = true;
            key3 = false;
        }
       }


    if(key == 'w'){
        flag = 2;
        key4 = true;
       }
        if(key == 'b'){
          //  key5 = true;
         player[clientID].pos.x = player[clientID].pos.x-sin(player[clientID].turn1)*cos(player[clientID].turn2);
         player[clientID].pos.z =player[clientID].pos.z-cos(player[clientID].turn1)*cos(player[clientID].turn2);
         player[clientID].pos.y = player[clientID].pos.y - sin(player[clientID].turn2);
         player[clientID].collider.pos = player[clientID].pos;
	player[clientID].dir = glm::vec3(-sin(player[clientID].turn1)*cos(player[clientID].turn2), - sin(player[clientID].turn2), -cos(player[clientID].turn1)*cos(player[clientID].turn2));
      
        //printf("%f\n",player[clientID].pos.x);
       }

       if(key == ' '){
        key7 = true;
        alSourcePlay( source );
    } 
    if(key == 'l'){
        if(key8){
            key8 = false;
        }
        else{
            key8 = true;
        }
    }  
    
    if(key == 'n'){
        for(int i=0; i<gClientNum; i++)
            printf("score:%lf\n",player[i].score);
    }
      
    glutPostRedisplay();
    x = y = 0;
}

void keyboard2(unsigned char key, int x, int y)
{
    key6 = false;
     switch (key) {
    case '\033':
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

    case 'z':
        useSpecial();
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
            
            create_bullet();
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
        }
    else if(key2 == true && key3 == true&&key7 == true){
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
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
            create_bullet();
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
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        if(can_attack == true){
            create_bullet();
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
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        if(can_attack == true){
            create_bullet();
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
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
        }
    else if(key2 == true && key3 == true){
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
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
            create_bullet();
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
       }


   else if(key2 == true&&key7 == true){
         flag = 2;
        key2 = true;
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
        if(player[clientID].turn2 == 2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 - (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
         if(can_attack == true){
            create_bullet();
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
            create_bullet();
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
       }


    else if(key4 == true&&key7==true){
        flag = 2;
        key4 = true;
        if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
        if(player[clientID].turn2 == -2 * M_PI){
            player[clientID].turn2= 0;
        }
          player[clientID].turn3 = player[clientID].turn3 + (M_PI / 180);
        if(player[clientID].turn3 == 2 * M_PI){
            player[clientID].turn3= 0;
        }
         if(can_attack == true){
            create_bullet();
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
       }




    else if(key1 == true){
        flag = 0;
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
        key2 = true;
        if(player[clientID].turn2<0){
            double a;
            a = 2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 + (M_PI / 180);
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
        if(player[clientID].turn2>0){
            double a;
            a = -2 * M_PI + player[clientID].turn2;
            player[clientID].turn2 = a;
        }
        player[clientID].turn2 = player[clientID].turn2 - (M_PI / 180);
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
            create_bullet();
            glutTimerFunc(1000, add_lifetime, 0);
            can_attack = false;
            glutTimerFunc(1000, interval_attack, 0);
        }
    }

     float rate_vel = 1.0f;
    if(player[clientID].isSpecial > 0.0f && player[clientID].special == SPECIAL_DAMAGEAREA){
        rate_vel = (float)DAMAGEAREA_SPEED;
    }
    player[clientID].pos.x = player[clientID].pos.x-sin(player[clientID].turn1)*cos(player[clientID].turn2)*0.1*player[clientID].speed * rate_vel;
    player[clientID].pos.z =player[clientID].pos.z-cos(player[clientID].turn1)*cos(player[clientID].turn2)*0.1*player[clientID].speed * rate_vel;
    player[clientID].pos.y = player[clientID].pos.y - sin(player[clientID].turn2)*0.1*player[clientID].speed *rate_vel;
    player[clientID].collider.pos = player[clientID].pos;
	player[clientID].dir = glm::vec3(-sin(player[clientID].turn1)*cos(player[clientID].turn2), - sin(player[clientID].turn2), -cos(player[clientID].turn1)*cos(player[clientID].turn2));
      

      if(player[clientID].pos.z>WORLDSIZE_Z||player[clientID].pos.z<-WORLDSIZE_Z){
        player[clientID].pos.x =0;
        player[clientID].pos.y = 0;
        player[clientID].pos.z = 0;
      }
      if(player[clientID].pos.x>WORLDSIZE_X||player[clientID].pos.x<-WORLDSIZE_X){
        player[clientID].pos.x =0;
        player[clientID].pos.y = 0;
        player[clientID].pos.z = 0;
      }
      if(player[clientID].pos.y>WORLDSIZE_Y||player[clientID].pos.y<-WORLDSIZE_Y){
        player[clientID].pos.x =0;
        player[clientID].pos.y = 0;
        player[clientID].pos.z = 0;
      }
        
       if(key6==false){
         key1 = false;
         key2 = false;
         key3 = false;
         key4 = false;
         key5 = false;
         key6 = false;
         key7 = false;
       }
       if( player[clientID].pos.x >1000|| player[clientID].pos.x<-1000){
         player[clientID].pos.x = 0;
          player[clientID].pos.z = 0;
           player[clientID].pos.y = 0;
       }
        if( player[clientID].pos.z >1000|| player[clientID].pos.z<-1000){
         player[clientID].pos.x = 0;
          player[clientID].pos.z = 0;
           player[clientID].pos.y = 0;
       }
        if( player[clientID].pos.y >500|| player[clientID].pos.y<-500){
         player[clientID].pos.x = 0;
          player[clientID].pos.z = 0;
           player[clientID].pos.y = 0;
       }       
}

void joystick(unsigned int buttonMask, int x, int y, int z)
{
        if (x>0)
        {

                key1 = true;
                if(cos(player[clientID].turn2)<0){
            key1 = false;
            key3 = true;
        }
        }
        else if(x<0){
            key3  = true;
            if(cos(player[clientID].turn2)<0){
            key1 = true;
            key3 = false;
        }
        }
        if (y > 0)
        {
            key2 = true;
        }
        else if(y < 0){
            key4 = true;
        }
        if (z > 0)
        {
               cameraflag = true;
        turn4 = turn4 + (M_PI / 180);
        if(turn4 == 2 * M_PI){
            player[clientID].turn1 = 0;
            turn4 = 0;
        }
        }
        else if(z < 0){
            cameraflag = true;
        }
        else {
            cameraflag = false;
            turn4 = player[clientID].turn1;
            turn5 = player[clientID].turn3;
        }

        CameraX2 = player[clientID].pos.x +sin(player[clientID].turn1+ M_PI)*5 *cos(player[clientID].turn3/*+ M_PI*/);
        CameraY2 = player[clientID].pos.y + sin(player[clientID].turn2)*5;
        CameraZ2 = player[clientID].pos.z +cos(player[clientID].turn1+ M_PI)*5 *cos(player[clientID].turn3/*+ M_PI*/);

        switch (buttonMask) {
                case GLUT_JOYSTICK_BUTTON_A:
                    useSpecial();
               break;

                case GLUT_JOYSTICK_BUTTON_B:
                if(key8){
                    key8 = false;
                }
                else{
                    key8 = true;
                }
                break;
                case GLUT_JOYSTICK_BUTTON_C:
                    if(player[clientID].mp > 0){
                    player[clientID].pos.x = player[clientID].pos.x-sin(player[clientID].turn1)*cos(player[clientID].turn2);
                    player[clientID].pos.z =player[clientID].pos.z-cos(player[clientID].turn1)*cos(player[clientID].turn2);
                    player[clientID].pos.y = player[clientID].pos.y - sin(player[clientID].turn2);
                    player[clientID].collider.pos = player[clientID].pos;
                    player[clientID].dir = glm::vec3(-sin(player[clientID].turn1)*cos(player[clientID].turn2), - sin(player[clientID].turn2), -cos(player[clientID].turn1)*cos(player[clientID].turn2));
                    player[clientID].mp = player[clientID].mp - 1;
                    }
                break;
                case GLUT_JOYSTICK_BUTTON_D:
                    key7 = true;
                    
                break;
                case GLUT_JOYSTICK_BUTTON_E:
                 key7 = true;
                break;
                case GLUT_JOYSTICK_BUTTON_F:
                 key7 = true;
                break;
                case GLUT_JOYSTICK_BUTTON_G:
                    useSpecial();
                break;
                case GLUT_JOYSTICK_BUTTON_H:
                    useSpecial();
                break;



                
         }
}

void myInit(char *windowTitle)
{
    int winWidth  = 1100;
    int winHeight = 1000;
    float aspect = (float)winWidth / (float)winHeight;
    glutInitWindowPosition(0, 0);            
    glutInitWindowSize(winWidth, winHeight); 
    glutCreateWindow(windowTitle);                      
    glClearColor(0.658, 0.88, 0.925, 1.0);                   
    glutKeyboardUpFunc(keyboard2);
    glutKeyboardFunc(keyboard);  
    glutJoystickFunc(joystick,0);
    glutReshapeFunc(resize);
    glutDisplayFunc(display);    
    glutTimerFunc(15, timer, 0); 
    createScoreBall();
    glutTimerFunc(15000, PopEnemy, 0);
    glMatrixMode(GL_PROJECTION);            
    glLoadIdentity();                       
    gluPerspective(45.0, aspect, 1.0, 200.0);                                     
    glEnable(GL_DEPTH_TEST);                
    glClearColor(0.658, 0.88, 0.925, 1.0);     
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_WIDTH, TEX_HEIGHT,0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);    
}

void interval_attack(int interval_attackID){
    can_attack = true;
}

void add_lifetime(int add_lifetimeID){
    for(int i = 0; i < bullet_Num; i++){
        array_bullet[i].lifetime++;
    }  
    del_bullet();             
    if(bullet_Num > 0){
        glutTimerFunc(1000, add_lifetime, 0);
    }
}
