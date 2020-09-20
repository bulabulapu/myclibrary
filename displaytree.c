/*����������ʾ*/
/*GBK����*/
/*�������飺�����һ��������ʾ��*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef char TElemType;
typedef struct BiTNode
{
    TElemType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree; //������
typedef struct number
{
    int NUM;
    struct number *lchild, *rchild;
} NumTNode, *NumTree; //�������Ϊ����ʾ���еĽڵ�����,˳��Ϊ�������˳��
typedef struct array
{
    int **DArray;
    int pos;            //�����λ��
    int row, col;       //����������
} Array;                //�þ����ʾ��ʾ����λ��
TElemType *Data = NULL; //���ڴ洢�ڵ������

BiTree CreatBT(BiTree T, int i); //������

NumTree CreatNT(BiTree T, NumTree NT);       //���������
void DisplayBT(BiTree T);                    //��ӡ��
int Find_NUM(NumTree NT);                    //�����Ľڵ����
void FailApply();                            //�ڴ�����ʧ��
void PutDataIntoArray(BiTree T, NumTree NT); //�����ݴ���������
Array SurePosition(NumTree NT);              //ȷ�����ھ����е�λ��

int main()
{
    BiTree T = NULL;
    T = CreatBT(T, 5);
    DisplayBT(T);
    getchar();
    return 0;
}

BiTree CreatBT(BiTree T, int i)
{
    if (i > 0)
    {
        T = (BiTree)malloc(sizeof(BiTNode));
        if (!T)
            FailApply();
        T->data = (char)i + 48;
        T->lchild = NULL;
        T->rchild = NULL;
        T->lchild = CreatBT(T->lchild, i - 1);
        T->rchild = CreatBT(T->rchild, i - 2);
    }
    return T;
}

void DisplayBT(BiTree T)
{
    NumTree NT = NULL;
    Array arr;
    NT = CreatNT(T, NT);    //��������������Ӧ�����
    int num = Find_NUM(NT); //���ڵ�ĸ���
    int i, j;
    Data = (TElemType *)malloc((num + 1) * sizeof(TElemType));
    if (!Data)
        FailApply();
    PutDataIntoArray(T, NT); //�����ݴ���������
    arr = SurePosition(NT);  //�õ�λ�þ���
    if (arr.col < 64)
    {
        for (i = 0; i < arr.row; i++)
        {
            for (j = 0; j < arr.col; j++)
                switch (arr.DArray[i][j])
                {
                case 0:
                    printf("  ");
                    break;
                case -1:
                    printf("����");
                    break;
                case -2:
                    printf("��");
                    break;
                case -3:
                    printf(" ����");
                    break;
                case -4:
                    printf("����");
                    break;
                default:
                    printf("%2c", Data[arr.DArray[i][j]]);
                    break;
                }
            printf("\n");
        }
    }
    /*if (arr.col < 64)
	{
		for (i = 0; i < arr.row; i++)
		{
			for (j = 0; j < arr.col; j++)
				switch (arr.DArray[i][j])
				{
				case 0:printf(" "); break;
				case -1:printf("-"); break;
				case -2:printf("^"); break;
				case -3:printf("-"); break;
				case -4:printf("-"); break;
				default:printf("%c", Data[arr.DArray[i][j]]); break;
				}
			printf("\n");
		}
	}*/
    else
    {
        for (i = arr.col - 1; i >= 0; i--)
        {
            for (j = 0; j < arr.row; j++)
            {
                if (j < arr.row - 1)
                    if ((arr.DArray[j + 1][i] == -1) ||
                        (arr.DArray[j + 1][i] == -3) ||
                        (arr.DArray[j + 1][i] == -4))
                        printf(" ");
                switch (arr.DArray[j][i])
                {
                case 0:
                    printf("  ");
                    break;
                case -1:
                    printf("��");
                    break;
                case -2:
                    printf("��");
                    break;
                case -3:
                    printf("��");
                    break;
                case -4:
                    printf("��");
                    break;
                default:
                    printf("%3c", Data[arr.DArray[j][i]]);
                    break;
                }
            }
            printf("\n");
        }
    }
}
NumTree CreatNT(BiTree T, NumTree NT)
{
    static int i = 1;
    if (T)
    {
        NT = (NumTree)malloc(sizeof(NumTNode));
        if (!NT)
            FailApply();
        NT->NUM = i;
        NT->lchild = NULL;
        NT->rchild = NULL;
        i++;
        NT->lchild = CreatNT(T->lchild, NT->lchild);
        i++;
        NT->rchild = CreatNT(T->rchild, NT->rchild);
    }
    else
        i--;
    return NT;
}
int Find_NUM(NumTree NT)
{
    NumTree p = NT;
    if (!NT)
        exit(0);
    do
    {
        while (p->rchild)
            p = p->rchild;
        if (p->lchild)
            p = p->lchild;
    } while (p->lchild || p->rchild);
    return p->NUM;
}
void FailApply()
{
    printf("�ڴ�����ʧ��");
    exit(0);
}
void PutDataIntoArray(BiTree T, NumTree NT)
{
    if (T)
    {
        Data[NT->NUM] = T->data;
        PutDataIntoArray(T->lchild, NT->lchild);
        PutDataIntoArray(T->rchild, NT->rchild);
    }
}
Array SurePosition(NumTree NT)
{
    Array arr, arr_l, arr_r;
    int i, j, max;
    if (!NT)
    {
        arr.DArray = NULL;
        arr.pos = 0;
        arr.col = 0;
        arr.row = 0;
    }
    else
    {
        arr_l = SurePosition(NT->lchild);
        arr_r = SurePosition(NT->rchild);   //ȷ������������λ��
        if (!arr_l.DArray && !arr_r.DArray) //NTΪҶ��ʱ
        {
            arr.col = 1;
            arr.row = 1;
            arr.pos = 1;
            arr.DArray = (int **)malloc(sizeof(int *));
            if (!arr.DArray)
                FailApply();
            arr.DArray[0] = (int *)malloc(sizeof(int));
            if (!arr.DArray[0])
                FailApply();
            arr.DArray[0][0] = NT->NUM;
            return arr;
        }
        arr.pos = arr_l.col + 1;
        arr.col = arr_l.col + arr_r.col + 1;
        max = arr_l.row >= arr_r.row ? arr_l.row : arr_r.row;
        arr.row = max + 2; //ȷ��arr�Ĵ�С
        arr.DArray = (int **)malloc(arr.row * sizeof(int *));
        if (!arr.DArray)
            FailApply();
        for (i = 0; i < arr.row; i++)
        {
            arr.DArray[i] = (int *)malloc(arr.col * sizeof(int));
            if (!arr.DArray[i])
                FailApply();
            memset(arr.DArray[i], 0, sizeof(int) * arr.col); //��0
        }
        for (i = 0; i < arr_l.row; i++) //���������
            for (j = 0; j < arr_l.col; j++)
                arr.DArray[i + 2][j] = arr_l.DArray[i][j];
        for (i = 0; i < arr_r.row; i++) //�����Ҿ���
            for (j = 0; j < arr_r.col; j++)
                arr.DArray[i + 2][arr.pos + j] = arr_r.DArray[i][j];
        arr.DArray[0][arr.pos - 1] = NT->NUM; //ȷ��NT->NUM��λ��
        for (i = arr_l.pos; i < arr.pos + arr_r.pos - 1; i++)
            arr.DArray[1][i] = -1;                   //-1��ʾ��
        arr.DArray[1][arr_l.pos - 1] = -3;           //-3��ʾ��
        arr.DArray[1][arr.pos + arr_r.pos - 1] = -4; //-4��ʾ��
        arr.DArray[1][arr.pos - 1] = -2;             //-2��ʾ��
    }
    return arr;
}
