#include <unistd.h>		 // open() close()
#include <fcntl.h>		 // O_* constants
#include <sys/stat.h>	 // mkfifo()
#include <sys/types.h> // mkfifo()
#include <string.h>		 // strlen() strcmp()
#include <stdio.h>		 // printf() scanf()
#include <stdlib.h>		 // exit()
#include <iostream>
#include <array>
#include "my_const.h"

#define BOARD_CAPACITY 3

using namespace std;

bool isValidPlayLocation(int);
void displayBoard(array<array<char, BOARD_CAPACITY>, BOARD_CAPACITY> &);
void playInLocation(int, array<array<char, BOARD_CAPACITY>, BOARD_CAPACITY> &, char);
bool isWinningMove(array<array<char, BOARD_CAPACITY>, BOARD_CAPACITY> &);
bool isBoardFull(array<array<char, BOARD_CAPACITY>, BOARD_CAPACITY> &);
void resetBoard(array<array<char, BOARD_CAPACITY>, BOARD_CAPACITY> &);
void printStats(struct Points &);

char winingTag = 'X';

struct Points
{
	int p1 = 0;
	int p2 = 0;
};

struct Data
{
	struct Points points;
	array<array<char, BOARD_CAPACITY>, BOARD_CAPACITY> board;
	bool quit;
	bool pause;
};

int main()
{
	// create the named pipe (FIFO) if not exist
	int f1 = mkfifo(player1to2, 0666);
	int f2 = mkfifo(player2to1, 0666);
	printf("@p1: f1 = %d  f2 = %d\n", f1, f2);

	// main board data to be shared
	array<array<char, BOARD_CAPACITY>, BOARD_CAPACITY> board = {{'.', '1', '2', '3', '4', '5', '6', '7', '8'}};
	struct Points points = {p1 : 0, p2 : 0};
	struct Data data = {points : points, board : board, quit : false, pause : false};

	char playTag = 'O';
	int location;

	printf("waiting for named pipes open... \n");

	// P1&P2: order of open() is important to unblock processes
	// open() for WR will be blocked until the other side is open for RD
	int fd_wr = open(player1to2, O_WRONLY);

	// open() for RD will be blocked until the other side is open for WR
	int fd_rd = open(player2to1, O_RDONLY);

	printf("named pipes opened and ready\n");

	// player 1: engage first
	displayBoard(data.board);
	while (true)
	{
		printf("Where do you want to play? Select between cells 0 through 8 (Otherwise to quit): ");
		cin >> location;

		if (!cin) // check if user input does not matche an integer value
		{
			cin.clear();
			cout << "You entered a non-digit character." << endl;
			cout << "Quiting appliction" << endl;
			break;
		}
		else
		{
			if (isValidPlayLocation(location))
			{
				playInLocation(location, data.board, playTag);
				if (isWinningMove(data.board))
				{
					data.points.p1++; // add the points of p1
					data.quit = true; // set quit to true
					displayBoard(data.board);
					cout << "Winner: Player 1" << endl;
					printStats(data.points);
					write(fd_wr, &data, sizeof(data));
					break;
				}

				if (isBoardFull(data.board))
				{
					cout << "The game has ended in a tie." << endl;
					data.quit = true;
					printStats(data.points);
					write(fd_wr, &data, sizeof(data));
					break;
				}
			}
			else
			{
				cout << "Please select a valid location from 0 to 8" << endl;
				printf("Where do you want to play? Select between cells 0 through 8 (Otherwise to quit): ");
				cin >> location;
			}
		}
		// writing to player 2
		displayBoard(data.board);
		write(fd_wr, &data, sizeof(data));

		// reading from player 2
		read(fd_rd, &data, sizeof(data));
		displayBoard(data.board);

		// check if there is a winning move from previous play
		if (isWinningMove(data.board))
		{
			cout << "Winner: Player 2" << endl;
		}

		if (isBoardFull(data.board))
		{
			cout << "The game has ended in a tie." << endl;
			data.quit = true;
			printStats(data.points);
			write(fd_wr, &data, sizeof(data));
			break;
		}

		//check if it's time to quite the application
		if (data.quit)
		{
			// show game stats
			printStats(data.points);
			break;
		}
	}
	close(fd_wr);
	close(fd_rd);
	unlink(player1to2);
	unlink(player2to1);
}

bool isValidPlayLocation(int loc)
{
	return (loc < 0 || loc > 8) ? false : true;
}

void displayBoard(array<array<char, BOARD_CAPACITY>, BOARD_CAPACITY> &board)
{
	cout << "+-+-+-+" << endl;
	for (auto const &row : board)
	{
		for (auto const &entry : row)
		{
			cout << "|" << entry;
		}
		cout << "|" << endl
				 << "+-+-+-+" << endl;
	}
}

void playInLocation(int loc, array<array<char, BOARD_CAPACITY>, BOARD_CAPACITY> &board, char tag)
{
	int row = (int)loc / BOARD_CAPACITY;
	int col = (int)loc % BOARD_CAPACITY;

	if (board[row][col] == 'O' || board[row][col] == 'X')
	{
		cout << "That location is used already. Try something else" << endl;
		return;
	}
	board[row][col] = tag;
	cout << "You have played in cell " << loc << endl;
}

// check validation criteria
bool isWinningMove(array<array<char, BOARD_CAPACITY>, BOARD_CAPACITY> &board)
{
	// check for rows
	for (int i = 0; i < BOARD_CAPACITY; i++)
	{
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
		{
			winingTag = board[i][0];
			return true;
		}
	}

	// check for columns
	for (int i = 0; i < BOARD_CAPACITY; i++)
	{
		if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
		{
			winingTag = board[0][i];
			return true;
		}
	}

	// check for diagonals
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
	{
		winingTag = board[0][0];
		return true;
	}

	if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
	{
		winingTag = board[0][2];
		return true;
	}

	return false;
}

bool isBoardFull(array<array<char, BOARD_CAPACITY>, BOARD_CAPACITY> &board)
{
	for (auto const &row : board)
	{
		for (auto const &entry : row)
		{
			if (entry != 'O' && entry != 'X')
				return false;
		}
	}
	return true;
}

void resetBoard(array<array<char, BOARD_CAPACITY>, BOARD_CAPACITY> &board)
{
	board = {{'.', '1', '2', '3', '4', '5', '6', '7', '8'}};
}

void printStats(Points &points)
{
	cout << "================= Game Stats ================" << endl;
	cout << "Player one points: " << points.p1 << endl;
	cout << "Player two points: " << points.p2 << endl;
	printf("==============================================\n\n");
}