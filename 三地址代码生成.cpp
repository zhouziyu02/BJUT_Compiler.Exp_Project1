#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define IDN 0
#define IF 1
#define THEN 2
#define ELSE 3
#define WHILE 4
#define DO 5
#define DEC 6
#define OCT 7
#define HEX 8
#define ADD 9
#define SUB 10
#define MUL 11
#define DIV 12
#define EQ 13
#define GT 14
#define LT 15
#define LP 16
#define RP 17
#define SEMI 18
#define ASG 19
#define EOS 20
#define GE 21
#define LE 22
#define GL 23
#define BEGIN 24
#define END 25
#define ILOCT 26
#define ILHEX 27
#define ERROR 28

struct F
{
	char code[500];
	char place[500];
};
struct T {
	char code[500];
	char place[500];
};
struct E {
	char code[500];
	char place[500];
};
struct C {
	char code[500];
	int  False;
	int  True;
};
struct S {
	char code[500];
	int  begin;
	int  next;
};
struct L {
	char code[500];
};
struct P {
	char code[500];
};
struct ID {
	char id_name[500];
	int  id_entry;
};
static struct {
	const char* name;
	int type;
}reservedWord[7] = { {"if",IF},{"then",THEN},{"else",ELSE},{"while",WHILE},{"do",DO},{"begin",BEGIN},{"end",END} };

struct ID ID_list[100];
int c;//当前扫描到的单词
char token[1024];
char str[1024];//输入字符串
char strr[1024];//输出字符串 打印的
char val[1024];//具体单词的值得
int scan_pos = 0;//scan当前扫描下标
int newTempCount = 0;//临时t标号
int newlab = 0;//新label标号
int id_num = 0;
int s_next = 0;

int P_Attr(struct P* P);
int L_Attr(struct L* L);
int S_Attr(struct S* S);
int C_Attr(struct C* C);
int E_Attr(struct E* E);
int T_Attr(struct T* T);
int F_Attr(struct F* F);

int judgeReservedWords(char ss[]);
int searchIDN(char ss[]);
double toDec_Oct_Hex(char s[], int n);
int scan(char ss[]);

char* newtemp();
int  newLabel();
void checkNext(int s);

//产生新的中间变量t
char* newtemp() {
	char p[100] = "";
	char s[100] = "t";
	_itoa(++newTempCount, p, 10);//用十进制把计数的值转化到p里
	return strcat(s, p);//字符串连接
}

//生成新标号L
int  newLabel() {
	return ++newlab;
}

void checkNext(int s)
{
	memset(token, 0, 32);//用于对数组或结构体进行清零初始化。
	if (c == s)
	{
		c = scan(str);
	}
	else
	{
		printf("\nerror:no next!%d", c);
		exit(0);
	}
	return;
}
//从P推出的
int P_Attr(struct P* P)
{
	struct L L;
	struct L P1;
	L_Attr(&L);
	if (c == EOS)//如果读到最后发现就是#了，真正的最后了
	{
		checkNext(EOS);
		sprintf(P->code, "%s", L.code);
		strcpy(L.code, P->code);
	}
	else//分号后面还有代码段
	{
		while (c != EOS)
		{
			L_Attr(&P1);
			sprintf(P->code, "%s%s", L.code, P1.code);
			strcpy(L.code, P->code);
		}
	}
	return 1;
}

//从L推出的
int L_Attr(struct L* L)
{
	struct S S;
	S.next = s_next++;
	S_Attr(&S);
	S.next = S.next--;
	if (c == SEMI)//下一位是不是分号
	{
		checkNext(SEMI);
		sprintf(L->code, "\nL%d: \t%s", S.next, S.code);
		return 1;
	}
}

int S_Attr(struct S* S)
{
	int temp_pos;
	struct E E;
	struct C C;
	struct C C1;
	struct S S1;
	struct S S2;
	struct S S3;
	char id_place[500];
	switch (c) //扫描下一个单词是什么
	{
	case IDN:						//S-> id=E
		checkNext(IDN);//往后看一位放到c里
		strcpy(id_place, val);//暂存id的val
		checkNext(EQ);//再往后扫描一个词
		E_Attr(&E);//进入E
		sprintf(S->code, "%s\n\t%s=%s", E.code, id_place, E.place);//E的place，val，code都存到S的code里
		return 1;
	case IF:                          //看到一个if但是不能确定else
		temp_pos = scan_pos;//暂存全局下标
		checkNext(IF);
		C.True = newLabel();//新建一个label
		S1.begin = C.True;// C真则进入S
		S1.next = C.False = S->next; // c假则进入S->next
		C_Attr(&C);//进入c了
		checkNext(THEN);
		S_Attr(&S1);
		if (c != ELSE) //不是else 确定为S->IF C THEN S1
		{
			sprintf(S->code, "\n\t%s\nL%d:\t%s", C.code, C.True, S1.code);
			return 1;
		}
		//那么确定为S-> IF C THEN S2 ELSE S3
		scan_pos = temp_pos;//还原scan下标
		c = IF;
		checkNext(IF);
		C1.True = newLabel();//C的两个输出都新建存储
		C1.False = newLabel();
		S2.next = S3.next = S->next;// 如果C真进入S2，否则C假，进入S3
		C_Attr(&C1);
		checkNext(THEN);
		S_Attr(&S2);
		checkNext(ELSE);
		S_Attr(&S3);
		sprintf(S->code, "\n\t%s\nL%d:\t%s\nL%d:\t%s", C.code, C.True, S2.code, C.False, S3.code);
		return 1;
	case WHILE:                           //S-> WHILE C DO S1
		checkNext(WHILE);
		S->begin = newLabel();
		C.True = newLabel();
		C.False = S->next;
		S1.next = S->begin;
		C_Attr(&C);
		checkNext(DO);
		S_Attr(&S1);
		sprintf(S->code, "\nL%d:\t%s\nL%d:\t%s\n\tgoto L%d", S->begin, C.code, C.True, S1.code, S->begin);
		return 1;
	default:
		printf("\nS failed!%d", c);
		exit(0);
	}
	return 1;
}

int C_Attr(struct C* C) {
	struct E E1;
	struct E E2;
	E_Attr(&E1);
	switch (c) {
	case GT:          //C->E1>E2
		checkNext(GT);
		E_Attr(&E2);
		sprintf(C->code, "%s%s\tif %s>%s goto L%d\n\tgoto L%d", E1.code, E2.code, E1.place, E2.place, C->True, C->False);
		return 1;
	case LT:           //C->E1<E2
		checkNext(LT);
		E_Attr(&E2);
		sprintf(C->code, "%s%sif %s<%s goto L%d\n\tgoto L%d", E1.code, E2.code, E1.place, E2.place, C->True, C->False);
		return 1;
	case EQ:           //C->E1=E2
		checkNext(EQ);
		E_Attr(&E2);
		sprintf(C->code, "%s%sif %s=%s goto L%d\n\tgoto L%d", E1.code, E2.code, E1.place, E2.place, C->True, C->False);
		return 1;
	case GE:            //C->E1>=E2
		checkNext(GE);
		E_Attr(&E2);
		sprintf(C->code, "%s%sif %s>=%s goto L%d\n\tgoto L%d", E1.code, E2.code, E1.place, E2.place, C->True, C->False);
		return 1;
	case LE:             //C->E1<=E2
		checkNext(LE);
		E_Attr(&E2);
		sprintf(C->code, "%s%sif %s<=%s goto L%d\n\tgoto L%d", E1.code, E2.code, E1.place, E2.place, C->True, C->False);
		return 1;
	case GL:            //C->E1<>E2
		checkNext(GL);
		E_Attr(&E2);
		sprintf(C->code, "%s%sif %s<>%s goto L%d\n\tgoto L%d", E1.code, E2.code, E1.place, E2.place, C->True, C->False);
		return 1;
	default:
		printf("\nC failed!%d", c);
		exit(0);
	}
	return 1;
}

int E_Attr(struct E* E) {
	struct T T1;
	struct T T2;
	T_Attr(&T1);
	if ((c != ADD) && (c != SUB))                          //E->T
	{
		strcpy(E->place, T1.place);
		sprintf(E->code, "%s", T1.code);
		return 1;
	}
	while ((c == ADD) || (c == SUB))                     //E->加号或者减号了
	{
		if (c == ADD)           //E->E+T
		{
			checkNext(ADD);
			T_Attr(&T2);
			strcpy(E->place, newtemp());
			sprintf(E->code, "%s%s\n\t%s=%s+%s", T1.code, T2.code, E->place, T1.place, T2.place);
			strcpy(T1.code, E->code);
			strcpy(T1.place, E->place);
		}
		else            //E->E-T
		{
			checkNext(SUB);
			T_Attr(&T2);
			strcpy(E->place, newtemp());
			sprintf(E->code, "%s%s\n\t%s=%s-%s", T1.code, T2.code, E->place, T1.place, T2.place);
			strcpy(T1.code, E->code);
			strcpy(T1.place, E->place);
		}
	}
	return 1;
}

int T_Attr(struct T* T)
{
	struct F F1;
	struct F F2;
	F_Attr(&F1);
	if ((c != MUL) && (c != DIV))    //T->F
	{
		strcpy(T->place, F1.place);
		sprintf(T->code, "%s", F1.code);
		return 1;
	}
	while ((c == MUL) || (c == DIV))     //下一个是乘号或者除号
	{
		if (c == MUL)      //T->T*F
		{
			checkNext(MUL);
			F_Attr(&F2);
			strcpy(T->place, newtemp());
			sprintf(T->code, "%s%s\n\t%s=%s*%s", F1.code, F2.code, T->place, F1.place, F2.place);
			strcpy(F1.code, T->code);
			strcpy(F1.place, T->place);
		}
		else            //T->T/F
		{
			checkNext(DIV);
			F_Attr(&F2);
			strcpy(T->place, newtemp());
			sprintf(T->code, "%s%s\n\t%s=%s/%s", F1.code, F2.code, T->place, F1.place, F2.place);
			strcpy(F1.code, T->code);
			strcpy(F1.place, T->place);
		}
	}
	return 1;
}

int F_Attr(struct F* F)
{
	struct E E;
	if ((c == OCT) || (c == DEC) || (c == HEX) || (c == IDN))
	{
		strcpy(F->place, val);
		sprintf(F->code, "\0");
		switch (c)                  //F->id或者各种进制数了
		{
		case OCT:
			checkNext(OCT); return 1;
		case DEC:
			checkNext(DEC); return 1;
		case HEX:
			checkNext(HEX); return 1;
		case IDN:
			checkNext(IDN);	return 1;
		default:
			return 1;
		}
	}
	else if (c == LP) 					//E->(E)
	{
		checkNext(LP);
		E_Attr(&E);
		strcpy(F->place, E.place);
		sprintf(F->code, "%s", E.code);
		checkNext(RP);
		return 1;
	}
	else
	{
		printf("\nF failed!%d", c);
		exit(0);
	}
	return 1;
}

//检查是否是保留词
int judgeReservedWords(char ss[])
{
	int i;
	for (i = 0; i < 7; i++)
	{
		if (!strcmp(ss, reservedWord[i].name))
			return i;//返回为第几个关键词
	}
	return -1;
}

//变量查找
int searchIDN(char ss[])
{
	int i = 0;
	for (i = 0; i <= id_num; i++)
	{
		if (!strcmp(ID_list[i].id_name, ss))//ID_list[i].id_name与ss不同  在变量list里面比对 找到曾经定义过
			return i;//返回变量编号
	}
	strcpy(ID_list[id_num++].id_name, ss);//ID_list[i].id_name与ss不同  在变量list里面比对 未曾定义过 加入
	return id_num - 1;//返回变量编号
}

//进制转换到十/八/十六进制
double toDec_Oct_Hex(char s[], int n)
{
	int i, j = 0;
	double val1{}, val{};
	switch (n)
	{
	case 1:
		val = atof(s);
		return val;
	case 10:
		for (i = 0; s[i] != '\0'; i++)
		{
			val = val * 10 + (s[i] - '0');
		}
		return val;
	case 8:
		for (i = 0; s[i] != '\0'; i++)
		{
			val = val * 8 + (s[i] - '0');
		}
		return val;
	case 16:
		for (i = 2; s[i] != '\0'; i++)
		{
			if (isdigit(s[i]))
			{
				val = val * 16 + (s[i] - '0');
			}
			else	val = val * 16 + (s[i] - 'a' + 10);
		}
		return val;
	default:	break;
	}
	return 0;
}
//词法分析，识别到号之后返回
int scan(char ss[])
{
	int i = 0;
	int j = 0;
	while (ss[scan_pos] == ' ')
	{
		scan_pos++;//scan_pos位置
	}
	for (; ss[scan_pos] != '\0'; scan_pos++)
	{
		if (isdigit(ss[scan_pos])) //第一个是数字
		{
			if ((ss[scan_pos] - '0') == 0) //第一个是0
			{
				if ((ss[scan_pos + 1]) == 'x') //第二个是x
				{
					token[i++] = ss[scan_pos++];//给到token
					token[i++] = ss[scan_pos++];//给到token
					if (!(isdigit(ss[scan_pos])) && (ss[scan_pos] < 'a' || ss[scan_pos]>'f')) //既不是数字也不是a-f字符 那就不是十六进制
					{
						scan_pos--;
						token[0] = '0';
						strcpy(val, "0");//只能是十进制 数字0
						return DEC;
					}
					else
					{
						while (isdigit(ss[scan_pos]) || (ss[scan_pos] >= 'a' && ss[scan_pos] <= 'f'))
						{
							token[i++] = ss[scan_pos++];//将全部十六进制数放入token
						}
						itoa(toDec_Oct_Hex(token, 16), val, 10);//十六进制转化为十进制 整形转化为字符串
						return HEX;
					}
				}
				else if ((ss[scan_pos + 1]) == '.')
				{
					int n = 10;
					char* str1;
					token[i++] = ss[scan_pos++];//给到token
					token[i++] = ss[scan_pos++];//给到token
					while (isdigit(ss[scan_pos]))	token[i++] = ss[scan_pos++];
					itoa(toDec_Oct_Hex(token, 1), val, 10);
					str1 = gcvt(toDec_Oct_Hex(token, 1), n, val);
					return DEC;
				}

				else
				{
					if ((ss[scan_pos + 1] >= '0') && (ss[scan_pos + 1] <= '7')) //第二位是0-7 那就是八进制
					{
						token[i++] = ss[scan_pos++];//将首位放入token
						while (isdigit(ss[scan_pos]))	token[i++] = ss[scan_pos++];//将其余全部八进制数放入token
						itoa(toDec_Oct_Hex(token, 8), val, 10);//八进制转化为十进制 整形转化为字符串
						return OCT;
					}
					else
					{
						token[0] = '0';
						scan_pos++;
						itoa(toDec_Oct_Hex(token, 10), val, 10);
						return DEC;
					}
				}
			}
			else //第一位不是0
			{
				token[i++] = ss[scan_pos++];
				while (isdigit(ss[scan_pos]))	token[i++] = ss[scan_pos++];
				if (ss[scan_pos] - '.' == 0)//匹配到了小数点 说明是一个小数
				{
					int n = 10;
					char* str1;
					token[i++] = ss[scan_pos++];
					while (isdigit(ss[scan_pos]))	token[i++] = ss[scan_pos++];
					itoa(toDec_Oct_Hex(token, 1), val, 10);
					str1 = gcvt(toDec_Oct_Hex(token, 1), n, val);

				}
				else
				{
					itoa(toDec_Oct_Hex(token, 10), val, 10);
				}
				return DEC;
			}
		}
		if (ss[scan_pos] - '-' == 0)//第一位是- 识别十进制负数
		{
			if (isdigit(ss[scan_pos + 1]))
			{
				if ((ss[scan_pos + 1] - '0') == 0)//符号后面紧跟一个0   应该是负小数  -0.xxxxxx
				{
					if ((ss[scan_pos + 2] - '.') == 0)
					{
						int n = 10;
						char* str1;
						token[i++] = ss[scan_pos++];
						token[i++] = ss[scan_pos++];
						token[i++] = ss[scan_pos++];
						while (isdigit(ss[scan_pos]))	token[i++] = ss[scan_pos++];
						itoa(toDec_Oct_Hex(token, 1), val, 10);
						str1 = gcvt(toDec_Oct_Hex(token, 1), n, val);
						return DEC;
					}
				}
				else//负号后面不是0 -xx.xxxxx
				{
					token[i++] = ss[scan_pos++];
					while (isdigit(ss[scan_pos]))	token[i++] = ss[scan_pos++];
					if (ss[scan_pos] - '.' == 0)//匹配到了小数点 说明是一个小数
					{
						int n = 10;
						char* str1;
						token[i++] = ss[scan_pos++];
						while (isdigit(ss[scan_pos]))	token[i++] = ss[scan_pos++];
						itoa(toDec_Oct_Hex(token, 1), val, 10);//返回浮点数
						str1 = gcvt(toDec_Oct_Hex(token, 1), n, val);
					}
					return DEC;

				}

			}
		}

		if (isalpha(ss[scan_pos])) //是不是字母
		{
			token[i++] = ss[scan_pos++];
			while (isalpha(ss[scan_pos]) || isdigit(ss[scan_pos]) || ss[scan_pos] == '_')	token[i++] = ss[scan_pos++];
			switch (judgeReservedWords(token)) //关键词比对
			{
			case 0:	return IF;
			case 1: return THEN;
			case 2:	return ELSE;
			case 3:	return WHILE;
			case 4:	return DO;
			case 5:	return BEGIN;
			case 6:	return END;
			case -1://不是上述关键词之一 那就有可能是变量
				if ((j = searchIDN(token)) >= 0)
				{
					strcpy(val, ID_list[j].id_name);
					return IDN;
				}
			default:
				printf("\nInput Error~!"); 	break;
			}
		}
		if (ss[scan_pos] == '+') { scan_pos++; return ADD; }
		if (ss[scan_pos] == '-') { scan_pos++;	return SUB; }
		if (ss[scan_pos] == '*') { scan_pos++;	return MUL; }
		if (ss[scan_pos] == '/') { scan_pos++;	return DIV; }
		if (ss[scan_pos] == '=') { scan_pos++; return EQ; }
		if (ss[scan_pos] == '(') { scan_pos++;	return LP; }
		if (ss[scan_pos] == ')') { scan_pos++;	return RP; }
		if (ss[scan_pos] == ';') { scan_pos++;	return SEMI; }
		if (ss[scan_pos] == '#') { scan_pos++;	return EOS; }
		if (ss[scan_pos] == '>')
		{
			if (ss[scan_pos + 1] == '=')
			{
				scan_pos++; scan_pos++; return GE;
			}
			else { scan_pos++; return GT; }
		}
		if (ss[scan_pos] == '<')
		{
			if (ss[scan_pos + 1] == '=')
			{
				scan_pos++; scan_pos++; return LE;
			}
			else if (ss[scan_pos + 1] == '>')
			{
				scan_pos++; scan_pos++; return GL;
			}
			else
			{
				scan_pos++; return LT;
			}
		}
		if (ss[scan_pos] == ':')
		{
			if (ss[scan_pos + 1] == '=')
			{
				scan_pos++; return ASG;
			}
			else	printf("\nInput fail!");
		}
		if (ss[scan_pos] == '_')
		{
			token[i++] = ss[scan_pos++];
			while (isalpha(ss[scan_pos]) || isdigit(ss[scan_pos]) || (ss[scan_pos] == '_'))	token[i++] = ss[scan_pos++];
			if ((j = searchIDN(token)) >= 0)
			{
				strcpy(val, ID_list[j].id_name);
				return IDN;
			}
			else {
				printf("\nInput Error~!"); 	break;
			}
		}
	}
	return 2;
}

void read_file()
{
	FILE* fp; // 文件指针
	FILE* fp1;
	int i = 0, c; // 循环计数变量，文件读取的字符变量
	int j = 0, c1;
	// 打开文件
	fp = fopen("try.txt", "r");
	if (fp == NULL)
	{
		printf("打开文件失败2！\n");
		exit(1);
	}

	// 逐个字符读取文件内容
	while ((c = fgetc(fp)) != EOF && i < 1000 - 1)
	{
		str[i++] = (char)c;
	}
	str[i] = '\0'; // 最后加上字符串终止符
	printf("%s", str);


	// 关闭文件
	fclose(fp);
}

int main()
{
	struct P P;

	read_file();


	c = 0;//当前扫描到的单词
	memset(token, 0, sizeof(token));

	scan_pos = 0;//scan当前扫描下标
	id_num = 0;
	c = scan(str);
	P_Attr(&P);
	printf("\n");
	printf("\n三地址代码：");
	printf("%s", P.code + 5);

	return 0;
}