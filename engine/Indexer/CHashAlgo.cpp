#include"CHashAlgo.h"

       
//////////////////////////////////////////////////////////////////////////     
// 预处理     
void CHashAlgo::prepareCryptTable()     
{      
    unsigned long seed =0x00100001, index1 =0, index2 =0, i;     
    
    for( index1 =0; index1 <0x100; index1++ )     
    {      
        for( index2 = index1, i =0; i <5; i++, index2 +=0x100 )     
        {      
            unsigned long temp1, temp2;     
            seed = (seed *125+3) %0x2AAAAB;     
            temp1 = (seed &0xFFFF) <<0x10;     
            seed = (seed *125+3) %0x2AAAAB;     
            temp2 = (seed &0xFFFF);     
            cryptTable[index2] = ( temp1 | temp2 );      
        }      
    }      
}     
    
//////////////////////////////////////////////////////////////////////////     
// 求取哈希值     
unsigned long CHashAlgo::HashString(const char*lpszFileName, unsigned long dwHashType)     
{      
    unsigned char*key = (unsigned char*)lpszFileName;     
    unsigned long seed1 =0x7FED7FED, seed2 =0xEEEEEEEE;     
    int ch;     
    
    while(*key !=0)     
    {      
        ch = toupper(*key++);     
    
        seed1 = cryptTable[(dwHashType <<8) + ch] ^ (seed1 + seed2);     
        seed2 = ch + seed1 + seed2 + (seed2 <<5) +3;      
    }     
    return seed1;      
}     
    
//////////////////////////////////////////////////////////////////////////     
// 得到在定长表中的位置     
long CHashAlgo::GetHashTablePos(const char*lpszString)     
    
{      
    const unsigned long HASH_OFFSET =0, HASH_A =1, HASH_B =2;     
    unsigned long nHash = HashString(lpszString, HASH_OFFSET);     
    unsigned long nHashA = HashString(lpszString, HASH_A);     
    unsigned long nHashB = HashString(lpszString, HASH_B);     
    unsigned long nHashStart = nHash % m_tablelength,     
        nHashPos = nHashStart;     
    
    while ( m_HashIndexTable[nHashPos].bExists)     
    {      
        if (m_HashIndexTable[nHashPos].nHashA == nHashA && m_HashIndexTable[nHashPos].nHashB == nHash)      
            return nHashPos;      
        else      
            nHashPos = (nHashPos +1) % m_tablelength;     
    
        if (nHashPos == nHashStart)      
            break;      
    }     
    
    return-1; //没有找到     
}     
//////////////////////////////////////////////////////////////////////////     
// 通过传入字符串，将相应的表项散列到索引表相应位置中去     
bool CHashAlgo::SetHashTable(const char*lpszString )     
{     
    const unsigned long HASH_OFFSET =0, HASH_A =1, HASH_B =2;     
    unsigned long nHash = HashString(lpszString, HASH_OFFSET);     
    unsigned long nHashA = HashString(lpszString, HASH_A);     
    unsigned long nHashB = HashString(lpszString, HASH_B);     
    unsigned long nHashStart = nHash % m_tablelength,     
        nHashPos = nHashStart;     
    
    while ( m_HashIndexTable[nHashPos].bExists)     
    {      
        nHashPos = (nHashPos +1) % m_tablelength;     
        if (nHashPos == nHashStart)      
        {     
    
#if DEBUGTEST     
            testid =-1;     
#endif  
    
            return false;      
        }     
    }     
    m_HashIndexTable[nHashPos].bExists =true;     
    m_HashIndexTable[nHashPos].nHashA = nHashA;     
    m_HashIndexTable[nHashPos].nHashB = nHash;     
    strcpy_s( m_HashIndexTable[nHashPos].test_filename, strlen(m_HashIndexTable[nHashPos].test_filename) + strlen(lpszString) + 1,lpszString );
    
#if DEBUGTEST     
    testid = nHashPos;     
#endif  
    
    return true;     
}     
    
//////////////////////////////////////////////////////////////////////////     
// 取得哈希索引表长     
unsigned long CHashAlgo::GetTableLength(void)     
{     
    return m_tablelength;     
}     
    
//////////////////////////////////////////////////////////////////////////     
// 设置哈希索引表长     
void CHashAlgo::SetTableLength( const unsigned long nLength )     
{     
    m_tablelength = nLength;     
    return;     
}    