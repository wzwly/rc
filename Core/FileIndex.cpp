#include "FileIndex.h"


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