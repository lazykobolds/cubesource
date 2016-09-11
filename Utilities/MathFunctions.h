#pragma once

class IMathFunction
{
public:
	virtual float operator() ( float input0, float input1 ) = 0;
};

template < class F >
class MathFunction : public IMathFunction
{
public:
	float operator()( float input0, float input1 ) { return F()( input0, input1 ); }
};

class Add
{
public:
	float operator () ( float input0, float input1 ) { return input0 + input1; }
};

class Subtract
{
public:
	float operator () ( float input0, float input1 ) { return input0 - input1; }
};

class Multiply
{
public:
	float operator () ( float input0, float input1 ) { return input0 * input1; }
};

template < int V >
class MultiplyVal
{
public:
	float operator() ( float input0, float ) { return input0 * V; }
};

template < int V >
class AddVal
{
public:
	float operator() ( float input0, float ) { return input0 + V; }
};

template < int V >
class SubtractVal
{
public:
	float operator() ( float input0, float ) { return input0 - V; }
};

template < class F0, class F1 >
class CompositeFunction
{
public:
	float operator ()( float input0, float input1 ) { return F1()( F0()( input0, input1 ), input1 ); }
};