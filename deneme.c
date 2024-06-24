#include <stdio.h>
#include <stdlib.h>

int manin(){

	int i;
	int j;
	int mat[4][2] = {{1,2},{3,4},{5,6},{7,8}};

	for ( i = 0; i < 10; i++)
	{
		for ( j = 0; j < 10; j++)
		{
			printf("%d ",mat[i][j]);
		}
		printf("\n");
	}
	



	return 0;

}