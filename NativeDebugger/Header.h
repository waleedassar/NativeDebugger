#pragma once

#include "windows.h"


#define ulong unsigned long
#define ulonglong unsigned long long
#define longlong long long
#define ULONG unsigned long
#define ULONGLONG unsigned long long
#define ushort unsigned short
#define USHORT unsigned short
#define uchar unsigned char




#define OBJ_PROTECT_CLOSE 		0x00000001
#define OBJ_INHERIT             0x00000002
#define OBJ_AUDIT_OBJECT_CLOSE  0x00000004
#define OBJ_PERMANENT           0x00000010
#define OBJ_EXCLUSIVE           0x00000020
#define OBJ_CASE_INSENSITIVE    0x00000040
#define OBJ_OPENIF              0x00000080
#define OBJ_OPENLINK            0x00000100
#define OBJ_KERNEL_HANDLE       0x00000200
#define OBJ_FORCE_ACCESS_CHECK  0x00000400
#define OBJ_VALID_ATTRIBUTES    0x000007F2
#define OBJ_CREATOR_INFO_MINE   0x00010000


#define SYMBOLIC_LINK_QUERY 0x0001
#define SYMBOLIC_LINK_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0x1)




typedef struct _IO_STATUS_BLOCK {
  union {
    int Status;
    void*    Pointer;
  } DUMMYUNIONNAME;
  ulonglong Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;





struct _UNICODE_STRING
{
	unsigned short Length;
	unsigned short MaxLength;
	unsigned long Pad;
	wchar_t* Buffer;
};


struct _OBJECT_ATTRIBUTES
{
  ULONGLONG           Length;
  HANDLE          RootDirectory;
  _UNICODE_STRING* ObjectName;
  ULONGLONG           Attributes;
  PVOID           SecurityDescriptor;
  PVOID           SecurityQualityOfService;
};



struct _CLIENT_ID
{
	ulonglong UniqueProcess;
	ulonglong UniqueThread;
};


void DumpHex_8(const void* data, size_t size);






struct _USER_STATE_CHANGE
{
	ulong UserEventId;
	ulong Pad;
	ulonglong ProcessId;
	ulonglong ThreadId;
	uchar DebugBuffer[0xA0];
};



#define DBG_CONTINUEX 0x00010001L
#define DBG_CONTINUE 0x00010002L
#define DBG_EXCEPTION_NOT_HANDLED  0x80010001L 
#define DBG_REPLY_LATER  0x40010001L
#define DBG_REPLY_LATERX 0x40010003
#define DBG_REPLY_LATERY 0x40010004

struct _DBGKM_EXCEPTION
{
  _EXCEPTION_RECORD ExceptionRecord;
  ulong dwFirstChance;
  ulong Pad;
};


struct _DBGKM_CREATE_THREAD
{
  ulonglong lpThreadLocalBase;
  ulonglong Win32StartAddress;
};


struct _DBGKM_CREATE_PROCESS
{
  ulonglong Unk0;
  HANDLE hFile;
  ulonglong SectionBaseAddress;
  ulong dwDebugInfoFileOffset;
  ulong nDebugInfoSize;
  ulonglong Unk2;
  ulonglong lpStartAddress;
};


struct _DBGKM_EXIT_THREAD
{
  ulong dwExitCode;
};

struct _DBGKM_EXIT_PROCESS
{
  ulong dwExitCode;
};

struct _DBGKM_LOAD_DLL
{
  HANDLE hFile;
  ulonglong BaseOfDll;
  ulong DebugInfoFileOffset;
  ulong DebugInfoSize;
  ulonglong pTempDllName;
};



struct _DBGKM_UNLOAD_DLL
{
  ulonglong lpBaseOfDll;
};


#define ProcessDebugFlags 0x1F


extern "C"
{
	int ZwSetInformationProcess(HANDLE ProcessHandle,ulonglong ProcessInformationClass, void* ProcessInformation,ulonglong ProcessInformationLength);
}



struct _PS_CREATE_INFO
{
	ulonglong Size;
	ulonglong State;//at 0x8
	ulong InitFlags;//at 0x10
	ulong AdditionalFileAccess;//at 0x14
	ulonglong X0;
	ulonglong X1;
	ulonglong X2;
	ulonglong X3;
	ulonglong X4;
	ulonglong X5;
	ulonglong X6;
	ulonglong X7;
};


struct _PS_ATTRIBUTE
{
	ulonglong Attribute;
    ulonglong Size;
    ulonglong Value;
    ulonglong ReturnLength;
};


struct _PS_ATTRIBUTE_LIST
{
   ulonglong TotalLength;
   _PS_ATTRIBUTE Attributes[1];
};

#define 	RTL_USER_PROC_PARAMS_NORMALIZED   0x00000001

extern "C"
{
		void RtlInitUnicodeString(_UNICODE_STRING*         DestinationString, wchar_t* SourceString);

		int RtlCreateProcessParametersEx(
							void **pProcessParameters,
							_UNICODE_STRING* ImagePathName,
							_UNICODE_STRING* DllPath,
							_UNICODE_STRING* CurrentDirectory,
							_UNICODE_STRING* CommandLine,
							void* Environment,
							_UNICODE_STRING* WindowTitle,
							_UNICODE_STRING* DesktopInfo,
							_UNICODE_STRING* ShellInfo,
							_UNICODE_STRING* RuntimeData,
							ulonglong Flags);


		int ZwCreateUserProcess(HANDLE* ProcessHandle, 
							HANDLE* ThreadHandle,
							ACCESS_MASK ProcessDesiredAccess,
							ACCESS_MASK ThreadDesiredAccess,
							_OBJECT_ATTRIBUTES* ProcessObjectAttributes,
							_OBJECT_ATTRIBUTES* ThreadObjectAttributes,
							ulonglong ProcessFlags,
							ulonglong ThreadFlags,
							void* ProcessParameters, 
							void* CreateInfo,
							_PS_ATTRIBUTE_LIST* AttributeList);
}


int CreateProcessWithDebuggerNative_C(wchar_t* ImagePath,wchar_t* CommandLine, HANDLE hDebugObject,HANDLE* phProcess,HANDLE* phThread);




extern "C"
{
	int ZwOpenProcess(HANDLE* ProcessHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,_CLIENT_ID* ClientId );


	int ZwCreateDebugObject( HANDLE* DebugObjectHandle,ACCESS_MASK DesiredAccess,_OBJECT_ATTRIBUTES* ObjectAttributes,ulonglong Flags );
	
	int ZwSetInformationDebugObject(HANDLE DebugObjectHandle,ulonglong DebugObjectInformationClass,void* DebugInformation,ulonglong DebugInformationLength,  ulonglong* ReturnLength);

	int ZwDebugActiveProcess(HANDLE ProcessHandle,HANDLE DebugObjectHandle);

	int ZwWaitForDebugEvent(HANDLE DebugObjectHandle,ulonglong Alertable,_LARGE_INTEGER* Timeout,void* WaitStateChange );

	int ZwDebugContinue(HANDLE DebugObjectHandle,_CLIENT_ID* ClientId,ulonglong ContinueStatus);


	int ZwRemoveProcessDebug(HANDLE ProcessHandle, HANDLE DebugObjectHandle);


	int ZwWaitForSingleObject(HANDLE Handle,bool Alertable,_LARGE_INTEGER* Timeout);

	int ZwClose(HANDLE Handle);
}