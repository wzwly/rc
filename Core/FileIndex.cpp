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
      for(int _i = m_vLineArray.size(); _i <= _nArry; ++_i)
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
  for(int _i = m_vLineArray.size() - 1; _i > 0; --_i)
  {
      if(m_vLineArray[_i] != NULL)
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



//打开文件 文件指针m_pBufer
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
  
  if (m_nSize > MAX_LOAD_FILE_SIZE)
  {
    //直接读入到内存
    m_pBuffer = (char*)malloc(m_nSize);
    if (m_pBuffer == NULL)
       goto __finally;
    
    int _nBlockSize;
    unsigned int _nRead;
    if (m_nSize < BUFFER_SIZE)
      _nBlockSize = m_nSize;
    else 
      _nBlockSize = BUFFER_SIZE;
    
    _nRead = read(m_hFile,m_pBuffer, _nBlockSize);
    if (_nRead == _nBlockSize)
    {
      m_bUseFileMap = false;
      m_StartPos.m_nFilePos = m_StartPos.m_nPosIndex = 0;
      m_EndPos.m_nFilePos = m_EndPos.m_nPosIndex = _nBlockSize - 1;
    }
  }
  else
  {
     //采用文件映射
     m_pBuffer = (char*)mmap(NULL,m_nSize + 2, PROT_READ,MAP_SHARED,m_hFile,0);
     m_bUseFileMap = true;
  }
  
  return true;
  
__finally:
    CloseGfile();
  return false;
}


void CGCodeFile::ReadFilePos(int nPos_, char& szChar_)
{
  if (m_bUseFileMap)
  {
    szChar_ = m_pBuffer[nPos_]; 
    return;
  }
  
 __read_repeat:
  if (nPos_ >= m_StartPos.m_nFilePos && nPos_ <= m_EndPos.m_nFilePos)
  {
    if(m_StartPos.m_nPosIndex > m_EndPos.m_nPosIndex)
    {
	int _nOffset = nPos_ - m_StartPos.m_nFilePos;
	_nOffset = _nOffset - (BUFFER_SIZE - m_StartPos.m_nPosIndex);
	szChar_ = *(m_pBuffer + _nOffset);
    }
    else
      szChar_ = *(m_pBuffer + nPos_ - m_StartPos.m_nFilePos);
    if (szChar_ == '\r')
      szChar_ = ' ';
    return;
  }
  
  if (nPos_ >= m_EndPos.m_nFilePos)
  {
    assert(nPos_ <= m_nSize);
    if (nPos_ == m_nSize)
    {  szChar_ = EOF; return;}
    
  
  }
  else if(nPos_ < m_StartPos.m_nFilePos)
  {
    ReloadFileData(nPos_);
    goto __read_repeat;
  }
}

void CGCodeFile::ReloadFileData(int nPos_)
{

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
    
 }