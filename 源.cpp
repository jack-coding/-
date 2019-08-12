#include<iostream>
#include<stdlib.h>
#include<string>
#include<graphics.h>
#include<conio.h>
#include"boxman.h"
using namespace std;
POS man;
POS Boxdes_pos; //用于保存箱子目的地的坐标
int steps = 0;  //用于保存小人移动的步数
int Boxdes_num = 0;    //用于保存箱子目的地的数量
int map[LINE][CLOUMN] = { { 0,0,0,0,0,0,0,0,0,0,0,0 },
                          { 0,1,0,1,1,1,1,1,1,1,0,0 },
                          { 0,1,4,1,0,2,1,0,2,1,0,0 },
                          { 0,1,0,1,0,1,0,0,1,1,1,0 },
                          { 0,1,0,2,0,1,1,4,1,1,1,0 },
                          { 0,1,1,1,0,3,1,1,1,4,1,0 },
                          { 0,1,2,1,1,4,1,1,1,1,1,0 },
                          { 0,1,0,0,1,0,1,1,0,0,1,0 },
                          { 0,0,0,0,0,0,0,0,0,0,0,0 } };
IMAGE images[ALL];
IMAGE bg_img;      //用于保存游戏开始屏幕背景
IMAGE success_img; //用于保存游戏胜利屏幕背景
IMAGE defeat_img;  //用于保存游戏失败屏幕背景

int main(void) {
	inimap();
	key_Control();
	return 0;
}
/******************
*功能:实现游戏地图的打印
*输入:加载图片
*输出:在游戏地图的特定位置根据该位置在二维数组中的值打印特定的图片
*返回值:无
******************/
void inimap(void) {
	char move_rules[60];
	TCHAR _Tmove_rules[60];
	sprintf_s(move_rules,sizeof(move_rules),"限定步数: %d步",limit_step);//将整型变量写入char数组
	MultiByteToWideChar(CP_ACP, 0, move_rules, -1, _Tmove_rules, 100);//char类型转换成TCHAR类型
	initgraph(SCREEN_LENGTH, SCREEN_WIDTH);
	//设置背景
	loadimage(&bg_img, _T("blackground.bmp"), 960, 768, true);
	loadimage(&success_img, _T("胜利.jpg"), 534, 300, true);
	loadimage(&defeat_img, _T("失败.jpg"), 534, 300, true);
	putimage(0, 0, &bg_img);
	//加载图片道具
	loadimage(&images[WALL], _T("wall_right.bmp"), RATIO, RATIO, true);
	loadimage(&images[FLOOR], _T("floor.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX_DES], _T("des.bmp"), RATIO, RATIO, true);
	loadimage(&images[MAN], _T("man.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX], _T("box.bmp"), RATIO, RATIO, true);
	loadimage(&images[HIT], _T("box.bmp"), RATIO, RATIO, true);
	//打印整个游戏地图
	setbkmode(TRANSPARENT);
	for (int i = 0; i<LINE; i++) {
		for (int j = 0; j<CLOUMN; j++) {
			if (map[i][j] == MAN) {
				man.x = i;
				man.y = j;
			}
			else if (map[i][j] == BOX_DES) {
				Boxdes_num++;
			}
			putimage(START_X + RATIO*j, START_Y + RATIO*i, &images[map[i][j]]);
		}
	}
	//打印游戏说明
	settextcolor(YELLOW);
	settextstyle(25, 0, _T("幼圆"));
	outtextxy(120, 30, _T("操作说明: w:上移  a:左移  s:下移  d:右移  q:退出游戏"));
	outtextxy(120, 75, _Tmove_rules);
}
/******************
*功能:获取玩家按键,并对每一次输入进行处理
*输入:'w' 'a' 's' 'd' 'q'
*输出:实现小人的移动及推箱子,或者退出
*返回值:无
******************/
void key_Control(void) {
	bool quit = false;
	do {
		if (_kbhit()) {//玩家按键
			char ch = _getch();
			if (ch == KEY_UP) {
				game_Control(UP);
			}
			else if (ch == KEY_DOWN) {
				game_Control(DOWN);
			}
			else if (ch == KEY_LEFT) {
				game_Control(LEFT);
			}
			else if (ch == KEY_RIGHT) {
				game_Control(RIGHT);
			}
			else if (ch == QUIT) {
				quit = true;
				GameOverScene(&bg_img,ch);
			}
			if (isGameOver()) {//判断是否所有箱子到达目的地,如果是,则游戏结束,否则游戏继续
				quit = true;
				GameOverScene(&bg_img,ch);
			}
		}
		Sleep(100);
	} while (quit == false);//如果退出
	system("pause");
	closegraph();
}
/******************
*功能:改变游戏地图特定位置在二维数组中特定的道具值,同时在该位置输出新的道具
*输入:特定位置在二维数组中的坐标
*输出:在该位置输出新的道具
*返回值:无
******************/
void changeMap(POS *pos, PROP prop) {
	map[pos->x][pos->y] = prop;
	putimage(START_X + RATIO*pos->y, START_Y + RATIO*pos->x, &images[prop]);
	if (prop == MAN) {
		steps++;
	}
}
/******************
*功能:实现小人的移动及推箱子
*输入:小人移动的方向
*输出:小人的移动及推箱子
*返回值:无
******************/
void game_Control(DIRECTION direction) {
	struct _POS next_pos = man;
	struct _POS next_pos_next_pos = man;
	
	switch (direction) {
	case UP:
		next_pos.x--;
		next_pos_next_pos.x -= 2;
		break;
	case DOWN:
		next_pos.x++;
		next_pos_next_pos.x += 2;
		break;
	case LEFT:
		next_pos.y--;
		next_pos_next_pos.y -= 2;
		break;
	case RIGHT:
		next_pos.y++;
		next_pos_next_pos.y += 2;
		break;
	}

	if (isValid(next_pos) && (map[next_pos.x][next_pos.y] == FLOOR)) {//如果人前面是地板,有两种情况,一种是人的位置在之前是箱子目的地,二是人的位置在之前是地板
		 if ((Boxdes_pos.x == man.x) && (Boxdes_pos.y == man.y)) {//人的位置在之前是箱子目的地
			 changeMap(&next_pos, MAN);//小人往前走一步
			 changeMap(&Boxdes_pos,BOX_DES);//小人移动前的位置变成地板
		}
		 else {
			 changeMap(&next_pos, MAN);//小人往前走一步
			 changeMap(&man, FLOOR);//小人移动前的位置变成地板
		 }
		 man = next_pos;//小人的坐标随之改变
	}
	else if (isValid(next_pos) && (map[next_pos.x][next_pos.y] == BOX_DES)) {//如果人前面是箱子目的地
		Boxdes_pos = next_pos;//标记箱子目的地的位置
		changeMap(&next_pos, MAN);//小人往前走一步
		changeMap(&man,FLOOR);//小人移动前的位置变成箱子目的地
		man = next_pos;//小人的坐标随之改变
	}
	else if (isValid(next_pos_next_pos) && (map[next_pos.x][next_pos.y] == BOX)) {//如果人前面是箱子,分两种情况,一是箱子前面是地板,二是箱子前面是目的地
		if (map[next_pos_next_pos.x][next_pos_next_pos.y] == FLOOR) {//箱子前面是地板,分两种情况,一是小人的位置之前是箱子目的地,二是小人的位置之前是地板
			changeMap(&next_pos_next_pos,BOX);//箱子往前移一步
			if ((Boxdes_pos.x == man.x) && (Boxdes_pos.y == man.y)) {//如果小人的位置之前是箱子目的地
				changeMap(&next_pos, MAN);//小人往前走一步
				changeMap(&Boxdes_pos, BOX_DES);//小人移动前的位置变成箱子目的地
			}
			else {//如果小人的位置之前是地板
				changeMap(&next_pos, MAN);//小人往前走一步
				changeMap(&man, FLOOR);//小人移动前的位置变成地板
			}
			man = next_pos;//小人的坐标随之改变
		}
		else if (map[next_pos_next_pos.x][next_pos_next_pos.y] == BOX_DES) {//箱子前面是目的地
			changeMap(&next_pos, MAN);//小人往前走一步
			changeMap(&next_pos_next_pos, HIT);//箱子往前移一步
			changeMap(&man, FLOOR);//小人移动前的位置变成地板
			man = next_pos;//小人的坐标随之改变
		}
	}
	else if (isValid(next_pos) && (map[next_pos.x][next_pos.y] == HIT)) {//如果人前面是箱子命中目的地
		Boxdes_pos =next_pos;//标记箱子目的地
		changeMap(&next_pos_next_pos, BOX);//箱子往前移一步
		changeMap(&next_pos, MAN);//小人往前走一步
		changeMap(&man, FLOOR);//小人移动前的位置变成地板
		man = next_pos;//小人的坐标随之改变
	}
}
/***********************
*功能:判断游戏是否结束
*输入:无
*输出:无
*返回值:true或者false
***********************/
bool isGameOver(void) {
	int num = 0;
	if (steps > limit_step) {//如果超过限定步数,游戏结束
		return true;
	}
	for (int i = 0; i<LINE; i++)
	{
		for (int j = 0; j<CLOUMN; j++)
		{
			if (map[i][j] == HIT) {
				num++;
			}
		}
	}
	if (num == Boxdes_num) {//如果箱子全部命中目的地
		return true;
	}
	else{
		return false;
	}
}
/***********************
*功能:实现游戏结束场景
*输入:图片地址 按键输入
*输出:失败胜利图片或文字
*返回值:无
***********************/
void GameOverScene(IMAGE *end,char ch) {
	putimage(0, 0, end);
	settextcolor(YELLOW);
	settextstyle(25, 0, _T("幼圆"));
	RECT rec = { 0,0, SCREEN_LENGTH , SCREEN_WIDTH };
	if (ch == QUIT) {//如果中途退出游戏
		drawtext(_T("休息一下,下次再玩吧,按任意键退出~"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else {
		if (steps > limit_step) {//如果超过限定步数,游戏失败
			drawtext(_T("呜呜呜~,步数大于了限定步数,小可爱再来一次吧~"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			putimage(210, 50, &defeat_img);
		}
		else {
			drawtext(_T("大佬666~,恭喜您成为了一个合格的搬箱子老司机!~"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			putimage(210, 50, &success_img);
		}
	}
}