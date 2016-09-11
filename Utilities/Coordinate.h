#pragma once

struct Coordinate 
{ 
	Coordinate( void ) {}
	Coordinate( unsigned int _x, unsigned int _y ) { x = _x; y = _y; }

	unsigned int x; 
	unsigned int y; 
};