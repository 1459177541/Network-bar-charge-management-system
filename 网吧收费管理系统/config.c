#include"config.h"

//链表结构保存配置信息
pList attributeLists = NULL;

//从文件中读取配置信息加载到内存,程序每次运行只执行一次
//return：从文件读取成功返回0；读取失败创建空链表并返回1
int initAttri() {
	prPrompt("正在加载", "正在加载配置列表");
	FILE *fp = NULL;
	if (NULL == (fp = fopen("data\\config", "rb")))
	{
		return 1;
	}
	pAttribute p = (pAttribute)malloc(sizeof(attribute));
	if (NULL == attributeLists)
	{
		if (fread(p, sizeof(attribute), 1, fp)>0)
		{
			attributeLists = (pList)malloc(sizeof(List));
			attributeLists->next = NULL;
			attributeLists->last = NULL;
			attributeLists->type = d_attri;
			attributeLists->data.attri = p;
		}
		else
		{
			free(attributeLists);
			free(p);
			fclose(fp);
			return 1;
		}
	}
	pList q = attributeLists;
	pList o = (pList)malloc(sizeof(List));
	p = (pAttribute)malloc(sizeof(attribute));;
	while (fread(p, sizeof(attribute), 1, fp)>0)
	{
		o->type = d_attri;
		o->data.attri = p;
		o->last = q;
		o->next = NULL;
		q->next = o;
		q = q->next;
		p = (pAttribute)malloc(sizeof(attribute));
		o = (pList)malloc(sizeof(List));
	}
	free(p);
	free(o);
	fclose(fp);
	return 0;
}

pList getAttriList() {
	if (NULL==attributeLists)
	{
		if (initAttri())
		{
			setting();
		}
	}
	return attributeLists;
}

//key：需要获取值的键
//return：依据键获取的值，无该键返回NULL
char* getAttri(char * key) {
	pList p = attributeLists;
	while (NULL!=p)
	{
		if (0 == strcmp(p->data.attri->key,key))
		{
			return p->data.attri->value;
		}
		p = p->next;
	}
	return NULL;
}

//修改key对应的value，若无该键则新增
void setAttri(char *key, char *value, char *name) {
	pList p = attributeLists;
	while (NULL != p && NULL != p->next)
	{
		if (0==strcmp(p->data.attri->key, key))
		{
			strcpy(p->data.attri->value, value);
			return;
		}
		p = p->next;		
	}
	if (NULL!=p && 0 == strcmp(p->data.attri->key, key))
	{
		strcpy(p->data.attri->value, value);
		return;
	}
	pAttribute d = (pAttribute)malloc(sizeof(attribute));
	strcpy(d->key, key);
	strcpy(d->value, value);
	strcpy(d->name, name);
	if (NULL!=attributeLists)
	{
		pList q = (pList)malloc(sizeof(List));
		q->last = p;
		q->next = NULL;
		q->type = d_attri;
		q->data.attri = d;
		p->next = q;
	}
	else
	{
		attributeLists = (pList)malloc(sizeof(List));
		attributeLists->last = p;
		attributeLists->next = NULL;
		attributeLists->type = d_attri;
		attributeLists->data.attri = d;
	}
}

//修改值
void editAttri(pAttribute p) {
	prPrompt("请输入设置的值", "");
	char *in = (char *)malloc(sizeof(char) * 32);
	key k = input(26, 12, in, 0, INTER | LETTER | SYMBOL, NULL);
	if (enter==k)
	{
		setAttri(p->key, in, p->name);
		return;
	}
	else
	{
		editAttri(p);
	}
}

//输出
void prAttri(pAttribute p, int isOption) {
	printf("\n%3s %29s |  %-32s% -3s"
		, isOption ? getAttri("L") : getAttri("NL"), p->name, p->value, isOption ? getAttri("R") : getAttri("NR"));
}
