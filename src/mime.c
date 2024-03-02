/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "ostypes.h"

int    m_nInputSize;
int    m_nBitsRemaining;
DWORD  m_lBitStorage;
char*  m_szInput;

/* The 7-bit alphabet used to encode binary information*/


char* m_sBase64Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int   m_nMask[] = { 0, 1, 3, 7, 15, 31, 63, 127, 255 };


DWORD read_bits(int nNumBits, int* pBitsRead, int* lp)
{
    DWORD lScratch;

    while ((m_nBitsRemaining < nNumBits ) && (*lp < m_nInputSize))
    {
        int c = m_szInput[(*lp)++];
        m_lBitStorage <<= 8;
        m_lBitStorage |= (c & 0xff);
        m_nBitsRemaining += 8;
    }
    if( m_nBitsRemaining < nNumBits )
    {
        lScratch = m_lBitStorage << ( nNumBits - m_nBitsRemaining );
        *pBitsRead = m_nBitsRemaining;
        m_nBitsRemaining = 0;
    }
    else
    {
        lScratch = m_lBitStorage >> ( m_nBitsRemaining - nNumBits );
        *pBitsRead = nNumBits;
        m_nBitsRemaining -= nNumBits;
    }
    return lScratch & m_nMask[nNumBits];
}





void write_bits(int nBits, int nNumBits, char* szOutput, int* pos)
{
    DWORD nScratch;

    m_lBitStorage = (m_lBitStorage << nNumBits) | nBits;
    m_nBitsRemaining += nNumBits;
    while( m_nBitsRemaining > 7 )
    {
        nScratch = m_lBitStorage >> (m_nBitsRemaining - 8);
        szOutput[(*pos)] = nScratch & 0xFF;
        (*pos)++;
        m_nBitsRemaining -= 8;
    }
}



char* Encode(char* szEncoding, int nSize, char* sOutput)
{
    int nNumBits = 6;
    int nDigit;
    int lp = 0;
    int pos = 0;
    sOutput[0] = 0;

    if( szEncoding == NULL )    return sOutput;

    m_szInput = szEncoding;
    m_nInputSize = nSize;
    m_nBitsRemaining = 0;

    nDigit = read_bits( nNumBits, &nNumBits, &lp );

    while( nNumBits > 0 )
    {
        sOutput[pos] = m_sBase64Alphabet[(int)nDigit];
        nDigit = read_bits( nNumBits, &nNumBits, &lp );
        pos++;
    }
    sOutput[pos] =  0;
    // Pad with '=' as per RFC 1521
    while( strlen (sOutput) % 4 != 0 )
    {
        strcat (sOutput, "=");
    }
    return sOutput;
}



/* The size of the output buffer must not be less than
   3/4 the size of the input buffer. For simplicity,
   make them the same size. */

int Decode(char* szDecoding, char* szOutput)
{
    char* sInput;
    int c;
    int lp =0;
    int nDigit;
    int nDecode[256];
    int i;

    if( szOutput == NULL )        return 0;
    if( szDecoding == NULL )    return 0;

    sInput = szDecoding;


    if(strlen (sInput) == 0)    return 0;

    // Build Decode Table
    //
    for( i = 0; i < 256; i++ )          nDecode[i] = -2; // Illegal digit
    for( i=0; i < 64; i++ )
    {
        nDecode[m_sBase64Alphabet[i]] = i;
        nDecode[m_sBase64Alphabet[i] | 0x80 ] = i; // Ignore 8th bit
        nDecode['='] = -1;
        nDecode['=' | 0x80 ] = -1; // Ignore MIME padding char
    }

    // Clear the output buffer
    memset(szOutput, 0, strlen (sInput) + 1);

    // Decode the Input
    //
    for( lp = 0, i = 0; lp < (int)strlen (sInput); lp++ )
    {
        c = sInput[lp];
        nDigit = nDecode[c & 0x7F];
        if( nDigit < -1 )
            return 0;
        else
            if( nDigit >= 0 )
                // i (index into output) is incremented by write_bits()
                write_bits( nDigit & 0x3F, 6, szOutput, &i );
    }
    return i;
}



/* IMPORTANT: The number of bytes we read must be
  a multiple of 3 because CBase64's Encode()
  method will append padding characters ('=')
  to make the output's size a multiple of 4.
  (Base64 treats 3 8-bit bytes as 4 6-bit 'bytes').
  MIME decoders are free to treat '=' as a signal
  that there's no more data, so we don't want to pad
  until we're supposed to.
  When at the end of the file, the # of bytes read
  may not be a multiple of 3, but that's okay
  because we DO want the padding chars then. */


/*

CString wrap_text(char* szText)
{
    CString sTemp;
    CString sLeft;
    CString sRight;
    int lp = 0;
    UINT nCount = 0;
    int nSpacePos = 0;

    if( szText == NULL )        return sTemp;


    sTemp = szText;
    
 while( lp < sTemp.GetLength() )
    {
        if(sTemp[lp] == ' ')  nSpacePos = lp;
        // Reset counter on newline

        if( sTemp.Mid( lp, 2 ) == _T( "\r\n" ) )
            nCount = 0;
        // Wrap text at last found space
        
  if( nCount > m_nWrapPos )
        {
             sLeft = sTemp.Left( nSpacePos );
             sRight = sTemp.Right( sTemp.GetLength() - nSpacePos );
             sLeft.TrimRight();
             sRight.TrimLeft();
             sLeft += _T( "\r\n" );
             sTemp = sLeft + sRight;
             nCount = 0;
  }
        else
            nCount++;
        lp++;
    }
    return sTemp;
}
*/



