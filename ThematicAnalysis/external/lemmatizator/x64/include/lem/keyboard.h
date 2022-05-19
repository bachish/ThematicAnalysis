#ifndef LEM_SCANFS__H
#define LEM_SCANFS__H
#pragma once

 #if !defined LEM_NO_MOUT

 #include <lem/console_application.h>
 #include <lem/fstring.h>
 #include <lem/cstring.h>
 #include <lem/streams.h>
 #include <lem/smart_pointers.h>

 #undef getch

 namespace lem
 {
  // ***********************************************
  // ��������� ��� ����� ������ � ����������.
  // ***********************************************
  class IKbdFormatter
  {
   private:
    lem::Ptr<KbdStream> stream; // ����� ��� ����� ������

    int dev_cp; // ������� ������� ���������� ���������
    int local_cp; // � ����� ������� �������� ���� ���������� ������� �
                        // ���������������� ���

    const lem::FString edline(
                              const char *question="",
                              bool psw=false,
                              lem::OFormatter &s=*mout
                             );

   public:
    IKbdFormatter(void);
    IKbdFormatter( lem::Ptr<KbdStream> s );

    void SetCP( int Device, int Local );

    bool ask_bool( const char *question="", lem::OFormatter &s=*mout );

    bool isready(void) const;

    #if !defined LEM_NOREAL
    double ask_real( const char *question="", lem::OFormatter &s=*mout );
    #endif

    int ask_int( const char *question="", lem::OFormatter &s=*mout );

    const lem::FString ask_fstring(
                                   const char* question="",
                                   lem::OFormatter &s=*mout,
                                   bool psw=false
                                  );

    const lem::UFString ask_ufstring(
                                     const char* question="",
                                     lem::OFormatter &s=*mout,
                                     bool psw=false
                                    );

    const lem::CString ask_string(
                                  const char* question="",
                                  lem::OFormatter &s=*mout,
                                  bool psw=false
                                 );

    void wait(void);

    int getch(void);
  };

  // *********************************************************************
  // ��������� ��� ���� ���������� �������� ������, ������� ������������
  // ��������� ���� � ����������.
  // *********************************************************************
  extern IKbdFormatter *mkey;

  // ����, ������������ �������� lem_getch() ��� ������� ������������ ������.
  #define LEM_KEY__RARROW 0x4d00              // -->
  #define LEM_KEY__LARROW 0x4b00              // <--
  #define LEM_KEY__UARROW 0x4800              // ^
  #define LEM_KEY__DARROW 0x5000              // v
  #define LEM_KEY__TAB    0x0009              // <-->
  #define LEM_KEY__ESC    0x001b              // Esc
  #define LEM_KEY__PGUP   0x4900              // PgUp
  #define LEM_KEY__PGDN   0x5100              // PgDn
  #define LEM_KEY__INS    0x5200              // Ins
  #define LEM_KEY__DEL    0x5300              // Del
  #define LEM_KEY__HOME   0x4700              // Home
  #define LEM_KEY__END    0x4f00              // End
  #define LEM_KEY__SPACE  0x0020              // Space

 } // end of namespace 'lem'

 #endif // !defined LEM_NO_MOUT

#endif
