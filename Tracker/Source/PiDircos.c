//
//	PiDircos.c
//
/////////////////////////////////////////////////////////////

#include <stdio.h>
#include "..\Include\PiDircos.h"

void Print(struct PiDirCos* ptr)
{
	int i, j;
	for (i=0; i<3; i++)
	{
		for (j=0; j<3; j++)
		{
			printf(" %f", ptr->dc[i][j]);
		};
		printf("\n");
	};
}; // end of function
