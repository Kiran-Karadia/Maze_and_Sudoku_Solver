#include <stdio.h>
#include <Windows.h>

#define MAXSTR 10000
int CheckExit();
int CheckMazeFile(FILE *fp, char **argv);
int AllMazeInfo(FILE *fp, char **argv);
int GetMazeSize(FILE *fp, int *numOfRows, int *numOfCols);
int GetStartEndPoints(FILE *fp, int *startX, int *startY, int *endX, int *endY);
int GetWalls(FILE *fp, int numOfRows, int numOfCols, int **mazeWalls, char **entireMaze, int startX, int startY, int endX, int endY);
int DrawMaze(int numOfRows, int numOfCols, char **entireMaze);
int MazeSolver(int currentRow, int currentCol, int numOfRows, int numOfCols, char **entireMaze, int startX, int startY, int endX, int endY, char *solution);
int PathSolver(int currentRow, int currentCol, int endX, int endY, int numOfRows, int numOfCols, char **entireMaze, char *solution, int count);
int WriteSolution(char **argv, int numOfSteps, char *solution);


int main(int argc, char **argv)
{

	FILE *fp;
	if (argv[3] == NULL)
	{
		fopen_s(&fp, argv[1], "r");
	}
	else
	{
		fopen_s(&fp, argv[2], "r");
	}
	if (CheckMazeFile(fp, argv) == 0)
	{
		CheckExit();
		return 0;
	}
	AllMazeInfo(fp, argv);
	fclose(fp);
	CheckExit();
	return 0;
}
int CheckExit()
{
	char exitKey;	//Char used check if program should exit
	printf("Press ENTER to exit.");
	//Exit the program when ENTER is pressed
	scanf_s("%c", &exitKey);
	if (exitKey != NULL)
	{
		return 0;
	}
	return 0;
}
int CheckMazeFile(FILE *fp, char**argv)
{
	if (fp == NULL)					//Check for error when opening file
	{
		perror("Error: ");
		return 0;
	}
	else
	{
		printf("Maze has been opened...\n");
		return 1;

	}
}
int AllMazeInfo(FILE *fp, char **argv)
{
	int numOfRows = 0, numOfCols = 0;	//Integers to hold number of rows and columns in the maze
	int startX, startY;	//Integers to hold the starting column(X) and starting row(Y)
	int endX, endY;	//Integers to hold the ending column(C) and ending row(Y)

	int **mazeWalls;		//2D array used to draw the maze
							//Replicates the wall information part of the text file
	char **entireMaze;		//2D array used draw and solve the maze.
	char *solution;			//Array to hold the solution for writing to the solution file

	GetMazeSize(fp, &numOfRows, &numOfCols);
	printf("The maze is %dx%d\n", numOfRows, numOfCols);

	GetStartEndPoints(fp, &startX, &startY, &endX, &endY);
	printf("Starting coordinates: %d,%d\nEnding coordinates: %d,%d\n", startY, startX, endY, endX);

	//Making mazeWalls have dynamic memroy allocation
	mazeWalls = (int **)malloc(sizeof(int) * numOfRows * 2);
	for (int i = 0; i <= numOfRows * 2; i++)
	{
		mazeWalls[i] = (int *)malloc(sizeof(int) * numOfCols * 2);
	}

	//Craete 2D array for the maze and make it all walls to begin
	entireMaze = malloc(numOfRows*2 * sizeof(char *));
	for (int i = 0; i <= numOfRows * 2; i++)
	{
		entireMaze[i] = malloc(numOfCols*2 * sizeof(char));
		for (int j = 0; j < ((numOfCols * 2)); j++)
		{
			entireMaze[i][j] = ' ';
		}
	}

	GetWalls(fp, numOfRows, numOfCols, mazeWalls, entireMaze, startX, startY, endX, endY);


	solution = malloc(numOfRows*numOfCols* sizeof(char *));

	MazeSolver(startY, startX, numOfRows, numOfCols, entireMaze, startX, startY, endX, endY, solution);

	//Count the number of characters in solution. If 0 then no solution 
	int numOfSteps = 0;
	while (solution[numOfSteps] == 'R' || solution[numOfSteps] == 'L' || solution[numOfSteps] == 'U' || solution[numOfSteps] == 'D')
	{
		numOfSteps++;
	}
	if (numOfSteps == 0)
	{
		printf("There was no solution\n");
	}
	else
	{
		printf("It took %d steps\n", numOfSteps / 2);
	}
	//Print the solution. Increment in 2 to skip every other character. Since each step is repeated due to each square being represented as a 3x3 block
	//for (int i = 0; i <= numOfSteps - 1; i = i + 2)
	//{
	//	printf("%c", solution[i]);
	//}
	WriteSolution(argv, numOfSteps, solution);
	printf("\n");

	//Check if maze should be displayed
	if (argv[3] != NULL)
	{
		DrawMaze(numOfRows, numOfCols, entireMaze);
		printf("\n");
	}
	return 0;
}
int GetMazeSize(FILE *fp, int *numOfRows, int *numOfCols)
{
	//Read the first line and store the 2 integers as the dimensions of the maze
	char buf[MAXSTR];
	fgets(buf, MAXSTR, fp);

	sscanf_s(buf, "%d %d", &*numOfRows, &*numOfCols);
	return 0;
}
int GetStartEndPoints(FILE *fp, int *startX, int *startY, int *endX, int *endY)
{
	//Read the second line and store the 2 integers as the starting coordinates
	char buf[MAXSTR];
	fgets(buf, MAXSTR, fp);
	sscanf_s(buf, "%d %d", &*startY, &*startX);
	//Read the third line and store the 2 integers as the ending coordinates
	fgets(buf, MAXSTR, fp);
	sscanf_s(buf, "%d %d", &*endY, &*endX);

	return 0;
}
int GetWalls(FILE *fp, int numOfRows, int numOfCols, int **mazeWalls, char **entireMaze, int startX, int startY, int endX, int endY)
{
	int wall = 219;
	int colPos, rowPos;
	//Go through the rest of the lines in the text file
	//Store each integer in the 2D array mazeWalls
	//This array replicates the contents of the text file representing the walls (location and order)
	for (int i = 0; i <= numOfRows - 1; i++)
	{
		for (int j = 0; j <= numOfCols - 1; j++)
		{
			fscanf_s(fp, "%d", &mazeWalls[i][j]);
			colPos = 2 * j + 1;
			rowPos = 2 * i + 1;
			entireMaze[rowPos][colPos] = ' ';
			switch (mazeWalls[i][j])
			{
			case 14:
				entireMaze[rowPos - 1][colPos - 1] = 219;
				entireMaze[rowPos - 1][colPos] = 219;
				entireMaze[rowPos - 1][colPos + 1] = 219;

				entireMaze[rowPos][colPos - 1] = 219;
				entireMaze[rowPos + 1][colPos - 1] = 219;

				entireMaze[rowPos + 1][colPos] = 219;
				entireMaze[rowPos + 1][colPos + 1] = 219;
				break;
			case 13:
				entireMaze[rowPos - 1][colPos - 1] = 219;
				entireMaze[rowPos - 1][colPos] = 219;
				entireMaze[rowPos - 1][colPos + 1] = 219;

				entireMaze[rowPos][colPos - 1] = 219;
				entireMaze[rowPos + 1][colPos - 1] = 219;

				entireMaze[rowPos][colPos + 1] = 219;
				entireMaze[rowPos + 1][colPos + 1] = 219;
				break;
			case 12:
				entireMaze[rowPos - 1][colPos - 1] = 219;
				entireMaze[rowPos - 1][colPos] = 219;
				entireMaze[rowPos - 1][colPos + 1] = 219;

				entireMaze[rowPos][colPos - 1] = 219;
				entireMaze[rowPos + 1][colPos - 1] = 219;
				break;
			case 11:
				entireMaze[rowPos - 1][colPos - 1] = 219;
				entireMaze[rowPos - 1][colPos] = 219;
				entireMaze[rowPos - 1][colPos + 1] = 219;

				entireMaze[rowPos + 1][colPos - 1] = 219;
				entireMaze[rowPos + 1][colPos] = 219;
				entireMaze[rowPos + 1][colPos + 1] = 219;

				entireMaze[rowPos][colPos + 1] = 219;
				break;
			case 10:
				entireMaze[rowPos - 1][colPos - 1] = 219;
				entireMaze[rowPos - 1][colPos] = 219;
				entireMaze[rowPos - 1][colPos + 1] = 219;

				entireMaze[rowPos + 1][colPos - 1] = 219;
				entireMaze[rowPos + 1][colPos] = 219;
				entireMaze[rowPos + 1][colPos + 1] = 219;
				break;
			case 9:
				entireMaze[rowPos - 1][colPos - 1] = 219;
				entireMaze[rowPos - 1][colPos] = 219;
				entireMaze[rowPos - 1][colPos + 1] = 219;

				entireMaze[rowPos][colPos + 1] = 219;
				entireMaze[rowPos + 1][colPos + 1] = 219;
				break;
			case 8:
				entireMaze[rowPos - 1][colPos - 1] = 219;
				entireMaze[rowPos - 1][colPos] = 219;
				entireMaze[rowPos - 1][colPos + 1] = 219;
				break;
			case 7:
				entireMaze[rowPos - 1][colPos - 1] = 219;
				entireMaze[rowPos][colPos - 1] = 219;
				entireMaze[rowPos + 1][colPos - 1] = 219;

				entireMaze[rowPos + 1][colPos] = 219;
				entireMaze[rowPos + 1][colPos + 1] = 219;

				entireMaze[rowPos][colPos + 1] = 219;
				entireMaze[rowPos - 1][colPos + 1] = 219;
				break;
			case 6:
				entireMaze[rowPos - 1][colPos - 1] = 219;
				entireMaze[rowPos][colPos - 1] = 219;
				entireMaze[rowPos + 1][colPos - 1] = 219;

				entireMaze[rowPos + 1][colPos] = 219;
				entireMaze[rowPos + 1][colPos + 1] = 219;
				break;
			case 5:
				entireMaze[rowPos - 1][colPos - 1] = 219;
				entireMaze[rowPos][colPos - 1] = 219;
				entireMaze[rowPos + 1][colPos - 1] = 219;

				entireMaze[rowPos - 1][colPos + 1] = 219;
				entireMaze[rowPos][colPos + 1] = 219;
				entireMaze[rowPos + 1][colPos + 1] = 219;
				break;
			case 4:
				entireMaze[rowPos - 1][colPos - 1] = 219;
				entireMaze[rowPos][colPos - 1] = 219;
				entireMaze[rowPos + 1][colPos - 1] = 219;
				break;
			case 3:
				entireMaze[rowPos + 1][colPos - 1] = 219;
				entireMaze[rowPos + 1][colPos] = 219;
				entireMaze[rowPos + 1][colPos + 1] = 219;

				entireMaze[rowPos][colPos + 1] = 219;
				entireMaze[rowPos - 1][colPos + 1] = 219;
				break;
			case 2:
				entireMaze[rowPos + 1][colPos - 1] = 219;
				entireMaze[rowPos + 1][colPos] = 219;
				entireMaze[rowPos + 1][colPos + 1] = 219;
				break;
			case 1:
				entireMaze[rowPos - 1][colPos + 1] = 219;
				entireMaze[rowPos][colPos + 1] = 219;
				entireMaze[rowPos + 1][colPos + 1] = 219;
				break;
			}
		}
	}
	entireMaze[startY * 2 + 1][startX * 2 + 1] = 'S';
	entireMaze[endY * 2 + 1][endX * 2 + 1] = 'E';
	return 0;
}
int DrawMaze(int numOfRows, int numOfCols, char **entireMaze)
{
	//Loop through every element in entireMaze and print the chracter.
	for (int i = 0; i <= numOfRows * 2; i++)
	{
		for (int j = 0; j <= numOfCols * 2; j++)
			if (j < numOfCols * 2)				//Check if new line must be started
			{
				printf("%c", entireMaze[i][j]);
			}
			else
			{
				printf("%c\n", entireMaze[i][j]);
			}
	}


	return 0;
}
int MazeSolver(int currentRow, int currentCol, int numOfRows, int numOfCols, char **entireMaze, int startX, int startY, int endX, int endY, char *solution)
{

	int count = 0;	//Count used to keep track of the number of steps
					//Recursive solve. Start with the starting point
	PathSolver(startY * 2 + 1, startX * 2 + 1, endX, endY, numOfRows, numOfCols, entireMaze, solution, count);
	entireMaze[startY * 2 + 1][startX * 2 + 1] = 'S';
	printf("\n");
	return 0;
}
int WriteSolution(char **argv, int numOfSteps, char *solution)
{
	FILE *fpw;		//Pointer to the file stream
					//Check which parameter the solution file is
					//Index changes if '-d' is present in the parameters
	if (argv[3] == NULL)
	{
		fopen_s(&fpw, argv[2], "w");
	}
	else
	{
		fopen_s(&fpw, argv[3], "w");
	}

	//If there is a solution, then write to the file
	if (numOfSteps != 0)
	{
		fprintf_s(fpw, "%d\n", numOfSteps / 2);
		for (int i = 0; i <= numOfSteps - 1; i = i + 2)
		{
			fprintf_s(fpw, "%c", solution[i]);
		}
		printf("\nSolution has been written!\n");
	}

	//Close the solution file
	fclose(fpw);

	return 0;
}
int PathSolver(int currentRow, int currentCol, int endX, int endY, int numOfRows, int numOfCols, char **entireMaze, char *solution, int count)
{
	//Check if current square is the end point
	if (entireMaze[currentRow][currentCol] == 'E')
	{
		return 1;
	}

	//Check if this square is a wall OR if square has already been checked
	if (entireMaze[currentRow][currentCol] == -37 || entireMaze[currentRow][currentCol] == '+')
	{
		return 0;
	}
	//Set this square as part of the solution path
	entireMaze[currentRow][currentCol] = '+';
	//Check Right
	if (PathSolver(currentRow, currentCol + 1, endX, endY, numOfRows, numOfCols, entireMaze, solution, count + 1) == 1)
	{
		entireMaze[currentRow][currentCol] = '+';
		solution[count] = 'R';

		return 1;
	}
	//Check Down
	if (PathSolver(currentRow + 1, currentCol, endX, endY, numOfRows, numOfCols, entireMaze, solution, count + 1) == 1)
	{
		entireMaze[currentRow][currentCol] = '+';
		solution[count] = 'D';

		return 1;
	}

	//Check Left
	if (PathSolver(currentRow, currentCol - 1, endX, endY, numOfRows, numOfCols, entireMaze, solution, count + 1) == 1)
	{
		entireMaze[currentRow][currentCol] = '+';
		solution[count] = 'L';

		return 1;
	}
	//Check Up
	if (PathSolver(currentRow - 1, currentCol, endX, endY, numOfRows, numOfCols, entireMaze, solution, count + 1) == 1)
	{
		entireMaze[currentRow][currentCol] = '+';
		solution[count] = 'U';

		return 1;
	}






	//If no conditions returned true, remove this square from the solution path
	entireMaze[currentRow][currentCol] = ' ';
	return 0;
}

