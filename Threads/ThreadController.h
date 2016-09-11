//#pragma once
//
//#include <pthread.h>
//
//typedef void*(*ThreadStartFcn)(void*);
//
//#define MAX_THREAD_COUNT 64
//
//class ThreadController
//{
//public:
//	ThreadController( void );
//
//	bool AddThread( ThreadStartFcn function, void * data );
//
//	pthread_t Threads[ MAX_THREAD_COUNT ];
//	unsigned int NumActiveThreads;
//
//};