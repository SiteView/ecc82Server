/*****************RSA Code******************/

#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
void print(const char *promptStr,unsigned char *data,int len)
{
	int i;
	printf("\n===%s[长度=%d字节]======\n",promptStr,len);
	for(i = 0; i < len; i++) printf("%02x", data[i]);
	printf("\n=======================\n");
}
#define MODULUS "C8FBCF21"
#define PUBLIC_EXPONENT RSA_F4
#define PRIVATE_EXPONENT "97B55D7D"

//作为一条规则，使用静态调用，
static void prime_generate_status(int code, int arg, void *cb_arg)
{
	if( arg>0 && (arg%10) )
	{
		return;
	}
	if (code == 0)
		printf("\n找到潜在素数: %d", (arg + 1));
	else if (code != 1)
		printf("\n成功获取一个素数!");
}

//如果成功返回包装了RSA参数的EVP_PKEY，否则返回NULL
EVP_PKEY* getRSA()
{
	EVP_PKEY* pkey=NULL;
	RSA* rsa=RSA_generate_key(1024,//公钥模长
		RSA_3,                 //第三个费尔玛数作为公钥中的e
		prime_generate_status, //素数产生状态的回调函数
		NULL                   //传给回调函数的参数
		);
	if(NULL==rsa)
	{
		printf("生成RSA密钥对失败\n");
		return NULL;
	}
	//隐藏RSA密钥抵御定时攻击
	RSA_blinding_on(rsa,NULL);
	printf("\n成功生成RSA密钥对\n");

	pkey=EVP_PKEY_new();
	if(NULL==pkey)
	{
		printf("EVP_PKEY_new failed\n");
		RSA_free(rsa);
		return NULL;
	}
	//将rsa对象赋给EVP_PKEY结构
	EVP_PKEY_assign_RSA(pkey,rsa);
	return pkey;
}

void main(int argc, char *argv[])
{
	RSA* rsa=NULL;
	EVP_PKEY* pkey=NULL;
	int len=-1;
	//要加密的明文
	unsigned char plainText[]="[For test to public/private key encryption/decryption]";
	unsigned char encData[512];//加密后的数据
	unsigned char decData[512];//解密后的数据，应该和明文相同

	OpenSSL_add_all_ciphers();

	pkey=getRSA();
	if(pkey==NULL)
	{
		return;
	}
   printf("okok   \n");
	len=EVP_PKEY_encrypt(
		encData,            //加密后的数据
		plainText,          //明文
		sizeof(plainText), //明文长度
		pkey                //公钥
		);
	if(len==-1)
	{
		printf("EVP_PKEY_encrypt加密失败\n");
		exit(-1);
	}

	print("加密后的数据",encData,len);

	//解密
	len=EVP_PKEY_decrypt(
		decData,    //解密后的数据
		encData,    //密文
		len,        //密文长度
		pkey        //私钥(事实上，公私钥都在此数据结构中)
		);
	if(len==-1)
	{
		printf("EVP_PKEY_decrypt解密失败\n");
		return;
	}

	print("解密后的数据",decData,len);
	printf("\n明文是:[长度=%d字节]:%s\n",len,decData);
	print("明文",plainText,sizeof(plainText));
	//释放EVP_PKEY对象，其关联的RSA对象也被同时释放。
	EVP_PKEY_free(pkey);
	getchar();
}