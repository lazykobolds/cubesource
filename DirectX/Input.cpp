#include <stdio.h>
#include "input.h"

InputBuffer::InputBuffer( void )
{
}

void InputBuffer::Initilize( HWND hwnd, HINSTANCE hinst )
{
  controllerFound = true;
  hWnd = hwnd;
	HRESULT         hr;
	// Create the direct input object
	hr = DirectInput8Create(hinst, DIRECTINPUT_VERSION, 
			IID_IDirectInput8, (void**)&g_lpDI, NULL);

	if FAILED(hr) 
	{ 
		printf( "Direct input failed to initilize!!" );
			// DirectInput not available; take appropriate action 
	} 

	/***************************************************************
	******* Create Devices******************************************
	***************************************************************/
	//create keyboard device
	hr = g_lpDI->CreateDevice(GUID_SysKeyboard, &g_lpDIDevice, NULL); 
	if FAILED(hr) 
	{
		printf( "Unable to get a Direct Input keyboard device!!\n" );
	}

	//create the mouse device
	hr = g_lpDI->CreateDevice(GUID_SysMouse, &g_pMouse, NULL);
	if (FAILED(hr)) 
	{
		printf( "Unable to get a Direct Input mouse device!!\n" );
	}

	hr = g_lpDI->CreateDevice(GUID_Joystick, &g_pCont, NULL);
	if(FAILED(hr)) 
	{
		printf( "Unable to get a Direct Input controller device!!\n" );
		controllerFound = false;
	}

	/***************************************************************
	******* Set Date format*****************************************
	***************************************************************/
	//set keyboard
	hr = g_lpDIDevice->SetDataFormat(&c_dfDIKeyboard); 
	if FAILED(hr) 
	{ 
    printf( "Failed to set the Direct Input keyboard data format!!\n" );
	} 

	// set mouse
	hr = g_pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) 
	{
    printf( "Failed to set the Direct Input mouse data format!!\n" );
	}

  if(controllerFound)
  {
    hr = g_pCont->SetDataFormat(&c_dfDIJoystick);
	  if (FAILED(hr)) 
	  {
      printf( "Failed to set the Direct Input mouse controller format!!\n" );    
	  }
  }



	/***************************************************************
	******* Set Cooperative level***********************************
	***************************************************************/
	//Set keyboard level
	hr = g_lpDIDevice->SetCooperativeLevel(hwnd, 
			DISCL_BACKGROUND | DISCL_NONEXCLUSIVE); 

	 if FAILED(hr) 
	 { 
     printf( "Failed to set Direct Input keyboard cooperative level!!\n" );
	 }

	 //set mouse level
	 hr = g_pMouse->SetCooperativeLevel(hwnd,
               DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) 
	{
     printf( "Failed to set Direct Input mouse cooperative level!!\n" );
	}

  //set controller level
  if(controllerFound)
  {
    hr = g_pCont->SetCooperativeLevel(hwnd, 
			  DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_BACKGROUND | DISCL_EXCLUSIVE); 

	   if FAILED(hr) 
	   { 
       printf( "Failed to set Direct Input controller cooperative level!!\n" );
	   }
  }

	/***************************************************************
	******* Prepare Mouse for buffered input************************
	***************************************************************/

	DIPROPDWORD dipdw;
    // the header
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    // the data
    dipdw.dwData            = INPUT_BUFFER_SIZE;

	hr = g_pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (FAILED(hr)) 
	{
    printf( "Failed to set Direct Input mouse buffer!!\n" );
	}

	/***************************************************************
	******* Acquire Devices ****************************************
	***************************************************************/
	 if (g_lpDIDevice) g_lpDIDevice->Acquire(); 
	 if (g_pMouse) g_pMouse->Acquire();
	//if (g_pCont) g_pCont->Acquire();
   
   
   //DEFAULT MOUSE POSITION
   XPos = YPos = 300;
  }

void InputBuffer::UpdateInputBuffer( void )
{
	/***************************************************************
	******* Keyboard Update ****************************************
	***************************************************************/
	HRESULT  hr; 
	memcpy(PrevKeyState, KeyBuffer, sizeof(char)*256);
  hr = g_lpDIDevice->GetDeviceState(sizeof(KeyBuffer), (LPVOID)KeyBuffer);
  
	if FAILED(hr)
	{
		if (g_lpDIDevice) g_lpDIDevice->Acquire(); 
	}

  /***************************************************************
	******* Controller Update **************************************
	***************************************************************/
	if(controllerFound)
	{
	  prevControllerState = controllerState;
	  hr = g_pCont->GetDeviceState(sizeof(controllerState), &controllerState);
	  
		if FAILED(hr)
		{
			if (g_pCont) g_pCont->Acquire(); 
		}
	}

	/***************************************************************
	******* Mouse Update *******************************************
	***************************************************************/

	DIDEVICEOBJECTDATA didod[ INPUT_BUFFER_SIZE ];  // Receives buffered data 
    DWORD dwElements;
	DWORD i;


	dwElements = INPUT_BUFFER_SIZE;
    hr = g_pMouse->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ),
                                  didod, &dwElements, 0 );

	if( hr != DI_OK )
    {
		hr = g_pMouse->Acquire();
        while( hr == DIERR_INPUTLOST )
            hr = g_pMouse->Acquire();
	}

	if( FAILED( hr ) )
	{
    //printf( "Failed to acquire Direct Input mouse input!!\n" );

	}

  //RightClick = LeftClick = MiddleClick = XDiff = YDiff = 0;
  PrevClickStatus = ClickStatus;
  XDiff = YDiff = ZDiff = 0;
 
  for( i = 0; i < dwElements; i++ )
  {
    switch( didod[ i ].dwOfs )
    {
      case DIMOFS_BUTTON0://left click
        //if( didod[i].dwData & 0x80 ) LeftClick = 1;
        //else LeftClick = 0;
        if( didod[i].dwData & 0x80 ) ClickStatus |= LEFT_CLICK_BIT;
        else ClickStatus &= !LEFT_CLICK_BIT;
        break;
      case DIMOFS_BUTTON1://right click
        //if( didod[i].dwData & 0x80 ) RightClick = 1;
        //break;
        if( didod[i].dwData & 0x80 ) ClickStatus |= RIGHT_CLICK_BIT;
        else ClickStatus &= !RIGHT_CLICK_BIT;
        break;
      case DIMOFS_BUTTON2://middle click
        //if( didod[i].dwData & 0x80 ) MiddleClick = 1;
        //break;
        if( didod[i].dwData & 0x80 ) ClickStatus |= MIDDLE_CLICK_BIT;
        else ClickStatus &= !MIDDLE_CLICK_BIT;
        break;
      case DIMOFS_BUTTON7:
	      //didod[ i ].dwData & 0x80 if the button is up, else down
	      break;
      case DIMOFS_X:
	    //didod[ i ].dwData for mouse offset from last position
	      //printf("X %i \n", (int)didod[i].dwData);
        XDiff = (int)didod[i].dwData;
        XPos += XDiff;
        //printf("XDiff: %i\n", XDiff );
        //XDiff = (int)didod[i].dwData - XPos;
        //XPos = (int)didod[i].dwData;
        
	      break;
      case DIMOFS_Y:
	      //didod[ i ].dwData for mouse offset from last position
	      //printf("Y %i \n", (int)didod[i].dwData);
        YDiff = (int)didod[i].dwData;
        YPos += YDiff;
        //printf("YDiff: %i\n", YDiff );
        //YDiff = (int)didod[i].dwData - YPos;
        //YPos = (int)didod[i].dwData;
	      break;
      case DIMOFS_Z:
	      //didod[ i ].dwData for mouse offset from last position(Z is scroll bar)
        ZDiff = (int)didod[i].dwData;
	      break;
    }
  }

 POINT point;
 GetCursorPos( &point );
 ScreenToClient( hWnd, &point );
 XPos = point.x;
 YPos = point.y;// + 15;
}