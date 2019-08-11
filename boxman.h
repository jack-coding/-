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
*函数声明`
**************************/
void inimap(void);
void key_Control(void);
void game_Control(DIRECTION direction);
bool isGameOver(void);
void GameOverScene(IMAGE *bg,char ch);
void changeMap(POS *pos, PROP prop);
