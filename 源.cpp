#include<iostream>
#include<stdlib.h>
#include<string>
#include<graphics.h>
#include<conio.h>
#include"boxman.h"
using namespace std;
POS man;
POS Boxdes_pos; //���ڱ�������Ŀ�ĵص�����
int steps = 0;  //���ڱ���С���ƶ��Ĳ���
int Boxdes_num = 0;    //���ڱ�������Ŀ�ĵص�����
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
	char move_rules[60];
	TCHAR _Tmove_rules[60];
	sprintf_s(move_rules,sizeof(move_rules),"�޶�����: %d��",limit_step);//�����ͱ���д��char����
	MultiByteToWideChar(CP_ACP, 0, move_rules, -1, _Tmove_rules, 100);//char����ת����TCHAR����
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
	//��ӡ��Ϸ˵��
	settextcolor(YELLOW);
	settextstyle(25, 0, _T("��Բ"));
	outtextxy(120, 30, _T("����˵��: w:����  a:����  s:����  d:����  q:�˳���Ϸ"));
	outtextxy(120, 75, _Tmove_rules);
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
				GameOverScene(&bg_img,ch);
			}
			if (isGameOver()) {//�ж��Ƿ��������ӵ���Ŀ�ĵ�,�����,����Ϸ����,������Ϸ����
				quit = true;
				GameOverScene(&bg_img,ch);
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
	if (prop == MAN) {
		steps++;
	}
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

	if (isValid(next_pos) && (map[next_pos.x][next_pos.y] == FLOOR)) {//�����ǰ���ǵذ�,���������,һ�����˵�λ����֮ǰ������Ŀ�ĵ�,�����˵�λ����֮ǰ�ǵذ�
		 if ((Boxdes_pos.x == man.x) && (Boxdes_pos.y == man.y)) {//�˵�λ����֮ǰ������Ŀ�ĵ�
			 changeMap(&next_pos, MAN);//С����ǰ��һ��
			 changeMap(&Boxdes_pos,BOX_DES);//С���ƶ�ǰ��λ�ñ�ɵذ�
		}
		 else {
			 changeMap(&next_pos, MAN);//С����ǰ��һ��
			 changeMap(&man, FLOOR);//С���ƶ�ǰ��λ�ñ�ɵذ�
		 }
		 man = next_pos;//С�˵�������֮�ı�
	}
	else if (isValid(next_pos) && (map[next_pos.x][next_pos.y] == BOX_DES)) {//�����ǰ��������Ŀ�ĵ�
		Boxdes_pos = next_pos;//�������Ŀ�ĵص�λ��
		changeMap(&next_pos, MAN);//С����ǰ��һ��
		changeMap(&man,FLOOR);//С���ƶ�ǰ��λ�ñ������Ŀ�ĵ�
		man = next_pos;//С�˵�������֮�ı�
	}
	else if (isValid(next_pos_next_pos) && (map[next_pos.x][next_pos.y] == BOX)) {//�����ǰ��������,���������,һ������ǰ���ǵذ�,��������ǰ����Ŀ�ĵ�
		if (map[next_pos_next_pos.x][next_pos_next_pos.y] == FLOOR) {//����ǰ���ǵذ�,���������,һ��С�˵�λ��֮ǰ������Ŀ�ĵ�,����С�˵�λ��֮ǰ�ǵذ�
			changeMap(&next_pos_next_pos,BOX);//������ǰ��һ��
			if ((Boxdes_pos.x == man.x) && (Boxdes_pos.y == man.y)) {//���С�˵�λ��֮ǰ������Ŀ�ĵ�
				changeMap(&next_pos, MAN);//С����ǰ��һ��
				changeMap(&Boxdes_pos, BOX_DES);//С���ƶ�ǰ��λ�ñ������Ŀ�ĵ�
			}
			else {//���С�˵�λ��֮ǰ�ǵذ�
				changeMap(&next_pos, MAN);//С����ǰ��һ��
				changeMap(&man, FLOOR);//С���ƶ�ǰ��λ�ñ�ɵذ�
			}
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
		Boxdes_pos =next_pos;//�������Ŀ�ĵ�
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
	int num = 0;
	if (steps > limit_step) {//��������޶�����,��Ϸ����
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
	if (num == Boxdes_num) {//�������ȫ������Ŀ�ĵ�
		return true;
	}
	else{
		return false;
	}
}
/***********************
*����:ʵ����Ϸ��������
*����:ͼƬ��ַ ��������
*���:ʧ��ʤ��ͼƬ������
*����ֵ:��
***********************/
void GameOverScene(IMAGE *end,char ch) {
	putimage(0, 0, end);
	settextcolor(YELLOW);
	settextstyle(25, 0, _T("��Բ"));
	RECT rec = { 0,0, SCREEN_LENGTH , SCREEN_WIDTH };
	if (ch == QUIT) {//�����;�˳���Ϸ
		drawtext(_T("��Ϣһ��,�´������,��������˳�~"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else {
		if (steps > limit_step) {//��������޶�����,��Ϸʧ��
			drawtext(_T("������~,�����������޶�����,С�ɰ�����һ�ΰ�~"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			putimage(210, 50, &defeat_img);
		}
		else {
			drawtext(_T("����666~,��ϲ����Ϊ��һ���ϸ�İ�������˾��!~"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			putimage(210, 50, &success_img);
		}
	}
}