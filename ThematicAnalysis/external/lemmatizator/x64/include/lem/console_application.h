// -----------------------------------------------------------------------------
// File CONSOLE_APPLICATION.H
//
// (c) by Elijah Koziev     all rights reserved 
//
// SOLARIX Intellectronix Project http://www.solarix.ru
//                                http://sourceforge.net/projects/solarix  
//
// Licensed under the terms of GNU Lesser GPL
//
// Content:
// ������������� ��������� ������� ��� ������� ���� (MS DOS, Linux,
// ������� � Win32). ���������� ��������� ���������� ������� ������ �� �������
// (������ mout ������ OFormatter) �� �������� �� ��������� �����������
// ���������. ��� ����������� ������������ ���� ����������� �� ������ ����������
// � ������� �������������.
//
// Parsing the command line, splitting it to options.
//
// -----------------------------------------------------------------------------
//
// CD->23.10.2000
// LC->08.06.2008
// --------------

#ifndef LEM_COAP__H
#define LEM_COAP__H
#pragma once

 #include <lem/console_streams.h>

 #if defined LEM_WINDOWS
  #if defined LEM_MFC
   #include <afxwin.h>
  #else 
   #include <windows.h>
  #endif
  #undef AddForm
 #endif


 namespace lem
 {
  // *******************************************************
  // ��������� ��� �������� ����� ����� ��������� ������.
  // *******************************************************
  class ArgOption
  {
   private:
    int code;          // ���������� ��� ��� ������������ ������.
    char key;      // ������ - ��� �����.
    char *content; // �������� �����.
    char *option;  // ������ ����� ���������.
    // ���� ���� ����� ��� /Z866, �� ���� ������ ��������:
    //   option  = "Z866"
    //   key     = 'Z'
    //   content = "866"

   public:
    ArgOption(void);
    ArgOption( const ArgOption &x );
    ArgOption(
              const int Code,
              const char *Option,
              const char Key,
              const char *Content
             );

    ~ArgOption(void);

    void operator=( const ArgOption &x );

    inline const int GetCode(void) const { return code; }
    inline const char GetKey(void) const { return key; }
    inline const char* GetContent(void) const { return content; }
    inline const char* string(void) const { return content; }
    inline const char* GetOption(void) const { return option; }
  };

  extern ArgOption* ParseArgList(
                                 const int narg,
                                 char *argv[],
                                 const char *Template=""
                                );

  extern ArgOption* ParseArgList(
                                 const char *cmdline,
                                 int *argc,
                                 const char *Template=""
                                );

  namespace Cfg_Parser
  {
   class Ini_Parser;
  }

  class Path;

  class Base_Application
  {
   protected:
    virtual void TryRead( Cfg_Parser::Ini_Parser &ini );

   public:
    Base_Application(void) {}
    virtual ~Base_Application(void) {}

    void Open_Ini_Parser( Cfg_Parser::Ini_Parser &ini, const lem::Path& filename );

    // Reading configuration options from ini file
    virtual bool Read_Ini( const lem::Path& ini_file );
    virtual bool Read_Ini( lem::Stream& ini_file );
  };


 } // end of namespace 'lem'

#endif
