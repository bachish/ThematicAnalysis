// -----------------------------------------------------------------------------
// File LEM_PIPE.H
//
// (c) Koziev Elijah
//
// Content:
// ��������� ��������������� ������ ����� ������ �����-������ �� ������ pipe'��.
// ���� ������ ��� WinNT/Win2000
// -----------------------------------------------------------------------------
//
// CD->05.11.2000
// LC->16.08.2007
// --------------

#ifndef LEM_PIPE__H
#define LEM_PIPE__H
#pragma once

 #include <lem/config.h> 

 #if defined LEM_WINDOWS

 #include <lem/streams.h>
 #include <windows.h>
 #undef AddForm
 #include <lem/ints.h>

 namespace lem
 {

 // *************************************************************
 // ����� ��������� ������ �� ������ ���������� ������� pipe'��.
 // *************************************************************
 class PipeStream : public Stream
 {
  private:
   HANDLE hPipe; // ��������� ���������� ������ �������� ������.
   bool failed; // ��������� ������ ����������� �����.
   bool attached; // ��������� �� ����...
   bool is_it_server; // ��� ������� �������� true, ��� client'� = false.

   SECURITY_ATTRIBUTES saPipe; // ��� ������� � WinNT.

   void AttachToServer( int TimeOut );
   void DetachFromServer(void);

   void AttachToClient( int TimeOut );
   void DetachFromClient(void);

  public:
   PipeStream(
              const lem::Path& Name,
              bool r=true,
              bool w=true,
              bool IsItServer=false,
              int TimeOut=0
             );

   virtual ~PipeStream(void);

   // ������ ����� ����������, �� ������� ��������� src, �������� size, �
   // ����� � ������� �������.
   virtual void write( const void *src, pos_type size );

   // ������ ����� ���������� � ������� ������, �� ������� ��������� dest,
   // �������� size, � ������� �������.
   virtual pos_type read( void *dest, pos_type size );

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

   virtual bool is_failed(void) const { return failed; }
 };

 } // namespace 'lem'

 #endif

#endif
// ------------------------ End Of File [LEM_PIPE.H] ---------------------------
