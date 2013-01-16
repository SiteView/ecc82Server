#pragma once


/*@ file rsa.h
rsa with c,not c++
complied ok with minggw by chinayaosir
*/
#include<stdio.h>
#include<string.h>
#include <stdlib.h>   
#include <time.h>  
#include <math.h>
#include <malloc.h>

#define MAX 100
#define LEN sizeof(struct slink)
void sub(int a[MAX],int b[MAX] ,int c[MAX] );

struct slink
{ 
	int  bignum[MAX];/*bignum[98]用来标记正负号，1正，0负bignum[99]来标记实际长度*/	
    struct slink *next;
};

class CRsa
{
public:
	CRsa(void);
public:
	~CRsa(void);

public:
	void SetKey(const char* pkey, const char* skey);
	bool SetKey(const char* ekey, const char* dkey, const char* nkey);
	bool SetPKey(const char* ekey, const char* nkey);
	bool SetSKey(const char* dkey, const char* nkey);
	bool Encrypt(const char* src, char* res);
	bool Decrypt(const char* src, char* res);

private:
	void rsad(int e[MAX],int g[MAX],int *d);
	void  erand(int e[MAX],int m[MAX]);
	void prime_random(int *p,int *q);
	 /* 求两个大数之间是否互质*/
	int coprime(int e[MAX],int s[MAX]);
	int  is_prime_san(int p[MAX] );
	/*计算 m=a^p  mod n*/
	void expmod(int a[MAX] ,int p[MAX] ,int n[MAX],int *m);
	/*计算m=a*b mod n*/
	void mulmod(int a[MAX] ,int b[MAX] ,int n[MAX],int *m);
	/*//试商法//调用以后w为a mod b, C为a  div b;*/
	void  divt(int t[MAX],int b[MAX],int  *c ,int *w);
	/* c=a mod b, 注意：经检验知道此处A和C的数组都改变了。*/
	void  mod(int a[MAX],int b[MAX],int  *c);
	void sub(int a1[MAX],int a2[MAX],int *c);
	void add(int a1[MAX],int a2[MAX],int *c);
	void mul(int a1[MAX],int a2[MAX],int *c);
	int  cmp(int a1[MAX],int a2[MAX]);
	void mov(int a[MAX],int *b);

private:
	int d[MAX];
	int e[MAX];
	int n[MAX];
};
