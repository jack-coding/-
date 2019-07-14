#include<iostream>
#include<stdlib.h>
#include<string>
#include<graphics.h>
#include<conio.h>
#include"boxman.h"
using namespace std;
POS man;
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
/**************************
*函数声明`
**************************/
void inimap(void);
void key_Control(void);
void game_Control(DIRECTION direction);
bool isGameOver(void);
void GameOverScene(IMAGE *bg);
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
	for (int i = 0; i<LINE; i++) {
		for (int j = 0; j<CLOUMN; j++) {
			if (map[i][j] == MAN) {
				man.x = i;
				man.y = j;
			}
			putimage(START_X + RATIO*j, START_Y + RATIO*i, &images[map[i][j]]);
		}
	}
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
				GameOverScene(&defeat_img);
			}
			if (isGameOver()) {//判断是否所有箱子到达目的地,如果是,则游戏结束,否则游戏继续
				quit = true;
				GameOverScene(&success_img);
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
	struct _POS before_pos;
	switch (direction) {
	case UP:
		next_pos.x--;
		next_pos_next_pos.x -= 2;
		before_pos.x = next_pos.x + 2;
		break;
	case DOWN:
		next_pos.x++;
		next_pos_next_pos.x += 2;
		before_pos.x = next_pos.x - 2;
		break;
	case LEFT:
		next_pos.y--;
		next_pos_next_pos.y -= 2;
		before_pos.y = next_pos.y + 2;
		break;
	case RIGHT:
		next_pos.y++;
		next_pos_next_pos.y += 2;
		before_pos.y = next_pos.y - 2;
		break;
	}

	if (isValid(next_pos) && (map[next_pos.x][next_pos.y] == FLOOR)) {//如果人前面是地板
		changeMap(&next_pos, MAN);//小人往前走一步
		changeMap(&man, FLOOR);//小人移动前的位置变成地板
		man = next_pos;//小人的坐标随之改变
	}
	else if (isValid(next_pos_next_pos) && (map[next_pos.x][next_pos.y] == BOX)) {//如果人前面是箱子,分两种情况,一是箱子前面是地板,二是箱子前面是目的地
		if (map[next_pos_next_pos.x][next_pos_next_pos.y] == FLOOR) {//箱子前面是地板
			changeMap(&next_pos, MAN);//小人往前走一步
			changeMap(&next_pos_next_pos, BOX);//箱子往前移一步
			changeMap(&man, FLOOR);//小人移动前的位置变成地板
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
	for (int i = 0; i<LINE; i++)
	{
		for (int j = 0; j<CLOUMN; j++)
		{
			if (map[i][j] == BOX_DES)
				return false;
		}
	}
	return true;
}
/***********************
*功能:实现游戏结束场景
*输入:无
*输出:王者胜利图片
*返回值:无
***********************/
void GameOverScene(IMAGE *end) {
	closegraph();
	initgraph(534, 300);
	putimage(0, 0, end);
}