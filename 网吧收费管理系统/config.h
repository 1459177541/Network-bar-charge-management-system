#pragma once
/*****************************
 *
 * 声明函数头及全局结构文件，以便调用
 *
 *****************************/
#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<string.h>
#include<time.h>
#include<malloc.h>

//左移运算
#define SHL(x) (1<<x)

/*****************************
*
* 数据结构
*
*****************************/
//时间
typedef struct tm tm, * pTm;

//管理员
typedef struct {
	char name[32];
	char password[32];
	int power;			//权限码，使用二进制保存对应权限
}admin,* pAdmin;

//会员卡类型
typedef struct {
	char name[32];
	int price;		//为真实值的100倍
}cardType,* pCardType;

//会员卡
typedef struct {
	char type[16];
	int id;
	char masterName[16];
	char idcardNum[24];
	char password[16];
	double balance;
}card,* pCard;

//电脑类型
typedef struct {
	char type[16];
	int num;
	int startId;
}PCtype,* pPCtype;

//电脑
typedef struct {
	char type[16];
	int id;
	pCard user;
	tm startTime;
}PC,* pPC;

//收费标准
typedef struct {
	char pc[16];
	char card[16];
	char rule[16];
}rate,* pRate;

//历史记录类型
typedef enum {
	ALL_T
	,C_CARD_T, D_CARD_T, U_CARD_T
	,C_RATE_T, D_RATE_T, U_RATE_T
	,C_ADMIN_T, D_ADMIN_T, U_ADMIN_T
	,C_CARD_TYPE_T, D_CARD_TYPE_T, U_CARD_TYPE_T
	,C_PC_TYPE_T, D_PC_TYPE_T, U_PC_TYPE_T
	,UP_T, DOWN_T, RECHARGE_T
	,FINAL_T
}historyType;

//记录
typedef struct {
	historyType type;
	char text[128];
	char editor[16];
	tm time;
	double money;
}history,* pHistory;

//统计
typedef struct List List, *pList;
typedef struct {
	tm time;
	pList startHistory;				//开始的记录
	int	stat_up;					//上机次数
	double stat_up_money;			//上机金额
	double stat_recharge_money;		//充值金额
	int stat_card_login;			//总注册卡数
	int stat_card_logout;			//注销卡数
}stat, *pStat;

//属性
typedef struct {
	char key[16];
	char value[64];
	char name[32];
}attribute, *pAttribute;

//列表
typedef union {
	pAdmin admin;
	pPC pc;
	pCard card;
	pPCtype pcType;
	pCardType cardType;
	pRate rate;
	pHistory history;
	pStat statisticsByTime;
	pAttribute attri;
}data,* pData;

typedef enum {
	d_admin, d_pc, d_card, d_pcType, d_cardType, d_rate, d_history, d_statistics, d_statistics_more, d_attri
}dataType;

typedef struct List{
	dataType type;
	data data;
	struct List *last;
	struct List *next;
}List,* pList;

//键盘按键
typedef enum {
	no, left, right, up, down, pgup, pgdn, ins, del, home, end, tab, esc, enter, backspace, number, letter, symbol
}key;

//筛选
typedef enum {
	condition, fuzzy
}filterType;

typedef union {
	struct pc {
		pPCtype pcType;
		enum isUse{
			C_PC_ALL, C_PC_IS, C_PC_NOT
		}isUse;
	} pc;
	struct history
	{
		historyType historyType;
		pAdmin editor;
	} history;
	pCardType card;
}conditionDate;

typedef struct {
	filterType type;
	conditionDate condition;
	char Criteria[16];
}Criteria, *pCriteria;

/*****************************
*
* 函数头
*
*****************************/
//tool
void gotoxy(int x, int y);
void myCls();
int stringToInt(char * string);
char * intToString(int num);
void splitString(char *src, char *ret, int start, int length);
void replaceString(char *src, char old, char *new);
void printfPassword(char *src, char *ret);
key isKey(int in);
	//输入限定，使用左移运算得到对应的二进制位
	#define NUM SHL(0)
	#define LETTER SHL(1)
	#define SYMBOL SHL(2)
	#define CHINESE SHL(3)
	#define OTHER SHL(4)
	#define INTER SHL(5)
int isPower(int powerCode, int point);
	//默认外观
	#define OPTION_OK(isOption) prOption("确定", isOption, 7);
void prOption(char * name, int isOption, int length);
void prPrompt(char *title, char *body);
key input(int x, int y, char *in, int isPassword, int power, char * other);
int saveExit();

//config
int initAttri();
pList getAttriList();
char* getAttri(char * key);
void setAttri(char *key, char *value, char *name);
void prAttri(pAttribute p, int isOption);
void editAttri(pAttribute p);

//init
void initialization();
void setting();

//user
	//权限，使用左移运算得到对应的二进制位
	#define C_CARD SHL(0)
	#define D_CARD SHL(1)
	#define U_CARD SHL(2)
	#define C_RATE SHL(3)
	#define D_RATE SHL(4)
	#define U_RATE SHL(5)
	#define UP_DOWN SHL(6)
	#define HISTORY SHL(7)
	#define C_ADMIN SHL(8)
	#define D_ADMIN SHL(9)
	#define U_ADMIN SHL(10)
	#define C_CARD_TYPE SHL(11)
	#define D_CARD_TYPE SHL(12)
	#define U_CARD_TYPE SHL(13)
	#define C_PC_TYPE SHL(14)
	#define D_PC_TYPE SHL(15)
	#define U_PC_TYPE SHL(16)
	#define STATISTICS SHL(17)
	#define RECHARGE SHL(18)
int initAdminList();
pList getAdminHead();
void setRoot(pAdmin root);
void setUser(pAdmin u);
pAdmin getUser();
void prUser(pAdmin p);
void editUser(int type, pAdmin p, char *pass1, char *pass2);
void helpFromUser();
pList newAdmin(pList list);

//menu
void login(pAdmin user,int type,char* text);

//pcType
int isEditPCType();
void setEdit(int edit);
pList getPCtypeList();
void prPCtype(pPCtype p);
void editPCtype(int type, pPCtype p);
int editPCTypePS(int type);
pList newPCType(pList list);

//PC
pList getPCs();
void logoutPCAll();
void prPC(pPC p, int isOption);
pList selectToPC();
void showPC(pPC p);
void logPC(pPC p);
int hasLoginPC();

//cardType
pList getCardTypeList();
void prCardType(pCardType p);
void editCardType(int type, pCardType p);
pList newCardType(pList list);

//card
pCard getCard(int id);
pList getCards();
void prCard(pCard p, int isOption);
void showCard(int type, pCard p, char * text, char *password, char *password2);
pList newCard(pList list);
pList selectToCard();
void recharge(pCard p);
void sortCard();
void delCard(pList op);

//rate
pList getRateList();
double results(pPC pc, pCard user, pTm time);
void prRate(pRate p);
void editRate(int type, int option[], pRate p);
pList newRate(pList list);

//history
pList getHistorys();
void prHistory(pHistory p, int isOption);
void showHistory(pHistory p);
pList selectToHistory();
void addHistory(historyType type, data data, double other);

//list
void save(dataType type);
void saveAll();
void close(dataType type);
void closeAll();
pList sort(pList list, int(*isUP)(pList a, pList b), int isNot);
pList paginationMenu(pList list);	//上/下机、会员卡、历史记录
pList scrollMenu(pList list);		//电脑类型、会员卡类型、管理员、收费标准

//stztistics
void prStatByTime(pStat p, int isOption);
void prStatMoreByTime(pStat p, int isOption);
pList getStatByTime();
pList getMoreStatByTime(pList start);
void showStatByTime(pStat p,int isMore);
