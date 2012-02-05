
/*
 * windows/daemon.c -- модуль связи c операционной системой
 *
 * Разработал: Муковников М. Ю.
 * Версия:     1.0.0
 * Изменён:    16.04.2010
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <winsock.h>

#include "build.h"

#define TIME_TO_WAIT 5
#define PING_PACKET_SIZE 64

typedef struct
{
   unsigned char Ttl;                         
   unsigned char Tos;                         
   unsigned char Flags;                       
   unsigned char OptionsSize;                 
   unsigned char *OptionsData;                
} IP_OPTION_INFORMATION, *PIP_OPTION_INFORMATION;

typedef struct
{
   DWORD Address;                             
   unsigned long  Status;                     
   unsigned long  RoundTripTime;              
   unsigned short DataSize;                   
   unsigned short Reserved;                   
   void *Data;                                
   IP_OPTION_INFORMATION Options;             
   char Answer[PING_PACKET_SIZE];             
} IP_ECHO_REPLY, *PIP_ECHO_REPLY;

typedef HANDLE (WINAPI* pfnHV)(VOID);
typedef BOOL (WINAPI* pfnBH)(HANDLE);
typedef DWORD (WINAPI* pfnDHDPWPipPDD)(HANDLE, DWORD, LPVOID, WORD,
                                       PIP_OPTION_INFORMATION,LPVOID,
                                       DWORD, DWORD);

const char *hklm_run = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

HANDLE hIP;
WSADATA wsaData;
HINSTANCE hIcmp;
pfnHV pIcmpCreateFile;
pfnBH pIcmpCloseHandle;
pfnDHDPWPipPDD pIcmpSendEcho;

void
daemonize()
{
   char *p;
   char buf[260];

   CreateMutex(NULL, TRUE, "keusbd");
   if (GetLastError() == ERROR_ALREADY_EXISTS)
      die("daemonize: daemon is already running");

   GetModuleFileName(0, buf, 260);

   p = buf;
   while(*p++);
   while(*p != '\\') p--;
   *p = '\0';
   
   chdir(buf);
   FreeConsole();
}

void
icmp_free()
{
   pIcmpCloseHandle(hIP);
   FreeLibrary(hIcmp);
   WSACleanup();
}

int
icmp_init()
{
   static int icmp_done = 0;

   if (icmp_done) return 0;

   if (WSAStartup(MAKEWORD(1, 1), &wsaData))
      return die("icmp_init: can't initialize winsock");

   hIcmp = LoadLibrary("icmp.dll");
   die_p(hIcmp, "icmp_init: unable to load icmp.dll");

   pIcmpCreateFile = (pfnHV) GetProcAddress(hIcmp, "IcmpCreateFile");
   pIcmpCloseHandle = (pfnBH) GetProcAddress(hIcmp, "IcmpCloseHandle");
   pIcmpSendEcho = (pfnDHDPWPipPDD) GetProcAddress(hIcmp, "IcmpSendEcho");
   
   if (!pIcmpCreateFile || !pIcmpCloseHandle || !pIcmpSendEcho)
	  return die("icmp_init: cannot find icmp functions");

   hIP = pIcmpCreateFile();
   if (hIP == INVALID_HANDLE_VALUE)
      return die("icmp_init: unable to open ping service");

   icmp_done = 1;
   atexit(icmp_free);
   return 0;
}

int
icmp_ping(const char *host)
{
   IP_ECHO_REPLY Ipe;
   struct hostent* phe;
   char acPingBuffer[PING_PACKET_SIZE];
   DWORD dwStatus;

   phe = gethostbyname(host);
   if (!phe) return 1;

   memset(&Ipe, '\0', sizeof(IP_ECHO_REPLY));
   memset(acPingBuffer, '\xAA', sizeof(acPingBuffer));

   Ipe.Data = acPingBuffer;
   Ipe.DataSize = sizeof(acPingBuffer);      

   dwStatus = pIcmpSendEcho(hIP, *((DWORD*)phe->h_addr_list[0]), 
                            acPingBuffer, sizeof(acPingBuffer), NULL, &Ipe, 
                            sizeof(IP_ECHO_REPLY), TIME_TO_WAIT * 1000);

   if (dwStatus && memcmp(Ipe.Answer, acPingBuffer, PING_PACKET_SIZE))
      dwStatus = 0;

   return !dwStatus;
}

int
ping(const char *host, int tries)
{
   int i, failed = 0;
	
   icmp_init();
   for (i = 0; i < tries; i++)
      failed += icmp_ping(host);
     
   return (failed == tries);
}

int
cmdline_parse_rest(int argc, char *argv[])
{
   HKEY hKey;

   if (eq("-ay"))
   {
      char buf[260];

      GetModuleFileName(0, buf, 260);
      strcat(buf, " --daemon");
      RegOpenKeyEx(HKEY_LOCAL_MACHINE, hklm_run, 0, KEY_SET_VALUE, &hKey);
      RegSetValueEx(hKey, "keusbd", 0, REG_SZ, (const BYTE*) buf, strlen(buf));
      RegCloseKey(hKey);
      exit(0);
   }

   if (eq("-an"))
   {
      RegOpenKeyEx(HKEY_LOCAL_MACHINE, hklm_run, 0, KEY_SET_VALUE, &hKey);
      RegDeleteValue(hKey, "keusbd");
      RegCloseKey(hKey);
      exit(0);
   }

   return 0;
}

