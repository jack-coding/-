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
IMAGE bg_img;      //���ڱ�����Ϸ��ʼ��Ļ����
IMAGE success_img; //���ڱ�����Ϸʤ����Ļ����
IMAGE defeat_img;  //���ڱ�����Ϸʧ����Ļ����
/**************************
*��������`
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
*����:ʵ����Ϸ��ͼ�Ĵ�ӡ
*����:����ͼƬ
*���:����Ϸ��ͼ���ض�λ�ø��ݸ�λ���ڶ�ά�����е�ֵ��ӡ�ض���ͼƬ
*����ֵ:��
******************/
void inimap(void) {
	initgraph(SCREEN_LENGTH, SCREEN_WIDTH);
	//���ñ���
	loadimage(&bg_img, _T("blackground.bmp"), 960, 768, true);
	loadimage(&success_img, _T("ʤ��.jpg"), 534, 300, true);
	loadimage(&defeat_img, _T("ʧ��.jpg"), 534, 300, true);
	putimage(0, 0, &bg_img);
	//����ͼƬ����
	loadimage(&images[WALL], _T("wall_right.bmp"), RATIO, RATIO, true);
	loadimage(&images[FLOOR], _T("floor.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX_DES], _T("des.bmp"), RATIO, RATIO, true);
	loadimage(&images[MAN], _T("man.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX], _T("box.bmp"), RATIO, RATIO, true);
	loadimage(&images[HIT], _T("box.bmp"), RATIO, RATIO, true);
	//��ӡ������Ϸ��ͼ
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
*����:��ȡ��Ұ���,����ÿһ��������д���
*����:'w' 'a' 's' 'd' 'q'
*���:ʵ��С�˵��ƶ���������,�����˳�
*����ֵ:��
******************/
void key_Control(void) {
	bool quit = false;
	do {
		if (_kbhit()) {//��Ұ���
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
			if (isGameOver()) {//�ж��Ƿ��������ӵ���Ŀ�ĵ�,�����,����Ϸ����,������Ϸ����
				quit = true;
				GameOverScene(&success_img);
			}
		}
		Sleep(100);
	} while (quit == false);//����˳�
	system("pause");
	closegraph();
}
/******************
*����:�ı���Ϸ��ͼ�ض�λ���ڶ�ά�������ض��ĵ���ֵ,ͬʱ�ڸ�λ������µĵ���
*����:�ض�λ���ڶ�ά�����е�����
*���:�ڸ�λ������µĵ���
*����ֵ:��
******************/
void changeMap(POS *pos, PROP prop) {
	map[pos->x][pos->y] = prop;
	putimage(START_X + RATIO*pos->y, START_Y + RATIO*pos->x, &images[prop]);
}
/******************
*����:ʵ��С�˵��ƶ���������
*����:С���ƶ��ķ���
*���:С�˵��ƶ���������
*����ֵ:��
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

	if (isValid(next_pos) && (map[next_pos.x][next_pos.y] == FLOOR)) {//�����ǰ���ǵذ�
		changeMap(&next_pos, MAN);//С����ǰ��һ��
		changeMap(&man, FLOOR);//С���ƶ�ǰ��λ�ñ�ɵذ�
		man = next_pos;//С�˵�������֮�ı�
	}
	else if (isValid(next_pos_next_pos) && (map[next_pos.x][next_pos.y] == BOX)) {//�����ǰ��������,���������,һ������ǰ���ǵذ�,��������ǰ����Ŀ�ĵ�
		if (map[next_pos_next_pos.x][next_pos_next_pos.y] == FLOOR) {//����ǰ���ǵذ�
			changeMap(&next_pos, MAN);//С����ǰ��һ��
			changeMap(&next_pos_next_pos, BOX);//������ǰ��һ��
			changeMap(&man, FLOOR);//С���ƶ�ǰ��λ�ñ�ɵذ�
			man = next_pos;//С�˵�������֮�ı�
		}
		else if (map[next_pos_next_pos.x][next_pos_next_pos.y] == BOX_DES) {//����ǰ����Ŀ�ĵ�
			changeMap(&next_pos, MAN);//С����ǰ��һ��
			changeMap(&next_pos_next_pos, HIT);//������ǰ��һ��
			changeMap(&man, FLOOR);//С���ƶ�ǰ��λ�ñ�ɵذ�
			man = next_pos;//С�˵�������֮�ı�
		}
	}
	else if (isValid(next_pos) && (map[next_pos.x][next_pos.y] == HIT)) {//�����ǰ������������Ŀ�ĵ�
		changeMap(&next_pos_next_pos, BOX);//������ǰ��һ��
		changeMap(&next_pos, MAN);//С����ǰ��һ��
		changeMap(&man, FLOOR);//С���ƶ�ǰ��λ�ñ�ɵذ�
		man = next_pos;//С�˵�������֮�ı�
	}
}
/***********************
*����:�ж���Ϸ�Ƿ����
*����:��
*���:��
*����ֵ:true����false
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
*����:ʵ����Ϸ��������
*����:��
*���:����ʤ��ͼƬ
*����ֵ:��
***********************/
void GameOverScene(IMAGE *end) {
	closegraph();
	initgraph(534, 300);
	putimage(0, 0, end);
}