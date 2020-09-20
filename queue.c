#include <stdio.h>
#include "queue.h"

int visit(void *a);
void copy(void *a, void *b);
int compare(void *a, void *b);

int main()
{
	Queue Q;
	int i = 0;
	char ch = 'a' - 1;
	char *st = NULL;
	Q = InitQueue(&ch, 0, sizeof(char));
	for (i = 1; i < 10; i++)
	{
		ch++;
		EnQueue(&Q, &ch, copy);
	}
	printf("初始入队：");
	QueueTraverse(Q, visit);
	printf("\n");
	getchar();

	ch = 't';
	EnQueue(&Q, &ch, copy);
	printf("入队    ：");
	QueueTraverse(Q, visit);
	printf("\n");
	getchar();

	for (i = 0; i < 5; i++)
	{
		DeQueue(&Q, &st, copy);
		printf("出队    ：");
		printf("%c  ", *st);
		QueueTraverse(Q, visit);
		printf("\n");
	}
	getchar();

	GetHead(Q, &st, copy);
	printf("队头元素：");
	printf("%c  ", *st);
	QueueTraverse(Q, visit);
	printf("\n");
	getchar();

	ClearQueue(&Q);
	printf("清空队列：");
	QueueTraverse(Q, visit);
	printf("\n");
	getchar();

	EnQueue(&Q, &ch, copy);
	printf("入队    ：");
	QueueTraverse(Q, visit);
	printf("\n");
	getchar();

	DestoryQueue(&Q);
	printf("销毁队列：");
	QueueTraverse(Q, visit);
	printf("\n");
	getchar();

	return 0;
}
int visit(void *a)
{
	char *b = a;
	return printf("%c ", *b);
}
void copy(void *a, void *b)
{
	char *p = a;
	char *q = b;
	*p = *q;
}
int compare(void *a, void *b)
{
	char *p = a;
	char *q = b;
	if (*p > *q) return 0;
	else if (*p < *q) return 0;
	else return 1;
}