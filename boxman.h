#define LINE    9  //定义二维数组的行和列
#define CLOUMN  12

#define START_X 100  //定义图片在画布中的初始坐标
#define START_Y 150

#define SCREEN_LENGTH  960   //定义画布大小
#define SCREEN_WIDTH   768

#define RATIO 61   //定义图片尺寸

//控制键 上,下,左,右 退出
#define KEY_UP      'w'    //上
#define	KEY_DOWN    's'    //下
#define	KEY_LEFT    'a'    //左
#define	KEY_RIGHT   'd'    //右
#define	QUIT        'q'    //退出

//宏定义小人移动的条件
#define isValid(pos) (pos.x>=0)&&(pos.x<=LINE)&&(pos.y>=0)&&(pos.y<=CLOUMN)
#define limit_step 70

/************************
*运用类型定义给类型起别名
*目的:简化代码  提高代码的可移植性
************************/
typedef struct _POS       POS;
typedef enum _PROP        PROP;
typedef enum _DIRECTION   DIRECTION;

/*************
*运用枚举类型提高代码可读性
*************/
enum _PROP {
	WALL,     //0表示墙
	FLOOR,    //1表示地板
	BOX_DES,  //2表示箱子位置
	MAN,      //3表示小人
	BOX,      //4表示箱子
	HIT,      //5表示箱子到达目的地
	ALL       //6   总共有6种情形
};
enum _DIRECTION {   //表示 上 下 左 右
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct _POS {   //用于表示小人在二维数组中的位置
	int x;
	int y;
}; 
/**************************
*全局变量声明及定义
**************************/
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

/**************************
*函数声明`
**************************/
void inimap(void);
void key_Control(void);
void game_Control(DIRECTION direction);
bool isGameOver(void);
void GameOverScene(IMAGE *bg,char ch);
void changeMap(POS *pos, PROP prop);
void step_Show(int step);