#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include "console.h"

#define consoleWidth	80
#define consoleHeight	25
#define roadWidth		25



//-----------------------------------------------------

struct MotO	// một ô (vertex)
{
	unsigned char kitu;
	int mau;
};


MotO buffer[consoleHeight][roadWidth+1];

void GanO (int dong, int cot, int kitu, int mau=7)
{
	buffer[dong][cot].kitu = kitu;
	buffer[dong][cot].mau = mau;
}

//-----------------------------------------------------


struct ToaDo
{
	int y, x;	// y là tung độ, x là hoành độ
};


struct HinhDang
{
	char a[3][3];
};


struct Xe
{
	ToaDo td;	// tọa độ tại vị trí hd.a[1][1]
	HinhDang hd;
	int diem;
};


struct VatCan
{
	ToaDo td;
	HinhDang hd;
};


void KhoiTao (Xe &xe, VatCan &vc)
{
	//------ Xe
#define pHD xe.hd.a

	xe.diem = 0;
	xe.td.y = consoleHeight - 2;
	xe.td.x = roadWidth / 2;

	pHD[0][0] = pHD[0][2] = pHD[2][0] = pHD[2][2] = 'o';	// bánh xe
	pHD[0][1] = '^';	// đầu xe
	pHD[1][0] = pHD[1][2] = '|';	// thân xe
	pHD[2][1] = '-';
	pHD[1][1] = ' ';


	//---------vật cản

	vc.td.x = 2 + rand() % (roadWidth - 4);
	vc.td.y = -2;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			vc.hd.a[i][j] = 173;


	//---------- buffer
	for (int i = 0; i < consoleHeight; i++)
		for (int j = 0; j < roadWidth; j++)
			GanO (i, j, ' ');
}



void HienThi (Xe xe, VatCan vc)
{
	// hiển thị biên
	for (int i = 0; i < consoleHeight; i++)
	{
		GanO (i, 0, 179);
		GanO (i, roadWidth, 179);
	}


	// hiển thị xe
	for (int kDong = -1; kDong <= 1; kDong++)
		for (int kCot = -1; kCot <= 1; kCot++)
		{
			int x = kCot + xe.td.x;
			int y = kDong + xe.td.y;

			GanO (y, x, xe.hd.a[kDong+1][kCot+1], ColorCode_Cyan);
		}


	// hiển thị vật cản
	for (int kDong = -1; kDong <= 1; kDong++)
		for (int kCot = -1; kCot <= 1; kCot++)
		{
			int x = kCot + vc.td.x;
			int y = kDong + vc.td.y;

			if (y >= 0 && y < consoleHeight)
			{
				GanO (y, x, vc.hd.a[kDong+1][kCot+1], ColorCode_Red);
			}
		}


	
	// in toàn bộ buffer ra màn hình
	gotoXY (0, 0);

	for (int i = 0; i < consoleHeight; i++)
	{
		for (int j = 0; j <= roadWidth; j++)
		{
			TextColor (buffer[i][j].mau);
			putchar (buffer[i][j].kitu);

			buffer[i][j].kitu = ' ';
		}

		if (i < consoleHeight - 1)
			putchar ('\n');
	}


	// hiển thị trạng thái game
	gotoXY (roadWidth + 2, 10);
	printf ("Diem = %d", xe.diem);
}


void DieuKhien (Xe &xe)
{
	if (_kbhit())
	{
		int key = _getch();

		if ((key == 'A' || key == 'a') && xe.td.x > 2)
			xe.td.x--;
		else if ((key == 'D' || key == 'd') && xe.td.x < roadWidth - 2)
			xe.td.x++;
	}
}

// trả về 0 nếu ko có gì
// trả về -1 là va chạm
int XuLy (Xe &xe, VatCan &vc, int &ThoiGianSleep)
{
	// tạo cảm giác xe di chuyển --> vật cản đi xuống dưới
	vc.td.y++;


	// nếu vật cản đi xuống dưới đáy là xe đã vượt qua vật cản
	// thì tăng điểm
	if (vc.td.y > consoleHeight)
	{
		xe.diem++;
		vc.td.x = 2 + rand() % (roadWidth - 4);
		vc.td.y = -2;
		
		if (ThoiGianSleep > 20)
			ThoiGianSleep -= 20;
	}

	// xử lý tình huống va chạm
	int dX = abs (xe.td.x - vc.td.x);
	int dY = abs (xe.td.y - vc.td.y);

	if (dX < 3 && dY < 3)
	{
		return -1;
	}


	return 0;
}


int main()
{
	srand (time (NULL));	// khởi tạo bộ sinh số ngẫu nhiên

	Xe xe;
	VatCan vc;
	
	KhoiTao (xe, vc);

	int ma = 0;
	int ThoiGianSleep = 140;

	while (1)
	{
		// hiển thị
		HienThi (xe, vc);

		// điều khiển
		DieuKhien (xe);
		
		// xử lý
		ma = XuLy (xe, vc, ThoiGianSleep);

		// game over
		if (ma == -1)
		{
			gotoXY (0,0);
			printf ("Ban thua game rui, liu liu :P ");

			while (_getch() != 13)	;

			break;
		}

		//Sleep (ThoiGianSleep);
	}


	return 0;
}