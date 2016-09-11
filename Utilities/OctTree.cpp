#include "OctTree.h"

#include <assert.h>

OctTree::OctTree( void ) 
{ 
	Initilize( 0, 0, 0, 0, 0 ); 
}

OctTree::OctTree( unsigned int r_size, unsigned int m_levels, float cx, float cy, float cz )
{
	Initilize( r_size, m_levels, cx, cy, cz );
}

void OctTree::Initilize( unsigned int r_size, unsigned int m_levels, float cx, float cy, float cz )
{
	root_size = r_size;
	half_size = root_size >> 1;
	quarter_size = half_size >> 1;
	max_levels = m_levels;
	for( unsigned int i = 0; i < 8; ++i ) nodes[i] = 0;
	center[0] = cx;
	center[1] = cy;
	center[2] = cz;
	leaf_node = true;
}

void OctTree::GetNodeCenter( unsigned int node_pos, float * pos )
{
	switch( node_pos )
	{
	case 0:
		pos[0] = center[0] - quarter_size;
		pos[1] = center[1] + quarter_size;
		pos[2] = center[2] + quarter_size;
		return;
	case 1:
		pos[0] = center[0] + quarter_size;
		pos[1] = center[1] + quarter_size;
		pos[2] = center[2] + quarter_size;
		return;
	case 2:
		pos[0] = center[0] - quarter_size;
		pos[1] = center[1] + quarter_size;
		pos[2] = center[2] - quarter_size;
		return;
	case 3:
		pos[0] = center[0] + quarter_size;
		pos[1] = center[1] + quarter_size;
		pos[2] = center[2] - quarter_size;
		return;
	case 4:
		pos[0] = center[0] - quarter_size;
		pos[1] = center[1] - quarter_size;
		pos[2] = center[2] + quarter_size;
		return;
	case 5:
		pos[0] = center[0] + quarter_size;
		pos[1] = center[1] - quarter_size;
		pos[2] = center[2] + quarter_size;
		return;
	case 6:
		pos[0] = center[0] - quarter_size;
		pos[1] = center[1] - quarter_size;
		pos[2] = center[2] - quarter_size;
		return;
	case 7:
		pos[0] = center[0] + quarter_size;
		pos[1] = center[1] - quarter_size;
		pos[2] = center[2] - quarter_size;
		return;
	}
}

void OctTree::Expand( unsigned int node_pos )
{
	assert( nodes[node_pos] == 0 && max_levels > 0 && "ERROR: Not enough levels to expand!" );
	
	float node_center[3];

	GetNodeCenter( node_pos, node_center );
	
	leaf_node = false;
	nodes[ node_pos ] = new OctTree( root_size >> 1, max_levels - 1, node_center[0], node_center[1], node_center[2] );
}