/*栈*/
/*环境：win10_1809 VS2017*/

#pragma once

#ifndef FAILMEMORY   //内存申请失败
#define FAILMEMORY

#include <stdio.h>
#include <stdlib.h>

void apply_judge(void *p)
{
	if (!p)
	{
		printf("内存申请失败");
		getchar();
		exit(1);
	}
}

#endif

#ifndef STACK_H
#define STACK_H

#include <malloc.h>

typedef struct             //顺序存储结构(top节点不存放数据)
{
	void **base, **top;    //栈底与栈顶指针，在构造之前和销毁之后，top值为NULL
	int stacksize;        //当前已分配的存储空间
}Stack_Ss;
typedef struct SNode      //链式存储结构节点
{
	void *elem;          //节点元素
	struct SNode *next;         //后驱指针
}SNode;
typedef struct            //链式存储结构(top节点不存放数据)
{
	SNode *base, *top;  //栈底与栈顶指针，在构造之前和销毁之后，base值为NULL
}Stack_Cs;
typedef struct
{
	union
	{
		Stack_Ss SS;
		Stack_Cs CS;
	}STACK_KIND;          //栈的存储结构
	int STACK_KIND_FLAG;  //栈的存储结构标识,0:顺序,1:链式
	int ELEM_SIZE;		  //栈的元素所占字节大小
}Stack;              //栈

Stack InitStack(void *elem_type, int STACK_KIND_, int ELEM_SIZE_) //elem_type为元素类型指针,STACK_KIND_为存储结构标识,ELEM_SIZE_为元素所占字节大小，返回一个栈S
//构造一个空栈S
{
	Stack S;
	S.STACK_KIND_FLAG = STACK_KIND_;  //确定栈存储结构
	S.ELEM_SIZE = ELEM_SIZE_;         //确定栈的元素所占字节大小
	switch (STACK_KIND_)
	{
	case 0:
		S.STACK_KIND.SS.base = &elem_type;
		S.STACK_KIND.SS.base = (void**)malloc(sizeof(void*));
		apply_judge(S.STACK_KIND.SS.base);
		*S.STACK_KIND.SS.base = (void*)malloc(S.ELEM_SIZE);
		apply_judge(*S.STACK_KIND.SS.base);
		S.STACK_KIND.SS.top = S.STACK_KIND.SS.base;
		S.STACK_KIND.SS.stacksize = 1;
		break;
	case 1:
		S.STACK_KIND.CS.base = (SNode*)malloc(sizeof(SNode));
		apply_judge(S.STACK_KIND.CS.base);
		S.STACK_KIND.CS.base->elem = (void*)malloc(S.ELEM_SIZE);
		apply_judge(S.STACK_KIND.CS.base->elem);
		S.STACK_KIND.CS.base->next = NULL;
		S.STACK_KIND.CS.top = S.STACK_KIND.CS.base;
		break;
	default:
		printf("类别错误，构造失败\n");
		break;
	}
	return S;
}

int StackExist(Stack S)
//栈S存在返回1
{
	if (S.STACK_KIND_FLAG != 0 && S.STACK_KIND_FLAG != 1)
		return 0;
	return 1;
}

void DestoryStack(Stack *S)
//销毁栈S
{
	SNode *p = NULL, *pr = NULL;
	void **pp = NULL, **ppr = NULL;
	int i;
	if (StackExist(*S))
	{
		switch (S->STACK_KIND_FLAG)
		{
		case 0:
			ppr = S->STACK_KIND.SS.base;
			pp = ppr;
			for (i = 0; i < S->STACK_KIND.SS.stacksize - 1; i++)           //释放二级指针
			{
				ppr = pp;
				*pp++;
				free(*ppr);
			}
			free(*pp);   //释放栈头
			free(S->STACK_KIND.SS.base);           //释放一级指针
			S->STACK_KIND.SS.stacksize = 0;
			break;
		case 1:
			p = S->STACK_KIND.CS.top;
			while (p)
			{
				pr = p;
				p = p->next;
				free(pr->elem);
				free(pr);
			}
			S->STACK_KIND.CS.base=S->STACK_KIND.CS.top=NULL;
			break;
		}
		S->STACK_KIND_FLAG = -1;
		printf("栈已销毁\n");
	}
	else printf("栈不存在\n");
}

void ClearStack(Stack *S)
//把S置为空栈
{
	SNode *p = NULL, *pr = NULL;
	if (StackExist(*S))
	{
		switch (S->STACK_KIND_FLAG)
		{
		case 0:
			S->STACK_KIND.SS.top = S->STACK_KIND.SS.base;
			break;
		case 1:
			p = S->STACK_KIND.CS.top->next;
			S->STACK_KIND.CS.top->next = NULL;
			S->STACK_KIND.CS.base = S->STACK_KIND.CS.top;
			while (p)
			{
				pr = p;
				p = p->next;
				free(pr->elem);
				free(pr);
			}
			break;
		}
		printf("栈已清空\n");
	}
	else printf("栈不存在\n");
}

int StackEmpty(Stack S)
//若S为空栈，返回1
{
	int n = 1;
	if (StackExist(S))
	{
		switch (S.STACK_KIND_FLAG)
		{
		case 0:
			if (S.STACK_KIND.SS.top != S.STACK_KIND.SS.base)
				n = 0;
			break;
		case 1:
			if (S.STACK_KIND.CS.top->next)
				n = 0;
			break;
		}
	}
	else printf("栈不存在\n");
	return n;
}

int StackLength(Stack S)
//返回S中数据元素个数
{
	SNode *p = NULL;
	void **pp = NULL;
	int n = 0;
	if (StackExist(S))
	{
		switch (S.STACK_KIND_FLAG)
		{
		case 0:
			pp = S.STACK_KIND.SS.base;
			while (pp != S.STACK_KIND.SS.top)
			{
				n++;
				*pp++;
			}
			break;
		case 1:
			p = S.STACK_KIND.CS.top->next;
			while (p)
			{
				n++;
				p = p->next;
			}
			break;
		}
	}
	else printf("线性表不存在\n");
	return n;
}

void GetTop(Stack S, void **e, void(*COPY)(void*, void*))
//不能通过void*指针直接操作内存，因此用一个copy函数将后面指针指向的内存中的值复制到前面
//用e返回S的栈顶元素
{
	void **pp = NULL;
	if (!StackEmpty(S))
	{
		if (!*e)     //假如传进来的e为空指针
		{
			*e = (void*)malloc(S.ELEM_SIZE);
			apply_judge(*e);
		}
		switch (S.STACK_KIND_FLAG)
		{
		case 0:
			pp = S.STACK_KIND.SS.top;
			*pp--;
			COPY(*e, *pp);
			break;
		case 1:
			COPY(*e, S.STACK_KIND.CS.top->next->elem);
			break;
		}
	}
	else
	{
		printf("操作失败\n");
	}
}

void Push(Stack *S, void *e, void(*COPY)(void*, void*))
//插入元素e为新的栈顶元素
{
	SNode *p = NULL;
	void **pp = NULL;
	int i = 0;
	int l = StackLength(*S);
	if (StackExist(*S))
	{
		switch (S->STACK_KIND_FLAG)
		{
		case 0:
			if (l == S->STACK_KIND.SS.stacksize - 1) //分配空间不足
			{
				S->STACK_KIND.SS.stacksize++;
				S->STACK_KIND.SS.base = (void**)realloc(S->STACK_KIND.SS.base, S->STACK_KIND.SS.stacksize * sizeof(void*));
				apply_judge(S->STACK_KIND.SS.base);
				S->STACK_KIND.SS.top = S->STACK_KIND.SS.base;
				for (i = 0; i < l; i++)     //确定栈顶的位置
					*S->STACK_KIND.SS.top++;
				pp = S->STACK_KIND.SS.top;
				*pp++;
				*pp = (void*)malloc(S->ELEM_SIZE);
				apply_judge(*pp);
			}
			pp = S->STACK_KIND.SS.top;
			COPY(*pp, e);
			*S->STACK_KIND.SS.top++;
			break;
		case 1:
			p = (SNode*)malloc(sizeof(SNode));
			apply_judge(p);
			p->elem = (void*)malloc(S->ELEM_SIZE);
			apply_judge(p->elem);
			COPY(p->elem, e);
			p->next = S->STACK_KIND.CS.top->next;
			S->STACK_KIND.CS.top->next = p;
			break;
		}
	}
	else
	{
		printf("入栈失败\n");
	}
}

void Pop(Stack *S, void **e, void(*COPY)(void*, void*))
//出栈栈顶元素，用e返回其值
{
	SNode *p = NULL;
	if (!StackEmpty(*S))
	{
		if (!*e)     //假如传进来的e为空指针
		{
			*e = (void*)malloc(S->ELEM_SIZE);
			apply_judge(*e);
		}
		switch (S->STACK_KIND_FLAG)
		{
		case 0:
			*S->STACK_KIND.SS.top--;
			COPY(*e, *S->STACK_KIND.SS.top);
			break;
		case 1:
			p = S->STACK_KIND.CS.top->next;
			COPY(*e, p->elem);
			S->STACK_KIND.CS.top->next = p->next;
			free(p);
			break;
		}
	}
	else
	{
		printf("出栈失败\n");
	}
}

void StackTraverse(Stack S, int(*VISIT)(void *))
//从栈顶到栈底依次对S的每个数据元素调用VISIT(),一旦VISIT()失败，则操作失败。
{
	SNode *p = NULL;
	void **pp = NULL;
	if (!StackEmpty(S))
	{
		switch (S.STACK_KIND_FLAG)
		{
		case 0:
			pp = S.STACK_KIND.SS.top;
			*pp--;
			while (pp != S.STACK_KIND.SS.base)
			{
				if (!VISIT(*pp))
				{
					printf("操作失败\n");
					return;
				}
				*pp--;
			}
			if (!VISIT(*pp))    //visit栈底
			{
				printf("操作失败\n");
				return;
			}
			break;
		case 1:
			p = S.STACK_KIND.CS.top->next;
			while (p)
			{
				if (!VISIT(p->elem))
				{
					printf("操作失败\n");
					return;
				}
				p = p->next;
			}
			break;
		}
	}
}

#endif
