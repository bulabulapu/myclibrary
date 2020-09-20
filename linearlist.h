/*线性表*/
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

#ifndef LINEARLIST_H
#define LINEARLIST_H

#include <malloc.h>

typedef struct         //顺序表
{
	void **elem;    //存储空间基址
	int length;        //当前长度
	int listsize;      //当前分配的长度
}List_SL;
typedef struct LNode  //链式表(带头结点)
{
	void *elem;      //结点元素
	struct LNode *next; //后驱指针
}LNode;
typedef struct       //线性表
{
	union
	{
		List_SL SL;
		LNode CL;
	}LIST_KIND;           //线性表的存储结构
	int LIST_KIND_FLAG;   //线性表的存储结构标识,0:顺序表,1:链式表
	int ELEM_SIZE;        //线性表的元素所占字节大小
}List;

List InitList(void *elem_type, int LIST_KIND_, int ELEM_SIZE_)  //elem_type为元素类型指针,LIST_KIND_为线性表的存储结构标识,ELEM_SIZE_为线性表的元素所占字节大小，返回一个线性表L
//构造一个空的线性表
{
	List L;
	L.LIST_KIND_FLAG = LIST_KIND_;  //确定线性表存储结构
	L.ELEM_SIZE = ELEM_SIZE_;       //确定线性表的元素所占字节大小
	switch (LIST_KIND_)
	{
	case 0:
		L.LIST_KIND.SL.elem = &elem_type;
		L.LIST_KIND.SL.elem = (void**)malloc(sizeof(void*));
		apply_judge(L.LIST_KIND.SL.elem);
		*L.LIST_KIND.SL.elem = (void*)malloc(L.ELEM_SIZE);
		apply_judge(*L.LIST_KIND.SL.elem);
		L.LIST_KIND.SL.length = 0;
		L.LIST_KIND.SL.listsize = 1;
		break;
	case 1:
		L.LIST_KIND.CL.elem = elem_type;
		L.LIST_KIND.CL.next = NULL;
		break;
	default:
		printf("类别错误，构造失败\n");
		break;
	}
	return L;
}

int ListExist(List L)
//线性表L存在返回1
{
	if (L.LIST_KIND_FLAG != 0 && L.LIST_KIND_FLAG != 1)
		return 0;
	return 1;
}

void DestoryList(List *L)
//销毁线性表L
{
	LNode *p = NULL, *pr = NULL;
	void **pp = NULL, **ppr = NULL;
	int i;
	if (ListExist(*L))
	{
		switch (L->LIST_KIND_FLAG)
		{
		case 0:
			ppr = L->LIST_KIND.SL.elem;
			pp = ppr;
			for (i = 0; i < L->LIST_KIND.SL.listsize - 1; i++)           //释放二级指针
			{
				ppr = pp;
				*pp++;
				free(*ppr);
			}
			free(*pp);   //释放最后一个元素
			free(L->LIST_KIND.SL.elem);           //释放一级指针
			L->LIST_KIND.SL.length = 0;
			L->LIST_KIND.SL.listsize = 0;
			break;
		case 1:
			p = L->LIST_KIND.CL.next;
			L->LIST_KIND.CL.next = NULL;
			while (p)
			{
				pr = p;
				p = p->next;
				free(pr->elem);
				free(pr);
			}
			break;
		}
		L->LIST_KIND_FLAG = -1;
		printf("线性表已销毁\n");
	}
	else printf("线性表不存在\n");
}

void ClearList(List *L)
//清空线性表L
{
	LNode *p = NULL, *pr = NULL;
	if (ListExist(*L))
	{
		switch (L->LIST_KIND_FLAG)
		{
		case 0:
			L->LIST_KIND.SL.length = 0;
			break;
		case 1:
			p = L->LIST_KIND.CL.next;
			L->LIST_KIND.CL.next = NULL;
			while (p)
			{
				pr = p;
				p = p->next;
				free(pr);
			}
			break;
		}
		printf("线性表已清空\n");
	}
	else printf("线性表不存在\n");
}

int ListEmpty(List L)
//判断L为空表，返回1
{
	int n = 1;
	if (ListExist(L))
	{
		switch (L.LIST_KIND_FLAG)
		{
		case 0:
			if (L.LIST_KIND.SL.length)
				n = 0;
			break;
		case 1:
			if (L.LIST_KIND.CL.next)
				n = 0;
			break;
		}
	}
	else printf("线性表不存在\n");
	return n;
}

int ListLength(List L)
//返回L中数据元素个数
{
	LNode *p = NULL;
	int n = 0;
	if (ListExist(L))
	{
		switch (L.LIST_KIND_FLAG)
		{
		case 0:
			n = L.LIST_KIND.SL.length;
			break;
		case 1:
			p = L.LIST_KIND.CL.next;
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

void GetElem(List L, int i, void **e, void(*COPY)(void*, void*))
//不能通过void*指针直接操作内存，因此用一个copy函数将后面指针指向的内存中的值复制到前面
//用e返回L中第i个数据的元素
{
	LNode *p = NULL;
	void **pp = NULL;
	int n = 1;
	if (i >= 1 && i <= ListLength(L) && ListExist(L))
	{
		if (!*e)     //假如传进来的e为空指针
		{
			*e = (void*)malloc(L.ELEM_SIZE);
			apply_judge(*e);
		}
		switch (L.LIST_KIND_FLAG)
		{
		case 0:
			pp = L.LIST_KIND.SL.elem;
			while (n != i)
			{
				n++;
				*pp++;
			}
			COPY(*e, *pp);
			break;
		case 1:
			p = L.LIST_KIND.CL.next;
			while (n != i)
			{
				n++;
				p = p->next;
			}
			COPY(*e, p->elem);
			break;
		}
	}
	else
	{
		printf("操作失败\n");
	}
}

int LocateElem(List L, void *e, int(*COMPARE)(void*, void*))
//不能通过void*指针直接操作内存，因此用一个copy函数将后面指针指向的内存中的值复制到前面
//返回L中第一个与e满足关系COMPARE的数据元素的位序。若这样的数据元素不存在，则返回值为0。满足关系COMPARE时返回值为1
{
	LNode *p = NULL;
	void **pp = NULL;
	int i = 1;
	int l = ListLength(L);
	if (ListExist(L))
	{
		switch (L.LIST_KIND_FLAG)
		{
		case 0:
			pp = L.LIST_KIND.SL.elem;
			while (!COMPARE(*pp, e) && i <= l)
			{
				*pp++;
				i++;
			}
			break;
		case 1:
			p = L.LIST_KIND.CL.next;
			while (!COMPARE(p->elem, e) && i <= l)
			{
				i++;
				p = p->next;
			}
			break;
		}
		if (i > l) return 0;
		else return i;
	}
	else printf("线性表不存在\n");
	return 0;
}

void PriorElem(List L, void *cur_e, void **pre_e, int(*COMPARE)(void*, void*), void(*COPY)(void*, void*))
//不能通过void*指针直接操作内存，因此用一个copy函数将后面指针指向的内存中的值复制到前面)
//若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱，否则操作失败,pre_e无定义
{
	LNode *p = NULL, *pr = NULL;
	void **pp = NULL, **ppr = NULL;
	int i = 1;
	int l = ListLength(L);
	if (ListExist(L))
	{
		if (!*pre_e)     //假如传进来的e为空指针
		{
			*pre_e = (void*)malloc(L.ELEM_SIZE);
			apply_judge(*pre_e);
		}
		switch (L.LIST_KIND_FLAG)
		{
		case 0:
			ppr = L.LIST_KIND.SL.elem;
			pp = ppr;
			*pp++;
			if (!COMPARE(*ppr, cur_e))
			{
				while (!COMPARE(*pp, cur_e) && i < l - 1)
				{
					ppr = pp;
					*pp++;
					i++;
				}
				if (i == l - 1) ppr = pp;
				COPY(*pre_e, *ppr);
			}
			break;
		case 1:
			pr = L.LIST_KIND.CL.next;
			p = pr->next;
			if (!COMPARE(pr->elem, cur_e))
			{
				while (!COMPARE(p->elem, cur_e) && i < l - 1)
				{
					pr = p;
					p = p->next;
					i++;
				}
				if (i == l - 1) pr = p;
				COPY(*pre_e , pr->elem);
			}
			break;
		}
	}
	else printf("线性表不存在\n");
}

void NextElem(List L, void *cur_e, void **next_e, int(*COMPARE)(void*, void*), void(*COPY)(void*, void*))
//不能通过void*指针直接操作内存，因此用一个copy函数将后面指针指向的内存中的值复制到前面
//若cur_e是L的数据元素，且不是最后一个，则用pre_e返回它的后继，否则操作失败,pre_e无定义
{
	LNode *p = NULL;
	void **pp = NULL;
	int i = 0;
	int l = ListLength(L);
	if (ListExist(L))
	{
		if (!*next_e)     //假如传进来的e为空指针
		{
			*next_e = (void*)malloc(L.ELEM_SIZE);
			apply_judge(*next_e);
		}
		switch (L.LIST_KIND_FLAG)
		{
		case 0:
			pp = L.LIST_KIND.SL.elem;
			while (!COMPARE(*pp, cur_e) && i < l - 1)
			{
				*pp++;
				i++;
			}
			*pp++;
			COPY(*next_e, *pp);
			break;
		case 1:
			p = L.LIST_KIND.CL.next;
			while (!COMPARE(p->elem, cur_e) && i < l - 1)
			{
				i++;
				p = p->next;
			}
			p = p->next;
			COPY(*next_e, p->elem);
			break;
		}
	}
	else printf("线性表不存在\n");
}

void ListInsert(List *L, int i, void *e, void(*COPY)(void*, void*))
//不能通过void*指针直接操作内存，因此用一个copy函数将后面指针指向的内存中的值复制到前面
//在L中第i个位置前插入新的数据元素e,L的长度加1
{
	LNode *p = NULL, *pr = NULL;
	void **pp = NULL, **ppr = NULL;
	int n = 1;
	int l = ListLength(*L);
	if (i >= 1 && i <= l + 1 && ListExist(*L))
	{
		switch (L->LIST_KIND_FLAG)
		{
		case 0:
			if (L->LIST_KIND.SL.length == L->LIST_KIND.SL.listsize - 1) //分配空间不足
			{
				L->LIST_KIND.SL.listsize++;
				L->LIST_KIND.SL.elem = (void**)realloc(L->LIST_KIND.SL.elem, L->LIST_KIND.SL.listsize * sizeof(void*));
				apply_judge(L->LIST_KIND.SL.elem);
				pp = L->LIST_KIND.SL.elem;
				for (n = 1; n < L->LIST_KIND.SL.listsize; n++)  //新申请的元素结点申请存储空间
					*pp++;
				*pp = (void*)malloc(L->ELEM_SIZE);
				apply_judge(*pp);
			}
			if (ListEmpty(*L))   //为空表时
			{
				COPY(*L->LIST_KIND.SL.elem, e);
				L->LIST_KIND.SL.length++;
			}
			else
			{
				L->LIST_KIND.SL.length++;   //长度加1
				l = L->LIST_KIND.SL.length;
				pp = L->LIST_KIND.SL.elem;
				for (n = 0; n < L->LIST_KIND.SL.length; n++)
					*pp++;
				ppr = pp;
				*ppr--;
				while (n > i - 1)
				{
					n--;
					COPY(*pp, *ppr);
					pp = ppr;
					if (n > 0) //第一个位置时,ppr不能往前移动
						*ppr--;
				}
				COPY(*pp, e);
			}
			break;
		case 1:
			p = (LNode*)malloc(sizeof(LNode));
			apply_judge(p);
			p->elem = (void*)malloc(L->ELEM_SIZE);
			apply_judge(p->elem);
			COPY(p->elem, e);
			if (i == 1)
			{
				p->next = L->LIST_KIND.CL.next;
				L->LIST_KIND.CL.next = p;
			}
			else
			{
				pr = L->LIST_KIND.CL.next;
				while (n < i - 1)
				{
					n++;
					pr = pr->next;
				}
				p->next = pr->next;
				pr->next = p;
			}
			break;
		}
	}
	else
	{
		printf("插入失败\n");
	}
}

void ListDelete(List *L, int i, void **e, void(*COPY)(void*, void*))
//不能通过void*指针直接操作内存，因此用一个copy函数将后面指针指向的内存中的值复制到前面
//删除L的第i个数据元素,并用e返回其值,L长度减1
{
	LNode *p = NULL, *pr = NULL;
	void **pp = NULL, **ppr = NULL;
	int n = 1;
	int l = ListLength(*L);
	if (i >= 1 && i <= ListLength(*L) && l)
	{
		if (!*e)     //假如传进来的e为空指针
		{
			*e = (void*)malloc(L->ELEM_SIZE);
			apply_judge(*e);
		}
		switch (L->LIST_KIND_FLAG)
		{
		case 0:
			pp = L->LIST_KIND.SL.elem;
			while (n != i)  //找到第i个元素
			{
				*pp++;
				n++;
			}
			COPY(*e, *pp);
			for (; n < l; n++) //将后面的元素前移
			{
				ppr = pp;
				*pp++;
				COPY(*ppr, *pp);
			}
			L->LIST_KIND.SL.length--;
			break;
		case 1:
			p = L->LIST_KIND.CL.next;
			if (i == 1)
			{
				L->LIST_KIND.CL.next = p->next;
				COPY(*e, p->elem);
				free(p->elem);
				free(p);
			}
			else
			{
				while (n != i)
				{
					n++;
					pr = p;
					p = p->next;
				}
				COPY(*e, p->elem);
				pr->next = p->next;
				free(p->elem);
				free(p);
			}
			break;
		}
	}
	else
	{
		printf("删除失败\n");
	}
}

void ListTraverse(List L, int(*VISIT)(void *))
//依次对L的每个数据元素调用VISIT(),一旦VISIT()失败，则操作失败。
{
	LNode *p = NULL;
	void **pp = NULL;
	int i = 0;
	int l = ListLength(L);
	if (l)
	{
		switch (L.LIST_KIND_FLAG)
		{
		case 0:
			pp = L.LIST_KIND.SL.elem;
			for (; i < l; i++)
			{
				if (!VISIT(*pp))
				{
					printf("操作失败\n");
					return;
				}
				*pp++;
			}
			break;
		case 1:
			p = L.LIST_KIND.CL.next;
			for (; i < l; i++)
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
