#pragma once

#include <Windows.h>


class ThreadBase
{
public:

	HANDLE m_hThread;
	DWORD m_threadID;
	void* unknown0;
	int unknown1;

	bool unknown2;

	char* m_threadName;
	LPVOID unknown3;

	int unknown4;


	virtual ~ThreadBase();
};
