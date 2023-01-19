#include "client.h"

/*UIの図形をこちらに向ける*/
void uiSetting(void){
    /*
        glPushMatrix();
    */
    glTranslatef(player[clientID].pos.x, player[clientID].pos.y,player[clientID].pos.z);
    glRotatef(player[clientID].turn1*57.5, 0, 1, 0);
    glRotatef(player[clientID].turn2*57.5*-1, 1, 0, 0);

    GLdouble m[16],l;
    glGetDoublev(GL_MODELVIEW_MATRIX, m);
    m[8] = -m[12];
    m[9] = -m[13];
    m[10] = -m[14];
    l = sqrt(m[8] * m[8] + m[9] * m[9] + m[10] * m[10]);
    m[8] /= l;
    m[9] /= l;
    m[10] /= l;
    
    m[0] = -m[14];
    m[1] = 0.0;
    m[2] = m[12];
    l = sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
    m[0] /= l;
    m[1] /= l;
    m[2] /= l;

    m[4] = m[9] * m[2] - m[10] * m[1];
    m[5] = m[10] * m[0] - m[8] * m[2];
    m[6] = m[8] * m[1] - m[9] * m[0];
    glLoadMatrixd(m);
    /* 
        この関数の後に 
        glRectf( 1, 1, 2,2);
        glPopMatrix();
    */
}


/* 参考　http://gameprogrammingunit.web.fc2.com/gl/bitmap_character.htm */
void DrawString(std::string str,int x0, int y0, void *font)
{

    // 画面上にテキスト描画
    glRasterPos2f(x0, y0);
    int size = (int)str.size();
    for(int i = 0; i < size; ++i){
        char ic = str[i];
        glutBitmapCharacter(font, ic);
    }

}

void drawScoreBall(void){
    for(int i=0; i<scoreBallNum; i++){
        glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(ary_scoreBall[i].pos.x, ary_scoreBall[i].pos.y, ary_scoreBall[i].pos.z);
        glCallList(model_list[8]);
        glPopMatrix();
    }
}
