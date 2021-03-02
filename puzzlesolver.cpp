/*####################################################
# A 3x3, 2x3, 3x2 sliding block puzzle solver
# Author: Alok Yadav
# Tested In: Windows[CodeBlocks]
# Updated: 24-11-2017
# URL: https://alokyadav.in
# Created: 20-10-2017 alok@alokyadav.in
#####################################################*/
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
#define C 3
#define R 3
unsigned int delaystatus = 0;
struct Node
{
	Node* parent;
	int mat[R][C];
	int x, y;
	int cost;
	int level;
};
void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());

}
COORD coord={0,0}; // this is global variable
//center of axis is set to the top left cornor of the screen
 void gotoxy(int x,int y)
 {
   coord.X=x;
 coord.Y=y;
 SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
 }
// A utility function to count inversions in given array 'arr[]'
int getInvCount(int arr[])
{
int inv_count = 0,i,j;
for (i = 0; i < (R*C - 1); i++)
	for (j = i+1; j < (R*C)+1; j++)
	// Value 0 is used for empty space
	if (arr[j] && arr[i] && arr[i] > arr[j])
		inv_count++;
return inv_count;
}

// This function returns true if given 8 puzzle is solvable.
bool isSolvablePuzzle(int puzzle[R][C])
{
	// Count inversions in given puzzle
	int invCount = getInvCount((int *)puzzle);
	// return true if inversion count is even.
	return (invCount%2 == 0);
}

int display(int mat[R][C])
{
    static unsigned int count=0;
    char ld=220;
    char cl=219;
    char dl=223;
    gotoxy(0,9);
    if(delaystatus == 0){
        printf("\tSolving Puzzle...\n");
    }else if(delaystatus >0 && delaystatus < 6){
        printf("\tSolving Puzzle...\n");
        delay(1000*delaystatus);
    }
    printf("\tSteps Count: %d",++count);
    printf("\n\t%c%c%c%c%c%c%c%c%c%c%c%c%c\n",ld,ld,ld,ld,ld,ld,ld,ld,ld,ld,ld,ld,ld);
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++){
            if(j == 0)
                printf("\t%c",cl);
			printf(" %d %c", mat[i][j],cl);
		}
		printf("\n\t%c%c%c%c%c%c%c%c%c%c%c%c%c\n",cl,ld,ld,ld,cl,ld,ld,ld,cl,ld,ld,ld,cl);
	}
    if(delaystatus < 0 || delaystatus >= 6){
        system("pause");
        fflush(stdin);
    }
}

Node* newNode(int mat[R][C], int x, int y, int newX,
			int newY, int level, Node* parent)
{
	Node* node = new Node;
	node->parent = parent;
	memcpy(node->mat, mat, sizeof node->mat);
	swap(node->mat[x][y], node->mat[newX][newY]);
	node->cost = INT_MAX;
	node->level = level;
	node->x = newX;
	node->y = newY;
	return node;
}

// botton, left, top, right
int row[] = { 1, 0, -1, 0 };
int col[] = { 0, -1, 0, 1 };

int moveCost(int puzzle[R][C], int final[R][C])
{
	int count = 0;
	for (int i = 0; i < R; i++)
	for (int j = 0; j < C; j++)
		if (puzzle[i][j] && puzzle[i][j] != final[i][j])
		count++;
	return count;
}

int isSolvable(int x, int y)
{
	return (x >= 0 && x < R && y >= 0 && y < C);
}

void Path(Node* root)
{
	if (root == NULL)
		return;
	Path(root->parent);
	display(root->mat);
	printf("\n");
}

struct comp
{
	bool operator()(const Node* lhs, const Node* rhs) const
	{
		return (lhs->cost + lhs->level) > (rhs->cost + rhs->level);
	}
};

void solvePuzzle(int puzzle[R][C], int x, int y,int final[R][C])
{
	priority_queue<Node*, std::vector<Node*>, comp> pq;
	Node* root = newNode(puzzle, x, y, x, y, 0, NULL);
	root->cost = moveCost(puzzle, final);
	pq.push(root);
	while (!pq.empty())
	{
		Node* min = pq.top();
		pq.pop();
		if (min->cost == 0)
		{
			Path(min);
			return;
		}
		for (int i = 0; i < 4; i++)
		{
			if (isSolvable(min->x + row[i], min->y + col[i]))
			{
				Node* child = newNode(min->mat, min->x,
							min->y, min->x + row[i],
							min->y + col[i],
							min->level + 1, min);
				child->cost = moveCost(child->mat, final);
				pq.push(child);
			}
		}
	}
}
int getBlankLocation(int puzzle[R][C], int *x, int *y)
{
    int i,j,flag=0;
    for (i = 0; i < R; i++){
        for (j = 0; j < C; j++){
            if (puzzle[i][j] == 0){
                flag=1;
                break;
            }
        }
        if(flag==1){
	    *x=i;
        *y=j;
        printf("\tBlank Location is: %d, %d\n",i,j);
        return 1;
        }
    }
    return 0;
}
int main()
{
	int puzzle[R][C] =
	{
		{4, 3, 1},
		{7, 8, 6},
		{5, 0, 2}
	};
	int final[R][C] =
	{
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 0}
	};
	int x = 1, y = 1;
	printf("\n\tPuzzle Solver Starting...\n");
    if(isSolvablePuzzle(puzzle)){
        printf("\tPuzzle is Solvable\n");
    }
    else{
        printf("\tPuzzle is Not Solvable\n");
        return 0;
    }

    int ret = getBlankLocation(puzzle, &x, &y);
    if(ret == 0){
        printf("\tUnable to find Blank Location!!!\n");
        return 0;
    }
    printf("\tEnter Run Mode: \n\t  Press 0 for No delay run.\n\t  Press 1-5 for delay run.\n");
    printf("\t  Press any other key to step by step run.\n\tYour Input : ");
    scanf("%d",&delaystatus);
	solvePuzzle(puzzle, x, y, final);
	return 0;
}
