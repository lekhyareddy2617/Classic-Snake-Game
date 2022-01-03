#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define N 20 //rows
#define M 40 //columns

int i,j,playField[N][M];
int x,y; //(coordinates of snake's position)
int head,tail,temp;
int game,bait;
int dir,var;
int score,highscore;

FILE *fptr;

void snakeDefault() //default values of snake game i.e when game starts
{
    fptr=fopen("highscore.txt","r");
    fscanf(fptr,"%d",&highscore); //read file f for an integer and store it as highscore variable
    fclose(fptr);

    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            playField[i][j]=0;
        }
    }
    x=N/2;
    y=M/2;
    temp=y;
    head=5;
    tail=1;
    game=0;
    bait=0;
    dir='d'; //snake moving right by default
    score=0;
    //btwn 1 and 5 we have the body of the snake
    for(i=0;i<head;i++)
    {
        temp++;
        playField[x][temp-head]=i+1; //from playField[10][16] to playField[10][20] values are 1 to 5
    }
}

void random() //for random position of bait
{
    srand(time(0));
    int a,b;
    a=1+rand()%18; /*random number should be btwn 0 to 20 and 0 to 40 BuT we don't want the
    bait to be in corner positions so we keep the range till 18 and to make sure 0 doesn't
    occur, we add 1*/
    b=1+rand()%38;
    if(bait==0 && playField[a][b]==0) //check if bait is already there AND that it is being placed in the middle and not borders
    {
        playField[a][b]=-1; //so that we can use -1 to print bait
        bait=1;
    }
}

int ascii()
{
    if(_kbhit()) //if keyboard is hit
        return _getch(); //gets ascii of character hit
    else
        return -1;
}

void movement()
{
    var=ascii();
    var =tolower(var);
    if(((var=='a'||var=='d')||(var=='s'||var=='w'))&&abs(dir-var)>5)
        dir=var;
        /*difference in ascii values of A and D keys is less than 5 and that of S and W too is less than 5
        suppose a snake is moving in A key direction, if it wants to move in D key direction, it cannot directly change its direction
        first it have to turn to S or W key direction and then can move to D key direction so to limit snake not to move in these absurd directions,
        it is checked whether the ascii difference is greater than 5 or not*/
    if(dir=='a') //left
    {
        y--;
        if(playField[x][y]!=0 && playField[x][y]!=-1) //not empty space and no frog which means snake's head is over its own body
            coincide();
        if(y==0) //reaches border
            y=M-1;
        if(playField[x][y]==-1) //snake's head is above bait
        {
            bait=0; //so that after bait is consumed, new bait is generated
            score+=5;
            tail-=2; //increases snake length
        }
        head++;
        playField[x][y]=head;
    }
    if(dir=='s') //down
    {
        x++;
        if(playField[x][y]!=0 && playField[x][y]!=-1) //not empty space and no frog which means snake's head is over its own body
            coincide();
        if(x==N-1) //reaches border
            x=0;
        if(playField[x][y]==-1) //snake's head is above bait
        {
            bait=0;
            score+=5;
            tail-=2;
        }
        head++;
        playField[x][y]=head;
    }
    if(dir=='d') //right
    {
        y++;
        if(playField[x][y]!=0 && playField[x][y]!=-1) //not empty space and no frog which means snake's head is over its own body
            coincide();
        if(y==M-1) //reaches border
            y=0;
        if(playField[x][y]==-1) //snake's head is above bait
        {
            bait=0;
            score+=5;
            tail-=2;
        }
        head++;
        playField[x][y]=head;
    }
    if(dir=='w') //up
    {
        x--; //because 0,0 is assigned to the top left corner
        if(playField[x][y]!=0 && playField[x][y]!=-1) //not empty space and no frog which means snake's head is over its own body
            coincide();
        if(x==-1) //reaches border
            x=N-1;
        if(playField[x][y]==-1) //snake's head is above bait
        {
            bait=0;
            score+=5;
            tail-=2;
        }
        head++;
        playField[x][y]=head;
    }
}

void printSquare()
{
    //printf("Hello World\n"); square made when we select two adjacent characters
    //to create square as field of play, we take double the number of rows as columns
    //top line
    for(i=0;i<=M+1;i++)
    {
      if(i==0)
      {
          printf("%c",201);
      }
      else if(i==M+1)
      {

          printf("%c",187);
      }
      else
      {
          printf("%c",205);
      }
    }
    printf("   Current Score: %d  HighScore: %d",score,highscore);
    printf("\n");
    //side lines (columns) and middle portionn
    for(i=0;i<N;i++)
    {
        printf("%c",186);
        for(j=0;j<M;j++)
        {
            if(playField[i][j]==0)
                printf(" ");
            if(playField[i][j]>0 && playField[i][j]!=head)
                printf("%c",176);
            if(playField[i][j]==head)
                printf("%c",178);
            if(playField[i][j]==-1) //bait
                printf("%c",15);
            if(j==M-1)
                printf("%c\n",186);
        }
    }
    //bottom line
    for(i=0;i<=M+1;i++)
    {
      if(i==0)
      {
          printf("%c",200);
      }
      else if(i==M+1)
      {

          printf("%c",188);
      }
      else
      {
          printf("%c",205);
      }
    }
}

void resetScreen()
{
    HANDLE hOut;
    COORD position;
    hOut=GetStdHandle(STD_OUTPUT_HANDLE);
    position.X=0;
    position.Y=0;
    SetConsoleCursorPosition(hOut,position);
}

void tailUpdate() //updating tail length on moving
{
    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            if(playField[i][j]==tail)
            {
                playField[i][j]=0;
            }
        }
    }
    tail++;
}

void coincide() //to end game when the snake goes over its own body
{
    printf("\a"); //beep
    sleep(1500);
    system("cls");
    if(score>highscore)
    {
        printf("New High Score: %d!", highscore);
        system("pause");
        fptr=fopen("highscore.txt","w");
        fprintf(fptr,"%d",score); //old highscore erased and replaced
    }
    system("cls");
    printf("Game Over!");
    printf("\n");
    printf("Score %d\t",score);
    printf("High Score %d",highscore);
    game=1;
}

int main()
{
    snakeDefault();
    while(game==0) //while game is on
    {
        printSquare();
        resetScreen();
        random();
        movement();
        tailUpdate();
        //sleep(99);
    }
    return 0;
}
