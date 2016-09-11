#include "GridUtilities.h"


void CombineGrids( float * output_grid, float * grid0, float * grid1, unsigned int width, unsigned int height, IMathFunction * Func )
{
	unsigned int index;
	unsigned int xi;
	for( unsigned int x = 0; x < width; ++x )
	{
		xi = x * width;
		for( unsigned int y = 0; y < height; ++y )
		{
			index = xi + y;
			output_grid[index] = (*Func)( grid0[index], grid1[index] );
		}  
	}
}