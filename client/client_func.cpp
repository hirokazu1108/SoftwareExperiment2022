#include "client.h"

/* 球同士の当たり判定 */
bool OnColliderSphere(Sphere a, Sphere b){
  
  bool result = false;
  glm::vec3 diff = a.pos - b.pos;

  if(a.radius + b.radius > glm::length(diff)){
    result = true;
  }
  return result;
}

/* playerの当たり判定をだす */
void drawPlayerCollider(void){
    for(int i = 0; i < gClientNum; i++){
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(player[i].collider.pos.x, player[i].collider.pos.y, player[i].collider.pos.z);
        glutSolidSphere(player[i].collider.radius, 200, 200);
        glPopMatrix();
    }
}