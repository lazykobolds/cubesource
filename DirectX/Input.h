#pragma once

#include <Dinput.h>

//include DirectInput libraries
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

#define BUFFER_SIZE 256
#define INPUT_BUFFER_SIZE 16

#define LEFT_CLICK_BIT    0x01
#define RIGHT_CLICK_BIT   0x02
#define MIDDLE_CLICK_BIT  0x04
#define LEFT_CLICK        LEFT_CLICK_BIT
#define RIGHT_CLICK       RIGHT_CLICK_BIT
#define MIDDLE_CLICK      MIDDLE_CLICK_BIT


class InputBuffer
{
public:
  InputBuffer( void );
  
  void Initilize( HWND hwnd, HINSTANCE hinst );
  void UpdateInputBuffer( void );
  
  inline bool MousePressed( unsigned char button ) { return  ( ( ( ClickStatus & button ) == button ) ? true : false ) && !( ( ( PrevClickStatus & button ) == button ) ? true : false ); }
  inline bool MouseReleased( unsigned char button ) { return  !( ( ( ClickStatus & button ) == button ) ? true : false) && ( ( ( PrevClickStatus & button ) == button ) ? true : false ); }
  inline bool MouseDown( unsigned char button ) { return ( ClickStatus & button ) == button ; }
  inline bool MouseUp( unsigned char button ) { return !( ( ClickStatus & button ) == button ); }
  
  inline bool KeyDown( unsigned char key ) { return ( KeyBuffer[ key ] ? true : false ); }
  inline bool KeyUp( unsigned char key ) { return !KeyBuffer[ key ]; }
  inline bool KeyPressed( unsigned char key ) { return ( ( KeyBuffer[ key ] ? true : false ) && !( PrevKeyState[ key ] ? true : false ) ); }
  inline bool KeyReleased( unsigned char key ) { return ( !( KeyBuffer[ key ] ? true : false ) && ( PrevKeyState[ key ] ? true : false ) ); }

  HWND hWnd;

  LPDIRECTINPUT8  g_lpDI;
  LPDIRECTINPUTDEVICE8 g_lpDIDevice;
  LPDIRECTINPUTDEVICE8 g_pMouse;
  LPDIRECTINPUTDEVICE8 g_pCont;

  bool controllerFound;
  DIJOYSTATE controllerState;
  DIJOYSTATE prevControllerState;

  char KeyBuffer[BUFFER_SIZE];
  char PrevKeyState[BUFFER_SIZE];

  unsigned char ClickStatus;
  unsigned char PrevClickStatus;
  
  //x and y change in position since last frame
  int XDiff;
  int YDiff;
  int ZDiff;
  int XPos;
  int YPos;
};