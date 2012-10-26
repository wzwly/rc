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
  LbType iLbType; //�������
  int	iLbNo;    //����κ�
  int  	iLbCount; //�κż���  
  int	iFpStart; //�ļ�ָ��λ��
  IndexItem(LbType type_, int no_, int count_, int fpStart_)
  {
    iLbType = type_; iLbNo = no_;
    iLbCount = count_; iFpStart = fpStart_;
  }
};
//�����ļ����кŶ�Ӧ��λ��
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
  int GetSize(){return m_nValidIndex;}
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
    BUFFER_SIZE = 12288,//�����С
    MAX_LOAD_FILE_SIZE = 20000000,//22M file
  };
  struct FILE_POS
  {
    int m_nPosIndex;
    int m_nFilePos;
  };
  CGCodeFile(){};
  ~CGCodeFile(){};
  
  
public:
    bool OnpenGFile(const char* pszPath_);
    void ReadFilePos(int nPos_, char& szChar_);
   
    const char* GetBuffer()const {return m_pBuffer;}  


private:
     bool CloseGfile(){;}
     void ReloadFileData(int nPos_);
public:
    int m_nSize;
    CIntArray* m_pArrayIndex;
    vector<IndexItem*> m_SubIndex;
private:
   char* m_pBuffer;
   HANDLE m_hFile;
   HANDLE  m_hMapping;
   bool m_bUseFileMap;
   FILE_POS m_StartPos;
   FILE_POS m_EndPos;
    
};

//

class CFileIndex
{
public:
  CFileIndex();
  virtual ~CFileIndex();

public:
  bool Create(const char* pszPath_);
private:
  CIntArray m_arrayIndex;
  char m_szFileName[PATH_MAX];
};



#endif
