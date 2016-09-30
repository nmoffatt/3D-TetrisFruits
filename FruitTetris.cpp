#include <cstdlib>
#include <iostream>
#include <string>

#include "include/FruitTetris.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Detection of allowed movement
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// detects collision
bool noCollision()
{
	for (int i = 0; i < 4; i++)
	{
		int x = tilepos.x + tile[i].x;
		int y = tilepos.y + tile[i].y;
		if (x < 0 || x >= 10 || y < 0 || y >= 20 || board[x][y] == FULL)
			return false;
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//detects if in board
bool inBoard()
{
	for (int i = 0; i < 4; i++)
	{
		int x = tilepos.x + tile[i].x;
		int y = tilepos.y + tile[i].y;
		if (x < 0 || x >= 10 || y < 0 || y >= 20)
			return false;
		
		
	}

	return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//tile logic
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//when drawn or rotated updates position of tile

void updatetile()
{
	// Bind the VBO containing current tile vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

	// For each of the 4 'cells' of the tile,
	for (int i = 0; i < 4; i++)
	{
		// Calculate the grid coordinates of the cell
        	GLfloat x = tilepos.x + tile[i].x;
       		GLfloat y = tilepos.y + tile[i].y;

        	// Create the 4 corners of the square - these vertices are using location in pixels
        	// These vertices are later converted by the vertex shader
        	vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), 33.50, 1); // front left bottom
        	vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), 33.50, 1); // front left top
       		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), 33.50, 1); // front right bottom
        	vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), 33.50, 1); // front right top
        	vec4 p5 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), -33.50, 1); // back left bottom
        	vec4 p6 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), -33.50, 1); // back left top
        	vec4 p7 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), -33.50, 1); // back right bottom
        	vec4 p8 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), -33.50, 1); // back right top

        	// Two points are used by two triangles each
        	vec4 newpoints[36] = {	p1, p2, p3, p2, p3, p4,
            				p5, p6, p7, p6, p7, p8,
           				p1, p2, p5, p2, p5, p6,
            				p3, p4, p7, p4, p7, p8,
           				p2, p4, p6, p4, p6, p8,
            				p1, p3, p5, p3, p5, p7};

        	// Put new data in the VBO
        	glBufferSubData(GL_ARRAY_BUFFER, 36*i*sizeof(vec4), 36*sizeof(vec4), newpoints);
	}

	glBindVertexArray(0);
}

//-------------------------------------------------------------------------------------------------------------------
// Called at the start of play and every time a tile is placed
void newtile()
{

	int x,y;

	armPosition(x, y);
	tilepos = vec2(x,y);

	switch (rand() % 6) {
		case 0:
			currTileType = &allRotationsIshape;
			break;
		case 1:
			currTileType = &allRotationsLshape;
			break;
		case 2:
			currTileType = &allRotationsSshape;
			break;
		case 3:
			currTileType = &allRotationsTshape;
			break;
		case 4:
			currTileType = &allRotationsRSshape;
			break;
		case 5:
			currTileType = &allRotationsRLshape;
			break;
	}

	// Update the geometry VBO of current tile
	for (int i = 0; i < 4; i++)
		tile[i] = (*currTileType)[0][i]; // Get the 4 pieces of the new tile
	updatetile();

	//define colour of new tile

	for(int i = 0; i < 4; i++) 
	{
		vec4 col = allcols[rand() % 5];

		for (int j = 0; j < 6; ++j)
		{
			for (int k = 0; k < 6; ++k)
			{
				newcolours[(i * 6 + j) * 6 + k] = col; // You should randomize the color
				curColours[(i * 6 + j) * 6 + k] = col;
			}
		}
	}

	

	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);


	// Update the color VBO of current tile
	for(int i=0; i<4 ; i++)
	{
		int x = tilepos.x + tile[i].x;
		int y = tilepos.y + tile[i].y;
					
		if(y >= 20 || board[x][y] == FULL)
		{
			for (int j = 0; j < 6; ++j)
				for (int k = 0; k < 6; ++k)
				{
					newcolours[(i * 6 + j) * 6 + k] = lightgrey;
				}
		}
		else
		{
			for (int j = 0; j < 6; ++j)
				for (int k = 0; k < 6; ++k)
				{
					newcolours[(i * 6 + j) * 6 + k] = curColours[(i * 6 + j) * 6 + k];
				}
		}
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]); // Bind the VBO containing current tile vertex colours
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours); // Put the colour data in the VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}
	
	
	

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]); // Bind the VBO containing current tile vertex colours
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours); // Put the colour data in the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}
//-------------------------------------------------------------------------------------------------------------------------------------
// initiates tile
void initCurrentTile()
{
	glBindVertexArray(vaoIDs[2]);
	glGenBuffers(2, &vboIDs[4]);

	// Current tile vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	glBufferData(GL_ARRAY_BUFFER, 24 * 6 *sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// Current tile vertex colours
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferData(GL_ARRAY_BUFFER, 24 * 6 *sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);
}
//--------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
//robot arm logic
//-------------------------------------------------------------------------------------------------------------------

//draws the face of a cube
void drawFace(vec4 boardpoints[],int index,vec4 &p1,vec4 &p2,vec4 &p3,vec4 &p4){
    boardpoints[index 	 ] = p1;
    boardpoints[index + 1] = p2;
    boardpoints[index + 2] = p3;
    boardpoints[index + 3] = p2;
    boardpoints[index + 4] = p3;
    boardpoints[index + 5] = p4;
}
//---------------------------------------------------------------------------------------------------------------------
//drawscube
void drawCube(vec4* armPoints,int index,vec4 leftFront,vec4 leftBack,vec4 rightBack,vec4 rightFront,GLfloat height,GLfloat NGL)
{
    vec4 angle = vec4(height*cos(NGL),height*sin(NGL),0,0);
    vec4 p1 = leftFront;
    vec4 p2 = leftFront + angle;
    vec4 p3 = rightFront;
    vec4 p4 = rightFront + angle;
    vec4 p5 = leftBack;
    vec4 p6 = leftBack + angle;
    vec4 p7 = rightBack;
    vec4 p8 = rightBack + angle;


    drawFace(armPoints, index,      p1, p2, p3, p4); //front
    drawFace(armPoints, index + 6 , p5, p6, p7, p8); //back
    drawFace(armPoints, index + 12, p1, p2, p5, p6); //left
    drawFace(armPoints, index + 18, p3, p4, p7, p8); //right
    drawFace(armPoints, index + 24, p2, p4, p6, p8); //top
    drawFace(armPoints, index + 30, p1, p3, p5, p7); //bottom
}
//-----------------------------------------------------------------------------------------------------------------------------------
//keeps track of arm position

void armPosition(int &x, int &y)
{
	vec4 temp = upArmrightFront + point_offset(armLength1,theta1) + point_offset(armLength2, theta2);
	x = (temp.x - 33.0) / 33.0;
	y = (temp.y) / 33.0;
}

void armPosition( )
{
	int x,y;
	armPosition(x,y);
}
//-----------------------------------------------------------------------------------------------------------------------------------
//initiates the arm
void initArm() {

	vec4 uALF = upArmleftFront + point_offset(armLength1, theta1);
	vec4 uALB = upArmleftBack + point_offset(armLength1, theta1);
	vec4 uARB = upArmrightBack + point_offset(armLength1, theta1);
	vec4 uARF = upArmrightFront + point_offset(armLength1, theta1);

	//stand
    drawCube(armPoints, 0, stndleftFront,  stndleftBack,  stndrightBack,  stndrightFront, 33.0,M_PI/2);
	//lower arm
    drawCube(armPoints, 36, lowArmleftFront, lowArmleftBack, lowArmrightBack, lowArmrightFront, armLength1, theta1);
	//upper arm
    drawCube(armPoints, 72, uALF, uALB, uARB, uARF, armLength2, theta2);

    
    for (int i = 0; i < 36; i++) //stand
        armColors[i] = ARM_COLOR;

    for (int i = 36; i < 72; i++) //lower arm
        armColors[i] = ARM_COLOR;

    for (int i = 72; i < 108; i++) //upper arm
        armColors[i] = ARM_COLOR;


    // *** set up buffer objects
    // Set up first VAO (representing grid lines)
    glBindVertexArray(vaoIDs[armVAO]); // Bind the first VAO
    glGenBuffers(2, &vboIDs[armPosVBO]); // Create two Vertex Buffer Objects for this VAO (positions, colors)

    // Grid vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[armPosVBO]); // Bind the first grid VBO (vertex positions)
    glBufferData(GL_ARRAY_BUFFER, allPoints*sizeof(vec4), armPoints, GL_DYNAMIC_DRAW); // Put the grid points in the VBO
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition); // Enable the attribute

    
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[armColorVBO]); // Bind the second grid VBO (vertex colors)
    glBufferData(GL_ARRAY_BUFFER, allPoints*sizeof(vec4), armColors, GL_DYNAMIC_DRAW); // Put the grid colors in the VBO
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColor); // Enable the attribute

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
//---------------------------------------------------------------------------------------------------------------------------------
//updates arm
void updateArm(){
	if(hold)
	{
		int x,y;
		armPosition(x,y);
		tilepos = vec2(x,y);
		updatetile();
	}

    vec4 uALF  = upArmleftFront + point_offset(armLength1, theta1);
    vec4 uALB   = upArmleftBack + point_offset(armLength1, theta1);
    vec4 uARB  = upArmrightBack + point_offset(armLength1, theta1);
    vec4 uARF = upArmrightFront + point_offset(armLength1, theta1);


    drawCube(armPoints, 0, stndleftFront, stndleftBack, stndrightBack, stndrightFront, 33.0,M_PI/2);
    drawCube(armPoints, 36, lowArmleftFront, lowArmleftBack, lowArmrightBack, lowArmrightFront, armLength1, theta1);
    drawCube(armPoints, 72, uALF, uALB, uARB, uARF, armLength2, theta2);

    // *** set up buffer objects
    // Set up first VAO (representing grid lines)
    glBindVertexArray(vaoIDs[armVAO]); // Bind the first VAO
    glGenBuffers(2, &vboIDs[armPosVBO]); // Create two Vertex Buffer Objects for this VAO (positions, colors)

    // Grid vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[armPosVBO]); // Bind the first grid VBO (vertex positions)
    glBufferData(GL_ARRAY_BUFFER, allPoints*sizeof(vec4), armPoints, GL_DYNAMIC_DRAW); // Put the grid points in the VBO
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition); // Enable the attribute

	
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
//----------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//Grid logic
//-----------------------------------------------------------------------------------------------------------------------------

//initiated the grid
void initGrid()
{
	// ***Generate geometry data
	vec4 gridpoints[allPoints]; // Array containing the 64 points of the 32 total lines to be later put in the VBO
	vec4 gridcolours[allPoints]; // One colour per vertex

	// Vertical lines
    for (int i = 0; i < 11; i++){
        gridpoints[2*i]      = vec4((33.0 + (33.0 * i)), 33.0, 33.5, 1);
        gridpoints[2*i + 1]  = vec4((33.0 + (33.0 * i)), 693.0, 33.5, 1);
        gridpoints[2*i + 64] = vec4((33.0 + (33.0 * i)), 33.0, -33.5, 1);
        gridpoints[2*i + 65] = vec4((33.0 + (33.0 * i)), 693.0, -33.5, 1);
    }
	// Horizontal lines
    for (int i = 0; i < 21; i++){
        gridpoints[22 + 2*i] 		= vec4(33.0, (33.0 + (33.0 * i)), 33.5, 1);
        gridpoints[22 + 2*i + 1] 	= vec4(363.0, (33.0 + (33.0 * i)), 33.5, 1);
        gridpoints[22 + 2*i + 64]	= vec4(33.0, (33.0 + (33.0 * i)), -33.5, 1);
        gridpoints[22 + 2*i + 65] 	= vec4(363.0, (33.0 + (33.0 * i)), -33.5, 1);
    }
	// z direction lines
    for (int i = 0; i < BOARD_HEIGHT + 1; i++){
        for (int j = 0; j < BOARD_WIDTH + 1; j++) {
            gridpoints[128 + 22*i + 2*j] 		= vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), 33.5, 1); // front left bottom
            gridpoints[128 + 22*i + 2*j + 1] 	= vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), -33.5, 1); // back left bottom
        }
    }

	// Make all grid lines color
	for (int i = 0; i < allPoints; i++)
		gridcolours[i] = GRID_COLOR;


	// *** set up buffer objects
	// Set up first VAO (representing grid lines)
	glBindVertexArray(vaoIDs[0]); // Bind the first VAO
	glGenBuffers(2, vboIDs); // Create two Vertex Buffer Objects for this VAO (positions, colours)

	// Grid vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]); // Bind the first grid VBO (vertex positions)
	glBufferData(GL_ARRAY_BUFFER, allPoints*sizeof(vec4), gridpoints, GL_STATIC_DRAW); // Put the grid points in the VBO
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition); // Enable the attribute

	// Grid vertex colours
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]); // Bind the second grid VBO (vertex colours)
	glBufferData(GL_ARRAY_BUFFER, allPoints*sizeof(vec4), gridcolours, GL_STATIC_DRAW); // Put the grid colours in the VBO
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor); // Enable the attribute
}

//--------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
//Board logic
//---------------------------------------------------------------------------------------------------------------------------------
void initBoard()
{
	// *** Generate the geometric data
	vec4 boardpoints[BOARD_POINTS];
    for (int i = 0; i < BOARD_POINTS; i++)
        boardcolours[i] = BOARD_COLOR; // Let the empty cells on the board be black

    // Each cell is a square (2 triangles with 6 vertices)
    for (int i = 0; i < BOARD_HEIGHT; i++)
	{
        	for (int j = 0; j < 10; j++)
		{
            		vec4 p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), 33.5, 1);  //front-left-bottom
            		vec4 p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), 33.5, 1);  //front-left-top
           		vec4 p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), 33.5, 1);  //front-right-bottom
            		vec4 p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), 33.5, 1);  //front-right-top
            		vec4 p5 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), -33.5, 1); //back-left-bottom
            		vec4 p6 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), -33.5, 1); //back-left-top
            		vec4 p7 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), -33.5, 1); //back-right-bottom
            		vec4 p8 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), -33.5, 1); //back-right-top

            		int index = 36 * (BOARD_WIDTH * i + j);
			
           		drawFace(boardpoints, index, p1, p2, p3, p4); //front
			drawFace(boardpoints, index + 6 , p5, p6, p7, p8); //back
            		drawFace(boardpoints, index + 12, p1, p2, p5, p6); //left
            		drawFace(boardpoints, index + 18, p3, p4, p7, p8); //right
            		drawFace(boardpoints, index + 24, p2, p4, p6, p8); //top
            		drawFace(boardpoints, index + 30, p1, p3, p5, p7); //bottom
        	}
    	}

	// Initially no cell is occupied
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
			board[i][j] = false;

// *** set up buffer objects
	glBindVertexArray(vaoIDs[1]);
	glGenBuffers(2, &vboIDs[2]);

	// Grid cell vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, BOARD_POINTS * sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// Grid cell vertex colours
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, BOARD_POINTS * sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
 //main game logic section
//-----------------------------------------------------------------------------------------------------------------------------------------------
//initiation of all objects
void init()
{
	// Load shaders and use the shader program
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// Get the location of the attributes (for glVertexAttribPointer() calls)
	vPosition = glGetAttribLocation(program, "vPosition");
	vColor = glGetAttribLocation(program, "vColor");

	model_view = glGetUniformLocation( program, "model_view" );
    	projection = glGetUniformLocation( program, "projection" );

	// Create 3 Vertex Array Objects, each representing one 'object'. Store the names in array vaoIDs
	glGenVertexArrays(4, &vaoIDs[0]);

	// Initialize the grid, the board, and the current tile
	initArm();
	initGrid();
	initBoard();
	initCurrentTile();

	// The location of the uniform variables in the shader program
	locMVP = glGetUniformLocation(program, "MVP");

	// Board is now in unit lengths
    	vec4 eye = vec4(0, BOARD_HEIGHT*2, 1500,0);
    	vec4 at = vec4(0, BOARD_HEIGHT/2, 0,0);
    	View = LookAt(eye, at,vec4(0, 1, 0,0));


	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");

	// Game initialization
	newtile(); // create new next tile

	// set to default
	glBindVertexArray(0);
	glClearColor(1, 1, 1, 1);

	// Antialiasing
    	glEnable(GL_MULTISAMPLE);
    	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    	glEnable(GL_LINE_SMOOTH);
   	 glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    	glEnable(GL_POINT_SMOOTH);
    	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
}


//-------------------------------------------------------------------------------------------------------------------

//logic for detection and deletion of tiles
//-------------------------------------------------------------------------------------------------------------------
void Collapse(int x, int y)
{
	if (x < 0 || x >= 10 || y < 0 || y >= 20)
		return ;

	while (++y < 20)
	{
		for (int j = 0; j < 36; ++j)
		{
			boardcolours[(x + (y - 1) * 10) * 36 + j] = boardcolours[(x + y * 10) * 36 + j];
		}
		board[x][y - 1] = board[x][y];
	}

	for (int j = 0; j < 36; ++j)
	{
		boardcolours[(x + (y - 1) * 10) * 36 + j] = BOARD_COLOR;
	}
	board[x][y - 1] = NOT_FULL;

}

//-------------------------------------------------------------------------------------------------------------------------
void checkSameColor(int x, int y)
{
	struct
	{
		int x,y;
		vec4 color;
	} arr[3];
	
	for (size_t i = 0; i < 3; i++)
	{
		bool allowed = true;
		for (size_t j = 0; j < 3; j++)
		{
			arr[j].x = x - 2 + i + j;
			arr[j].y = y;
			if (arr[j].x < 0 || arr[j].x > 9 || board[arr[j].x][y] == NOT_FULL)
			{
				allowed = false;
				break;
			}
			arr[j].color = showColor(arr[j].x, arr[j].y);
		}
		if (allowed)
		{
			if (arr[0].color == arr[1].color && arr[1].color == arr[2].color && arr[0].color != BOARD_COLOR)
			{
				for(int j = 0; j < 3 ; j++)
					Collapse(arr[j].x, arr[j].y);
			}
		}
	}
	for (int i = 0; i < 3; ++i) //check column
	{
		bool allowed = true;
		for (int j = 0; j < 3; ++j)
		{
			arr[j].x = x;
			arr[j].y = y - 2 + i + j;
			if (arr[j].y < 0 || arr[j].y > 19 || board[x][arr[j].y] == NOT_FULL)
			{
				allowed = false;
				break;
			}
			arr[j].color = showColor(arr[j].x, arr[j].y);
		}

		if (allowed)
		{
			if (arr[0].color == arr[1].color && arr[1].color == arr[2].color && arr[0].color != BOARD_COLOR)
			{
				for(int j = 3; j >= 0 ; j--)
					Collapse(arr[j].x, arr[j].y);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
void checkfullrow(int row)
{
	bool check = FULL;
	for (int i = 0; i < 10; i++)
		check = check && board[i][row];
	if (check == FULL)
	{
		for (int i = 0; i < 10; ++i)
		{
			Collapse(i,row);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------

// Places the current tile - update the board vertex colour VBO and the array maintaining occupied cells
void settile()
{
	hold = true;
	//cout << "settile" << endl;
	for (int i = 0; i < 4; i++)
	{
		// Calculate the grid coordinates of the cell
		int x = tilepos.x + tile[i].x;
		int y = tilepos.y + tile[i].y;
		board[x][y] = FULL;
		for (int j = 0; j < 36; j++) {
			boardcolours[(x + y * 10) * 36 + j] = newcolours[i * 36 + j];
		}
	}
	for (int ii = 0; ii < 4; ii++)
		for (int i = 0; i < 4; i++)
		{
			// Calculate the grid coordinates of the cell
			int y = tilepos.y + tile[i].y;
			checkfullrow(y);
		}

	for (int ii = 0; ii < 1; ii++)
		for (int i = 0; i < 4; i++)
		{
			// Calculate the grid coordinates of the cell
			int x = tilepos.x + tile[i].x;
			int y = tilepos.y + tile[i].y;
			checkSameColor(x, y);
		}

	//hit_bottom=true;

	//cout<<"did hit bottom"<<hit_bottom;
	glBindVertexArray(vaoIDs[1]);
	glGenBuffers(2, &vboIDs[2]);

	// Grid cell vertrex colours
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, BOARD_POINTS * sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);
	
	
}
//------------------------------------------------------------------------------------------------------

// Rotates the current tile, if there is room
void rotate()
{
    tIdx = (tIdx + 1) % 4; // Get the next rotation value
	vec2 temp[4];
	for (int i = 0; i < 4; i++)
	{
		temp[i] = tile[i];
        tile[i] = (*currTileType)[tIdx][i]; // Change the 4 cells of the tile to correspond to the new orientation
	}
	if(!noCollision())
	{
		for (int i = 0; i < 4; i++)
			tile[i] = temp[i];
		tIdx = (tIdx - 1) % 4;
	}
    updatetile(); // Update the current tile vertex position VBO
}
//------------------------------------------------------------------------------------------------------------
// Given (x,y), tries to move the tile x squares to the right and y squares down
// Returns true if the tile was successfully moved, or false if there was some issue
bool movetile(vec2 direction)
{
	if (hold)
		return false;
	
	vec2 p = tilepos;
	p += direction;
	for (int i = 0; i < 4; i++)
	{
		// Calculate the grid coordinates of the cell
		int x = p.x + tile[i].x;
		int y = p.y + tile[i].y;
		if ( x < 0 || x >= 10)
			return false;
		if (board[x][y] == FULL || y < 0 )
		{
			vec2 t = vec2(0, -1);
			if (t == direction)
				settile();
			return false;
		}
	}
	tilepos = p;
	updatetile();
	return true;
}

//-------------------------------------------------------------------------------------------------------------------

void fall()
{
	movetile(vec2(0,-1));
	glutPostRedisplay();
}
//---------------------------------------------------------------------------------------------------------------------

//Text functions
void drawText(const string str, GLdouble txt_X, GLdouble txt_Y) {
	glColor3f(1.0,0.0,1.0);

	glRasterPos2f(txt_X,txt_Y);

	int i;
	for(i=0; i< str.length(); i++);
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
}

// Starts the game over - empties the board, creates new tiles, resets line counters
void restart()
{
	init();
}
//-------------------------------------------------------------------------------------------------------------------

// Draws the game
void display()
{
	Projection = Perspective(45, 1.0*xsize/ysize, 1000, 15);//10
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	mat4 Model = mat4();

    	Model *= Translate(transX, transY , transZ);
    	Model *= Scale(1.0/33 , 1.0/33 , 1.0/33);

   	 mat4 mvp =  Projection *  Model * View;
	glUniformMatrix4fv(locMVP, 1, GL_TRUE, mvp);

	eTime=glutGet(GLUT_ELAPSED_TIME);
	
	dTime=eTime-lTime;

	//fall animation
	if(noCollision() && dTime>fallTime)
	{		
		fall();
		lTime=eTime;
	}

	//settile
	for(int i=0; i<4 ; i++)
	{
		int x = tilepos.x + tile[i].x;
		int y = tilepos.y + tile[i].y;
					
		if((board[x][y-1]==true && hold==false)||y==0)
		{
				settile();

				lTime= 0;
				eTime= 0;
				dTime= 0;

				fallTime= 500;
				tilepos.x = 5;
				tilepos.y = 19;

				newtile();
		}
	}

	//color tiles if in or out of board
	if(hold==true)
	{
		for(int i=0; i<4 ; i++)
		{
			int x = tilepos.x + tile[i].x;
			int y = tilepos.y + tile[i].y;
					
			if(y >= 20 || board[x][y] == FULL)
			{
				for (int j = 0; j < 6; ++j)
					for (int k = 0; k < 6; ++k)
					{
						newcolours[(i * 6 + j) * 6 + k] = lightgrey;
					}
			}
			else
			{
				for (int j = 0; j < 6; ++j)
					for (int k = 0; k < 6; ++k)
					{
						newcolours[(i * 6 + j) * 6 + k] = curColours[(i * 6 + j) * 6 + k];
					}
			}

			glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]); // Bind the VBO containing current tile vertex colours
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours); // Put the colour data in the VBO
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);
		}
	}
	
	

	glUniform1i(locxsize, xsize); // x and y sizes are passed to the shader program to maintain shape of the vertices on screen
	glUniform1i(locysize, ysize);

	glBindVertexArray(vaoIDs[1]); // Bind the VAO representing the grid cells (to be drawn first)
	glDrawArrays(GL_TRIANGLES, 0, BOARD_POINTS); // Draw the board (10*20*2 = 400 triangles)

	glBindVertexArray(vaoIDs[2]); // Bind the VAO representing the current tile (to be drawn on top of the board)
	glDrawArrays(GL_TRIANGLES, 0, 24 * 6); // Draw the current tile (8 triangles)

	glBindVertexArray(vaoIDs[0]); // Bind the VAO representing the grid lines (to be drawn on top of everything else)
	glDrawArrays(GL_LINES, 0, allPoints); // Draw the grid lines (21+11 = 32 lines)

	glBindVertexArray(vaoIDs[armVAO]); // Bind the VAO representing the current tile (to be drawn on top of the board)
    	glDrawArrays(GL_TRIANGLES, 0, allPoints); // Draw the current tile (8 triangles)

	//Text

	drawText("woot",1.0,1.0);

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_ALWAYS);
	glEnable( GL_DEPTH_TEST );

	glutSwapBuffers();
}

//-------------------------------------------------------------------------------------------------------------------

// Reshape callback will simply change xsize and ysize variables, which are passed to the vertex shader
// to keep the game the same from stretching if the window is stretched
void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;

	glViewport(0, 0, w, h);
}

//-------------------------------------------------------------------------------------------------------------------

// Handle arrow key keypresses
void special(int key, int x, int y)
{
	bool CTRLpress = 0;
	int mod_key = glutGetModifiers();
	if (mod_key != 0)
   	{
    		if (mod_key == GLUT_ACTIVE_CTRL)
	    	{
	    		CTRLpress=1;
	    	}
	}
    else if (CTRLpress)
    {
    	CTRLpress=0;
    }


    switch(key)
    {
        case GLUT_KEY_UP:
		if(CTRLpress)
			View *= RotateX(10); //move camera up
		else
            		rotate(); // Up key rotates the current tile
            break;
	case GLUT_KEY_DOWN:
		if(CTRLpress)
			View *= RotateX(-10); //move camera down
		else
			movetile(vec2(0,-1));
		break;
	case GLUT_KEY_LEFT:
		if(CTRLpress)
			View *= RotateY(-10);
		else
			movetile(vec2(-1,0));
		break;
	case GLUT_KEY_RIGHT:
		if(CTRLpress)
			View *= RotateY(10);
		else
			movetile(vec2(1,0));
		break;
    }
}

//-------------------------------------------------------------------------------------------------------------------

// Handles standard keypresses
void keyboard(unsigned char key, int x, int y)
{
	//vec4 temp[6];
	switch(key)
	{
		case 033: // Both escape key and 'q' cause the game to exit
		    exit(EXIT_SUCCESS);
		    break;
		case 'Q':
			exit (EXIT_SUCCESS);
			break;
		case 'q':
			exit (EXIT_SUCCESS);
			break;
		case 'r': // 'r' key restarts the game
			restart();
			break;
		case 32: //space
			if (inBoard() && noCollision()) {
				hold = false;
			};
			if (hold)
				updateArm();
			break;
		case 'a':
			theta1 += 0.1; updateArm(); 
			break;
        	case 'd':
			theta1 -= 0.1; updateArm(); 
			break;
        	case 'w':
			theta2 += 0.1; updateArm(); 
			break;
        	case 's':
			theta2 -= 0.1; updateArm(); 
			break;
		case 'p':
			armPosition();
			break;
		case 'u':
			transZ+=1.0;
			break;
		case 'o':
			transZ-= 1.0;
			break;
		case 'k':
			transY+= 1.0;
			break;
		case 'i':
			transY-= 1.0;
			break;
		case 'j':
			transX+= 1.0;
			break;
		case 'l':
			transX-= 1.0;
			break;
		case 'x':
			transX = 0;
			transY = BOARD_HEIGHT/2;
			transZ = 0;
			break;

	}
	glutPostRedisplay();
}
//-------------------------------------------------------------------------------------------------------------------

void idle(void)
{
	glutPostRedisplay();
}

//-------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_MULTISAMPLE | GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(xsize, ysize);
	glutInitWindowPosition(680, 178); // Center the game window (well, on a 1920x1080 display)
	glutCreateWindow("Fruit Tetris");
	#ifndef __APPLE__
		glewInit();
	#endif

	init();

	// Callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	//Object text{"WOOT"};

	glutMainLoop(); // Start main loop
	return 0;
}




