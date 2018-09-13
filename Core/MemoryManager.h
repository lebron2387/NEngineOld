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
unsigned char *memory = new unsigned char[1024]; //вся выделенна память

int *a = (int *)memory;                                            // Сказал, что у тебя а указывает на данные типа int (4 байта). 
int *b = (int *)(unsigned char *)memory+sizeof(i)   // Теперь ты сказал что b указывает тоже на данные типа int (4 байта).

Динамически память распределилась один раз, а использовать её ты можешь как хочешь. Арифметика с указателями. Естественно нужен контроль свободного места в блоке. Освобождать память по указателям a и b нельзя. Освобождается целиком блок по указателю memory. Естественно нужен контроль ссылок на блок. 
И судя по тому что ты написал, тебе далеко до этого. Разберись сначало как происходит распределение памяти через CRT и WinAPI. Как работают new/delete. А уж потом тебе станет понятно как написать свой менеджер памяти.



Вот функция распределения памяти с сохранением информации о распределениии struct SALLOCINFO
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

  // Распределяем блок памяти
  if (pNew = AllocMem(nSize))
  {
    // Блокируем
    ::EnterCriticalSection(&__gs_SMemMangerCS);
    // Не смущайся по поводу цикла. Это не цикл, а один из вариантов замены оператора goto
    for (;;)
    {
      // Проверяем, есть ли место для хранение информации о распределенном блоке
      if (__gs_pFreeAllocInfoBegin)
      {
        // Место есть, сохраняем информацию. 
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
        // Места нет. Выделяем новой блок для хранения информации.
        if (__gs_pFreeAllocInfoBegin = (SALLOCINFO*)AllocMem(3744*sizeof(SALLOCINFO)))
        {
          // Память получена
          __gs_nAllocatedFreeAllocInfo+=3744;
          for (U3264 i=0; i<__gs_nAllocatedFreeAllocInfo-1; ++i)
            __gs_pFreeAllocInfoBegin[i].pNext = &__gs_pFreeAllocInfoBegin[i+1];
          __gs_pFreeAllocInfoBegin[__gs_nAllocatedFreeAllocInfo-1].pNext=NULL;
        }
        else
        {
          // Ошибка в распределении памяти. Освобождаем всё и выходим с NULL.
          FreeMem(pNew), pNew=NULL;
          break;
        }
      }
    }
    // Разблокировка
    ::LeaveCriticalSection(&__gs_SMemMangerCS);
  }
  return pNew;
}
*/