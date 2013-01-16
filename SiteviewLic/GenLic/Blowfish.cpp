// Blowfish.cpp: implementation of the Blowfish class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Blowfish.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlowfish::~CBlowfish()
{

}

CBlowfish::CBlowfish(unsigned char* key,int Length)
{
	int nI;

	
	for (nI = 0; nI < PBOX_ENTRIES; nI++)
	{
		m_pbox[nI] = pbox_init[nI];
	}

	
	for (nI = 0; nI < SBOX_ENTRIES; nI++) 
	{
		m_sbox1[nI] = sbox_init_1[nI];
		m_sbox2[nI] = sbox_init_2[nI];
		m_sbox3[nI] = sbox_init_3[nI];
		m_sbox4[nI] = sbox_init_4[nI];
	}
	
	// xor the key over the p-boxes
	
	int nLen = Length;
	if (nLen == 0) return; 
	int nKeyPos = 0;
	unsigned int unBuild = 0;
	
	for (nI = 0; nI < PBOX_ENTRIES; nI++)
	{
		for (int nJ = 0; nJ < 4; nJ++) 
		{
			unBuild = (unBuild << 8) | key[nKeyPos];
			
			if (++nKeyPos == nLen) 
			{ 
				nKeyPos = 0;
			}
		}
		m_pbox[nI] ^= unBuild;
	}
	
	
	// encrypt all boxes with the all zero string
	unsigned int unZeroHi = 0;
	unsigned int unZeroLo = 0;
	
	for (nI = 0; nI < PBOX_ENTRIES; nI += 2) 
	{
		BaseEncrypt(&unZeroHi, &unZeroLo);
		m_pbox[nI] = unZeroHi;
		m_pbox[nI + 1] = unZeroLo;
	}
	for (nI = 0; nI < SBOX_ENTRIES; nI += 2) 
	{
		BaseEncrypt(&unZeroHi, &unZeroLo);
		m_sbox1[nI] = unZeroHi;
		m_sbox1[nI + 1] = unZeroLo;
	}
	for (nI = 0; nI < SBOX_ENTRIES; nI += 2) 
	{
		BaseEncrypt(&unZeroHi, &unZeroLo);
		m_sbox2[nI] = unZeroHi;
		m_sbox2[nI + 1] = unZeroLo;
	}
	for (nI = 0; nI < SBOX_ENTRIES; nI += 2) 
	{
		BaseEncrypt(&unZeroHi, &unZeroLo);
		m_sbox3[nI] = unZeroHi;
		m_sbox3[nI + 1] = unZeroLo;
	}
	for (nI = 0; nI < SBOX_ENTRIES; nI += 2) 
	{
		BaseEncrypt(&unZeroHi, &unZeroLo);
		m_sbox4[nI] = unZeroHi;
		m_sbox4[nI + 1] = unZeroLo;
	}
	
	m_nIsWeakKey = -1;
}


BOOL CBlowfish::IsWeakKey()
{
	if (-1 == m_nIsWeakKey)
	{
		m_nIsWeakKey = 0;
		
		int nI, nJ;
		for (nI = 0; nI < SBOX_ENTRIES - 1; nI++) 
		{
			nJ = nI + 1;
			while (nJ < SBOX_ENTRIES) 
			{
				if ((m_sbox1[nI] == m_sbox1[nJ]) |
					(m_sbox2[nI] == m_sbox2[nJ]) | 
					(m_sbox3[nI] == m_sbox3[nJ]) |
					(m_sbox4[nI] == m_sbox4[nJ])) break;
				else nJ++;
			}
			if (nJ < SBOX_ENTRIES)
			{
				m_nIsWeakKey = 1;
				break;
			}
		}
	}	
	return (1 == m_nIsWeakKey);
}
void CBlowfish::Encrypt(unsigned int *unHiRef, unsigned int *unLoRef)
{
		BaseEncrypt(unHiRef,unLoRef);
}

void CBlowfish::BaseEncrypt(unsigned int *unHiRef, unsigned int *unLoRef)
{
			unsigned int unHi = *unHiRef; 
			unsigned int  unLo = *unLoRef; 

			// and use local references, too

			unsigned int  *sbox1 = m_sbox1;
			unsigned int  *sbox2 = m_sbox2;
			unsigned int  *sbox3 = m_sbox3;
			unsigned int  *sbox4 = m_sbox4;

			unsigned int *pbox = m_pbox;

			// encrypt the block, unrolled loop and odd/even changes to maximize the speed

			unHi ^= pbox[0];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[1];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[2];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[3];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[4];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[5];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[6];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[7];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[8];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[9];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[10];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[11];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[12];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[13];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[14];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[15];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[16];

			// finalize and copy back

			*unLoRef = unHi;
			*unHiRef = unLo ^ pbox[17];
}

int CBlowfish::Encrypt(unsigned char *dataIn,unsigned char * dataOut,int nPosIn,int nPosOut,int nCount)
{
			// count in blocks
			nCount >>= 3;

			for (int nI = 0; nI < nCount; nI++)
			{
				// load the bytes into two 32bit words, which together represent the block

				unsigned int unHi = (((unsigned int) dataIn[nPosIn + 3]) << 24) |
					(((unsigned int) dataIn[nPosIn + 2]) << 16) |
					(((unsigned int) dataIn[nPosIn + 1]) << 8) |
					dataIn[nPosIn];
 
				unsigned int unLo = (((unsigned int) dataIn[nPosIn + 7]) << 24) |
					(((unsigned int) dataIn[nPosIn + 6]) << 16) |
					(((unsigned int) dataIn[nPosIn + 5]) << 8) |
					dataIn[nPosIn + 4];

				// encrypt that construct

				Encrypt(&unHi,&unLo);

				// write the encrypted block back

				dataOut[nPosOut]     = (unsigned char)unHi;
				dataOut[nPosOut + 1] = (unsigned char)(unHi >> 8);
				dataOut[nPosOut + 2] = (unsigned char)(unHi >> 16);
				dataOut[nPosOut + 3] = (unsigned char)(unHi >> 24);
				dataOut[nPosOut + 4] = (unsigned char)unLo;
				dataOut[nPosOut + 5] = (unsigned char)(unLo >> 8);
				dataOut[nPosOut + 6] = (unsigned char)(unLo >> 16);
				dataOut[nPosOut + 7] = (unsigned char)(unLo >> 24);

				nPosIn += 8; 
				nPosOut += 8;
			}

			return nCount;

}

void CBlowfish::Decrypt(unsigned int *unHiRef,unsigned int *unLoRef)
{
			unsigned int unHi = *unHiRef; 
			unsigned int unLo = *unLoRef; 

			unsigned int* sbox1 = m_sbox1;
			unsigned int* sbox2 = m_sbox2;
			unsigned int* sbox3 = m_sbox3;
			unsigned int* sbox4 = m_sbox4;

			unsigned int* pbox = m_pbox;

			unHi ^= pbox[(int)(17)];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[16];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[15];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[14];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[13];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[12];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[11];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[10];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[9];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[8];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[7];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[6];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[5];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[4];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[3];
			unLo ^= (((sbox1[(int)(unHi >> 24)] + sbox2[(int)((unHi >> 16) & 0x0ff)]) ^ sbox3[(int)((unHi >> 8) & 0x0ff)]) + sbox4[(int)(unHi & 0x0ff)]) ^ pbox[2];
			unHi ^= (((sbox1[(int)(unLo >> 24)] + sbox2[(int)((unLo >> 16) & 0x0ff)]) ^ sbox3[(int)((unLo >> 8) & 0x0ff)]) + sbox4[(int)(unLo & 0x0ff)]) ^ pbox[1];

			*unLoRef = unHi;
			*unHiRef = unLo ^ pbox[0];
}

int CBlowfish::Decrypt(unsigned char *dataIn, unsigned char *dataOut, int nPosIn, int nPosOut, int nCount)
{
			nCount >>= 3;

			for (int nI = 0; nI < nCount; nI++)
			{
				// load the bytes into two 32bit words, which together represent the block

				unsigned int unHi = ((unsigned int)( dataIn[nPosIn + 3]) << 24) |
					((unsigned int)(dataIn[nPosIn + 2]) << 16) |
					((unsigned int)(dataIn[nPosIn + 1]) << 8) |
					dataIn[nPosIn];
 
				unsigned int unLo = ((unsigned int)(dataIn[nPosIn + 7]) << 24) |
					((unsigned int)(dataIn[nPosIn + 6]) << 16) |
					((unsigned int)(dataIn[nPosIn + 5]) << 8) |
					dataIn[nPosIn + 4];

				// encrypt that construct

				Decrypt(&unHi,&unLo);

				// write the encrypted block back

				dataOut[nPosOut]     = (unsigned char)unHi;
				dataOut[nPosOut + 1] = (unsigned char)(unHi >> 8);
				dataOut[nPosOut + 2] = (unsigned char)(unHi >> 16);
				dataOut[nPosOut + 3] = (unsigned char)(unHi >> 24);
				dataOut[nPosOut + 4] = (unsigned char)unLo;
				dataOut[nPosOut + 5] = (unsigned char)(unLo >> 8);
				dataOut[nPosOut + 6] = (unsigned char)(unLo >> 16);
				dataOut[nPosOut + 7] = (unsigned char)(unLo >> 24);

				nPosIn += 8; 
				nPosOut += 8;
			}
			return nCount;
}
