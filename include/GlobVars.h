#include "Angel.h"
// the index of current tile
#ifndef __GLOBVAR_H__
#define __GLOBVAR_H__


typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

//definitions
#define GRID_COLOR cyan
#define BOARD_HEIGHT 20
#define BOARD_COLOR white
#define BOARD_POINTS 7200 // (1200 * 6);
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define allPoints (((BOARD_WIDTH + BOARD_HEIGHT + 2) * 2 + (BOARD_WIDTH + 1) * (BOARD_HEIGHT + 1))*2)
#define NOT_FULL 0
#define FULL 1
#define ARM_COLOR grey;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

// xsize and ysize represent the window size - updated if window is reshaped to prevent stretching of the game
int xsize = 400;
int ysize = 720;

// current tile
vec2 tile[4]; // An array of 4 2d vectors representing displacement from a 'center' piece of the tile, on the grid
vec2 tilepos = vec2(5, 19); // The position of the current tile using grid coordinates ((0,0) is the bottom left corner)

// An array storing all possible orientations of all possible tiles
// The 'tile' array will always be some element [i][j] of this array (an array of vec2)
vec2 allRotationsLshape[4][4] =
	{{vec2(-1, -1), vec2(-1,0), vec2(0, 0), vec2(1,0)},
	{vec2(1, -1), vec2(0, -1), vec2(0, 0), vec2(0, 1)},
	{vec2(1, 1), vec2(1,0), vec2(0, 0), vec2(-1,  0)},
	{vec2(-1,1), vec2(0, 1), vec2(0, 0), vec2(0, -1)}};

vec2 allRotationsIshape[4][4] =
	{{vec2(-2, 0), vec2(-1, 0), vec2(0, 0), vec2(1, 0)},
	{vec2(0, -2), vec2(0, -1), vec2(0,0), vec2(0, 1)},
	{vec2(-2, 0), vec2(-1, 0), vec2(0, 0), vec2(1, 0)},
	{vec2(0, -2), vec2(0, -1), vec2(0,0), vec2(0, 1)}};

vec2 allRotationsSshape[4][4] =
	{{vec2(-1, -1), vec2(0, -1), vec2(0, 0), vec2(1, 0)},
	{vec2(1, -1), vec2(1, 0), vec2(0, 0), vec2(0, 1)},
	{vec2(-1, -1), vec2(0, -1), vec2(0, 0), vec2(1, 0)},
	{vec2(1, -1), vec2(1, 0), vec2(0, 0), vec2(0, 1)}};

vec2 allRotationsTshape[4][4] =
	{{vec2( 0, -1), vec2(-1,  0),vec2(0, 0), vec2( 1,  0)},
        {vec2( 1,  0), vec2( 0, -1), vec2(0, 0), vec2( 0,  1)},
        {vec2( 0,  1), vec2( 1,  0), vec2(0, 0), vec2(-1,  0)},
        {vec2(-1,  0), vec2( 0,  1), vec2(0, 0), vec2( 0, -1)}};
   
vec2 allRotationsRLshape[4][4] =
	{{vec2(-1,  1), vec2(-1,  0), vec2(0, 0), vec2( 1,  0)},
        {vec2(-1, -1), vec2( 0, -1), vec2(0, 0), vec2( 0,  1)},
        {vec2( 1, -1), vec2( 1,  0), vec2(0, 0), vec2(-1,  0)},
        {vec2( 1,  1), vec2( 0,  1), vec2(0, 0), vec2( 0, -1)}};

vec2 allRotationsRSshape[4][4] =
     	{{vec2( 1, -1), vec2(0, -1), vec2(0,  0), vec2(-1,  0)},
        {vec2( 1,  1), vec2(1,  0), vec2(0,  0), vec2( 0, -1)},
        {vec2(-1,  0), vec2(0,  0), vec2(0, -1), vec2( 1, -1)},
        {vec2( 0, -1), vec2(0,  0), vec2(1,  0), vec2( 1,  1)}};

// colors
vec4 purple = vec4(0.5, 0.0, 0.5, 1.0);
vec4 red    = vec4(1.0, 0.0, 0.0, 1.0);
vec4 yellow = vec4(1.0, 1.0, 0.0, 1.0);
vec4 green  = vec4(0.0, 1.0, 0.0, 1.0);
vec4 orange = vec4(1.0, 0.5, 0.0, 1.0);

vec4 white  = vec4(1.0, 1.0, 1.0, 0.0);
vec4 black  = vec4(0.0, 0.0, 0.0, 1.0);

vec4 grey   = vec4(0.3, 0.3, 0.3, 1.0);
vec4 lightgrey   = vec4( 0.658824 ,  0.658824,0.658824, 1.0);
vec4 cyan   = vec4(0.0, 1.0, 1.0, 1.0);

vec4 allcols[] = {purple, red, yellow, green, orange};
//board[x][y] represents whether the cell (x,y) is occupied
bool board[10][20];

//An array containing the colour of each of the 10*20*2*3 vertices that make up the board
//Initially, all will be set to black. As tiles are placed, sets of 6 vertices (2 triangles; 1 square)
//will be set to the appropriate colour in this array before updating the corresponding VBO
vec4 boardcolours[BOARD_POINTS];

// location of vertex attributes in the shader program
GLuint vPosition;
GLuint vColor;

//timing of falling pieces
int lTime=0;
int eTime=0;
int dTime=0;
int fallTime=500;
int spawnTime=500;

// locations of uniform variables in shader program
GLuint locxsize;
GLuint locysize;

// VAO and VBO
enum vaoIDs {

    gridVAO,
    boardVAO,
    tileVAO,
    armVAO,
    maxVAOID
};

enum vboIDs {
    gridPosVBO,
    gridColorVBO,
    boardPosVBO,
    boardColorVBO,
    tilePosVBO,
    tileColorVBO,
    armPosVBO,
    armColorVBO,
    maxVBOID
};

GLuint vaoIDs[maxVAOID]; // One VAO for each object: the grid, the board, the current piece
GLuint vboIDs[maxVBOID]; // Two Vertex Buffer Objects for each VAO (specifying vertex positions and colors, respectively)

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA colors
color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};
/*
GLfloat radius = 1.5;
GLfloat theta = 0.0;
GLfloat phi = 0.0;*/

mat4 Projection, View, Model;
GLuint locMVP;
int transX = 0;
int transY = BOARD_HEIGHT/2;
int transZ = 0;

const GLfloat  dr = 5.0 * DegreesToRadians;
GLuint  model_view;  // model-view matrix uniform shader variable location

GLfloat  lleft = -10.0, rright = 10;
GLfloat  bottom = -10.0, top = 10;
GLfloat  zNear = -1000.0, zFar = 1000.0;
GLuint  projection; // projection matrix uniform shader variable location

//arm logic
GLfloat theta1 = M_PI / 2;
GLfloat theta2 = M_PI / 4 ;
const GLfloat armLength1 = 420.0;
const GLfloat armLength2 = 400.0;

vec4 armPoints[allPoints];
vec4 armColors[allPoints];

vec4 stndleftFront = vec4(-160.0, 33.0,  60, 1);
vec4 stndleftBack  = vec4(-160.0, 33.0, -60, 1);
vec4 stndrightBack = vec4(-40.0 , 33.0, -60, 1);
vec4 stndrightFront = vec4(-40.0, 33.0,  60, 1);

vec4 lowArmleftFront  = vec4(-110.0, 66.0,  10, 1);
vec4 lowArmleftBack   = vec4(-110.0, 66.0, -10, 1);
vec4 lowArmrightBack  = vec4(-90.0 , 66.0, -10, 1);
vec4 lowArmrightFront = vec4(-90.0, 66.0,  10, 1);

vec4 upArmleftFront = lowArmleftFront;
vec4 upArmleftBack  = lowArmleftBack + vec4(0, -20, 0, 0);
vec4 upArmrightBack = lowArmrightBack;
vec4 upArmrightFront= lowArmrightBack + vec4(0, -20, 0, 0);

inline vec4 point_offset(GLfloat length, GLfloat angle) {return vec4(length * cos(angle),length * sin(angle),0,0);}

vec4 curColours[24 * 6];
vec4 newcolours[24 * 6];
vec2 (*currTileType)[4][4];

bool hold = true;
bool hit_bottom=false;

int tIdx = 0;



#endif // __GLOBVAR_H__
