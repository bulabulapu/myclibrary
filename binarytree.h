/*二叉树*/
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

#ifndef BINARYTREE
#define BINARYTREE

#include <malloc.h>
#include "stack.h"
#include "queue.h"

typedef struct       //顺序存储结构（空间浪费严重，时间少，适用于完全二叉树和满二叉树）
{
	void **elem;     //头结点(第0位不存储)
	int bitreesize;    //当前申请大小（第0位不存储数据）
	int *use_node;   //树的结点存在标志,1表示结点存在（第0位存储当前结点数量）
}BiTree_St;
typedef struct BiTNode      //链式存储结构结点
{
	void *elem;			   //结点元素
	struct BiTNode *lchild, *rchild;  //左右孩子指针
}BiTNode;
typedef struct  //链式存储结构
{
	BiTNode *root;   //根结点(存储数据)
}BiTree_Ct;
typedef struct
{
	union
	{
		BiTree_St ST;
		BiTree_Ct CT;
	}BITREE_KIND;            //二叉树的存储结构
	int BITREE_KIND_FLAG;	 //二叉树的存储结构标识,0:顺序,1:链式
	int ELEM_SIZE;			 //二叉树的元素所占字节大小
}BiTree;

BiTree InitBiTree(void *elem_type, int BITREE_KIND_, int ELEM_SIZE_)  //elem_type为元素类型指针,BITREE_KIND_为二叉树的存储结构标识,ELEM_SIZE_为二叉树的元素所占字节大小，返回一个二叉树T
//构造一个空二叉树
{
	BiTree T;
	T.BITREE_KIND_FLAG = BITREE_KIND_;  //确定二叉树存储结构
	T.ELEM_SIZE = ELEM_SIZE_;           //确定二叉树的元素所占字节大小
	switch (BITREE_KIND_)
	{
	case 0:
		T.BITREE_KIND.ST.elem = &elem_type;
		T.BITREE_KIND.ST.elem = (void**)malloc(sizeof(void*));
		apply_judge(T.BITREE_KIND.ST.elem);
		*T.BITREE_KIND.ST.elem = (void*)malloc(T.ELEM_SIZE);
		apply_judge(*T.BITREE_KIND.ST.elem);
		T.BITREE_KIND.ST.bitreesize = 1;
		T.BITREE_KIND.ST.use_node = (int*)malloc(sizeof(int));
		apply_judge(T.BITREE_KIND.ST.use_node);
		T.BITREE_KIND.ST.use_node[0] = 0;
		break;
	case 1:
		T.BITREE_KIND.CT.root = (BiTNode*)malloc(sizeof(BiTNode));
		apply_judge(T.BITREE_KIND.CT.root);
		T.BITREE_KIND.CT.root->elem = (void*)malloc(T.ELEM_SIZE);
		apply_judge(T.BITREE_KIND.CT.root->elem);
		T.BITREE_KIND.CT.root->lchild = NULL;
		T.BITREE_KIND.CT.root->rchild = NULL;
		break;
	default:
		printf("类别错误，构造失败\n");
		break;
	}
	return T;
}

int BiTreeExist(BiTree T)
//二叉树T存在返回1
{
	if (T.BITREE_KIND_FLAG != 0 && T.BITREE_KIND_FLAG != 1)
		return 0;
	return 1;
}

void DestoryBiTNode(BiTNode **p)
//销毁结点p(链式存储)
{
	if (*p)
	{
		DestoryBiTNode(&(*p)->lchild);
		DestoryBiTNode(&(*p)->rchild);
		(*p)->lchild = (*p)->rchild = NULL;
		free((*p)->elem);
		free(*p);
		*p = NULL;
	}
}

void DestoryBiTree(BiTree *T)
//销毁二叉树T
{
	BiTNode *p = NULL, *pr = NULL;
	void **pp = NULL, **ppr = NULL;
	int i;
	if (BiTreeExist(*T))
	{
		switch (T->BITREE_KIND_FLAG)
		{
		case 0:
			ppr = T->BITREE_KIND.ST.elem;
			pp = ppr;
			for (i = 0; i < T->BITREE_KIND.ST.bitreesize - 1; i++)   //释放二级指针
			{
				ppr = pp;
				*pp++;
				free(*ppr);
			}
			free(*pp);
			free(T->BITREE_KIND.ST.elem);             //释放一级指针
			free(T->BITREE_KIND.ST.use_node);
			T->BITREE_KIND.ST.bitreesize = 0;
			break;
		case 1:
			DestoryBiTNode(&T->BITREE_KIND.CT.root);
			T->BITREE_KIND.CT.root = NULL;
			break;
		}
		T->BITREE_KIND_FLAG = -1;
		printf("二叉树已销毁\n");
	}
	else printf("二叉树不存在\n");
}

void CreateBiTree(BiTree *T)
//构造二叉树
{
	char str[] = " 本库提供InsertChild函数，构造树还请自己动手，丰衣足食~(￣▽￣)~*";
	printf("%s\n", str);
}

void ClearBiTree(BiTree*T)
//清空二叉树
{
	int i;
	if (BiTreeExist(*T))
	{
		switch (T->BITREE_KIND_FLAG)
		{
		case 0:
			for (i = 0; i < T->BITREE_KIND.ST.bitreesize; i++)
				T->BITREE_KIND.ST.use_node[i] = 0;
			break;
		case 1:
			DestoryBiTNode(&T->BITREE_KIND.CT.root);
			T->BITREE_KIND.CT.root = NULL;
			break;
		}
		printf("二叉树已清空\n");
	}
	else printf("二叉树不存在\n");
}

int BiTreeEmpty(BiTree T)
//若T为空二叉树，返回1
{
	int n = 1;
	if (BiTreeExist(T))
	{
		switch (T.BITREE_KIND_FLAG)
		{
		case 0:
			if (T.BITREE_KIND.ST.use_node[0])
				n = 0;
			break;
		case 1:
			if (T.BITREE_KIND.CT.root)
				n = 0;
			break;
		}
	}
	else printf("二叉树不存在\n");
	return n;
}

int BiTNodeDepth(BiTNode *p)
//返回结点p的深度
{
	if (p)
	{
		if (BiTNodeDepth(p->lchild) > BiTNodeDepth(p->lchild))
			return BiTNodeDepth(p->lchild) + 1;
		else return BiTNodeDepth(p->rchild) + 1;
	}
	else return 0;
}

int BiTreeDepth(BiTree T)
//返回二叉树T的深度
{
	int d = 0, temp;
	if (!BiTreeEmpty(T))
	{
		switch (T.BITREE_KIND_FLAG)
		{
		case 0:
			for (temp = T.BITREE_KIND.ST.bitreesize; temp > 1; d++)
				temp /= 2;
			break;
		case 1:
			d = BiTNodeDepth(T.BITREE_KIND.CT.root);
			break;
		}
	}
	return d;
}

//void Root(BiTree T)返回二叉树的根，由树的结构体得到
//void Value(BiTree T,e)e是T中的某个结点,返回e的值(谷歌说e是位置序号)。看不懂看不懂_(:з)∠)_

int BiTNodeExist(BiTNode *p, void *e, int(*COMPARE)(void *, void*))
//如果p树中存在结点元素为e的结点，返回1，否则返回0 (链式)
{
	if (p)
	{
		if (COMPARE(p->elem, e) ||
			BiTNodeExist(p->lchild, e, COMPARE) ||
			BiTNodeExist(p->rchild, e, COMPARE))
			return 1;
	}
	return 0;
}

BiTNode* LocationBiTNodePare(BiTNode *p, void *e, int(*COMPARE)(void *, void*))
//COMPARE为比较函数，如果两个指针所指向的内存的值相等，则返回1
//返回结点元素为e的结点的父节点，若p结点就是该结点，则返回空  (链式)
{
	BiTNode *q = NULL;
	if (p)
	{
		if (COMPARE(e, p->elem)) q = NULL;                //当前结点元素比较
		else if (COMPARE(e, p->lchild->elem) || COMPARE(e, p->rchild->elem))
			q = p;
		else
		{
			q = LocationBiTNodePare(p->lchild, e, COMPARE);  //结点的左子树
			if (!q)
				q = LocationBiTNodePare(p->rchild, e, COMPARE);  //结点的右子树
		}
	}
	return q;
}

void Assign(BiTree *T, void *e, void *value, void(*COPY)(void*, void*), int(*COMPARE)(void *, void*))
//不能通过void*指针直接操作内存，因此用一个copy函数将后面指针指向的内存中的值复制到前面
//e是T的某个结点,结点e赋值为value
{
	int i;
	BiTNode *p = NULL;
	switch (T->BITREE_KIND_FLAG)
	{
	case 0:
		for (i = 1; i < T->BITREE_KIND.ST.bitreesize; i++)
			if (T->BITREE_KIND.ST.use_node[i] && COMPARE(e, T->BITREE_KIND.ST.elem[i]))
			{
				COPY(T->BITREE_KIND.ST.elem[i], value);
				break;
			}
		if (i == T->BITREE_KIND.ST.bitreesize) printf("结点不存在");
		break;
	case 1:
		if (BiTNodeExist(T->BITREE_KIND.CT.root, e, COMPARE))         //存在该结点
		{
			p = LocationBiTNodePare(T->BITREE_KIND.CT.root, e, COMPARE);
			if (!p) COPY(T->BITREE_KIND.CT.root->elem, value);
			else
			{
				if (p->lchild && COMPARE(e, p->lchild->elem))        //p的左孩子为该结点
					COPY(p->lchild->elem, value);
				else  COPY(p->rchild->elem, value);                  //p的右孩子为该结点
			}
		}
		else printf("结点不存在\n");
		break;
	}
	printf("赋值完成\n");
}

void Parent(BiTree T, void *e, void *par, void(*COPY)(void*, void*), int(*COMPARE)(void *, void*))
//e是T的结点。若是非根结点则用par返回它的双亲，否则返回空
{
	int i = 0;
	BiTNode *p = NULL;
	if (BiTreeExist(T))
	{
		switch (T.BITREE_KIND_FLAG)
		{
		case 0:
			if (T.BITREE_KIND.ST.use_node[1] && COMPARE(e, T.BITREE_KIND.ST.elem[1]))  //为根结点
				par = NULL;
			else
			{
				for (i = 2; i < T.BITREE_KIND.ST.bitreesize; i++)
					if (T.BITREE_KIND.ST.use_node[i] && COMPARE(e, T.BITREE_KIND.ST.elem[i]))
					{
						i /= 2;
						break;
					}
				if (i != T.BITREE_KIND.ST.bitreesize)
					COPY(par, T.BITREE_KIND.ST.elem[i]);
				else printf("结点不存在");
			}
			break;
		case 1:
			if (BiTNodeExist(T.BITREE_KIND.CT.root, e, COMPARE))         //存在该结点
				p = LocationBiTNodePare(T.BITREE_KIND.CT.root, e, COMPARE);
			else printf("结点不存在\n");
			if (p) COPY(par, p->elem);
			else par = NULL;
			break;
		}
	}
	else printf("二叉树不存在\n");
}

void LeftChild(BiTree T, void *e, void *left, void(*COPY)(void*, void*), int(*COMPARE)(void *, void*))
//e是T的结点。返回e的左孩子，不存在则返回空。
{
	int i = 0;
	BiTNode *p = NULL;
	if (BiTreeExist(T))
	{
		switch (T.BITREE_KIND_FLAG)
		{
		case 0:
			for (i = 1; i < T.BITREE_KIND.ST.bitreesize / 2; i++)
				if (T.BITREE_KIND.ST.use_node[i] && COMPARE(e, T.BITREE_KIND.ST.elem[i]))
					break;
			if (i < T.BITREE_KIND.ST.bitreesize / 2 && T.BITREE_KIND.ST.use_node[i * 2])
				COPY(left, T.BITREE_KIND.ST.elem[i * 2]);
			else left = NULL;
			break;
		case 1:
			if (BiTNodeExist(T.BITREE_KIND.CT.root, e, COMPARE))         //存在该结点
			{
				p = LocationBiTNodePare(T.BITREE_KIND.CT.root, e, COMPARE);
				if (p)
					if (p->lchild && COMPARE(p->lchild->elem, e))
						p = p->lchild;
					else p = p->rchild;
				else p = T.BITREE_KIND.CT.root;
				if (p->lchild)
					COPY(left, p->lchild->elem);
				else left = NULL;
			}
			else
			{
				left = NULL;
				printf("结点不存在\n");
			}
			break;
		}
	}
	else printf("二叉树不存在\n");
}

void RightChild(BiTree T, void *e, void *right, void(*COPY)(void*, void*), int(*COMPARE)(void *, void*))
//e是T的结点。返回e的右孩子，不存在则返回空。
{
	int i = 0;
	BiTNode *p = NULL;
	if (BiTreeExist(T))
	{
		switch (T.BITREE_KIND_FLAG)
		{
		case 0:
			for (i = 1; i < T.BITREE_KIND.ST.bitreesize / 2; i++)
				if (T.BITREE_KIND.ST.use_node[i] && COMPARE(e, T.BITREE_KIND.ST.elem[i]))
					break;
			if (i < T.BITREE_KIND.ST.bitreesize / 2 && T.BITREE_KIND.ST.use_node[i * 2 + 1])
				COPY(right, T.BITREE_KIND.ST.elem[i * 2 + 1]);
			else right = NULL;
			break;
		case 1:
			if (BiTNodeExist(T.BITREE_KIND.CT.root, e, COMPARE))         //存在该结点
			{
				p = LocationBiTNodePare(T.BITREE_KIND.CT.root, e, COMPARE);
				if (p)
					if (p->lchild && COMPARE(p->lchild->elem, e))
						p = p->lchild;
					else p = p->rchild;
				else p = T.BITREE_KIND.CT.root;
				if (p->rchild)
					COPY(right, p->rchild->elem);
				else right = NULL;
			}
			else
			{
				right = NULL;
				printf("结点不存在\n");
			}
			break;
		}
	}
	else printf("二叉树不存在\n");
}

void LeftSibling(BiTree T, void *e, void *left, void(*COPY)(void*, void*), int(*COMPARE)(void *, void*))
//e是T的结点。返回e的左兄弟，不存在或者e为左孩子则返回空。
{
	int i = 0;
	BiTNode *p = NULL;
	if (BiTreeExist(T))
	{
		switch (T.BITREE_KIND_FLAG)
		{
		case 0:
			if (T.BITREE_KIND.ST.use_node[1] && COMPARE(e, T.BITREE_KIND.ST.elem[1]))  //为根结点
				left = NULL;
			else
			{
				for (i = 2; i < T.BITREE_KIND.ST.bitreesize; i++)
					if (T.BITREE_KIND.ST.use_node[i] && COMPARE(e, T.BITREE_KIND.ST.elem[i]))
						break;
				if (i < T.BITREE_KIND.ST.bitreesize && i / 2 == 1 && T.BITREE_KIND.ST.use_node[i - 1])
					COPY(left, T.BITREE_KIND.ST.elem[i - 1]);
				else left = NULL;
			}
			break;
		case 1:
			if (BiTNodeExist(T.BITREE_KIND.CT.root, e, COMPARE))         //存在该结点
				p = LocationBiTNodePare(T.BITREE_KIND.CT.root, e, COMPARE);
			else printf("结点不存在\n");
			if (p && p->lchild)
				COPY(left, p->lchild->elem);
			else left = NULL;
			break;
		}
	}
	else printf("二叉树不存在\n");
}

void RightSibling(BiTree T, void *e, void *right, void(*COPY)(void*, void*), int(*COMPARE)(void *, void*))
//e是T的结点。返回e的右兄弟，不存在或者e为右孩子则返回空。
{
	int i = 0;
	BiTNode *p = NULL;
	if (BiTreeExist(T))
	{
		switch (T.BITREE_KIND_FLAG)
		{
		case 0:
			if (T.BITREE_KIND.ST.use_node[1] && COMPARE(e, T.BITREE_KIND.ST.elem[1]))  //为根结点
				right = NULL;
			else
			{
				for (i = 1; i < T.BITREE_KIND.ST.bitreesize; i++)
					if (T.BITREE_KIND.ST.use_node[i] && COMPARE(e, T.BITREE_KIND.ST.elem[i]))
						break;
				if (i < T.BITREE_KIND.ST.bitreesize && i / 2 == 0 && T.BITREE_KIND.ST.use_node[i + 1])
					COPY(right, T.BITREE_KIND.ST.elem[i + 1]);
				else right = NULL;
			}
			break;
		case 1:
			if (BiTNodeExist(T.BITREE_KIND.CT.root, e, COMPARE))         //存在该结点
				p = LocationBiTNodePare(T.BITREE_KIND.CT.root, e, COMPARE);
			else printf("结点不存在\n");
			if (p && p->rchild)
				COPY(right, p->rchild->elem);
			else right = NULL;
			break;
		}
	}
	else printf("二叉树不存在\n");
}

void ChangeNode(int flag, int n, BiTNode **e, void ***elem, int **use_node, int *size, int elemsize, void(*COPY)(void*, void*))
//递归调用，使e或elem的存储格式转换为对方的格式
//flag表示当前格式，n表示顺序存储的操作位置，e表示链式的操作结点(顺转链时默认*e为空),elem表示顺序存储的树的数组，use_node为顺序存储的结点标志,size为申请的数组大小,elem为元素字节大小
{
	int i;
	switch (flag)
	{
	case 0:    //顺序转换为链式
		*e = (BiTNode*)malloc(sizeof(BiTNode));
		apply_judge(*e);
		(*e)->elem = (void*)malloc(elemsize);
		apply_judge((*e)->elem);
		COPY((*e)->elem, elem[0][n]);
		(*e)->lchild = (*e)->rchild = NULL;
		if (n < *size / 2)
		{
			if (use_node[0][2 * n])       //左子树存在
				ChangeNode(flag, 2 * n, &(*e)->lchild, elem, use_node, size, elemsize, COPY);
			if (use_node[0][2 * n + 1])   //右子树存在
				ChangeNode(flag, 2 * n + 1, &(*e)->rchild, elem, use_node, size, elemsize, COPY);
		}
		break;
	case 1:    //链式转换为顺序
		if (n >= *size)    //顺序存储空间不够
		{
			i = *size;
			*size *= 2;
			*elem = (void**)realloc(*elem, *size * sizeof(void*));
			apply_judge(*elem);
			*use_node = (int*)realloc(*use_node, *size * sizeof(int));
			apply_judge(*use_node);
			for (; i < *size; i++)
			{
				elem[0][i] = (void*)malloc(elemsize);
				apply_judge(elem[0][i]);
				use_node[0][i] = 0;
			}
		}
		COPY(elem[0][n], (*e)->elem);
		use_node[0][0]++;
		use_node[0][n] = 1;
		if ((*e)->lchild || (*e)->rchild)       //子树存在
		{

			if ((*e)->lchild)
				ChangeNode(flag, 2 * n, &(*e)->lchild, elem, use_node, size, elemsize, COPY);
			if ((*e)->rchild)
				ChangeNode(flag, 2 * n + 1, &(*e)->rchild, elem, use_node, size, elemsize, COPY);
		}
		break;
	}
}

void StorageChange(BiTree *T, void(*COPY)(void*, void*))
//改变树的存储方式
{
	BiTree TT;
	switch (T->BITREE_KIND_FLAG)
	{
	case 0:
		TT = InitBiTree(*T->BITREE_KIND.ST.elem, 1, T->ELEM_SIZE);
		ChangeNode(T->BITREE_KIND_FLAG,
			1,
			&TT.BITREE_KIND.CT.root,
			&T->BITREE_KIND.ST.elem,
			&T->BITREE_KIND.ST.use_node,
			&T->BITREE_KIND.ST.bitreesize,
			T->ELEM_SIZE,
			COPY);
		printf("原");
		DestoryBiTree(T);
		*T = TT;
		break;
	case 1:
		TT = InitBiTree(*T->BITREE_KIND.ST.elem, 0, T->ELEM_SIZE);
		ChangeNode(T->BITREE_KIND_FLAG,
			1,
			&T->BITREE_KIND.CT.root,
			&TT.BITREE_KIND.ST.elem,
			&TT.BITREE_KIND.ST.use_node,
			&TT.BITREE_KIND.ST.bitreesize,
			T->ELEM_SIZE,
			COPY);
		printf("原");
		DestoryBiTree(T);
		*T = TT;
		break;
	}
}

void SeqCopy(int a, void ***elem_a, int **use_node_a, int *size_a, int b, void **elem_b, int *use_node_b, int size_b, int elemsize, void(*COPY)(void*, void*))
//均为顺序存储结构。将elem_b的第b个结点及其子树连接到elem_a的第a个结点，作为elem_a的子树,elem为元素大小
{
	int i = 0;
	if (a > *size_a)       //elem_a的存储空间不够时
	{
		i = *size_a;
		*size_a *= 2;
		*elem_a = (void**)realloc(*elem_a, *size_a * sizeof(void*));
		apply_judge(*elem_a);
		*use_node_a = (int*)realloc(*use_node_a, *size_a * sizeof(int));
		apply_judge(*use_node_a);
		for (; i < *size_a; i++)
		{
			elem_a[0][i] = (void*)malloc(elemsize);
			apply_judge(elem_a[0][i]);
			use_node_a[0][i] = 0;
		}
	}
	COPY(elem_a[0][a], elem_b[b]);
	use_node_a[0][a] = 1;
	use_node_a[0][0]++;
	use_node_b[b] = 0;
	use_node_b[0]--;
	if (b < size_b / 2)    //b存在子树
	{
		if (use_node_b[b * 2])   //b的左子树存在
			SeqCopy(a * 2, elem_a, use_node_a, size_a, b * 2, elem_b, use_node_b, size_b, elemsize, COPY);
		if (use_node_b[b * 2 + 1])   //b的右子树存在
			SeqCopy(a * 2 + 1, elem_a, use_node_a, size_a, b * 2 + 1, elem_b, use_node_b, size_b, elemsize, COPY);
	}
}

void InsertChild(BiTree *T, void *e, int TLR, BiTree *C, int CLR, void(*COPY)(void*, void*), int(*COMPARE)(void *, void*))
//二叉树T存在，e为T中的某个结点，LR为0或1，非空二叉树C与T不相交且左（CLR=0）或右(CLR=1)子树为空
//根据TLR为0或1插入C为T中e结点的左或右子树。e结点原有左或右子树则根据CLR为0或1成为C的左或右子树
{
	int i = 0;
	BiTNode *p = NULL, *q = NULL;
	if (BiTreeExist(*T) && BiTreeExist(*C))
	{
		if ((TLR == 1 || TLR == 0) && (CLR == 1 || CLR == 0))
		{
			switch (T->BITREE_KIND_FLAG)
			{
			case 0:
				if (C->BITREE_KIND_FLAG != T->BITREE_KIND_FLAG)   //使C为顺序存储结构
					StorageChange(C, COPY);
				for (i = 1; i < T->BITREE_KIND.ST.bitreesize; i++)
					if (T->BITREE_KIND.ST.use_node[i] && COMPARE(e, T->BITREE_KIND.ST.elem[i]))
						break;
				i = i * 2 + TLR;     //确定T的更改位置
				SeqCopy(2 + CLR, &C->BITREE_KIND.ST.elem, &C->BITREE_KIND.ST.use_node, &C->BITREE_KIND.ST.bitreesize,
					i, T->BITREE_KIND.ST.elem, T->BITREE_KIND.ST.use_node, T->BITREE_KIND.ST.bitreesize,
					C->ELEM_SIZE, COPY);    //将结点e的左（右）子树作为C的左（右）孩子
				SeqCopy(i, &T->BITREE_KIND.ST.elem, &T->BITREE_KIND.ST.use_node, &T->BITREE_KIND.ST.bitreesize,
					1, C->BITREE_KIND.ST.elem, C->BITREE_KIND.ST.use_node, C->BITREE_KIND.ST.bitreesize,
					C->ELEM_SIZE, COPY);    //将C接到T上
				break;
			case 1:
				if (C->BITREE_KIND_FLAG != T->BITREE_KIND_FLAG)  //使C为链式存储结构 
					StorageChange(C, COPY);
				if (BiTNodeExist(T->BITREE_KIND.CT.root, e, COMPARE))         //存在该结点
				{
					p = LocationBiTNodePare(T->BITREE_KIND.CT.root, e, COMPARE);
					if (p)
						if (p->lchild && COMPARE(p->lchild->elem, e))
							p = p->lchild;
						else p = p->rchild;
					else p = T->BITREE_KIND.CT.root;   //确定p为插入位置
					switch (TLR)
					{
					case 0:
						q = p->lchild;
						p->lchild = C->BITREE_KIND.CT.root;
						p = p->lchild;
						break;
					case 1:
						q = p->rchild;
						p->rchild = C->BITREE_KIND.CT.root;
						p = p->rchild;
						break;
					}
					switch (CLR)
					{
					case 0:
						p->lchild = q;
						break;
					case 1:
						p->rchild = q;
						break;
					}
					C->BITREE_KIND.CT.root = (BiTNode*)malloc(sizeof(BiTNode));  //使C与其子树断开，避免以后操作C时T也会改变
					if (C->BITREE_KIND.CT.root)  //申请成功
					{
						free(C->BITREE_KIND.CT.root);
						C->BITREE_KIND.CT.root = NULL;
					}
				}
				else printf("结点不存在\n");
				break;
			}
		}
		else printf("结点插入位置有误\n");
	}
	else printf("二叉树不存在\n");
}

void SeqDeleteBiTNode(int n, int *use_node, int size)
//删除第n个结点及其子树
{
	use_node[n] = 0;
	use_node[0]--;
	if (n < size / 2) //第n个结点存在子树
	{
		if (use_node[2 * n])  //存在左子树
			SeqDeleteBiTNode(2 * n, use_node, size);
		if (use_node[2 * n + 1])  //存在右子树
			SeqDeleteBiTNode(2 * n + 1, use_node, size);
	}
}

void DeleteChild(BiTree *T, void *e, int LR, int(*COMPARE)(void*, void*))
//二叉树T存在，e为T中的某个结点，LR为0或1，删除e结点的左或右子树
{
	int i;
	BiTNode *p = NULL,*q=NULL;
	if (BiTreeExist(*T))
	{
		if (LR == 1 || LR == 0)
		{
			switch (T->BITREE_KIND_FLAG)
			{
			case 0:
				for (i = 1; i < T->BITREE_KIND.ST.bitreesize; i++)
					if (T->BITREE_KIND.ST.use_node[i] && COMPARE(e, T->BITREE_KIND.ST.elem[i]))
						break;
				SeqDeleteBiTNode(i * 2 + LR, T->BITREE_KIND.ST.use_node, T->BITREE_KIND.ST.bitreesize);
				break;
			case 1:
				if (BiTNodeExist(T->BITREE_KIND.CT.root, e, COMPARE))         //存在该结点
				{
					p = LocationBiTNodePare(T->BITREE_KIND.CT.root, e, COMPARE);
					if (p)
						if (p->lchild && COMPARE(p->lchild->elem, e))
							p = p->lchild;
						else p = p->rchild;
					else p = T->BITREE_KIND.CT.root;   //确定e的位置
					switch (LR)
					{
					case 0:
						q = p;
						p = p->lchild;
						q->lchild = NULL;
						break;
					case 1:
						q = p;
						p = p->rchild;
						q->rchild = NULL;
						break;
					}
					DestoryBiTNode(&p);
				}
				else printf("结点不存在\n");
				break;
			}
		}
		else printf("结点删除位置有误\n");
	}
	else  printf("二叉树不存在\n");
}

void COPY_INT(void *a, void *b)
//int类型的copy函数
{
	int *p = (int*)a;  //此时需要强制转换，但在源文件中调用则不需要，不清楚（在另一个头文件中调用需要吧）
	int *q = (int*)b;
	*p = *q;
}

void COPY_BiTNode(void *a, void *b)
//BiTNode*类型的copy函数
{
	BiTNode **p = (BiTNode**)a;
	BiTNode **q = (BiTNode**)b;
	*p = *q;
}

void PreOrderTraverse(BiTree T, int(*VISIT)(void*))
//先序遍历T,对每个节点调用VISIT,一旦VISIST失败则操作失败
{
	int i = 1;
	int *t = NULL;
	BiTNode *p = NULL, **q = NULL;
	q = (BiTNode**)malloc(sizeof(BiTNode*));
	apply_judge(q);
	*q = p;
	Stack S;
	if (!BiTreeEmpty(T))
	{
		switch (T.BITREE_KIND_FLAG)
		{
		case 0:
			S = InitStack(&i, 0, sizeof(int));  //构建栈
			do
			{
				while (i < T.BITREE_KIND.ST.bitreesize && T.BITREE_KIND.ST.use_node[i])  //到达最左下方
				{
					if (!VISIT(T.BITREE_KIND.ST.elem[i]))
					{
						printf("遍历失败\n");
						return;
					}
					Push(&S, &i, COPY_INT);        //i结点入栈
					i = i * 2;
				}
				if (!StackEmpty(S))  //栈不为空
				{
					Pop(&S, (void**)&t, COPY_INT);
					i = *t;
					i = i * 2 + 1;  //访问右子树
				}
			} while ((i < T.BITREE_KIND.ST.bitreesize && T.BITREE_KIND.ST.use_node[i]) || !StackEmpty(S));
			DestoryStack(&S);
			break;
		case 1:
			S = InitStack(&p, 0, sizeof(BiTNode));  //构建栈，为BiTNode*类型
			p = T.BITREE_KIND.CT.root;
			do
			{
				while (p)  //到达最左下方
				{
					if (!VISIT(p->elem))
					{
						printf("遍历失败\n");
						return;
					}
					Push(&S, &p, COPY_BiTNode);
					p = p->lchild;
				}
				if (!StackEmpty(S))  //栈不为空
				{
					Pop(&S, (void**)&q, COPY_BiTNode);
					p = *q;
					p = p->rchild;    //访问p的右子树
				}
			} while (p || !StackEmpty(S));
			*q = NULL;
			DestoryStack(&S);
			break;
		}
	}
	free(q);
}

void InOrderTraverse(BiTree T, int(*VISIT)(void*))
//中序遍历T,对每个节点调用VISIT,一旦VISIST失败则操作失败
{
	int i = 1;
	int *t = NULL;
	BiTNode *p = NULL, **q = NULL;
	q = (BiTNode**)malloc(sizeof(BiTNode*));
	apply_judge(q);
	*q = p;
	Stack S;
	if (!BiTreeEmpty(T))
	{
		switch (T.BITREE_KIND_FLAG)
		{
		case 0:
			S = InitStack(&i, 0, sizeof(int));  //构建栈
			do
			{
				while (i < T.BITREE_KIND.ST.bitreesize && T.BITREE_KIND.ST.use_node[i])  //到达最左下方
				{
					Push(&S, &i, COPY_INT);    //i结点入栈
					i *= 2;           //访问左子树
				}
				if (!StackEmpty(S))  //栈不为空
				{
					Pop(&S, (void**)&t, COPY_INT);
					i = *t;
					if (!VISIT(T.BITREE_KIND.ST.elem[i]))
					{
						printf("遍历失败\n");
						return;
					}
					i = i * 2 + 1;//访问右子树
				}
			} while ((i < T.BITREE_KIND.ST.bitreesize && T.BITREE_KIND.ST.use_node[i]) || !StackEmpty(S));
			DestoryStack(&S);
			break;
		case 1:
			S = InitStack(&p, 0, sizeof(BiTNode));  //构建栈，为BiTNode*类型
			p = T.BITREE_KIND.CT.root;
			do
			{
				while (p)  //到达最左下方
				{
					Push(&S, &p, COPY_BiTNode);
					p = p->lchild;
				}
				if (!StackEmpty(S))  //栈不为空
				{
					Pop(&S, (void**)&q, COPY_BiTNode);
					p = *q;
					if (!VISIT(p->elem))
					{
						printf("遍历失败\n");
						return;
					}
					p = p->rchild;    //访问p的右子树
				}
			} while (p || !StackEmpty(S));
			*q = NULL;
			DestoryStack(&S);
			break;
		}
	}
	free(q);
}

void PostOrderTraverse(BiTree T, int(*VISIT)(void*))
//后序遍历T,对每个节点调用VISIT,一旦VISIST失败则操作失败（利用先序的思想进行逆序遍历）
{
	int i = 1;
	int *t = NULL;
	BiTNode *p = NULL, **q = NULL;
	q = (BiTNode**)malloc(sizeof(BiTNode*));
	apply_judge(q);
	*q = p;
	Stack elem_S;    //用于记录结点元素顺序
	Stack S;
	if (!BiTreeEmpty(T))
	{
		switch (T.BITREE_KIND_FLAG)
		{
		case 0:
			elem_S = InitStack(&i, 0, sizeof(int));
			S = InitStack(&i, 0, sizeof(int));  //构建栈
			do
			{
				while (i < T.BITREE_KIND.ST.bitreesize && T.BITREE_KIND.ST.use_node[i])  //到达最右下方
				{
					Push(&elem_S, &i, COPY_INT);  //结点入栈
					Push(&S, &i, COPY_INT);      //结点入栈
					i = i * 2 + 1;                        //继续遍历右子树
				}
				if (!StackEmpty(S))  //栈不为空
				{
					Pop(&S, (void**)&t, COPY_INT);
					i = *t;
					i *= 2;    //访问左子树
				}
			} while ((i < T.BITREE_KIND.ST.bitreesize && T.BITREE_KIND.ST.use_node[i]) || !StackEmpty(S));
			while (!StackEmpty(elem_S))    //输出遍历结果
			{
				Pop(&elem_S, (void**)&t, COPY_INT);
				i = *t;
				if (!VISIT(T.BITREE_KIND.ST.elem[i]))
				{
					printf("遍历失败\n");
					return;
				}
			}
			DestoryStack(&S);
			break;
		case 1:
			elem_S = InitStack(&p, 0, sizeof(BiTNode));
			S = InitStack(&p, 0, sizeof(BiTNode));  //构建栈，为BiTNode*类型
			p = T.BITREE_KIND.CT.root;
			do
			{
				while (p)  //到达最右下方
				{
					Push(&elem_S, &p, COPY_BiTNode);
					Push(&S, &p, COPY_BiTNode);
					p = p->rchild;
				}
				if (!StackEmpty(S))  //栈不为空
				{
					Pop(&S, (void**)&q, COPY_BiTNode);
					p = *q;
					p = p->lchild;    //访问p的左子树
				}
			} while (p || !StackEmpty(S));
			while (!StackEmpty(elem_S))    //输出遍历结果
			{
				Pop(&elem_S, (void**)&q, COPY_BiTNode);
				p = *q;
				if (!VISIT(p->elem))
				{
					printf("遍历失败\n");
					return;
				}
			}
			*q = NULL;
			DestoryStack(&S);
			break;
		}
	}
	DestoryStack(&elem_S);
	free(q);
}

void LevelOrderTraverse(BiTree T, int(*VISIT)(void*))
//层序遍历T,对每个节点调用VISIT,一旦VISIST失败则操作失败
{
	int i = 1;
	BiTNode *p = NULL, **q = NULL;
	q = (BiTNode**)malloc(sizeof(BiTNode*));
	apply_judge(q);
	*q = p;
	Queue Q;
	if (!BiTreeEmpty(T))
	{
		switch (T.BITREE_KIND_FLAG)
		{
		case 0:
			for (; i < T.BITREE_KIND.ST.bitreesize; i++)
				if (T.BITREE_KIND.ST.use_node[i])
					if (!VISIT(T.BITREE_KIND.ST.elem[i]))
					{
						printf("遍历失败\n");
						return;
					}
			break;
		case 1:
			Q = InitQueue(&p, 1, sizeof(BiTNode));  //构建队列，为BiTNode*类型
			p = T.BITREE_KIND.CT.root;
			EnQueue(&Q, &p, COPY_BiTNode);
			do
			{
				DeQueue(&Q, (void**)&q, COPY_BiTNode);
				p = *q;
				if (!VISIT(p->elem))
				{
					printf("遍历失败\n");
					return;
				}
				if (p->lchild)   //左子树入队
					EnQueue(&Q, &p->lchild, COPY_BiTNode);
				if (p->rchild)   //右子树入队
					EnQueue(&Q, &p->rchild, COPY_BiTNode);
			} while (!QueueEmpty(Q));
			*q = NULL;
			DestoryQueue(&Q);
			break;
		}
	}
	free(q);
}



void DisplayBiTree(BiTree T, int(*VISIT)(void*))
//显示二叉树T
{

}

#endif