#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
#include<ctime>
#include<math.h>
#include<windows.h>
using namespace std;
#define MAP_COL 9
#define MAP_ROW 9
#define WIN_WIDETH 900
#define WIN_LENGTH 660
#define cellFind(i,j,k,o) if(currentPos.X > i && currentPos.X < j && currentPos.Y>k && currentPos.Y < o) //查询是否在方格中
COORD currentPos = { 0,0 }, numPos = { 10,0 }, coord, waitPos;//位置坐标：鼠标触碰数字位置，键盘存储数字位置，小九宫格左上角数字行列号，数独数组上行列号
clock_t t1, t2;//显示时间
MOUSEMSG msg;
int sleepTime = 0;
int flag_R = 1;//定义键盘红框标志
int num = 1;//记录键盘数字
int process;//定义进程
int second = 0, minute = 0;//时间转换成分秒
int gameDifficulty = 0,  restart, faults, endGame,menu=0;//定义游戏难度、重新开始标志，错误个数，结束游戏，菜单栏选项
int isWin = 0, noMistake = 0;//定义胜利标志，错误标志
int Map1[MAP_ROW][MAP_COL], Map2[MAP_ROW][MAP_COL];
int Map_HL[MAP_ROW][MAP_COL], Map_Mistake[MAP_ROW][MAP_COL];//定义数独数组、标志数组，高亮数组，错误数组
int r=249, g=90, b=140,rgb_flag=1000;
int directFlag = 1;
int ChangeColor(int min,int max,int self)
{
	rgb_flag++;
	if (rgb_flag >= 10000000)
	{
		rgb_flag = 1000;
	}
	if (rgb_flag %12 == 0)
	{
		if (self < (min +2))
		{
			directFlag = 1;
		}
		if (self > (max - 2))
		{
			directFlag = 0;
		}
		if (directFlag)
		{
			self+=(rand()%2+1);
		}
		if (!directFlag)
		{
			self-= (rand() % 2 + 1);
		}
	}
	return self;
}
void Colorful(int& r,int &g,int &b)
{    
	
	r = ChangeColor(10,240,r) ;
	g = ChangeColor(10, 240, g);
	b = ChangeColor(10,255, b);
}
void DataInit();
void GameInit();
void DigNumber(int post);
void GameUpdate();
void GameDraw();
void start_1();
void select_1();
void start_2();
void select_2();
void start_3();
void select_3();
void start();
void HighLight_1(int a,int b,int c,int d);
void HighLight_2(int a);
void end();
int FindMistake();
int WinJudge();
int flag_1 = 0;
COORD FindLeftUpperCornor(COORD a);
int main()
{
	void (*pr[3])() = { select_1,select_2,select_3 };
	
	initgraph(WIN_WIDETH,WIN_LENGTH);
	while (1)
	{
		DataInit();
		while (process < 3)
			pr[process++]();
		t1 = clock();
		GameInit();
		while (1)
		{
			if (MouseHit())
				msg = GetMouseMsg();
			GameUpdate();
			GameDraw();
			if (noMistake == 0)
				isWin = WinJudge();
			if (isWin || restart)
			{
				Sleep(100);
				break;
			}
		}
		if (isWin)
			end();
		if (endGame)
			break;
		
	}
	closegraph();
	/*for (int i = 0; i < 9; i++)   测试Map1代码
	{
		for (int j = 0; j < 9; j++)
		{
			cout << Map1[i][j] << "  ";
		}
		cout << endl;
	}*/
	return 0;
}
void start()  //界面初始化
{
	BeginBatchDraw();
	setbkcolor(WHITE); //设置窗口背景颜色
	settextcolor(BLACK);
	setbkmode(1); //设置文字背景透明
	cleardevice();
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 3);
}
void DataInit()//数据初始化
{
	flag_1 = 0;
		process = 0;
		restart = 0;
		sleepTime = 0;
		faults = 0;
		endGame = 0;
		for (int i = 0; i < MAP_ROW; i++)
			for (int j = 0; j < MAP_COL; j++)
			{
				Map1[i][j] = Map2[i][j] = Map_HL[i][j] = Map_Mistake[i][j] = 0;
			}
}
void GameInit()//制作随机数独表
{   //生成1到9随机数
	srand((unsigned)time(NULL));
	int a[10] = {0};
	int b[10] = {0};
	for (int i = 1; i <= 9; i++)
	{
		do {
			a[i] = rand() % 9 + 1;
		} while (b[a[i]]);
		b[a[i]]++;
	}
	//表一
	int map1[9][9] =
	  { {9,8,2,5,3,6,1,7,4},
		{4,3,6,7,1,9,5,8,2},
		{1,5,7,4,8,2,3,6,9},
		{8,4,5,6,2,7,9,3,1},
		{2,6,3,1,9,5,7,4,8},
		{7,1,9,8,4,3,2,5,6},
		{3,2,4,9,5,8,6,1,7},
		{6,9,1,3,7,4,8,2,5},
		{5,7,8,2,6,1,4,9,3}
	  };
	//表二
	int map2[9][9]
	 {
        {1,2,3,7,8,9,4,5,6},
		{4,5,6,1,2,3,7,8,9},
		{7,8,9,4,5,6,1,2,3},
		{3,1,2,9,7,8,6,4,5},
		{6,4,5,3,1,2,9,7,8},
		{9,7,8,6,4,5,3,1,2},
		{2,3,1,8,9,7,5,6,4},
		{5,6,4,2,3,1,8,9,7},
		{8,9,7,5,6,4,2,3,1}
     };
	int flag = rand() % 2;

	for (int i = 0; i < 9; i++)//生成多种可能的数表
	{
		for (int j = 0; j < 9; j++)
		{
			if (flag)	Map1[i][j] = a[(map1[i][j])];
			else        Map1[i][j] = a[(map2[i][j])];
		}
	}
	DigNumber(gameDifficulty);
}
void DigNumber(int post)//挖掘置空部分数字
{
	srand((unsigned)time(NULL));
	//难度决定置空个数
	int way[3][9] = {
	  {2,2,2,3,3,3,3,3,4},
	  { 4,4,4,4,4,5,5,6,6 },
	  { 5,5,5,5,5,6,6,7,7 } };
	int n, a[9] = { 0 }, temp[9] = {0};
	for (int i = 0; i < 9; i++)
	{
		do {
			n = rand() % 9;
		} while (a[n]);
		a[n]++;
		temp[i] = way[post][n];//随机决定每个九宫格置空多少个
	}
	int cell = 0, x, y;//记录坐标及九宫格中位置
	for (int i = 0; i < 9; i += 3)//按九宫格置空数字，用Map2显示
		for (int j = 0; j < 9; j += 3)
		{
			int dig[3][3] = { 0 };//九宫格中数字位置
			for (int k = 0; k < temp[cell]; k++)
			{
				do {
					x = rand() % 3;
					y = rand() % 3;
				} while (dig[x][y]);
				dig[x][y] = 1;
				Map2[i + x][j + y] = 1;//标记被挖去的位置
				//Map1[i + x][j + y] = 0;
			}
			cell++;
		}
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (Map2[i][j])
				Map1[i][j] = 0;

};
void start_1()//界面一
{
	start();
	LOGFONT f;
	gettextstyle(&f);  
	f.lfWeight =500;// 获取当前字体设置
	f.lfHeight = 100;             
	_tcscpy_s(f.lfFaceName,"华文新魏");    
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f);                     // 设置字体样式
	outtextxy(330, 100,"数  独");
	settextstyle(25, 0, "华文新魏");
	HighLight_1(340, 290, 560, 360);
	outtextxy(350, 308, "学高数不如玩数独");
	HighLight_1(340, 390, 560, 460);
	settextstyle(30, 0, "华文新魏");
	outtextxy(387, 405, "啥也不是");
	if (flag_1 == 1)
	{
		outtextxy(220, 505, "说了啥也不是还点，szx你果然是个憨憨");
	}
	EndBatchDraw();
};
void select_1() //界面一 选择项
{
	int post = 1 ;
	while (post)
	{
		start_1();
		MOUSEMSG msg = GetMouseMsg(); // 获取鼠标信息
		int x = msg.x;
		int y = msg.y;
		switch (msg.uMsg)
		{
		case WM_MOUSEMOVE:
			currentPos.X = x;
			currentPos.Y = y;
			break;
		case WM_LBUTTONUP:
		cellFind(340,560,290,360)
			{
				menu = 1; 
				post = 0;
			}
		cellFind(340,560 , 390, 460)
		{
			flag_1 = 1;
		}
			break;
		default:
			break;
		}
	}


}
void start_2()//界面二
{
	start();
	settextstyle(40, 0, "华文新魏");
	HighLight_1(340, 90, 560, 160);
	outtextxy(380, 105, " 容    易");
	HighLight_1(340, 190, 560, 260);
	outtextxy(380, 205, " 一    般");
	HighLight_1(340, 290, 560, 360);
	outtextxy(380, 305, " 专    家");
	HighLight_1(340, 390, 560, 460);
	outtextxy(380, 405, " 返    回");
	EndBatchDraw();
}
void select_2()//界面二 选择项
{
	int post = 1;
	while (post)
	{
		start_2();
		MOUSEMSG msg = GetMouseMsg(); // 获取鼠标信息
		int x = msg.x;
		int y = msg.y;
		switch (msg.uMsg)
		{
		case WM_MOUSEMOVE:
			currentPos.X = x;
			currentPos.Y = y;
			break;
		case WM_LBUTTONUP:
			cellFind(340,560,90,160)
			{
				post = 0;
				gameDifficulty = 0;
			}
			cellFind(340, 560, 190, 260)
			{
				post = 0;
				gameDifficulty = 1;
			}
			cellFind(340, 560, 290, 360)
			{
				post = 0;
				gameDifficulty = 2;
			}
			cellFind(340, 560, 390, 460)
			{
				post = 0;
				process -= 2;
			}
			break;
		default:
			break;
		}
	}
}
void start_3()//界面三
{
	start();
	settextstyle(40, 0, "华文新魏");
	HighLight_1(340, 190, 560, 260);
	outtextxy(370, 205, "开始游戏");
	HighLight_1(340, 390, 560, 460);
	outtextxy(370, 405, "  返    回");
	EndBatchDraw();
}
void select_3()//界面三 选择项
{
	int post = 1;
	while (post)
	{
		start_3();
		MOUSEMSG msg = GetMouseMsg(); // 获取鼠标信息
		int x = msg.x;
		int y = msg.y;
		switch (msg.uMsg)
		{
		case WM_MOUSEMOVE:
			currentPos.X = x;
			currentPos.Y = y;
			break;
		case WM_LBUTTONUP:
			cellFind(340,560,190,260)
				post = 0;
			cellFind(340, 560, 390, 460)
			{
				post = 0;
				process -= 2;
			}
			break;
		default:
			break;
		}
	}
}
void HighLight_1(int a, int b, int c, int d) //选项框高亮
{
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 3);
	rectangle(a, b, c, d);
	if (currentPos.X > a && currentPos.X < c && currentPos.Y>b && currentPos.Y < d)
	{
		settextcolor(RGB(149, 216, 207));
		line(a+30, b+60,c-30, d-10);
	}
	else
		settextcolor(BLACK);
}
void HighLight_2(int a)//九宫格高亮
{
	int i, j;
	//清除上次的高亮数据
	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			Map_HL[i][j] = 0;
	if (waitPos.Y >= 0 && waitPos.Y <= 8 && waitPos.X >= 0 && waitPos.X <= 8)
	{
		/*两种高亮模式*/
		if (a)//1.高亮相同的数字
		{
			for (i = 0; i < 9; i++)
				for (j = 0; j < 9; j++)
					if (Map1[i][j] == Map1[waitPos.Y][waitPos.X])
						Map_HL[i][j] = 1;


		}
		else//2.高亮同行、同列、同宫
		{
			for (i = 0; i < 9; i++)
				Map_HL[i][waitPos.X] = 2;
			for (i = 0; i < 9; i++)
				Map_HL[waitPos.Y][i] = 2;
			//2.1根据每一宫的左上角的行列来高亮对应的那一宫
			for (i = coord.X; i < coord.X + 3; i++)
				for (j = coord.Y; j < coord.Y + 3; j++)
					if (i >= 0 && i < 9 && j >= 0 && j < 9)
						Map_HL[i][j] = 2;
		}
	}


}
void GameUpdate()//游戏界面更新
{
	if (MouseHit())
		msg = GetMouseMsg();

	int x = msg.x;
	int y = msg.y;
	switch (msg.uMsg)
	{
	case WM_MOUSEMOVE:
		if (x > 60 && x < 600 && y > 60 && y < 600)
		{
			flag_R = 1;
			waitPos.X = (x - 60) / 60;
			waitPos.Y = (y - 60) / 60;
			coord.X = waitPos.Y;
			coord.Y = waitPos.X;
			coord = FindLeftUpperCornor(coord);
			if (Map1[waitPos.Y][waitPos.X])
				HighLight_2(1);
			else
				HighLight_2(0);
		}
		else
		{
			//消除红框
			flag_R = 0;
			//消除高亮
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++)
					Map_HL[i][j] = 0;
		}
		currentPos.X = x;
		currentPos.Y = y;
		break;
	case WM_LBUTTONUP:
		if (x > 660 && x < 840 && y>60 && y < 240)
		{
			numPos.X = (x - 60) / 60;
			numPos.Y = (y - 60) / 60;
			num = 3 * numPos.Y + (numPos.X - 10) + 1;
		}
		else if (x > 720 && y > 240 && x < 780 && y < 300)
		{
			numPos.X = (x - 60) / 60;
			numPos.Y = (y - 60) / 60;
			num = 0;
		}
		else if (x > 60 && x < 600 && y > 60 && y < 600)
		{
			if (Map2[waitPos.Y][waitPos.X] == 1)
				Map1[waitPos.Y][waitPos.X] = num;
			else
			{
				clock_t t3, t4;
				t3 = clock();
				MessageBox(GetHWnd(), "提示数字不可修改", "注意", 0);
				t4 = clock();
				sleepTime += (int)(double)(t4 - t3) / CLOCKS_PER_SEC;
			}
			noMistake = FindMistake();
			faults += noMistake;
		}
		else if (currentPos.X > 660 && currentPos.X < 840 && currentPos.Y > 350 && currentPos.Y < 400)
		{
			clock_t t3, t4;
			t3 = clock();
			MessageBox(GetHWnd(), "暂停中...", "暂停了你在干嘛(。￣v￣)", 0);
			t4 = clock();
			sleepTime += (int)(double)(t4 - t3) / CLOCKS_PER_SEC;
		}
		else if (currentPos.X > 660 && currentPos.X < 840 && currentPos.Y > 420 && currentPos.Y < 470)
		{
			clock_t t3, t4;
			t3 = clock();
			int result = MessageBox(GetHWnd(), "重新开始么?", "这是重新开始哦o(￣▽￣)ｄ", MB_YESNO);
			switch (result)
			{
			case IDNO:
				break;
			case IDYES:
				process = 1;
				restart = 1;
				break;
			}
			t4 = clock();
			sleepTime += (int)(double)(t4 - t3) / CLOCKS_PER_SEC;
		}
		break;
	default:
		break;
	}
	msg.uMsg = WM_MBUTTONUP;
}
void GameDraw()//游戏界面绘制
{
	start();
	LOGFONT f;
	gettextstyle(&f);                    
	f.lfHeight = 40;                     
	_tcscpy_s(f.lfFaceName, "楷体");   
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f);
	setlinecolor(BLACK);
	for (int row = 0; row <= MAP_ROW ; row++) //横线
	{
		if ((row) % 3 == 0)
			setlinestyle(PS_SOLID, 3);
		else
			setlinestyle(PS_DASH, 1);
		line(60, row * 60+60, 600, row * 60+60);
	}
	for (int col = 0; col <= MAP_COL ; col++) //竖线
	{
		if ((col ) % 3 == 0)
			setlinestyle(PS_SOLID, 3);
		else
			setlinestyle(PS_DASH, 1);
		line(col * 60+60, 60, col * 60+60, 600);
	}
	//高亮提示数字
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (Map_HL[i][j])
			{
				if (Map_HL[i][j] == 1)
					setfillcolor(RGB(119, 213, 252));//相同数字用绿色显示
				else
					setfillcolor(RGB(174, 174, 174));//同行列宫用浅灰色显示
				solidrectangle((j + 1) * 60 + 2, (i + 1) * 60 + 2, (j + 2) * 60 - 2, (i + 2) * 60 - 2);//填充小矩形用于上色
			}

	if (flag_R)//存红框数字
	{
		setfillcolor(RGB(99, 99, 99));
		solidrectangle((waitPos.X + 1) * 60 + 2, (waitPos.Y + 1) * 60 + 2, (waitPos.X + 2) * 60 - 2, (waitPos.Y + 2) * 60 - 2);
	}
	//标记错误矩形
	setfillcolor(RGB(245,192,203));
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (Map_Mistake[i][j])
			{
				solidrectangle((j + 1) * 60 + 2, (i + 1) * 60 + 2, (j + 2) * 60 - 2, (i + 2) * 60 - 2);
			}
	//画宫格数字
	TCHAR str[10];
	for (int row = 0; row < MAP_ROW; row++)
	{
		for (int col = 0; col < MAP_COL; col++)
		{
			if (Map1[row][col])
			{
				if (Map2[row][col] == 1)
					settextcolor(RGB(133,160,210));
				else if (Map_HL[row][col] || Map_Mistake[row][col])
					settextcolor(WHITE);
				else
					settextcolor(RGB(61, 61, 61));
				_stprintf_s(str, "%d", Map1[row][col]);
				outtextxy((col + 1) * 60 + 20, (row + 1) * 60 + 10, str);
			}
		}
	}
	//画数字键盘内外框
	setlinestyle(PS_SOLID, 1);
	rectangle(660, 60, 840, 300);
	rectangle(650, 50, 850, 310);
	//画数字
	settextcolor(RGB(35, 150, 250));
	setlinestyle(PS_SOLID, 3);
	int n = 1;
	outtextxy(720, 250, "del");
	for (int col = 0; col <= 2; col++)//用数独数组的行列做数字键盘
	{
		for (int row = 10; row <= 12; row++)
		{
			_stprintf_s(str, "%d", n++);
			outtextxy((row + 1) * 60 + 20, (col + 1) * 60 + 10, str);
		}
	}
	//用红框标注数字键盘中的当前所选数字
	Colorful(r, g, b);
	setlinecolor(RGB(r,g,b/*64, 224, 208*/));
	setlinestyle(PS_SOLID, 3);
	rectangle((numPos.X + 1) * 60, (numPos.Y + 1) * 60, (numPos.X + 2) * 60, (numPos.Y + 2) * 60);
	//画计时器
	settextcolor(RGB(109, 214, 185));
	t2 = clock();
	second = (int)(double)(t2 - t1) / CLOCKS_PER_SEC;
	second -= sleepTime;//减去停止的时间
	minute = second / 60;
	second %= 60;
	_stprintf_s(str,"%02d:%02d", minute, second);
	outtextxy(0, 0, str);
	//画左下角暂停、重新开始
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 2);
	settextstyle(30, 0, _T("楷体"));
	setfillcolor(RGB(158, 160, 169));
	cellFind(660,840,350,400)
	{
		solidroundrect(660, 350, 830, 400,10,10);
		settextcolor(WHITE);
	}
	else
		settextcolor(BLACK);
	outtextxy(670, 360, " 暂    停");

	cellFind(660, 840, 420, 470)
	{
		solidroundrect(660, 420, 830, 470,10,10);
		settextcolor(WHITE);
	}
	else
		settextcolor(BLACK);
	outtextxy(670, 430, " 重新开始");
	setfillcolor(BLACK);
	solidcircle(650, 380, 6);
	solidcircle(650, 450, 6);
	EndBatchDraw();
}
COORD FindLeftUpperCornor(COORD a)//查找小九宫格左上角位置
{
	COORD b;
	for (int i = 0; i <= 6; i += 3)
	{
		b.X = i;
		for (int j = 0; j <= 6; j += 3)
		{
			b.Y = j;
			if (a.X >= b.X && a.X <= b.X + 2 && a.Y >= b.Y && a.Y <= b.Y + 2)
				return b;
		}
	}
	return b;
}
void end()//结束窗口绘制
{

	setbkcolor(WHITE);
	settextcolor(BLACK);
	setbkmode(1);
	setfillcolor(RGB(158, 160, 169));
	cleardevice();
	settextstyle(40, 0, "楷体");
	outtextxy(550, 60, "完成时间：");
	solidrectangle(550, 110, 850, 160);
	outtextxy(550, 190, "错误次数：");
	solidrectangle(550, 240, 850, 290);
	outtextxy(550, 330, "总得分：");
	solidrectangle(550, 380, 850, 430);
	TCHAR str[20];
	settextcolor(WHITE);
	_stprintf_s(str, "%02d分%02d秒", minute, second);
	outtextxy(620, 115, str);
	_stprintf_s(str, "%d", faults);
	outtextxy(680, 245, str);
	float a = (1 - (minute + (float)second / 60) / ((gameDifficulty + 1) * 10)) * 20;
	float b = (float)(80 - faults) / 80 * 80;
	if ((minute * 60 + second) > (600 * (gameDifficulty + 1)))
		a = 0;
	if (faults >= 30)
		b = 0;
	float scores = a + b;
	_stprintf_s(str, "%.2f", scores);
	outtextxy(650, 380, str);
	setbkcolor(WHITE);
	settextcolor(BLACK);
	setbkmode(1);
	setfillcolor(RGB(158, 160, 169));
	int r_Text=220,g_Text=160,b_Text=140;
	LOGFONT f;
	gettextstyle(&f);
	f.lfWeight = 200;
	f.lfHeight = 80;
	f.lfQuality = ANTIALIASED_QUALITY;
	_tcscpy_s(f.lfFaceName, "华文彩云");
	settextstyle(&f);

	int msg_Flag = 0;
	do
	{
		settextcolor(RGB(r_Text, r_Text, b_Text));
	Colorful(r_Text, r_Text, b_Text);
		outtextxy(30, 130, "G o o d j o b ！ ");
	outtextxy(30, 300, "szx小宝贝太棒了！");
	settextstyle(40, 0, "楷体");
	settextcolor(BLACK);
	/*outtextxy(30, 400, "触发暗号：");
	outtextxy(30, 460, "宋似星辰亚如轩，人鱼王子入凡间！嗷呜");*/
	settextstyle(&f);
	Sleep(1);
	msg_Flag++;
	} while (msg_Flag <=400);
	Sleep(50);

	int result = MessageBox(GetHWnd(), "憨批还玩不还玩不?<(￣︶￣)↗[GO!]", "啦噜啦噜啦噜", MB_YESNO);
	switch (result)
	{
	case IDNO:
		endGame = 1;
		break;
	case IDYES:
		break;
	}
}
int FindMistake()//查找错误并返回错误标志
{
	int i, j, r, c, post = 0;
	//清除上次的错误数据
	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			Map_Mistake[i][j] = 0;
	//对每列检查错误
	for (i = 0; i < 9; i++)
	{
		int arr[10] = { 0 };
		for (j = 0; j < 9; j++)
			arr[Map1[i][j]]++;
		for (j = 0; j < 9; j++)
			if (arr[Map1[i][j]] >= 2 && Map1[i][j] != 0)
			{
				Map_Mistake[i][j] = 1;
				post = 1;
			}
	}
	//对每行检查错误
	for (i = 0; i < 9; i++)
	{
		int arr[10] = { 0 };
		for (j = 0; j < 9; j++)
			arr[Map1[j][i]]++;
		for (j = 0; j < 9; j++)
			if (arr[Map1[j][i]] >= 2 && Map1[j][i] != 0)
			{
				post = 1;
				Map_Mistake[j][i] = 1;
			}


	}
	//对每宫检查错误
	for (i = 0; i <= 6; i += 3)
	{
		for (j = 0; j <= 6; j += 3)
		{
			int arr[10] = { 0 };
			for (r = i; r <= i + 2; r++)
				for (c = j; c <= j + 2; c++)
					arr[(Map1[r][c])]++;
			for (r = i; r <= i + 2; r++)
				for (c = j; c <= j + 2; c++)
					if (arr[Map1[r][c]] >= 2 && Map1[r][c] != 0)
					{
						Map_Mistake[r][c] = 1;
						post = 1;
					}


		}
	}
	return post;
}
int WinJudge()//是否开始判断胜利
{
	int post = 1;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (Map1[i][j] == 0)
			{
				post = 0;
				break;
			}
		}
	}
	return post;
}
