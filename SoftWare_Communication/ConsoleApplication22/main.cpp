#include <stdio.h>
#include <tchar.h>
#include <string>
#include "SerialClass.h"
#include <iostream>
#include <Windows.h>
#include <vector>
#include <process.h>
#include "packet_imu.h"





bool  MouseFlag = false;

struct node{
	int x;
	int y;
};

HWND hwnd = GetDesktopWindow();
HDC hdc = GetWindowDC(hwnd);
HWND current = GetForegroundWindow();
//std::vector<node> info;
//struct node temp;
//HANDLE hMutex;
//int mutexTime = 100;

/*
void thread_func(void *arg){
	while (1){
		WaitForSingleObject(hMutex, mutexTime);

		for (unsigned int k = 0; k < info.size(); ++k){
			for (int i = -3; i < 4; ++i){
				for (int j = -3; j < 4; ++j){
					SetPixel(hdc, info[k].x + i, info[k].y + j, RGB(255, 0, 0));
				}
			}
		}
		ReleaseMutex(hMutex);
	}
}
*/

int _tmain(int argc, _TCHAR* argv[])
{
	int HoldFlag = false;
	int before;
	int i, j;
	int pointX = 0, pointY = 0;
	int maxPixel;
	int maxX, maxY;
	bool PaintingFlag = false;
	int PrePage = 0, NextPage = 0;
//	info.clear();
//	info.reserve(1000);
//	HANDLE* threads = new HANDLE[1];
	
//	hMutex = CreateMutex(NULL, FALSE, NULL);

	POINT point;
	GetCursorPos(&point);

	maxX = GetSystemMetrics(SM_CXSCREEN) / 2;
	maxY = GetSystemMetrics(SM_CYSCREEN) / 2;


	if (GetSystemMetrics(SM_CXSCREEN) > GetSystemMetrics(SM_CYSCREEN))
		maxPixel = GetSystemMetrics(SM_CXSCREEN);
	else
		maxPixel = GetSystemMetrics(SM_CYSCREEN);
	printf("Welcome to the serial test app!\n\n");
	char comport[256] = { 0 };
	scanf("%s", comport);
	Serial* SP = new Serial(comport);
	//threads[0] = (HANDLE)_beginthread(thread_func, 0, 0);
	if (SP->IsConnected())
		printf("We're connected");
//	for (int i = 0; i < 90000000; ++i);
//	ShowWindow(current, SW_MINIMIZE);
	while (SP->IsConnected())
	{

		before = data_overarm.sw[0];
		serialCom(SP);
		printf("\n%.2f, %.2f, %.2f", data_overarm.vect_underarm[1], data_overarm.vect_underarm[8], data_overarm.vect_underarm[7]);
		//start
		if (data_overarm.vect_underarm[7] <= -0.70 && before == 1 && data_overarm.sw[0] == 2)
		{
			if (HoldFlag == false)
			{
				//WaitForSingleObject(hMutex, mutexTime);
				//info.clear();
				InvalidateRect(NULL, NULL, true);
				HoldFlag = true; 
				//ReleaseMutex(hMutex);
			}
			else
			{
				HoldFlag = false;
				continue;
			}
		}

		if (HoldFlag == true)
		{
			continue;
		}

		if (data_overarm.vect_underarm[7] >= 0.70 && before == 1 && data_overarm.sw[0] == 2)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, pointX, pointY, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, pointX, pointY, 0, 0);
		}
		else if (data_overarm.sw[0] == 1 && before == 2)
		{
			mouse_event(MOUSEEVENTF_LEFTUP, pointX, pointY, 0, 0);
			PaintingFlag = false;
		}


		else if (data_overarm.sw[0] == 2 && before == 1)      // LeftClick
		{                                 // un
			if (data_overarm.vect_underarm[8] < 0)   // +++ painting
			{
				PaintingFlag = true;
				//painting
			}
			if (data_overarm.vect_underarm[8]>0)
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, pointX, pointY, 0, 0);
			}
		}
		else if (data_overarm.pg[1] != NextPage)   // nextPage
		{
			NextPage = data_overarm.pg[1];
			keybd_event(VK_NEXT, 0x22, 0, 0);

			keybd_event(VK_NEXT, 0x22, KEYEVENTF_KEYUP, 0);
		}
		else if (data_overarm.pg[0] != PrePage) // prevPage
		{
			PrePage = data_overarm.pg[0];
			keybd_event(VK_PRIOR, 0x21, 0, 0);
			keybd_event(VK_PRIOR, 0x21, KEYEVENTF_KEYUP, 0);
		}
		if (PaintingFlag == true)
		{
		//	WaitForSingleObject(hMutex, mutexTime);
			//         printf("%d %d \n", pointX - (maxPixel / 2), (maxPixel / 2) + pointY);
			for (i = -8; i < 9; ++i){
				for (j = -8; j < 9; ++j){
					SetPixel(hdc, maxX + pointX + i, maxY + pointY + j, RGB(255, 0, 0));
				//	temp.x = maxX + pointX; temp.y = maxY + pointY;
					//info.push_back(temp);
					
				}
			}
			//ReleaseMutex(hMutex);
		}


		if (data_overarm.vect_handpoint[1] <= 1.5)
			pointX = (maxPixel / 2)*(data_overarm.vect_handpoint[1] / 1.5);
		if (data_overarm.vect_handpoint[2] <= 1.5)
			pointY = ((maxPixel / 2)*(data_overarm.vect_handpoint[2] / 1.5))*-1;

		SetCursorPos(maxX + pointX, maxY + pointY);    // move


	}
	return 0;
}


// 페인트 클리어