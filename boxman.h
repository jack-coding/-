#define LINE    9  //�����ά������к���
#define CLOUMN  12

#define START_X 100  //����ͼƬ�ڻ����еĳ�ʼ����
#define START_Y 150

#define SCREEN_LENGTH  960   //���廭����С
#define SCREEN_WIDTH   768

#define RATIO 61   //����ͼƬ�ߴ�

//���Ƽ� ��,��,��,�� �˳�
#define KEY_UP      'w'    //��
#define	KEY_DOWN    's'    //��
#define	KEY_LEFT    'a'    //��
#define	KEY_RIGHT   'd'    //��
#define	QUIT        'q'    //�˳�

//�궨��С���ƶ�������
#define isValid(pos) (pos.x>=0)&&(pos.x<=LINE)&&(pos.y>=0)&&(pos.y<=CLOUMN)
#define limit_step 70

/************************
*�������Ͷ�������������
*Ŀ��:�򻯴���  ��ߴ���Ŀ���ֲ��
************************/
typedef struct _POS       POS;
typedef enum _PROP        PROP;
typedef enum _DIRECTION   DIRECTION;

/*************
*����ö��������ߴ���ɶ���
*************/
enum _PROP {
	WALL,     //0��ʾǽ
	FLOOR,    //1��ʾ�ذ�
	BOX_DES,  //2��ʾ����λ��
	MAN,      //3��ʾС��
	BOX,      //4��ʾ����
	HIT,      //5��ʾ���ӵ���Ŀ�ĵ�
	ALL       //6   �ܹ���6������
};
enum _DIRECTION {   //��ʾ �� �� �� ��
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct _POS {   //���ڱ�ʾС���ڶ�ά�����е�λ��
	int x;
	int y;
}; 

/**************************
*��������`
**************************/
void inimap(void);
void key_Control(void);
void game_Control(DIRECTION direction);
bool isGameOver(void);
void GameOverScene(IMAGE *bg,char ch);
void changeMap(POS *pos, PROP prop);
