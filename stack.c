#include <stdio.h>
#include "stack.h"

int visit(void *a);
void copy(void *a, void *b);
int compare(void *a, void *b);

int main()
{
	Stack S;
	int i = 0;
	char ch = 'a' - 1;
	char *st = NULL;
	S = InitStack(&ch, 0, sizeof(char));
	for (i = 1; i < 10; i++)
	{
		ch++;
		Push(&S, &ch, copy);
	}
	printf("初始入栈：");
	StackTraverse(S, visit);
	printf("\n");

	ch = 't';
	Push(&S, &ch, copy);
	printf("入栈    ：");
	StackTraverse(S, visit);
	printf("\n");

	for (i = 0; i < 5; i++)
	{
		Pop(&S, &st, copy);
		printf("出栈    ：");
		printf("%c  ", *st);
		StackTraverse(S, visit);
		printf("\n");
	}

	GetTop(S, &st, copy);
	printf("栈顶元素：");
	printf("%c  ", *st);
	StackTraverse(S, visit);
	printf("\n");

	ClearStack(&S);
	printf("清空栈  ：");
	StackTraverse(S, visit);
	printf("\n");

	Push(&S, &ch, copy);
	printf("入栈    ：");
	StackTraverse(S, visit);
	printf("\n");

	DestoryStack(&S);
	printf("销毁栈  ：");
	StackTraverse(S, visit);
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