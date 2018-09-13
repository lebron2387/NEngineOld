#pragma once


struct MemoryInfo
{
	DWORD address;
	unsigned int size;
	char file[MAX_PATH];
	int	line;
};

class CMemoryManager
{
public:
	static int GetMemoryAddressId(DWORD address);
	static void * MemoryAllocate(unsigned int size,char *file,int line);
	static void * MemoryReAllocate(void *p,unsigned int size,char *file,int line);
	static void MemoryDeallocate(void *p);
	static int GetMemoryAllocationsCount();
	static void GetNotDeletedMemory();

	static MemoryInfo *memory_info;
	static int m_iMemoryCount;
	static int m_iEnableMemoryTracking;
private:
	CMemoryManager();
	~CMemoryManager();
};

/*
unsigned char *memory = new unsigned char[1024]; //��� ��������� ������

int *a = (int *)memory;                                            // ������, ��� � ���� � ��������� �� ������ ���� int (4 �����). 
int *b = (int *)(unsigned char *)memory+sizeof(i)   // ������ �� ������ ��� b ��������� ���� �� ������ ���� int (4 �����).

����������� ������ �������������� ���� ���, � ������������ � �� ������ ��� ������. ���������� � �����������. ����������� ����� �������� ���������� ����� � �����. ����������� ������ �� ���������� a � b ������. ������������� ������� ���� �� ��������� memory. ����������� ����� �������� ������ �� ����. 
� ���� �� ���� ��� �� �������, ���� ������ �� �����. ��������� ������� ��� ���������� ������������� ������ ����� CRT � WinAPI. ��� �������� new/delete. � �� ����� ���� ������ ������� ��� �������� ���� �������� ������.



��� ������� ������������� ������ � ����������� ���������� � �������������� struct SALLOCINFO
  {
    SALLOCINFO  *pPrev;
    SALLOCINFO  *pNext;
    PVOID       Addr;
    U3264       nSize;
    U32         nLine;
    TCHAR       szFile[MAX_PATH];
  };
  U3264 __gs_nAllocatedFreeAllocInfo=0;
  SALLOCINFO* __gs_pUsedAllocInfoBegin=NULL;
  SALLOCINFO* __gs_pFreeAllocInfoBegin=NULL;

PVOID AllocMem_dbg(U3264 nSize, PCTSTR pszFile, U32 nLine)
{
  PVOID pNew;

  // ������������ ���� ������
  if (pNew = AllocMem(nSize))
  {
    // ���������
    ::EnterCriticalSection(&__gs_SMemMangerCS);
    // �� �������� �� ������ �����. ��� �� ����, � ���� �� ��������� ������ ��������� goto
    for (;;)
    {
      // ���������, ���� �� ����� ��� �������� ���������� � �������������� �����
      if (__gs_pFreeAllocInfoBegin)
      {
        // ����� ����, ��������� ����������. 
        if (__gs_pUsedAllocInfoBegin)
           __gs_pUsedAllocInfoBegin->pNext = __gs_pFreeAllocInfoBegin;
        __gs_pFreeAllocInfoBegin->pPrev = __gs_pUsedAllocInfoBegin;
        __gs_pUsedAllocInfoBegin = __gs_pFreeAllocInfoBegin;
        __gs_pFreeAllocInfoBegin = __gs_pFreeAllocInfoBegin->pNext;
        __gs_pUsedAllocInfoBegin->pNext = NULL;
        __gs_pUsedAllocInfoBegin->Addr  = pNew;
        __gs_pUsedAllocInfoBegin->nSize = nSize;
        __gs_pUsedAllocInfoBegin->nLine = nLine;
        _tcscpy_s(__gs_pUsedAllocInfoBegin->szFile, MAX_PATH-sizeof(TCHAR), pszFile);
        
        --__gs_nAllocatedFreeAllocInfo;
        break;
      }
      else
      {
        // ����� ���. �������� ����� ���� ��� �������� ����������.
        if (__gs_pFreeAllocInfoBegin = (SALLOCINFO*)AllocMem(3744*sizeof(SALLOCINFO)))
        {
          // ������ ��������
          __gs_nAllocatedFreeAllocInfo+=3744;
          for (U3264 i=0; i<__gs_nAllocatedFreeAllocInfo-1; ++i)
            __gs_pFreeAllocInfoBegin[i].pNext = &__gs_pFreeAllocInfoBegin[i+1];
          __gs_pFreeAllocInfoBegin[__gs_nAllocatedFreeAllocInfo-1].pNext=NULL;
        }
        else
        {
          // ������ � ������������� ������. ����������� �� � ������� � NULL.
          FreeMem(pNew), pNew=NULL;
          break;
        }
      }
    }
    // �������������
    ::LeaveCriticalSection(&__gs_SMemMangerCS);
  }
  return pNew;
}
*/