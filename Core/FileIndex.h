#ifndef  __FILEINDEX_H__
#define  __FILEINDEX_H__

#include <vector>
#include "sefafx.h"

using std::vector;
typedef void (*CALLBACK_UPDATE_PROGRESS)(int nVal_);

struct IndexItem
{
    enum LbType
    {
        LABLE_N,
        LABLE_L,
        LABEL_O,
        LABEL_INVALID,
    };
    LbType iLbType; //程序段类
    int	iLbNo;    //程序段号
    int  	iLbCount; //段号计数
    int	iFpStart; //文件指针位置
    IndexItem(LbType type_, int no_, int count_, int fpStart_)
    {
        iLbType = type_;
        iLbNo = no_;
        iLbCount = count_;
        iFpStart = fpStart_;
    }
};
//保存文件的行号对应的位置
class CIntArray
{
public:
    enum {MAX_FILELINE_SIZE = 1000000};
    CIntArray();
    virtual ~CIntArray();
private:
    unsigned int m_nValidIndex;
    vector<int*> m_vLineArray;

public:
    int GetSize() {
        return m_nValidIndex;
    }
    int AddPos(int nPos_);
    int GetPos(int nIndex_);
    bool Reset();
    bool CreateArray(int nSize_ = MAX_FILELINE_SIZE);
};

//load the G code file to memeroy
class CGCodeFile
{
public:
    enum
    {
        BUFFER_SIZE = 12288,//缓存大小
        MAX_LOAD_FILE_SIZE = 20000000,//22M file
    };
    struct FILE_POS
    {
        int m_nPosIndex;
        int m_nFilePos;
    };
    CGCodeFile() ;
    ~CGCodeFile();


public:
    bool OnpenGFile(const char* pszPath_);
    void ReadFilePos(int nPos_, char& szChar_);
    bool CloseGfile();
    const char* GetBuffer()const {
        return m_pBuffer;
    }
    bool GetInt(int nStartPos_, int& nRet_);
    bool GetLineNoByFilePos(long nPos_, int& nLineNo_, int& nSegNo_);
    long int GetSize() {
        return m_nSize;
    }

private:



public:
    CIntArray* m_pArrayIndex;
private:
    char* m_pBuffer;
    HANDLE m_hFile;
    long int m_nSize;
};

//

class CFileIndex
{
public:
    CFileIndex();
    virtual ~CFileIndex();

public:
    bool Create(const char* pszPath_);
    void Destroy();
private:
    void DestroySegIndex();
  
    void ResetData();
public:
    CGCodeFile m_GCodeFile;

private:
    CIntArray m_arrayIndex;
    vector<IndexItem*> m_SegIndex;
    char m_szFileName[PATH_MAX];
    int  m_nDateLength;
    int  m_nTotalLines;
    int  m_nMaxLineLength;
    int  m_nCurLine;
    long int m_nCurpos;

    char* m_pLastAccessLine;
    int	m_nLastAccessLineNo;
    int m_nLastAccessLineLen;
};



#endif
