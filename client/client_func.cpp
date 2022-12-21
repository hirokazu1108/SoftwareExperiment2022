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

/* 球の削除を行う */
void deleteBullet(int index){
    array_bullet.erase(array_bullet.begin() + index);       //  i??????????????????????????
    bullet_Num--;
}


/* 機体の当たり判定を取る */
void Collider(void){
    for(int i=0; i<gClientNum; i++){
        for(int j=0; j<bullet_Num;j++){
            if(OnColliderSphere(Sphere(BULLET_RADIUS,array_bullet[j].pos),player[i].collider)){
                Ability(array_bullet[j].shooter_id);
                printf("speed:%f\n" ,player[array_bullet[j].shooter_id].speed);
                player[i].hp -= DAMAGE * player[array_bullet[j].shooter_id].rate_attack;
                deleteBullet(j);
                printf("hirokazu: hit client[%d]\n",i);
            }
        }
    }
}

void Ability(int id){
    switch(player[id].ability){
        
        case UP_ATTACK:
        player[id].rate_attack += 0.1;
        break;

        case HEAL:
        player[id].hp += 1.0;
        break;

        case UP_SPEED:
        player[id].speed += 0.5;
        break;

        default:
        break;
    }
}