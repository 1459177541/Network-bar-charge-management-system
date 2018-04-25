#include"config.h"

int maxCardId = 1;
pList cardLists = NULL;
int CardLength = 0;
int initCard() {
	FILE *fp;
	if (NULL == (fp = fopen("data\\card", "rb")))
	{
		return 1;
	}
	if (NULL== cardLists)
	{
		cardLists = (pList)malloc(sizeof(List));
		cardLists->next = NULL;
		cardLists->last = NULL;
		cardLists->date.card = NULL;
	}
	pList o = (pList)malloc(sizeof(List));
	pCard p = (pCard)malloc(sizeof(card));
	pList q = cardLists;
	q->date.card = p;
	q->type = d_card;
	p = (pCard)malloc(sizeof(card));;
	while (fread(p, sizeof(card), 1, fp)>0)
	{
		if (p->id>maxCardId)
		{
			maxCardId = p->id;
		}
		o->type = d_card;
		o->date.card = p;
		o->last = q;
		o->next = NULL;
		q->next = o;
		q = q->next;
		p = (pCard)malloc(sizeof(card));
		o = (pList)malloc(sizeof(List));
	}
	free(p);
	free(o);
	fclose(fp);
	return 0;
}

pList getCards() {
	if (NULL== cardLists)
	{
		if (initCard())
		{
			cardLists = (pList)malloc(sizeof(List));
			cardLists->date.card = NULL;
			cardLists->last = NULL;
			cardLists->next = NULL;
			cardLists->type = d_card;
		}
	}
	return cardLists;
}

pCard getCard(int id) {
	pList p = getCards();
	while (NULL!=p)
	{
		if (id==p->date.card->id)
		{
			return p->date.card;
		}
	}
	return NULL;
}

void prCard(pCard p, int isOption) {
	printf("%3s%15d |%19s |%19s |%13.2lf %-3s"
		, isOption ? getAttri("L") : getAttri("NL"), p->id, p->type, p->masterName, p->balance, isOption ? getAttri("R") : getAttri("NR"));
}

int showCard(int type, pCard p,char * text, char *password, char *password2) {
	if (0 > p->id)
	{
		p->id = maxCardId;
		maxCardId++;
	}
	char* balance = (char*)malloc(16 * sizeof(char));
	balance[0] = '\0';
	if (5 != type)
	{
		sprintf(balance, "%.2lf", p->balance);
	}
	int x = 16;
	int y = 2;
	gotoxy(x, y++);
	printf("=================================================");
	gotoxy(x, y++);
	printf("|                                               |");
	gotoxy(x, y++);
	printf("|                      id：%-21d|", p->id);
	gotoxy(x, y++);
	printf("|                                               |");
	gotoxy(x, y++);
	printf("|                   %c类型：%-21s|",0==type?'>':' ', p->type);
	gotoxy(x, y++);
	printf("|                                               |");
	gotoxy(x, y++);
	printf("|                  用户名：%-21s|", 1 == type ? "" : p->masterName);
	gotoxy(x, y++);
	printf("|                                               |");
	gotoxy(x, y++);
	printf("|                    密码：%-21s|", "");
	gotoxy(x + 27, y - 1); 
	printfPassword(password);
	gotoxy(x, y++);
	printf("|                                               |");
	gotoxy(x, y++);
	printf("|                再次输入：%-21s|", "");
	gotoxy(x + 27, y - 1);
	printfPassword(password2);
	gotoxy(x, y++);
	printf("|                                               |");
	gotoxy(x, y++);
	printf("|                    余额：%-21s|", balance);
	gotoxy(x, y++);
	printf("|                                               |");
	gotoxy(x, y++);
	printf("|                    %10s                 |",text);
	gotoxy(x, y++);
	printf("|                      ");
	OPTION_OK(5 == type);
	printf("                  |");
	gotoxy(x, y++);
	printf("|                                               |");
	gotoxy(x, y++);
	printf("=================================================");
	gotoxy(x, y++);
	free(balance);

	key k;
	switch (type)
	{
	case 0:
	{
		pList typeList = getCardTypeList();
		if ('\0'==p->type[0])
		{
			strcpy(p->type, typeList->date.cardType->name);
		}
		else
		{
			while (0!=strcmp(p->type,typeList->date.cardType->name) && NULL!=typeList->next)
			{
				typeList = typeList->next;
			}
			k = isKey(getch());
			switch (k)
			{
			case left:
				if (NULL!=typeList->last)
				{
					strcpy(p->type, typeList->last->date.cardType->name);
				}
				showCard(type, p, text, password, password2);
				return;
			case right:
				if (NULL != typeList->next)
				{
					strcpy(p->type, typeList->next->date.cardType->name);
				}
				showCard(type, p, text, password, password2);
				return;
			default:
				break;
			}
		}
		break;
	}
	case 1:
	{
		k = input(x + 27, 8, p->masterName, 0, NUM | LETTER | CHINESE, NULL);
		break;
	}
	case 2:
		k = input(x + 27, 10, password, 1, INTER | LETTER | SYMBOL, NULL);
		break;
	case 3:
		k = input(x + 27, 12, password2, 1, INTER | LETTER | SYMBOL, NULL);
		break;
	case 4:
	{
		char * temp = (char*)malloc(16 * sizeof(char));
		temp[0] = '\0';
		k = input(x + 27, 14, temp, 0, NUM, NULL);
		if ('\0' != temp)
		{
			p->balance = atof(temp);
		}
		break;
	}
	default:
		k = isKey(getch());
		break;
	}
	switch (k)
	{
	case up:
		if (0 == type)
		{
			showCard(5, p, text, password, password2);
		}
		else {
			showCard(type-1, p, text, password, password2);
		}
		break;
	case down:
	case tab:
		if (5 == type)
		{
			showCard(0, p, text, password, password2);
		}
		else {
			showCard(type+1, p, text, password, password2);
		}
		break;
	case enter:
		if (5 == type)
		{
			if ('\0' == password[0] || '\0' == password2[0])
			{
				password[0] = '\0';
				password2[0] = '\0';
				showCard(type, p, "[     err:请输入密码     ]", password, password2);
				return;
			}
			if (0 == strcmp(password, password2))
			{
				strcpy(p->password, password);
				return;
			}
			else
			{
				password[0] = '\0';
				password2[0] = '\0';
				showCard(type, p, "[err:两次输入的密码不一样]", password, password2);
				return;
			}
		}
		else
		{
			showCard(5, p, text, password, password2);
		}
		break;
	default:
		showCard(type, p, text, password, password2);
		break;
	}
}

//搜索条件
typedef struct
{
	int type;
	char CardType[16];
	char Criteria[16];
}CriteriaCard, *pCriteriaCard;
pCriteriaCard getDefaultCriteriaCard() {
	pCriteriaCard p = (pCriteriaCard)malloc(sizeof(CriteriaCard));
	p->type = 0;
	strcpy(p->CardType, getCardTypeList()->date.card->type);
	p->Criteria[0] = '\0';
	return p;
}

//依据条件获得列表
pList getListFromCardCriteria(pCriteriaCard criteria) {
	pList list = (pList)malloc(sizeof(List));
	list->last = NULL;
	list->next = NULL;
	pList o = list;
	pList p = getCards();
	int isAdd;
	char temp[32];
	while (NULL != p)
	{
		switch (criteria->type)
		{
		case 0:
			isAdd = 1;
			if (0 != strcmp(criteria->CardType, "所有类型"))
			{
				if (0 != strcmp(criteria->CardType, p->date.card->type))
				{
					isAdd = 0;
				}
			}
			break;
		case 1:
			isAdd = 0;
			///////////////////////////////////////////////////////
			break;
		default:
			break;
		}
		if (isAdd)
		{
			pList q = (pList)malloc(sizeof(List));
			q->next = NULL;
			q->last = o;
			q->date = p->date;
			o->next = q;
			o = o->next;
		}
		p = p->next;
	}
	return list->next;
}

//筛选
pList selectCard(int type, pCriteriaCard criteria, pList p) {
	key k;
	pList pt = getCardTypeList();
	system("title 筛选");
	system("cls");
	system("mode con cols=80 lines=24");
	printf("\n\n");
	printf("                       ============= 筛 选 =============                       \n\n");
	printf("                            %c方式:", 0 == type ? '>' : ' ');
	if (0 == criteria->type)
	{
		printf("条件搜索\n\n");
		printf("                         %c会员卡类型：", 1 == type ? '>' : ' ');
		while (strcmp(criteria->CardType, pt->date.cardType->name) != 0 && pt->next != NULL)
		{
			pt = pt->next;
		}
		if (NULL == pt->next)
		{
			pt = getCardTypeList();
		}
		printf("%s\n\n", pt->date.cardType->name);
		printf("\n\n");
		printf("                                ");
		OPTION_OK(2 == type);
		k = isKey(getch());
	}
	else if (1 == criteria->type)
	{
		printf("模糊搜索\n\n");
		printf("                         含有的内容：", 1 == type ? '>' : ' ');
		k = input(5, 39, criteria->Criteria, 0, NUM | LETTER | CHINESE | SYMBOL, NULL);
		printf("\n\n");
		printf("                                ");
		OPTION_OK(2 == type);
	}
	switch (k)
	{
	case esc:
		return p;
	case left:
		if (0 == type)
		{
			criteria->type--;
			if (0>criteria->type)
			{
				criteria->type = 1;
			}
			return selectCard(type, criteria, p);
		}
		if (criteria->type == 0)
		{
			if (NULL != pt->last)
			{
				strcpy(criteria->CardType, pt->last->date.cardType->name);
			}
			return selectCard(type, criteria, p);
		}
	case up:
		type--;
		if (type<0)
		{
			type = 2;
		}
		return selectCard(type, criteria, p);
	case right:
		if (0 == type)
		{
			criteria->type++;
			if (1<criteria->type)
			{
				criteria->type = 0;
			}
			return selectCard(type, criteria, p);
		}
		if (criteria->type == 0)
		{
			if (NULL != pt->next)
			{
				strcpy(criteria->CardType, pt->next->date.cardType->name);
			}
			return selectCard(type, criteria, p);
		}
	case down:
		type++;
		if (criteria->type != 0 && type>2)
		{
			type = 0;
		}
		return selectCard(type, criteria, p);
	case enter:
		if (2 != type)
		{
			type = 2;
			return selectCard(type, criteria, p);
		}
		return getListFromCardCriteria(criteria);
	default:
		return selectCard(type, criteria, p);
	}
}

pList selectToCard() {
	return selectCard(0, getDefaultCriteriaCard(), getCards());
}

