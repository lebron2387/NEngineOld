
#include "incdef.h"
#include "MemoryManager.h"

int CMemoryManager::m_iMemoryCount=0;
int CMemoryManager::m_iEnableMemoryTracking = 0;
MemoryInfo *CMemoryManager::memory_info = (MemoryInfo*)malloc(sizeof(MemoryInfo));

void CMemoryManager::GetNotDeletedMemory()
{
	if(!m_iEnableMemoryTracking)return;
	unsigned int all_size = 0;
	for(int i=0;i<m_iMemoryCount;i++)
	{
		if(memory_info[i].address != 0)
		{
			all_size += memory_info[i].size;
			Trace("%s,%d,%d,%d\n",memory_info[i].file,memory_info[i].line,memory_info[i].size,memory_info[i].address);
		}
	}
	Trace("--------------------\nTotal Leak Size:%d\n",all_size);
}

int CMemoryManager::GetMemoryAllocationsCount()
{
	return m_iMemoryCount;
}

int CMemoryManager::GetMemoryAddressId(DWORD address)
{
	if(!m_iEnableMemoryTracking)return -1;
	for(int i=0;i<m_iMemoryCount;i++)
	{
		if(memory_info[i].address == address)
		{
			return i;
		}
	}
	return -1;
}

void * CMemoryManager::MemoryReAllocate(void *p,unsigned int size,char *file,int line)
{
    if(p != NULL)
	{
		p = (void *)realloc(p,size);
	}
    if(p == NULL)
	{
		p = (void *)malloc(size);

		if(m_iEnableMemoryTracking)
		{
			m_iMemoryCount++;
			memory_info = (MemoryInfo*)realloc(memory_info,m_iMemoryCount*sizeof(MemoryInfo));
			memory_info[m_iMemoryCount-1].address = (DWORD)p;
			memory_info[m_iMemoryCount-1].size = size;
			strncpy(memory_info[m_iMemoryCount-1].file, file, MAX_PATH);
			memory_info[m_iMemoryCount-1].line = line;
		}
	}
    return p;
}

void * CMemoryManager::MemoryAllocate(unsigned int size,char *file,int line)
{
	void *ptr = (void *)malloc(size);

	if(m_iEnableMemoryTracking)
	{
		if(GetMemoryAddressId((DWORD)ptr) == -1)
		{
			m_iMemoryCount++;
			//MemoryInfo *newdata = (MemoryInfo*)malloc(m_iMemoryCount*sizeof(MemoryInfo));
			//if(memory_info != NULL)
			//{
			//	memcpy(newdata,memory_info,sizeof(MemoryInfo)*(m_iMemoryCount-1));
			//	free(memory_info);
			//}
			//memory_info = newdata;
			memory_info = (MemoryInfo*)realloc(memory_info,m_iMemoryCount*sizeof(MemoryInfo));
			memory_info[m_iMemoryCount-1].address = (DWORD)ptr;
			memory_info[m_iMemoryCount-1].size = size;
			strncpy(memory_info[m_iMemoryCount-1].file, file, MAX_PATH);
			memory_info[m_iMemoryCount-1].line = line;
		}
	}
	
	return ptr;
}

void CMemoryManager::MemoryDeallocate(void *p)
{
	free(p);
	if(m_iEnableMemoryTracking)
	{
		if(GetMemoryAddressId((DWORD)p) >= 0)
		{
			m_iMemoryCount--;
			int j = 0;
			for(int i=0;i<m_iMemoryCount+1;i++)
			{
				if(memory_info[i].address != (DWORD)p)
				{
					memory_info[j].address = memory_info[i].address;
					memory_info[j].size = memory_info[i].size;
					strncpy(memory_info[j].file,memory_info[i].file, MAX_PATH);
					memory_info[j].line = memory_info[i].line;
					j++;
				}
				else
				{
					memory_info[i].address = 0;
					memory_info[i].size = 0;
					strncpy(memory_info[i].file,"empty", MAX_PATH);
					memory_info[i].line = 0;
				}
			}
		}
	}
}