#include <stdio.h>
#include "linearlist.h"

int visit(void *a);
void copy(void *a, void *b);
int compare(void *a, void *b);

int main()
{
	List L;
	int i = 0, j;
	char ch = 'a' - 1;
	char *st = NULL;
	void **p = NULL;
	L = InitList(&ch, 1, sizeof(char));
	for (i = 1; i < 10; i++)
	{
		ch++;
		ListInsert(&L, i, &ch, copy);
		p = L.LIST_KIND.SL.elem;
		for (j = 0; j < i; j++)
			p++;
	}
	printf("初始线性表     ：");
	ListTraverse(L, visit);
	printf("\n");

	printf("插入元素到第4位：");
	ch = 't';
	ListInsert(&L, 4, &ch, copy);
	ListTraverse(L, visit);
	printf("\n");

	printf("第三个元素     ：");
	GetElem(L, 3, &st, copy);
	printf("%c  ", *st);
	ListTraverse(L, visit);
	printf("\n");

	ch = 'd';
	printf("元素%c位置      ：",ch);
	printf("%d\n", LocateElem(L, &ch, compare));

	PriorElem(L, &ch, &st, compare, copy);
	printf("%c的前驱        ：",ch);
	printf("%c  ", *st);
	ListTraverse(L, visit);
	printf("\n");

	NextElem(L, &ch, &st, compare, copy);
	printf("%c的后继        ：", ch);
	printf("%c  ", *st);
	ListTraverse(L, visit);
	printf("\n");

	ListDelete(&L, 4, &st, copy);
	printf("删除第4位元素  ：");
	printf("%c  ", *st);
	ListTraverse(L, visit);
	printf("\n");

	ClearList(&L);
	printf("清空表         ：");
	ListTraverse(L, visit);
	printf("\n");

	DestoryList(&L);
	printf("销毁表         ：");
	ListTraverse(L, visit);
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