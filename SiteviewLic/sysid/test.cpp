#include <openssl/rsa.h>
#include <openssl/pem.h>
#include<openssl/evp.h>
#include <string.h>
EVP_PKEY* open();
EVP_PKEY* Popen();
void print(const char *promptStr,unsigned char * data,int len);
void generate_key();
void main(){

	EVP_PKEY* pkey=NULL,*pkey1=NULL;
	generate_key();
	pkey1=Popen();
	if(pkey1==NULL)
	{
		printf("p����ʧ��\n");
		exit(-1);
	}
	pkey=open();
	if(pkey==NULL)
	{
		exit(-1);
	}
	int len=-1;
	char plainText[]="[For test to public/private key encryption/decryption]";
	char encData[512];
	char decData[512];
	OpenSSL_add_all_ciphers();
	len=EVP_PKEY_encrypt(
		(unsigned char *)encData,
		(const unsigned char *)plainText,
		sizeof(plainText),
		pkey1
		);
	if(len==-1)
	{
		printf("EVP_PKEY_encrypt����ʧ��\n");
		exit(-1);
	}
	print("���ܺ������",(unsigned char *)encData,len);
	len=EVP_PKEY_decrypt(
		(unsigned char *)decData,
		(const unsigned char *)encData,
		len,
		pkey
		);
	if(len==-1)
	{
		printf("EVP_PKEY_decrypt����ʧ��\n");
		exit(-1);
	}
	print("���ܺ������",(unsigned char *)decData,len);
	printf("\n������:[����=%d�ֽ�]:%s\n",len,decData);
	EVP_PKEY_free(pkey);
}
void generate_key()
{
	RSA *rsa=NULL;
	printf("���ڲ���RSA��Կ...\n");
	rsa = RSA_generate_key(1024, RSA_F4, NULL, NULL);
	if(rsa==NULL)
	{
		printf("gen rsa error\n");
		exit(-1);
	}
	// ��Կ
	BIO *bp = BIO_new(BIO_s_file());
	if(BIO_write_filename(bp, "public.pem")<=0)
	{
		printf("write error\n");
		exit(-1);
	}
	if(PEM_write_bio_RSAPublicKey(bp, rsa)!=1)
	{
		printf("write public key error\n");
		exit(-1);
	}
	printf("���湫Կ�ɹ�\n");
	BIO_free_all(bp);

	char passwd[]="1234";
	// ˽Կ
	bp = BIO_new_file("private.pem", "w+");
	if(PEM_write_bio_RSAPrivateKey(bp, rsa, EVP_des_ede3(), (unsigned char*)passwd, 4, NULL, NULL)!=1)
	{
		printf("write public key error\n");
		exit(-1);
	}
	printf("����˽Կ�ɹ�\n");
	BIO_free_all(bp);

}
EVP_PKEY* open()
{
	EVP_PKEY* pkey=NULL;
	RSA *rsaK;
	OpenSSL_add_all_algorithms();
	BIO *BP=BIO_new(BIO_s_file());;
	char FileName[]="private.pem";
	BIO_read_filename(BP,FileName);
	char PSW[]="1234";
	rsaK=PEM_read_bio_RSAPrivateKey(BP,NULL,NULL,PSW);
	if(rsaK==NULL)
	{
		printf("��ȡʧ��!\n");
		BIO_free(BP);
		RSA_free(rsaK);
		exit(-1);
	}
	printf("˽Կ��ȡ�ɹ�\n");
	pkey=EVP_PKEY_new();
	if(NULL==pkey)
	{
		printf("EVP_PKEY_new failed\n");
		RSA_free(rsaK);
		return NULL;
	}
	EVP_PKEY_assign_RSA(pkey,rsaK);
	return pkey;
}

EVP_PKEY* Popen()
{
	EVP_PKEY* pkey=NULL;
	RSA *rsaK;
	OpenSSL_add_all_algorithms();
	BIO *BP=BIO_new(BIO_s_file());;
	char FileName[]="public.pem";
	BIO_read_filename(BP,FileName);
	rsaK=PEM_read_bio_RSAPublicKey(BP,NULL,NULL,NULL);
	if(rsaK==NULL)
	{
		printf("��ȡʧ��!\n");
		BIO_free(BP);
		RSA_free(rsaK);
		exit(-1);
	}
	printf("˽Կ��ȡ�ɹ�\n");
	pkey=EVP_PKEY_new();
	if(NULL==pkey)
	{
		printf("EVP_PKEY_new failed\n");
		RSA_free(rsaK);
		return NULL;
	}
	EVP_PKEY_assign_RSA(pkey,rsaK);
	return pkey;
}

void print(const char *promptStr,unsigned char * data,int len)
{
	int i;
	printf("\n===%s[����=%d�ֽ�]======\n",promptStr,len);
	for(i=0;i<len;i++)printf("%02x",data[i]);
	printf("\n======================================\n");
}