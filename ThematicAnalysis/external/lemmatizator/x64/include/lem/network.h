// -----------------------------------------------------------------------------
// File NETWORK.H
//
// (c) 2005 by Koziev Elijah     all rights reserved 
//
// SOLARIX Intellectronix Project http://www.solarix.ru
//                                http://sourceforge.net/projects/solarix  
//
// Licensed under the terms of GNU Lesser GPL
// You must not eliminate, delete or supress these copyright strings
// from the file!
//
// Content:
// ��������� �������� ������ � ��������� ����� � ����������
// -----------------------------------------------------------------------------
//
// CD->17.03.2006
// LC->26.05.2008
// --------------

#ifndef LEM_LAN__H
#define LEM_LAN__H
#pragma once

 #include <lem/config.h>

 #if defined LEM_WINDOWS || defined LEM_DOT_NET
  #if defined LEM_MFC
   #include <afxwin.h>
  #else 
   #include <windows.h>
  #endif

  #include <winsock.h>
 #endif 

 #include <lem/containers.h>

 namespace lem
 {
  namespace Network
  {
   typedef unsigned long IP4;

   #if defined LEM_WINDOWS || defined LEM_DOT_NET
   inline IP4 Make_Ip4( const char *s )
   { return inet_addr(s); }
   #endif 

   extern void Ip_To_A( IP4 ip, char *buf );


   struct Host
   {
    char hostname[256];
    IP4 ip;
    bool online;

    Host(void) { *hostname=0; ip=0; online=false; }
   };


   // ************************************************
   // ��� ������������ �������� � ��������� ������
   // ************************************************
   class LAN
   {
    public:
     typedef void(*Callback)( const lem::Path &resource );
     typedef void(*Progress)( const char *MessageA );

     static bool icpm_init, winsock_init;

     #if defined LEM_WINDOWS || defined LEM_DOT_NET
     static WSADATA ws;
     #endif

    private:
     #if defined LEM_WINDOWS  || defined LEM_DOT_NET
     static bool EnumerateFunc(
                               lem::Collect<lem::Path> &list,
                               void *Ptr,
                               LAN::Callback callback,
                               LAN::Progress progress=NULL
                              );

     static bool EnumerateHostsFunc(
                                    lem::Collect<lem::Path> &list,
                                    void *Ptr,
                                    LAN::Callback callback,
                                    LAN::Progress progress=NULL
                                   );
     #endif

     static void Load_ICPM(void);
     static void Unload_ICPM(void);
     static void InitWinSocket(void);

     static bool ping_computer( IP4 ip, unsigned long timeout );
     static bool scan_computer(
                               IP4 ip,
                               const MCollect<unsigned> &ports,
                               unsigned long timeout,
                               unsigned *found_port
                              );
    public:
     LAN(void) {}
   
     // Returns the list of SMB network resources (disks only)
     static void Get_Disks(
                           lem::Collect<lem::Path> &list,
                           LAN::Callback callback=NULL,
                           LAN::Progress progress=NULL
                          );


     // ���������� ������ ��������� ������ � ��������� ����
     static void Get_Hosts(
                           lem::Collect<lem::Path> &list,
                           LAN::Callback callback=NULL,
                           LAN::Progress progress=NULL
                          );

     // ���������� ������ ��������� ����������� �����
     static void GetLocalShares(
                                lem::Collect<lem::UFString> &shares, 
                                lem::Collect<lem::Path> &folders
                               );

     // ���������� ������ ����������� ����� ��� ���������� ����� ( \\hostname )
     static void GetShares(
                           const wchar_t *Host,
                           lem::Collect<lem::UFString> &shares, 
                           lem::Collect<lem::Path> &folders
                          );

     static IP4 Get_Host_Ip( char *name );
     static bool Get_Host_Name( Host &j );

     // ���� ��� ������ ��������� IP ������� �����. ������ true ���� ���� �����������
     static bool Ping_Host( IP4 ip, unsigned Timeout );

     // �������� ������� ��� ������ � �������� ���������.
     // ���������� ������ �������� ������ � ���� "\\10.117.10.1\" ��� "\\host1\",
     // ������ �� ����������� ��������� ������ � ���������� �����
     //
     // ���� ����� �������, �� ��������� ������ ������������ ��������� ������ ��
     // �������� � �������� ���������.

     static void Ping_Hosts(
                            IP4 from, IP4 upto,
                            const MCollect<IP4> iplist,
                            const MCollect<unsigned> &ports,
                            unsigned Timeout,
                            lem::Collect<lem::Path> &list,
                            LAN::Callback callback=NULL,
                            LAN::Progress progress=NULL,
                            const char *www_prefix=NULL
                           );

    // ��������� ������ ���������� IP ��� �����
    static void GetIPs( lem::Collect<lem::FString> &ips );
   }; // class LAN

   // *************************************************************
   // ������� ���������������� ��� ������ � ����������: ���������
   // ����������, ������������ ��������.
   // *************************************************************
   class Web
   {
    private:
     static lem::FString proxy;
     static lem::FString locals;
     static lem::FString agent; // ����������� � ���������� �������� ��� �������     
     static bool init;

     Web(void);
     ~Web(void);

    public:
     // ������������� ���������� ���������� (�������� - �� �������).
     static void Init(void);

     // ����� � �������� ����� ������-������?
     static bool Proxy(void);

     static const lem::FString& GetAgent(void);
     static const lem::FString& GetProxy(void);
     static const lem::FString& GetLocals(void);

     // ��������������� ������������ ������
     static void SetProxy( const FString &cfg );
   };

  } // namespace Network
 } // namespace 'lem'

#endif
