#include "LibMemory.h"

// I tried to do it in C++ style just for curiosity but ...

// -------------- Process Utility ------------ //
void initProcess(Process* this) {
	this->pid = MEM_BAD;
	this->allocAddr = NULL;
	this->handle = NULL;
}

bool getPIDByName(Process* this, const char * _processName) {
	
	
	this->pid = MEM_BAD;
	this->name = _processName;
	bool success = FALSE;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if(hSnapshot != INVALID_HANDLE_VALUE){

		PROCESSENTRY32 lppe; lppe.dwSize = sizeof(PROCESSENTRY32);

		
		if(Process32First(hSnapshot,&lppe)){

			do{
				if(!strcmp(_processName,lppe.szExeFile)){
					this->pid = lppe.th32ProcessID;
					success = TRUE;
					break;
				}
			}
			while(Process32Next(hSnapshot,&lppe));
		}
	}

	CloseHandle(hSnapshot);
	return success;
}

bool getProcessHandle(Process* this) {
	bool success = FALSE;
	if(this->pid == MEM_BAD) return success; 

	HANDLE h = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE  | PROCESS_VM_READ | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION, FALSE, this->pid);

	if(h != NULL){
		success = TRUE;
		this->handle = h; 
	}

	return success; 
}

bool getProcessName(Process *this,pid_t _pid) {
	bool success = FALSE;
	this->pid = _pid;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if(hSnapshot != INVALID_HANDLE_VALUE){

		PROCESSENTRY32 lppe; lppe.dwSize = sizeof(PROCESSENTRY32);

		if(Process32First(hSnapshot,&lppe)){

			do{
				if(lppe.th32ProcessID == _pid){
					this->name = lppe.szExeFile;
					success = TRUE;
					break;
				}
			}
			while(Process32Next(hSnapshot,&lppe));
		}
	}

	CloseHandle(hSnapshot);
	return success;

}

LPVOID allocateMemory(Process *this) { 
	this->allocAddr = VirtualAllocEx(this->handle,NULL,4096,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE); 
	return this->allocAddr;
}

void releaseProcess(Process *this){
	if(this->allocAddr != NULL) VirtualFreeEx(this->handle,this->allocAddr,0,MEM_RELEASE);
	if(this->handle != NULL) CloseHandle(this->handle);
}

bool writeShellcode(Process *this,size_t _len,BYTE _shellcodes[]){
	SIZE_T bytesWritten = {0};
	WriteProcessMemory(this->handle, this->allocAddr,(LPVOID*)_shellcodes, _len, &bytesWritten); 
	return bytesWritten == _len;
}

void executeShellcode(Process *this) {
	SECURITY_ATTRIBUTES sec; 
    sec.nLength = sizeof(SECURITY_ATTRIBUTES);
    sec.lpSecurityDescriptor = NULL;
    sec.bInheritHandle = 0;

	int32_t value = 0;
    HANDLE th = CreateRemoteThread(this->handle,&sec,0,(LPTHREAD_START_ROUTINE)this->allocAddr,NULL,0,NULL);
    //WaitForSingleObject(th, 0xFFFFFFFF);
    //GetExitCodeThread(th,(LPDWORD)&value);
    
	return;
}


// -------------- Process Utility ------------ //