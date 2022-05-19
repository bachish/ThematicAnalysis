#ifndef LEM_MSTREAM__H
#define LEM_MSTREAM__H
#pragma once

 #include <lem/config.h>

 #if defined LEM_BORLAND
 #include <stdio.h>
 #else
 #include <cstdio>
 #endif

 #if defined LEM_WINDOWS
  #if defined LEM_MFC
   #include <afxwin.h>
  #else 
   #include <windows.h>
  #endif
 #endif
 
 #include <lem/integer.h>
 #include <lem/process.h>
 #include <lem/zeroed.h>


 namespace lem { class Stream; }
 
 namespace lem {

 const int LEM_MSTREAM_NFLAG=10;
 const lem::uint32_t LEM_MSTREAM_HTML   = 0x00000001L; // '\n' --> '<BR>\n'
 const lem::uint32_t LEM_MSTREAM_RAWTAB = 0x00000002L;

 class CodeConverter;
 class Path;
 class CString;
 class UCString;
 class FString;
 class UFString;

 // *****************************************************************
 // Basic output formatter.
 // It operates the argument the same way as c-lib printf(...) does.
 // *****************************************************************
 class OFormatter
 {
  protected:
   lem::Stream *stream;

   #if defined LEM_THREADS
    // ��� ���������� ������ � ������������� ��������� ����� �������.
    // ������ ������ ����� ����� ���� �������, ��� ��� ������ �������
    // ������ ��������� ����� �������� �����������. �� ������ ���������
    // ������ ���������������.
    lem::Process::CriticalSection lock_mstr;
   #endif

   int left_margin; // ����� ����, �� ��������� 0
   int page_width;  // ������ ��������, �� ��������� �� �������������� (0)

   OFormatter *doubler; // ��������� ����� ������ (��������� ���-������).

   lem::uint32_t CONTROLFLAG; // ������ ��� ����������� ��������

   int npos;
   int nflag;
   char flag_index[LEM_MSTREAM_NFLAG], flag_state[LEM_MSTREAM_NFLAG];
   zbool is_unsigned, is_normal, is_long, is_quatro, is_complex, is_dupel,
         is_complete;

   const CodeConverter *local_cp; // ������� �������� ��� �������� ���������
   const CodeConverter *out_cp;   // ������� �������� ��� ���������� �������������� -
                                   // �� ���� ������� �������� ���������� ������

   int exp_len,total_len,field_len,after,aligment,sign_out,caps,radix;
   int nrepeate;

   zbool autoflush; // ������������� ������ flush ������ ����� ��������� printf()

   void read_field_spec( const char *format, int &i );

   int read_digits( const char* format, int& i );
   char get_flag_state( int fi );
   void set_flag_state( int fi, int state );

   virtual void print_field( wchar_t *buffer );
   virtual void print_field( char *buffer );

   void VideoControl( const char *format, int &i );
   void Print_C_String( const char *val );
   void Print_Boolean( bool b );
   void Print_Tab( int to=INT_MAX, char ch=' ' );

   void Print_UINT( unsigned int val );
   void Print_INT( int val );
   void Print_UINT32( unsigned int val );
   void Print_INT32( int val );

   #if defined LEM_UI64
    void Print_UINT64( lem::uint64_t val );
   #endif

   #if defined LEM_I64
    void Print_INT64( lem::int64_t val );
   #endif

   void PrintInt( const char *aval, int val_signum );

   void ClearFlags(void);

   #if !defined(LEM_NOREAL)
   void MakeRealFormat( const char *format, int &i, char *f1 );
   void Print_Real8( const char *FORMAT, double val );
   void Print_Real10( const char *FORMAT, long double val );
   #endif // !defined LEM_NOREAL

   virtual void Print_C_String( const wchar_t *uStr );

   void Delete(void);
   void Init( FILE *file );
   void Init( const OFormatter &ms );

  protected:
   lem::zbool can_i_close;

   // ����� ���������� ����� ����, ��� printf() ����� ��� ������.
   virtual void End_Of_Printf(void) {}

  public:
   OFormatter(void);
   OFormatter( FILE *file );
   OFormatter( lem::Stream *file, bool CanClose=false );
   OFormatter( const lem::Path& filename );

   virtual ~OFormatter(void);

   void operator=( const OFormatter &ms );
   void operator=( FILE *file );

   virtual void Open( const lem::Path& filename, bool write_bom=false );
   virtual void Open( lem::Stream *file, bool CanClose=false );
   virtual void Open( FILE *file );

   // ��������� ������� ����� ������
   virtual void close(void);

   // ��� �������� - ������ ����� � ������� ������
   virtual void dot(void);

   virtual void eol(void);
   virtual void cr(void);

   void SetLocalCP( const CodeConverter *Page );
   void SetOutCP( const CodeConverter *Page );
   void SetCP( const CodeConverter *Device, const CodeConverter *Local );

   const CodeConverter* GetOutCP(void);

   void SetAutoflush(void);

   void printf( const char *format, ... );

   void cprintf( const char *format, ... );
   void cprintf( const wchar_t *format, ... );

   void print_string( const wchar_t *buffer );
   void print_string( const char *buffer );

   void put_direct( char ch );
   virtual void printf( char ch, bool do_recode=false );
   virtual void uprintf( wchar_t ch );

   // ����� ����������� ������ ������ � �����.
   void flush(void);

   void SetControlFlag( unsigned long Flags, bool SetAction );

   bool GetControlFlag( unsigned long Flags ) const;

   void ResetPos( int n ) { npos=n; }
   inline int GetPos(void) const { return npos; }
   inline bool IsOpened(void) const { return !!stream; }

   void SetDoubler( OFormatter *Doubler );
   void ClearDoubler(void);
   OFormatter* GetDoubler(void);
   lem::Stream* GetStream(void);
   void SetLeftMargin( int X );

   friend OFormatter& operator<<( OFormatter &to, const CString &s );
   friend OFormatter& operator<<( OFormatter &to, const UCString &s );
 };


 inline OFormatter& operator << ( OFormatter &o, const char *s )
 {
  o.printf( s );
  return o;
 }

 inline OFormatter& operator << ( OFormatter &o, const wchar_t *s )
 {
  o.printf( "%us", s );
  return o;
 }

 #if !defined LEM_NODUP
 inline OFormatter& operator << ( OFormatter &o, DUPEL &d )
 {
  o.printf( "%rde", d );
  return o;
 }
 #endif

 #if !defined LEM_NOREAL
 inline OFormatter& operator << ( OFormatter &o, double d )
 {
  o.printf( "%re", d );
  return o;
 }
 #endif

 extern OFormatter& operator << ( OFormatter &o, int d );
 extern OFormatter& operator<<( OFormatter &to, const FString &s );
 extern OFormatter& operator<<( OFormatter &to, const UFString &s );
 extern OFormatter& operator<<( OFormatter &to, const CString &s );
 extern OFormatter& operator<<( OFormatter &to, const UCString &s );

  // ************************************************************************
  // ������ ������������� ���������� ������� ����� � UNICODE-����� - �������
  // ��������� � �������� ��������� ���.
  // ************************************************************************
  class OUFormatter : public OFormatter
  {
   public:
    OUFormatter(void);

    // ���� ����������� ��������� utf8 ����� ��� ������
    OUFormatter( const lem::Path& filename, bool write_bom=true );

    OUFormatter( FILE *file );
    OUFormatter( lem::Stream *file, bool CanClose=false );
 
    virtual void Open( const lem::Path& filename, bool write_bom=true );

    virtual void uprintf( wchar_t ch );
  };


  class U16Formatter : public OFormatter
  {
   public:
    U16Formatter(void);

    // ���� ����������� ��������� utf16 ����� ��� ������
    U16Formatter( const lem::Path& filename, bool write_bom=true );

    virtual void Open( const lem::Path& filename, bool write_bom=true );

    virtual void uprintf( wchar_t ch );
  };


  // ***************************************************************
  // ������������� ���������� ��� ������������ XML ������ � utf-8
  // ***************************************************************
  class XmlFormatter : public OUFormatter
  {
   private:
    XmlFormatter(void) {}

   public:
    // ���� ����������� ��������� utf8 ����� ��� ������
    XmlFormatter( const lem::Path& filename );

    // Prints the standard tag <?xml...
    virtual void WriteBeginning(void);
  };

  class MemFormatter : public OUFormatter
  {
   public:
    MemFormatter(void);

    const UFString string(void) const;
    //const lem::FString stringa(void) const;
  };

 } // end of namespace lem

#endif

