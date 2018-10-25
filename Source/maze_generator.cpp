#include<iostream>
#include<stdlib.h>
#include<windows.h>
#include "rlutil.h"

void clearScreen(); 

using namespace std;

int main()
{
	/*Init size of maze*/
    int size;
    cout<<"Enter size of maze: ";
    cin>>size;
	int maze[size][size];

	bool finished = false;
    int unvisited_fields=0;

	/*Fill maze array with init values*/
    for (int i=0; i<size; i++)
    {
        for (int j=0; j<size; j++)
        {
            if (i%2==0 || j%2==0 || i==0 || j==0 || i==size-1 || j==size-1)
                maze[i][j] = 1;
            else
            {
                maze[i][j] = 2;
                unvisited_fields++;
            }
        }
    }

	/*Needed for random number*/
    srand(time(0));
	/*Init start point*/
    maze[1][1] = 0;
	
	/*Generate random maze*/
    while(!finished)
    {
        for (int i=1; i<size-1; i+=2)
        {
            for (int j=1; j<size-1; j+=2)
            {
                int r=rand() % 4 + 1;
                if(r==1 && i<size-2 && maze[i][j] == 0)
                {
                    if(maze[i+2][j]==2)
                    {
                        maze[i+1][j]=0;
                        maze[i+2][j]=0;
                        unvisited_fields--;
                    }
                }
                else if(r==2 && i>2 && maze[i][j] == 0)
                {
                    if(maze[i-2][j]==2)
                    {
                        maze[i-1][j]=0;
                        maze[i-2][j]=0;
                        unvisited_fields--;
                    }
                }
                else if(r==3 && j<size-2 && maze[i][j] == 0)
                {
                    if(maze[i][j+2]==2)
                    {
                        maze[i][j+1]=0;
                        maze[i][j+2]=0;
                        unvisited_fields--;
                    }
                }
                else if(r=4 && j>2 && maze[i][j] == 0)
                {
                    if(maze[i][j-2]==2)
                    {
                        maze[i][j-1]=0;
                        maze[i][j-2]=0;
                        unvisited_fields--;
                    }
                }
            }
        }
		/*Print maze*/ 

	Sleep(500);
        clearScreen();
	//system("clear");
        for (int i=0; i<size; i++)
        {
            for (int j=0; j<size; j++)
            {
                if (maze[i][j]==0)
                {
                    rlutil::setColor(15);
                    cout<<"  ";
                }
                else
                {
                    rlutil::setColor(14);
                    cout<<"||";
                }
            }
            cout<<endl;
        }
		
		/*Check if maze generation is done*/
        if (unvisited_fields==1)
            finished = true;
    }

    return 0;
}

void clearScreen()
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}


