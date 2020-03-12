/*
 *  Random maze generator on console/terminal v2.0
 *  Algorithm is based on hunt and kill.
 *  Other algorithms used as reference: https://en.wikipedia.org/wiki/Maze_generation_algorithm
 *  
 *  Brief overview:
 *
 *  1. Make starting point,
 *  2. Pick field in random available direction until there is no available direction,
 *	(Available direction is reduced by boundary check or if there is no nearby unvisited fields)
 *  3. Loop through maze fields until you find first visited field that has available direction or
 *  nearby unvisited fields in one of 4 directions(up, down, right, left),
 *  4. Loop through 2 and 3 steps until there is no visited field.
 *
 *  Author: Muhamed Trumic muhamedtrumic97@gmail.com, March 12 2020.
 */

#include <iostream>
#include <random>
#include <chrono>
#include <string>
#ifdef _WIN32
#include<windows.h>
#endif

// Used for styling color of text
#include "rlutil.h"
 	
struct Size
{
	unsigned int height = 8;
	unsigned int width	= 10;
};

struct Position
{
	unsigned int row = 1;
	unsigned int col = 1;
};


void calculateDirection (const std::vector<std::vector<unsigned int>> &maze_fields, std::vector<std::string> &next_dir, const struct Position &pos, const struct Size &size, bool &is_stuck);
void printMaze(const std::vector<std::vector<unsigned int>> &maze_fields, const struct Size &size);
#ifdef _WIN32
void clearScreen();
#endif

int main()
{
	Size size;
	Position pos;
	
	bool is_finished = false;
	bool is_stuck = false;
	
	std::vector<std::vector<unsigned int>> maze_fields;
	
	// Init random number generator
	std::random_device rd;
    std::mt19937 mt(rd());
    
	// Init size of maze.
    std::cout<<"Enter size of maze: "<<std::endl;
    std::cout<<"Height = ";
    std::cin>>size.height;
    std::cout<<"Width = ";
    std::cin>>size.width;
    
    // Start clock to see how fast it generates random maze.
	auto start = std::chrono::high_resolution_clock::now(); 
    
    // Since we will store in one 2D array floors and walls this will calculate actual 
    // 2D array size.
    size.height = (size.height * 2) + 1;
    size.width = (size.width * 2) + 1;

	// Fill maze_fields with init values.
	// 0 - visited field,
	// 1 - wall,
	// 2 - unvisited field.
    for (unsigned int i=0; i<size.height; ++i)
    {
    	std::vector<unsigned int> columns;
        for (unsigned int j=0; j<size.width; ++j)
        {
        	
        	if (i % 2 == 0 || j % 2 == 0 || i == 0 || j == 0 || 
        			i == size.height - 1 || j == size.width - 1)
                columns.push_back(1);
            else
            	columns.push_back(2);
        }
        maze_fields.push_back(columns);
    }
    
	// Init start point for maze.
    maze_fields[pos.row][pos.col] = 0;
    
	// Lets generate random maze!
    while(!is_finished)
    {	
    	unsigned int unvisited_fields = 0;
    	
    	// Decleare empty vector that will store available directions.
		std::vector<std::string> next_dir;
		
		// Lets calculate available direction.
	    calculateDirection(maze_fields, next_dir, pos, size, is_stuck);
	    
	    // In case we can not move anymore because of boundaries or there are no
	    // unvisited fields and maze is not finished, lets find visited field that
	    // has nearby unvisited field.
		for (int i = 1; i<size.height - 1 && is_stuck; i += 2)
		{
		    for (int j = 1; j < size.width - 1 && is_stuck; j += 2)
		    {
		    	// We need to find already visited field that has nearby unvisited field.
		    	// We will continue if there is no such field.
		    	if (maze_fields[i][j] != 0)
		    		continue;
		    	
		    	// Save current visited field.
		    	pos.row = i;
		    	pos.col = j;
		    	
		    	// Lets calculate again available directions.
		    	calculateDirection(maze_fields, next_dir, pos, size, is_stuck);
		    }
		}		

	    // If we are not stuck we can move and visit next field.
	    if (!is_stuck)
	    {
	    	// Generate random index.
			std::uniform_int_distribution<unsigned int> dist(0, next_dir.size()-1);
			unsigned int random_index = dist(mt);
			
			// Check where we can move based on random direction.
			// We need to move by 2 because in maze array are stored walls besides of fields.
			// When we move to next field we break wall.
			if (next_dir.at(random_index) == "UP")
			{
				pos.row--;
				maze_fields[pos.row][pos.col] = 0; // Remove wall.
				pos.row--;
				maze_fields[pos.row][pos.col] = 0; // Visit field.
			}
			else if (next_dir.at(random_index) == "DOWN")
			{
				pos.row++;
				maze_fields[pos.row][pos.col] = 0; // Remove wall.
				pos.row++;
				maze_fields[pos.row][pos.col] = 0; // Visit field.
			   
			}
			else if (next_dir.at(random_index) == "RIGHT")
			{
				pos.col++;
				maze_fields[pos.row][pos.col] = 0; // Remove wall.
				pos.col++;
				maze_fields[pos.row][pos.col] = 0; // Visit field.
			}
			else if (next_dir.at(random_index) == "LEFT")
			{
				pos.col--;
				maze_fields[pos.row][pos.col] = 0; // Remove wall.
				pos.col--;
				maze_fields[pos.row][pos.col] = 0; // Visit field.
			}
	    }
	    
	    // Real-time printing maze.
	    printMaze(maze_fields, size);
		
        // Check if all fields are visited.
        for (unsigned int i = 1; i<size.height - 1 && unvisited_fields == 0; i += 2)
        {
            for (unsigned int j = 1; j<size.width- 1 && unvisited_fields == 0; j += 2)
            {
            	if (maze_fields[i][j] == 2)
            		unvisited_fields++;
            }
        }

		// We have made our random maze, so let's escape from loop.
        if (unvisited_fields == 0)
            is_finished = true;
       
    }
    
    // Print here maze if you don't want to delay calculating maze and wants to finish
    // almost instantly.
    //printMaze(maze_fields, size);
    
    // Print time in seconds after which generation is finished.
    auto stop = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 
    std::cout<<"Time: "<<duration.count()/(1000.0*1000.0)<<" s"<<std::endl;

    return 0;
}

// Checks for boundaries and save available directions in next_dir.
void calculateDirection (const std::vector<std::vector<unsigned int>> &maze_fields, std::vector<std::string> &next_dir, const struct Position &pos, const struct Size &size, bool &is_stuck)
{
	// Checks if we are near up wall.
	if(pos.row > 2)
    {
    	// Checks if there is unvisited field up to the current position, if so save direction.
        if(maze_fields[pos.row - 2][pos.col] == 2) 
        {
        	next_dir.push_back("UP");
        	is_stuck = false;
        }
    }
    // Checks if we are near down wall.
	if(pos.row < size.height - 2)
	{
		// Check if there is unvisited field down to the current position, if so save direction.
        if(maze_fields[pos.row + 2][pos.col] == 2)
        {
        	next_dir.push_back("DOWN");
        	is_stuck = false;
        }
    }
    // Checks if we are near right wall.
    if(pos.col < size.width - 2)
    {
    	// Check if there is unvisited field right to the current position, if so save direction.
        if(maze_fields[pos.row][pos.col + 2] == 2)
        {
        	next_dir.push_back("RIGHT");
        	is_stuck = false;
        }
    }
    // Checks if we are near left wall.   	
    if(pos.col > 2)
    {
    	// Check if there is unvisited field left to the current position, if so save direction.
        if(maze_fields[pos.row][pos.col - 2] == 2)
       	{
        	next_dir.push_back("LEFT");
        	is_stuck = false;
        }
    }
    
    // If next_dir is empty that means we are stuck so lets set is_stuck to true.
    if (next_dir.empty())
	    is_stuck = true;
}

void printMaze(const std::vector<std::vector<unsigned int>> &maze_fields, const struct Size &size)
{
	// Delay printing by 0.1s.
	#ifdef _WIN32
	Sleep(100);
	#else
	usleep(0.1 * 1000 * 1000);
	#endif
	// Clear console/terminal.
	#ifdef _WIN32
	clearConsole();
	#else
	std::system("clear");
	#endif
	
	// Print maze.
    for (unsigned int i=0; i < size.height; ++i)
    {
        for (unsigned int j=0; j < size.width; ++j)
        {
            if (maze_fields[i][j] == 0)
            {
                rlutil::setColor(15);
                std::cout<<"  ";
            }
            else
            {
                rlutil::setColor(14);
                std::cout<<"||";
            }
        }
        std::cout<<std::endl;
    }	
}
// Clear console on windows
#ifdef _WIN32
void clearConsole()
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}
#endif
