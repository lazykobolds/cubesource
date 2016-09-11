#pragma once

class OctTree
{
public:

	OctTree( void );
	OctTree( unsigned int r_size, unsigned int m_levels, float cx, float cy, float cz );
	void Initilize( unsigned int r_size, unsigned int m_levels, float cx, float cy, float cz );

	void GetNodeCenter( unsigned int node_pos, float * pos );
	void Expand( unsigned int node_pos );

//protected:

	unsigned int root_size;
	unsigned int half_size;
	unsigned int quarter_size;
	unsigned int max_levels;
	float center[3];
	bool leaf_node;

	/*
		start -> top, left, front
		left -> right
		front -> back
		top -> bottom
	*/
	OctTree * nodes[8];

private:

};