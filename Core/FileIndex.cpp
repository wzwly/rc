#include "FileIndex.h"
#include <unistd.h>


CIntArray::CIntArray()
{
    m_nValidIndex = 0;
}

CIntArray::~CIntArray()
{
    Reset();
}

int CIntArray::AddPos(int nPos_)
{
    int _nTatol = MAX_FILELINE_SIZE * m_vLineArray.size();
    int _nArry = m_nValidIndex / MAX_FILELINE_SIZE;
    if (m_nValidIndex >= _nTatol)
    {
        int* _pCur = NULL;
        for (int _i = m_vLineArray.size(); _i <= _nArry; ++_i)
        {
            _pCur = new int[MAX_FILELINE_SIZE];
            if (_pCur == NULL)
                return false;
            m_vLineArray.push_back(_pCur);
        }
    }
    m_vLineArray[_nArry][m_nValidIndex % MAX_FILELINE_SIZE] = nPos_;
    ++m_nValidIndex;
    return true;
}

int CIntArray::GetPos(int nIndex_)
{
    if (nIndex_ >= m_nValidIndex)
    {
        assert(false);
        return false;
    }
    int _nArray = nIndex_ / MAX_FILELINE_SIZE;
    return m_vLineArray[_nArray][nIndex_ % MAX_FILELINE_SIZE];
}

bool CIntArray::Reset()
{
    for (int _i = m_vLineArray.size() - 1; _i > 0; --_i)
    {
        if (m_vLineArray[_i] != NULL)
            delete m_vLineArray[_i];
    }
    m_vLineArray.clear();
    m_nValidIndex = 0;
}

bool CIntArray::CreateArray(int nSize_)
{
    int* _pCur = new int[nSize_];
    if (_pCur == NULL)
        return false;

    m_vLineArray.push_back(_pCur);
    m_nValidIndex = 0;
    return true;
}
//==============================================

 CGCodeFile::CGCodeFile() 
 {
   m_pBuffer = NULL; 
   m_hFile = INVALID_HANDLE_VALUE; 
   m_nSize = 0;
 }

CGCodeFile::~CGCodeFile()
{
  CloseGfile();
}

//将文件读入到m_pBufer
bool CGCodeFile::OnpenGFile(const char* pszPath_)
{
    assert(pszPath_ != NULL);
    if (m_hFile == INVALID_HANDLE_VALUE)
    {
        CloseGfile();
    }
    m_hFile = open(pszPath_, O_RDONLY);

    if (m_hFile == INVALID_HANDLE_VALUE)
    {
        assert(false);
        return false;
    }
    m_nSize =  SefAxf::GetFileSize(m_hFile);
    if (m_nSize <= 0)
        goto __finally;
   
    //内存映射
    m_pBuffer = (char*)mmap(NULL,m_nSize, PROT_READ,MAP_SHARED,m_hFile,0);

    return true;
__finally:
    CloseGfile();
    return false;
}

void CGCodeFile::ReadFilePos(int nPos_, char& szChar_)
{
    if (nPos_ == m_nSize)
      szChar_ = '\0';
    else
      szChar_ = m_pBuffer[nPos_];  
}

 bool CGCodeFile::CloseGfile() 
 { 
   if (m_hFile =! INVALID_HANDLE_VALUE)
     close(m_hFile);
   m_hFile = INVALID_HANDLE_VALUE;
   
   if (m_pBuffer != NULL)
     munmap(m_pBuffer, m_nSize);
   m_pBuffer = NULL;
   m_nSize = 0;
   
   return true;
 }
 
bool CGCodeFile::GetInt(int nStartPos_, int& nRet_)
{
  const int _nSize = 128;
  char _ch;
  char _szBuffer[_nSize];
  int  _i = 0;
  int  _nPos = nStartPos_;
  while (_nPos < m_nSize && _i < (_nSize - 1))
  {
    ReadFilePos(_nPos++, _ch);
    if (_ch == EOF || _ch == '\n' || isspace(_ch))
	break;
    _szBuffer[_i++] = _ch;
  }
  _szBuffer[_i] = '\0';

  char*	_pEnd;
  nRet_ = strtol(_szBuffer, &_pEnd, 10);
  if ((_pEnd - _szBuffer == 0)
    ||(nRet_ == LONG_MAX) 
    ||(nRet_ == LONG_MIN))
  {
    return false;
  }
  return true;
}

bool CGCodeFile::GetLineNoByFilePos(long nPos_, int& nLineNo_, int& nSegNo_)
{
  if (nPos_ == m_nSize)
    nPos_ = m_nSize - 1;
  
  assert(nPos_ >= 0);
  assert(m_pArrayIndex != NULL);

  nLineNo_ = nSegNo_ = -1;
  if (nPos_ < 0 || nPos_ >= m_nSize)
    return false;
  
  int _nArrayCnt = m_pArrayIndex->GetSize();
  int _nLowIndex = 0;
  int _nUpIndex = _nArrayCnt - 1;
  int _nCurIndex, _nNextIndex;
  int _nCurPos;
  while(1)
  {
    _nCurIndex = (_nLowIndex + _nUpIndex) / 2;
    _nNextIndex = _nCurIndex + 1;
    
    if (_nNextIndex >= _nArrayCnt)
    {
      nLineNo_ = _nCurIndex;
      break;
    }
    _nCurPos = m_pArrayIndex->GetPos(_nCurIndex);
    if (_nCurPos <= nPos_ && m_pArrayIndex->GetPos(_nNextIndex) > nPos_)
    {
      nLineNo_ = _nCurIndex;
      break;
    }
    else if( _nLowIndex > _nUpIndex)
      return false;
    else 
    {
      if (nPos_ < _nCurPos)
	_nUpIndex = _nCurIndex - 1;
      else
	_nLowIndex = _nCurIndex + 1;
    }
  }
  //获取段号
  return true;
}    

//===============================================
CFileIndex::CFileIndex()
{

    TRACE("CFileIndex!");

}

CFileIndex::~CFileIndex()
{

}


bool CFileIndex::Create(const char* pszPath_)
{
   assert(m_arrayIndex.GetSize() <= 0);
   if (!m_GCodeFile.OnpenGFile(pszPath_)) 
     return false;
   DestroySegIndex();
   m_nDateLength = m_GCodeFile.GetSize();
   
   strncpy(m_szFileName, pszPath_, sizeof(m_szFileName));
   m_nTotalLines = 0;
   m_nMaxLineLength = -1;
   
   IndexItem::LbType _lbType = IndexItem::LABEL_INVALID;
   int _nLabNo = 0, _nLbCount = 0, _nFp = 0;
   int _nLineStartpos = 0, _nOldLines = -1;
   int _nFilePos = 0, _nPreFilePos = 0;
   char _szChar;
   long int _nLeftSize = m_nDateLength;
   
   m_GCodeFile.ReadFilePos(_nFilePos++, _szChar);
   
   while(_nLeftSize)
   {
      if (m_nTotalLines != _nOldLines)
      {
	  m_arrayIndex.AddPos(_nLineStartpos);
	  _nOldLines = m_nTotalLines;
      }
      while(_szChar != '\n')
      {
	  if (_szChar == 'O' || _szChar == 'o')
	  {
	      _lbType = IndexItem::LABEL_O;
	      if (!m_GCodeFile.GetInt(_nFilePos, _nLabNo))
		_lbType = IndexItem::LABEL_INVALID;
	      m_SegIndex.push_back(new IndexItem(_lbType, _nLabNo, _nLbCount, _nFp));
	  }
	  m_GCodeFile.ReadFilePos(_nFilePos++, _szChar);
	  if (_nFilePos > m_nDateLength)
	  {
	    _nFilePos = -1;
	    break;
	  }	
      }
       _nFp = _nFilePos;
      if (_nFilePos > 0)
      {
	    _nLineStartpos = _nFilePos;
	    m_GCodeFile.ReadFilePos(_nFilePos, _szChar);
	    if (_nFilePos - _nPreFilePos > m_nMaxLineLength)
	      m_nMaxLineLength = _nFilePos - _nPreFilePos;
	    
	    _nLeftSize = m_nDateLength - _nFilePos;
	    ++_nFilePos;
	    ++m_nTotalLines;
	    _nPreFilePos = _nFilePos;
	    if (m_nTotalLines % 10000 == 0)
	    {
		//更新进度条
	    }
      }
      else
      {
	      if (_nFilePos == 0)
	      {
		m_GCodeFile.ReadFilePos(_nFilePos++, _szChar);
	      }
	      else
	      {
		m_nTotalLines++;
		break;
	      }
      }
   }
   
   if (m_GCodeFile.m_pArrayIndex == NULL)
     m_GCodeFile.m_pArrayIndex = &m_arrayIndex;
   
   TRACE("total line %d \n", m_nTotalLines);
   
}

void CFileIndex::DestroySegIndex()
{
  for (int _i = 0; _i < m_SegIndex.size(); _i++)
    delete m_SegIndex[_i];
  
  m_SegIndex.clear();
}

void CFileIndex::Destroy()
{
  
  m_arrayIndex.Reset();
  DestroySegIndex();
  m_GCodeFile.CloseGfile();
  ResetData();
}

void CFileIndex::ResetData()
{
    m_nDateLength = 0;
    m_szFileName[0] = '\0';
    m_nCurLine = -1;
    m_nCurpos = 0;
    m_nTotalLines = 0;
}
