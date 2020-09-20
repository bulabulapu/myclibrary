/*队列*/
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

#ifndef QUEUE
#define QUEUE

#include <malloc.h>

typedef struct             //顺序存储结构(front结点不存放数据)
{
	void **front, **rear;    //队头指针与队尾指针，在构造之前和销毁之后，front值为NULL
	int queuesize;           //当前已分配的存储空间
}Queue_Sq;
typedef struct QNode      //链式存储结构结点
{
	void *elem;          //结点元素
	struct QNode *next;         //后驱指针
}QNode;
typedef struct            //链式存储结构(front结点不存放数据)
{
	QNode *front, *rear;  //队头与队尾指针，在构造之前和销毁之后，front值为NULL
}Queue_Cq;
typedef struct
{
	union
	{
		Queue_Sq SQ;
		Queue_Cq CQ;
	}QUEUE_KIND;          //队列的存储结构
	int QUEUE_KIND_FLAG;  //队列的存储结构标识,0:顺序,1:链式
	int ELEM_SIZE;		  //队列的元素所占字节大小
}Queue;              //队列

Queue InitQueue(void *elem_type, int QUEUE_KIND_, int ELEM_SIZE_) //elem_type为元素类型指针,QUEUE_KIND_为存储结构标识,ELEM_SIZE_为元素所占字节大小，返回一个队列Q
//构造一个空队列Q
{
	Queue Q;
	Q.QUEUE_KIND_FLAG = QUEUE_KIND_;  //确定队列存储结构
	Q.ELEM_SIZE = ELEM_SIZE_;         //确定队列的元素所占字节大小
	switch (QUEUE_KIND_)
	{
	case 0:
		Q.QUEUE_KIND.SQ.front = &elem_type;
		Q.QUEUE_KIND.SQ.front = (void**)malloc(sizeof(void*));
		apply_judge(Q.QUEUE_KIND.SQ.front);
		*Q.QUEUE_KIND.SQ.front = (void*)malloc(Q.ELEM_SIZE);
		apply_judge(*Q.QUEUE_KIND.SQ.front);
		Q.QUEUE_KIND.SQ.rear = Q.QUEUE_KIND.SQ.front;
		Q.QUEUE_KIND.SQ.queuesize = 1;
		break;
	case 1:
		Q.QUEUE_KIND.CQ.front = (QNode*)malloc(sizeof(QNode));
		apply_judge(Q.QUEUE_KIND.CQ.front);
		Q.QUEUE_KIND.CQ.front->elem = (void*)malloc(Q.ELEM_SIZE);
		apply_judge(Q.QUEUE_KIND.CQ.front->elem);
		Q.QUEUE_KIND.CQ.front->next = NULL;
		Q.QUEUE_KIND.CQ.rear = Q.QUEUE_KIND.CQ.front;
		break;
	default:
		printf("类别错误，构造失败\n");
		break;
	}
	return Q;
}

int QueueExist(Queue Q)
//队列Q存在返回1
{
	if (Q.QUEUE_KIND_FLAG != 0 && Q.QUEUE_KIND_FLAG != 1)
		return 0;
	return 1;
}

void DestoryQueue(Queue *Q)
//销毁队列Q
{
	QNode *p = NULL, *pr = NULL;
	void **pp = NULL, **ppr = NULL;
	int i;
	if (QueueExist(*Q))
	{
		switch (Q->QUEUE_KIND_FLAG)
		{
		case 0:
			ppr = Q->QUEUE_KIND.SQ.front;
			pp = ppr;
			for (i = 0; i < Q->QUEUE_KIND.SQ.queuesize - 1; i++)   //释放二级指针
			{
				ppr = pp;
				*pp++;
				free(*ppr);
			}
			free(*pp);
			free(Q->QUEUE_KIND.SQ.front);             //释放一级指针
			Q->QUEUE_KIND.SQ.queuesize = 0;
			break;
		case 1:
			p = Q->QUEUE_KIND.CQ.front;
			while (p)
			{
				pr = p;
				p = p->next;
				free(pr->elem);
				free(pr);
			}
			Q->QUEUE_KIND.CQ.rear = Q->QUEUE_KIND.CQ.front = NULL;
			break;
		}
		Q->QUEUE_KIND_FLAG = -1;
		printf("队列已销毁\n");
	}
	else printf("队列不存在\n");
}

void ClearQueue(Queue *Q)
//把Q置为空队列
{
	QNode *p = NULL, *pr = NULL;
	void **pp = NULL, **ppr = NULL;
	int i;
	if (QueueExist(*Q))
	{
		switch (Q->QUEUE_KIND_FLAG)
		{
		case 0:
			ppr = Q->QUEUE_KIND.SQ.front;
			*ppr++;
			pp = ppr;
			for (i = 0; i < Q->QUEUE_KIND.SQ.queuesize - 2; i++)   //释放二级指针
			{
				ppr = pp;
				*pp++;
				free(*ppr);
			}
			free(*pp);   //释放队列尾
			Q->QUEUE_KIND.SQ.rear = Q->QUEUE_KIND.SQ.front;
			Q->QUEUE_KIND.SQ.queuesize = 1;
			break;
		case 1:
			p = Q->QUEUE_KIND.CQ.front->next;
			Q->QUEUE_KIND.CQ.rear = Q->QUEUE_KIND.CQ.front;
			Q->QUEUE_KIND.CQ.front->next = NULL;
			Q->QUEUE_KIND.CQ.rear = Q->QUEUE_KIND.CQ.front;
			while (p)
			{
				pr = p;
				p = p->next;
				free(pr->elem);
				free(pr);
			}
			break;
		}
		printf("队列已清空\n");
	}
	else printf("队列不存在\n");
}

int QueueEmpty(Queue Q)
//若Q为空队列，返回1
{
	int n = 1;
	if (QueueExist(Q))
	{
		switch (Q.QUEUE_KIND_FLAG)
		{
		case 0:
			if (Q.QUEUE_KIND.SQ.rear != Q.QUEUE_KIND.SQ.front)
				n = 0;
			break;
		case 1:
			if (Q.QUEUE_KIND.CQ.front->next)
				n = 0;
			break;
		}
	}
	else printf("队列不存在\n");
	return n;
}

int QueueLength(Queue Q)
//返回Q中数据元素个数
{
	QNode *p = NULL;
	void **pp = NULL;
	int n = 0;
	if (QueueExist(Q))
	{
		switch (Q.QUEUE_KIND_FLAG)
		{
		case 0:
			n = Q.QUEUE_KIND.SQ.queuesize - 1;
			break;
		case 1:
			p = Q.QUEUE_KIND.CQ.front->next;
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

void GetHead(Queue Q, void **e, void(*COPY)(void*, void*))
//不能通过void*指针直接操作内存，因此用一个copy函数将后面指针指向的内存中的值复制到前面
//用e返回Q的队头元素
{
	void **pp = NULL;
	if (!QueueEmpty(Q))
	{
		if (!*e)     //假如传进来的e为空指针
		{
			*e = (void*)malloc(Q.ELEM_SIZE);
			apply_judge(*e);
		}
		switch (Q.QUEUE_KIND_FLAG)
		{
		case 0:
			pp = Q.QUEUE_KIND.SQ.front;
			*pp++;
			COPY(*e, *pp);
			break;
		case 1:
			COPY(*e, Q.QUEUE_KIND.CQ.front->next->elem);
			break;
		}
	}
	else
	{
		printf("操作失败\n");
	}
}

void EnQueue(Queue *Q, void *e, void(*COPY)(void*, void*))
//插入元素e为新的队尾元素
{
	QNode *p = NULL;
	int i = 0;
	if (QueueExist(*Q))
	{
		switch (Q->QUEUE_KIND_FLAG)
		{
		case 0:
			Q->QUEUE_KIND.SQ.queuesize++;
			Q->QUEUE_KIND.SQ.front = (void**)realloc(Q->QUEUE_KIND.SQ.front, Q->QUEUE_KIND.SQ.queuesize * sizeof(void*));
			apply_judge(Q->QUEUE_KIND.SQ.front);
			Q->QUEUE_KIND.SQ.rear = Q->QUEUE_KIND.SQ.front;
			for (i = 0; i < Q->QUEUE_KIND.SQ.queuesize - 1; i++)     //确定队尾的位置
				*Q->QUEUE_KIND.SQ.rear++;
			*Q->QUEUE_KIND.SQ.rear = (void*)malloc(Q->ELEM_SIZE);
			apply_judge(*Q->QUEUE_KIND.SQ.rear);
			COPY(*Q->QUEUE_KIND.SQ.rear, e);
			break;
		case 1:
			p = (QNode*)malloc(sizeof(QNode));
			apply_judge(p);
			p->elem = (void*)malloc(Q->ELEM_SIZE);
			apply_judge(p->elem);
			COPY(p->elem, e);
			Q->QUEUE_KIND.CQ.rear->next = p;
			p->next = NULL;
			Q->QUEUE_KIND.CQ.rear = p;
			break;
		}
	}
	else
	{
		printf("入队失败\n");
	}
}

void DeQueue(Queue *Q, void **e, void(*COPY)(void*, void*))
//出队头元素，用e返回其值
{
	QNode *p = NULL;
	void **pp = NULL, **ppr = NULL;
	int i;

	if (!QueueEmpty(*Q))
	{
		if (!*e)     //假如传进来的e为空指针
		{
			*e = (void*)malloc(Q->ELEM_SIZE);
			apply_judge(*e);
		}
		switch (Q->QUEUE_KIND_FLAG)
		{
		case 0:   //若使队列在内存中移动，即每次使front后移，则会丢失front的初始地址，导致之后free出现问题，因此使数据移动
			pp = Q->QUEUE_KIND.SQ.front;
			*pp++;
			COPY(*e, *pp);
			for (i = 0; i < Q->QUEUE_KIND.SQ.queuesize - 2; i++)
			{
				ppr = pp;
				pp++;
				COPY(*ppr, *pp);
			}
			Q->QUEUE_KIND.SQ.rear--;
			free(*pp);
			Q->QUEUE_KIND.SQ.queuesize--;
			break;
		case 1:
			p = Q->QUEUE_KIND.CQ.front->next;
			COPY(*e, p->elem);
			p = Q->QUEUE_KIND.CQ.front;
			Q->QUEUE_KIND.CQ.front = p->next;
			free(p);
			break;
		}
	}
	else
	{
		printf("出队失败\n");
	}
}

void QueueTraverse(Queue Q, int(*VISIT)(void *))
//从队头到队尾依次对Q的每个数据元素调用VISIT(),一旦VISIT()失败，则操作失败。
{
	QNode *p = NULL;
	void **pp = NULL;
	if (!QueueEmpty(Q))
	{
		switch (Q.QUEUE_KIND_FLAG)
		{
		case 0:
			pp = Q.QUEUE_KIND.SQ.front;
			*pp++;
			while (pp != Q.QUEUE_KIND.SQ.rear)
			{
				if (!VISIT(*pp))
				{
					printf("操作失败\n");
					return;
				}
				*pp++;
			}
			if (!VISIT(*pp))   //visit队尾
			{
				printf("操作失败\n");
				return;
			}
			break;
		case 1:
			p = Q.QUEUE_KIND.CQ.front->next;
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