#ifndef FRUITTETRIS_H
#define FRUITTETRIS_H

#include "Angel.h"
#include "GlobVars.h"

#include <unistd.h>

#define DEBUG_MODE true

// Include C header
// Include C++ header
// Include extern library
// Include project header

bool enoughRoom();
bool isInBoard();

void armPosition(int &x, int &y);
void armPosition();
void setFace(vec4 *boardpoints,int index,vec4 &p1,vec4 &p2,vec4 &p3,vec4 &p4);
void drawCuboid(vec4* armpoints,int index,vec4 leftFront,vec4 leftBack,vec4 rightBack,vec4 rightFront,float height,float angle);
void initArm();

void dragDown(int x, int y);
void checkSameColor(int x, int y);
inline vec4 showColor(int x, int y) {return boardcolours[(x + y * 10) * 36];}
void automove(int value);

void updatetile();
void newtile();
void initGrid();
void initBoard();
void initCurrentTile();
void init();
void rotate();
void checkfullrow(int row);
void settile();
bool movetile(vec2 direction);
void restart();
void display();
void reshape(GLsizei w, GLsizei h);
void special(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void idle(void);


#endif
