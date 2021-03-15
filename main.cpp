#include <GL/glut.h>
#include <stdio.h>
#include <iostream>

void reshape(int w, int h);
void display();
void drawPlayer1();
void drawPlayer2();
void inputFunc(int,int,int);
void drawGrid();
void keyBoardInput(unsigned char,int,int);
void drawAllBullets();

void checkHits();
int winner=0;

int gridSize=100;

GLint Player1X=2,Player1Y=10;
GLint Player2X=22,Player2Y=20;

int UP=1,DOWN=-1,LEFT=-2,RIGHT=2;

int Player1Dir=RIGHT;
int Player2Dir=LEFT;

int pOnebc=0;
int pTwobc=0;

bool isGameOver=false;

int Player1Health=100,Player2Health=100;
int bulletDmg=25;

struct bullet
{
   GLint thisX,thisY;
   int Bdir;
   GLfloat r=0.0,g=1.0,b=0.0;
   bool drawBullet=false;
};

struct bullet Player1Bullets[10],Player2Bullets[10];

int main(int argc,char**argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(500,0);
    glutCreateWindow("PVP Shooter");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(inputFunc);
    glutKeyboardFunc(keyBoardInput);

    glutMainLoop();
    return 0;

}


void drawString(float x, float y, void *font, char *str)
{
  char *c;
  glRasterPos2f(x,y);
  for (c=str; *c != '\0'; c++)
  {

    glutBitmapCharacter(font, *c);
  }
}


void drawHealthPts()
{
      char p1h[10];
      itoa(Player1Health,p1h,10);
      char msg1[25]="Player 1 health :";
      //strcat(msg1,p1h);

      char p2h[10];
      itoa(Player2Health,p2h,10);
      char msg2[25]="Player 2 health :";
      //strcat(msg2,p2h);
    glColor3f(1.0,1.0,1.0);
    drawString(102,75,GLUT_BITMAP_TIMES_ROMAN_10,msg1);
    glColor3f(1.0,1.0,1.0);
    drawString(102,65,GLUT_BITMAP_TIMES_ROMAN_24,p1h);
    glColor3f(1.0,1.0,1.0);
    drawString(102,45,GLUT_BITMAP_TIMES_ROMAN_10,msg2);
    glColor3f(1.0,1.0,1.0);
    drawString(102,35,GLUT_BITMAP_TIMES_ROMAN_24,p2h);

}

void reshape(int w, int h)
{
   // GLfloat wf=(GLfloat)w;
   // GLfloat hf=(GLfloat)h;

    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,120.0,0.0,100.0);
    glMatrixMode(GL_MODELVIEW);

}
void display()
{
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawGrid();
    drawPlayer1();
    drawPlayer2();
    drawAllBullets();
    checkHits();
    drawHealthPts();

    glFlush();
    glutSwapBuffers();
    if(isGameOver==true)
    {
        if(winner==1)
        {
            MessageBox(NULL,"Player 1 won ","Game Over",0);
        }
        else
        {
            MessageBox(NULL,"Player 2 won ","Game Over",0);
        }

        exit(0);
    }
}
void drawUnitSquare(int x, int y)
{
    GLfloat r=1.0,g=1.0,b=1.0;

    if(y==0||x==0||y==gridSize-1||x==gridSize-1)
    {
        r=1.0;g=0.0;b=0.0;

     glBegin(GL_LINE_LOOP);
     glColor3f(r,g,b);
     glVertex2i(x,y);
     glVertex2i(x+1,y);
     glVertex2i(x+1,y+1);
     glVertex2i(x,y+1);
     glEnd();
    }

}

void drawGrid()
{
    for(int i=0;i<gridSize;i++)
    {
        for(int j=0;j<gridSize;j++)
      {
          drawUnitSquare(i,j);
      }
    }
}

void drawPlayer1()
{
   glBegin(GL_POLYGON);
   glColor3f(0.0,1.0,0.0);
   glVertex2i(Player1X,Player1Y);
   glVertex2i(Player1X+1,Player1Y);
   glVertex2i(Player1X+1,Player1Y+1);
   glVertex2i(Player1X,Player1Y+1);
   glEnd();
}
void drawPlayer2()
{
   glBegin(GL_POLYGON);
   glColor3f(0.0,0.0,1.0);
   glVertex2i(Player2X,Player2Y);
   glVertex2i(Player2X+1,Player2Y);
   glVertex2i(Player2X+1,Player2Y+1);
   glVertex2i(Player2X,Player2Y+1);
   glEnd();
}
void movePlayer1(GLint x, GLint y)
{
   Player1X=x;
   Player1Y=y;
   glutPostRedisplay();
}
void movePlayer2(GLint x, GLint y)
{
   Player2X=x;
   Player2Y=y;
   glutPostRedisplay();
}
void inputFunc(int val,int,int)
{
    if(isGameOver!=true)
    {
        switch(val)
    {
    case GLUT_KEY_UP :
        if(Player2Y+1<gridSize-1)
        movePlayer2(Player2X,Player2Y+1);
        Player2Dir=UP;
        break;
    case GLUT_KEY_DOWN :
        if(Player2Y-1>0)
        movePlayer2(Player2X,Player2Y-1);
        Player2Dir=DOWN;
        break;
    case GLUT_KEY_LEFT :
        if(Player2X-1>0)
        movePlayer2(Player2X-1,Player2Y);
        Player2Dir=LEFT;
        break;
    case GLUT_KEY_RIGHT :
         if(Player2X+1<gridSize-1)
        movePlayer2(Player2X+1,Player2Y);
        Player2Dir=RIGHT;
        break;

    }
    }

}
void player1Fire()
{
    if(pOnebc>9)
    {
        pOnebc=0;
    }
    Player1Bullets[pOnebc].thisX=Player1X;
    Player1Bullets[pOnebc].thisY=Player1Y;
    Player1Bullets[pOnebc].Bdir=Player1Dir;
    Player1Bullets[pOnebc].drawBullet=true;
    Player1Bullets[pOnebc].r=0.8;
    Player1Bullets[pOnebc].g=0.5;
    Player1Bullets[pOnebc].b=0.1;
    pOnebc++;


}
void player2Fire()
{
    if(pTwobc>9)
    {
        pTwobc=0;
    }
    Player2Bullets[pTwobc].thisX=Player2X;
    Player2Bullets[pTwobc].thisY=Player2Y;
    Player2Bullets[pTwobc].Bdir=Player2Dir;
    Player2Bullets[pTwobc].drawBullet=true;
    Player2Bullets[pTwobc].r=0.1;
    Player2Bullets[pTwobc].g=0.5;
    Player2Bullets[pTwobc].b=0.8;
    pTwobc++;


}
void keyBoardInput(unsigned char key,int,int)
{

    if(isGameOver!=true)
    {

    if(key=='w')
   {
    if(Player1Y+1<gridSize-1)
    movePlayer1(Player1X,Player1Y+1);
    Player1Dir=UP;
   }
   if(key=='s')
   {
    if(Player1Y-1>0)
    movePlayer1(Player1X,Player1Y-1);
    Player1Dir=DOWN;
   }
   if(key=='a')
   {
    if(Player1X-1>0)
    movePlayer1(Player1X-1,Player1Y);
    Player1Dir=LEFT;
   }
   if(key=='d')
   {
    if(Player1X+1<gridSize-1)
    movePlayer1(Player1X+1,Player1Y);
    Player1Dir=RIGHT;
   }
   if(key=='f')
   {
       player1Fire();
   }
   if(key=='l'||key=='0')
   {
       player2Fire();
   }

    }


}
void drawBullet(GLint x,GLint y,GLfloat r,GLfloat g,GLfloat b)
{
    glBegin(GL_POLYGON);
     glColor3f(r,g,b);
     glVertex2i(x,y);
     glVertex2i(x+1,y);
     glVertex2i(x+1,y+1);
     glVertex2i(x,y+1);

    glEnd();
}
void drawAllBullets()
{
    for(int i=0;i<10;i++)
    {
        if(Player1Bullets[i].drawBullet==true)
        {
            if(Player1Bullets[i].thisX==0||Player1Bullets[i].thisX==gridSize-1||Player1Bullets[i].thisY==0||Player1Bullets[i].thisY==gridSize-1)
            {
                Player1Bullets[i].drawBullet=false;
            }
            else
            {
                if(Player1Bullets[i].Bdir==UP)
                {
                     Player1Bullets[i].thisY++;
                }
                else if(Player1Bullets[i].Bdir==DOWN)
                {
                     Player1Bullets[i].thisY--;
                }
                else if(Player1Bullets[i].Bdir==LEFT)
                {
                     Player1Bullets[i].thisX--;
                }
                else if(Player1Bullets[i].Bdir==RIGHT)
                {
                     Player1Bullets[i].thisX++;
                }
                drawBullet(Player1Bullets[i].thisX,Player1Bullets[i].thisY,Player1Bullets[i].r,Player1Bullets[i].g,Player1Bullets[i].b);
            }

        }
    }
     for(int i=0;i<10;i++)
    {
        if(Player2Bullets[i].drawBullet==true)
        {
            if(Player2Bullets[i].thisX==0||Player2Bullets[i].thisX==gridSize-1||Player2Bullets[i].thisY==0||Player2Bullets[i].thisY==gridSize-1)
            {
                Player2Bullets[i].drawBullet=false;
            }
            else
            {
                if(Player2Bullets[i].Bdir==UP)
                {
                     Player2Bullets[i].thisY++;
                }
                else if(Player2Bullets[i].Bdir==DOWN)
                {
                     Player2Bullets[i].thisY--;
                }
                else if(Player2Bullets[i].Bdir==LEFT)
                {
                     Player2Bullets[i].thisX--;
                }
                else if(Player2Bullets[i].Bdir==RIGHT)
                {
                     Player2Bullets[i].thisX++;
                }
                drawBullet(Player2Bullets[i].thisX,Player2Bullets[i].thisY,Player2Bullets[i].r,Player2Bullets[i].g,Player2Bullets[i].b);
            }

        }
    }
    glutPostRedisplay();
}
void checkHits()
{

    for(int i=0;i<10;i++)
    {
        if(Player1Bullets[i].thisX==Player2X&&Player1Bullets[i].thisY==Player2Y)
        {
            Player2Health-=bulletDmg;
             if(Player2Health<=0)
             {
                 isGameOver=true;
                 winner=1;
                 break;

             }


        }
    }
    for(int i=0;i<10;i++)
    {
        if(Player2Bullets[i].thisX==Player1X&&Player2Bullets[i].thisY==Player1Y)
        {
            Player1Health-=bulletDmg;
            if(Player1Health<=0)
            {
               isGameOver=true;
               winner=2;
               break;
            }

        }
    }
}
