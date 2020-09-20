#include <stdio.h>
#include "binarytree.h"

int visit(void *a);
void copy(void *a, void *b);
int compare(void *a, void *b);
BiTNode* Creat(BiTNode *T, int i); //构造树

int main()
{
	BiTree T,C;
	BiTNode *p = NULL;
	int i = 5, j;
	T = InitBiTree(&i, 1, sizeof(int));
	copy(T.BITREE_KIND.CT.root->elem, &i);
	p = T.BITREE_KIND.CT.root;
	p->lchild = Creat(p->lchild, 4);
	p->rchild = Creat(p->rchild, 4);

	//StorageChange(&T, copy);
/*
	printf("先序遍历:");
	PreOrderTraverse(T, visit);
	printf("\n");

	printf("中序遍历:");
	InOrderTraverse(T, visit);
	printf("\n");

	printf("后序遍历:");
	PostOrderTraverse(T, visit);
	printf("\n");

	printf("层序遍历:");
	LevelOrderTraverse(T, visit);
	printf("\n");

	printf("树的深度：%d\n",BiTreeDepth(T));

	i = 5;
	j = 0;
	Assign(&T, &i, &j,copy,compare);
	printf("值替换：");
	LevelOrderTraverse(T, visit);
	printf("\n");

	i = 5;
	j = 0;
	Parent(T, &i, &j, copy, compare);
	printf("4的根：%d", j);
	printf("\n");
	
	i = 4;
	j = 0;
	LeftChild(T, &i, &j, copy, compare);
	printf("4的左孩子：%d", j);
	printf("\n");

	i = 4;
	j = 0;
	RightChild(T, &i, &j, copy, compare);
	printf("4的右孩子：%d", j);
	printf("\n");

	i = 4;
	j = 0;
	Assign(&T, &i, &j, copy, compare);  //5的左孩子替换为0
	LevelOrderTraverse(T, visit);
	LeftSibling(T, &i, &j, copy, compare);
	printf("4的左兄弟：%d", j);
	printf("\n");

	i = 4;
	j = 0;
	Assign(&T, &i, &j, copy, compare);  //5的左孩子替换为0
	LevelOrderTraverse(T, visit);
	RightSibling(T, &j, &i, copy, compare);
	printf("0的右兄弟：%d", i);
	printf("\n");

	i = 10;
	C = InitBiTree(&i, 1, sizeof(int));
	copy(C.BITREE_KIND.CT.root->elem, &i);
	p = C.BITREE_KIND.CT.root;
	p->lchild = Creat(p->lchild, 2);
	i = 5;
	//StorageChange(&C, copy);
	//StorageChange(&T, copy);
	LevelOrderTraverse(T, visit);
	printf("\n");
	LevelOrderTraverse(C, visit);
	printf("\n");
	InsertChild(&T,&i,1,&C,1,copy,compare);
	LevelOrderTraverse(T, visit);
	printf("\n");
	LevelOrderTraverse(C, visit);
	printf("\n");

	i = 5;
	DeleteChild(&T,&i,0,compare);
	LevelOrderTraverse(T, visit);
	printf("\n");
*/	
	return 0;
}
int visit(void *a)
{
	int *b = a;
	return printf("%d ", *b);
}
void copy(void *a, void *b)
{
	int *p = a;
	int *q = b;
	*p = *q;
}
int compare(void *a, void *b)
{
	int *p = a;
	int *q = b;
	if (*p > *q) return 0;
	else if (*p < *q) return 0;
	else return 1;
}
BiTNode* Creat(BiTNode *T, int i)
{
	if (i)
	{
		T = (BiTNode*)malloc(sizeof(BiTNode));
		apply_judge(T);
		T->elem = (void*)malloc(sizeof(int));
		apply_judge(T->elem);
		copy(T->elem, &i);
		T->lchild = NULL;
		T->rchild = NULL;
		T->lchild = Creat(T->lchild, i - 1);
		T->rchild = Creat(T->rchild, i - 1);
	}
	return T;
}