#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>

#include<conio.h>
#include<windows.h>

#pragma warning(disable:4996) //使用fopen条件

#define N 26
#define M 2*N-1 //总结点个数
#define MAX 1000
#define MAXSIZE 100
typedef struct
{
	char ch;
	int weight, rchild, lchild, parent;
}hfm;
typedef struct
{
	char bits[N];   //位串
	int start;   //位串中编码起始位置
	char ch;
}codetype;
typedef struct
{
	char ch;
	int  weight;
}weighttype;
int Read_in(weighttype weight[N])
{
	int i, j, k = 0, n = 0, m;
	char ch, cd[MAX];//cd[MAX]中间数组
	FILE* fp;
	if ((fp = fopen("hfm.txt","r")) == NULL)
	{
		printf("Error!");
		exit(0);
	}
	while ((ch = fgetc(fp)) != EOF)  //EOF文件结束符：-1
		cd[n++] = ch;
	for (i = 0; i < n; i++)
	{
		m = 0;
		if (cd[i] == '$') continue;
		weight[k].ch = cd[i];
		for (j = i; j < n; j++)
		{
			if (cd[j] == weight[k].ch)
			{
				m++;
				cd[j] = '$';
			}
		}
		weight[k].weight = m;
		k++;
	}
	fclose(fp);
}
void Create_hfm(hfm HFM[], weighttype weight[N])
{
	char c;
	int i, j, f, p1, p2, s1, s2;
	for (i = 0; i < M; i++)
	{
		HFM[i].weight = 0;
		HFM[i].rchild = -1;
		HFM[i].lchild = -1;
		HFM[i].parent = 0;
		HFM[i].ch = '^';
	}
	for (i = 0; i < N; i++)
	{
		if (weight[i].weight != 0)
		{
			HFM[i].ch = weight[i].ch;
			HFM[i].weight = weight[i].weight;
		}
	}
	for (i = N; i < M; i++)
	{
		p1 = 0; p2 = 0;		s1 = s2 = MAX;
		for (j = 0; j < i; j++)
		{
			if (HFM[j].parent == 0)
				if (HFM[j].weight < s1)      //找权重最小与次小的节点
				{
					s1 = HFM[j].weight;
					p1 = j;
				}
				else if (HFM[j].weight < s2)
				{
					s2 = HFM[j].weight;
					p2 = j;                
				}
		}
		HFM[i].weight = HFM[p1].weight + HFM[p2].weight;   
		HFM[i].lchild = p1;       HFM[i].rchild = p2;      	
		HFM[p1].parent = i;   	HFM[p2].parent = i;      
	}
}
void hfmcode(codetype code[], hfm HFM[])
{
	int i, j, c, p;
	codetype cd;  //中间变量
	char ch;
	FILE* fp1, * fp2;
	for (i = 0; i < N; i++)
	{
		cd.start = N;
		cd.ch = HFM[i].ch;
		c = i;               
		p = HFM[i].parent;  
		while (p != 0)
		{
			cd.start--;
			if (HFM[p].lchild == c) 		
				cd.bits[cd.start] = '0';   //HFM[i]是左子树，生成代码'0'
			else                        
				cd.bits[cd.start] = '1';   //HFM[i]是右子树，生成代码'1'
			c = p;
			p = HFM[p].parent;
		}
		code[i] = cd;      //第i+1个字符的编码存入code[i]
	}
	printf("输出每个字符的哈夫曼编码:\n");
	for (i = 0; i < N; i++)
	{
		if (HFM[i].weight != 0)
		{
			printf("%c: ", code[i].ch);
			for (j = code[i].start; j < N; j++)
			{
				printf("%c ", code[i].bits[j]);
			}
			printf("\n");
		}
	}
	if ((fp1 = fopen("hfm.txt", "r")) == NULL)
	{
		printf("not open");
		exit(0);
	}
	if ((fp2 = fopen("hfmcode.txt", "wt")) == NULL)
	{
		printf("Error!");
		//_getch();
		exit(0);
	}
	while ((ch = fgetc(fp1)) != EOF)
	{
		for (i = 0; i < N; i++)
		{
			if (ch == code[i].ch)
			{
				for (j = code[i].start; j < N; j++)
				{
					fprintf(fp2, "%c", code[i].bits[j]); //向所建文件写一字符串	                 		  
				}
			}
		}
	}
	fprintf(fp2, "%d", 2);
	printf("Save Successfully!\n");
	fclose(fp2);
	fclose(fp1);
}
void Decode(hfm HFM[M])   //根据哈夫曼树译码
{
	int i, j = 0, s, g = 0;
	FILE* fp1, * fp2;
	char b[MAX];
	i = M - 1;            //从根结点开始往下搜索
	if ((fp1 = fopen("hfmcode.txt", "r")) == NULL)
	{
		printf("Read Error!");
		exit(0);
	}
	if ((fp2 = fopen("hfmdecode.txt", "wt")) == NULL)
	{
		printf("Write Error!");
		exit(0);
	}
	while ((s = fgetc(fp1)) != EOF)
	{
		b[g++] = s;
	}
	printf("译码后的字符为：\n");
	while (b[j] != '2')
	{
		if (b[j] == '0')
			i = HFM[i].lchild;   //走向左孩子
		else
			i = HFM[i].rchild;   //走向右孩子
		if (HFM[i].lchild == -1)  //tree[i]是叶结点
		{
			fprintf(fp2, "%c", HFM[i].ch);
			printf("%c", HFM[i].ch);
			i = M - 1;            //回到根结点
		}
		j++;
	}
	printf("\nSave Successfully!\n");
	fclose(fp2);
	fclose(fp1);
	//_getch();
}
void Save_in()//创建文件
{
	char ch='0';
	FILE* fp;
	if ((fp = fopen("hfm.txt", "wt")) == NULL)
	{
		printf("Error!");
		//_getch();
		exit(0);
	}
	printf("请输入原始文档(以^结束)：\n");
	while (ch != '^')
	{
		ch = getchar();
		fputc(ch, fp); //向所建文件写一字符串  
	}
	printf("Save Successfully!\n");
	fclose(fp);
	//_getch();
}
void Print(hfm HFM[MAX])
{
	int i;
	printf("字符\t权重\t左子树\t右子树\t父节点双亲\n");
	for (i = 0; i < M; i++)
	{
		//if(HFM[i].ch=='^') continue;
		if (HFM[i].ch != '^')
		{
			printf("%c\t%d\t%d\t%d\t%d\n", HFM[i].ch, HFM[i].weight, HFM[i].lchild, HFM[i].rchild, HFM[i].parent);
		}
	}
}
void Printcode()
{
	FILE* fp;
	char ch;
	if ((fp = fopen("hfmcode.txt", "r")) == NULL)
	{
		printf("Read Error!");
		//_getch();	
		exit(0);
	}
	while ((ch = fgetc(fp)) != EOF)
	{
		if ((ch - 48) == 2) break;
		printf("%d", ch - 48);
	}
	fclose(fp);
	//_getch();
}

int main()
{
	int i;
	weighttype weight[N] = { 0 };
	hfm HFM[MAX];
	codetype code[N]; //存放哈弗曼编码
	void Save_in();
	Read_in(weight); //获取各个字符的出现的频率
	Create_hfm(HFM, weight);

	Read_in(weight); //获取各个字符的出现的频率
	Create_hfm(HFM, weight);
	hfmcode(code, HFM);//编码

	Print(HFM);//输出哈弗曼树

	Read_in(weight);
	Create_hfm(HFM, weight);
	Printcode();

	Read_in(weight); 
	Create_hfm(HFM, weight);
	Decode(HFM);//解码 

	return 0;

}
