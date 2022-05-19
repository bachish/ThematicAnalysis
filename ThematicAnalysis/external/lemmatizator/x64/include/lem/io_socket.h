// -----------------------------------------------------------------------------
// File IO_SOCKET.H
//
// (c) by Koziev Elijah
//
// Content:
// ��������� ��������������� ������ ����� ������ �����-������ �� ������
// socket'��. ���� ������ ��� WinNT/Win2000
//
// ������� �� ������ ���������� Trapper McFerron ( trapzz@hotmail.ru )
// -----------------------------------------------------------------------------
//
// CD->11.04.2002
// LC->16.08.2007
// --------------

#ifndef LEM_SOCK__H
#define LEM_SOCK__H
#pragma once

 #if defined LEM_WINDOWS

 #include <lem/cstring.h>
 #include <lem/streams.h>

 #include <winsock.h>
 #include <cstdio>

 namespace lem
 {
  namespace Sockets
  {
   using lem::CString;
   using lem::Stream;

 // ****************************************************************
 // ��������� ������ IP-����� � �������� ������ ��� ��������������
 // � ��������� �������.
 // ****************************************************************
 class Lem_IP
 {
  private:
   bool null; // true, ���� ����� �� ����� ��� ����� �������.
   UINT8 addr[4]; // 4 ����� ������
   CString str; // � ��������� �������������

   int iport; // ����� �����.

  public:
   Lem_IP(void);
   Lem_IP( const char *Str );

   inline bool IsNull(void) const { return null; }

   inline int get_port(void) const { return iport; }
   inline void SetPort( int P ) { iport=P; }

   const CString get_ip(void) const;
 };

 // ********************************************************************
 // ����� ��������� ������ �� ������ ���������� ������� Windows Sockets.
 // ********************************************************************
 class SocketStream : public Stream
 {
  protected:
   static bool winsock_inited;
   static WSADATA ws;

   SOCKET sckt;
   sockaddr_in san;
   bool server; // ��������� �����.

   Lem_IP IP;

   int sanSize;
   bool connected; // connected boolean so I can see if they are connected or not
   bool failed; // ��������� ������ ��� �������� ������ ����������� �����.

   virtual void openConn(void);
   virtual void openServerConn(void);

   virtual void closeConn(void);

   void InitWinSocket(void);

  public:
   SocketStream( const char *str, bool bServer=false );
   ~SocketStream(void);

   inline const Lem_IP& getIP(void) const { return IP; }

   virtual bool isConnected(void) {return connected;}

   // ������ ����� ����������, �� ������� ��������� src, �������� size.
   virtual void write( const void *src, pos_type size );

   // ������ ����� ���������� � ������� ������, �� ������� ��������� dest,
   // �������� size.
   virtual pos_type read( void *dest, pos_type size );

   // ��������� ���������� ������ ������ � �������.
   virtual void Check(void) const;

   // ���������� ������ �� ������� ������.
   virtual void flush(void);

   // ��������� ���������� ����� ����� ��� ��������� ������.
   virtual bool eof(void) const;

   // �������� ������� ������� ������-������ � ������.
   virtual pos_type tellp(void) const;

   // ���������� ������ �� �������� �������: ����������� ������� �� ������
   // �����, ������� ������� ��� ����� �����, �������� ��������� where_to.
   // ��� ������� - ��. �������� ����������� ������� fseek(...).
   virtual pos_type seekp( off_type pos, int whereto=SEEK_SET );

   // ����������� ������� ������������ ������� �������, � ������ ��� � �����
   // ����� �������� ����� �������� offset.
   virtual pos_type move( off_type offset );

   // ���������� ������ ������ � ��������� �����.
   virtual void close(void);

   // ���������� ������� ������ ������.
   virtual pos_type fsize(void) const;

   // ������ �� ������ ��� ���������� ��� ����� �� ����� ��������� ������?
   // ����� ����� ��� �������, ������� �������� � ������������ ����
   // ���������� ��� �����.
   virtual bool isready(void) const;

   // ���� ���������� ������ ������.
   virtual void wait_ready(void) const;

   // ������ ���������� �������. ����� ���������� EOF (-1) ��� ����������
   // ����� �����. ������������� ������� ������ � ����� �����.
   virtual int get(void);

   // ��������� ������ � ������� ������� � �����.
   virtual void put( char ch );

   virtual bool is_failed(void) const { return failed; }
 };

  } // namespace Sockets
 } // namespace lem

 #endif
#endif
// ------------------------- End Of File [IO_SOCKET.H] ------------------------
