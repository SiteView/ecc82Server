/*****************RSA Code******************/

#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
void print(const char *promptStr,unsigned char *data,int len)
{
	int i;
	printf("\n===%s[����=%d�ֽ�]======\n",promptStr,len);
	for(i = 0; i < len; i++) printf("%02x", data[i]);
	printf("\n=======================\n");
}
#define MODULUS "C8FBCF21"
#define PUBLIC_EXPONENT RSA_F4
#define PRIVATE_EXPONENT "97B55D7D"

//��Ϊһ������ʹ�þ�̬���ã�
static void prime_generate_status(int code, int arg, void *cb_arg)
{
	if( arg>0 && (arg%10) )
	{
		return;
	}
	if (code == 0)
		printf("\n�ҵ�Ǳ������: %d", (arg + 1));
	else if (code != 1)
		printf("\n�ɹ���ȡһ������!");
}

//����ɹ����ذ�װ��RSA������EVP_PKEY�����򷵻�NULL
EVP_PKEY* getRSA()
{
	EVP_PKEY* pkey=NULL;
	RSA* rsa=RSA_generate_key(1024,//��Կģ��
		RSA_3,                 //�������Ѷ�������Ϊ��Կ�е�e
		prime_generate_status, //��������״̬�Ļص�����
		NULL                   //�����ص������Ĳ���
		);
	if(NULL==rsa)
	{
		printf("����RSA��Կ��ʧ��\n");
		return NULL;
	}
	//����RSA��Կ������ʱ����
	RSA_blinding_on(rsa,NULL);
	printf("\n�ɹ�����RSA��Կ��\n");

	pkey=EVP_PKEY_new();
	if(NULL==pkey)
	{
		printf("EVP_PKEY_new failed\n");
		RSA_free(rsa);
		return NULL;
	}
	//��rsa���󸳸�EVP_PKEY�ṹ
	EVP_PKEY_assign_RSA(pkey,rsa);
	return pkey;
}

void main(int argc, char *argv[])
{
	RSA* rsa=NULL;
	EVP_PKEY* pkey=NULL;
	int len=-1;
	//Ҫ���ܵ�����
	unsigned char plainText[]="[For test to public/private key encryption/decryption]";
	unsigned char encData[512];//���ܺ������
	unsigned char decData[512];//���ܺ�����ݣ�Ӧ�ú�������ͬ

	OpenSSL_add_all_ciphers();

	pkey=getRSA();
	if(pkey==NULL)
	{
		return;
	}
   printf("okok   \n");
	len=EVP_PKEY_encrypt(
		encData,            //���ܺ������
		plainText,          //����
		sizeof(plainText), //���ĳ���
		pkey                //��Կ
		);
	if(len==-1)
	{
		printf("EVP_PKEY_encrypt����ʧ��\n");
		exit(-1);
	}

	print("���ܺ������",encData,len);

	//����
	len=EVP_PKEY_decrypt(
		decData,    //���ܺ������
		encData,    //����
		len,        //���ĳ���
		pkey        //˽Կ(��ʵ�ϣ���˽Կ���ڴ����ݽṹ��)
		);
	if(len==-1)
	{
		printf("EVP_PKEY_decrypt����ʧ��\n");
		return;
	}

	print("���ܺ������",decData,len);
	printf("\n������:[����=%d�ֽ�]:%s\n",len,decData);
	print("����",plainText,sizeof(plainText));
	//�ͷ�EVP_PKEY�����������RSA����Ҳ��ͬʱ�ͷš�
	EVP_PKEY_free(pkey);
	getchar();
}