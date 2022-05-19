// -----------------------------------------------------------------------------
// File EXCEPTIONS.H
//
// (c) Koziev Elijah
//
// Content:
// ����������� ��������� ����� ����������, ������������ ����� Solarix'�.
// -----------------------------------------------------------------------------
//
// CD->14.11.2003
// LC->18.05.2009
// --------------

#ifndef SOL_EXCEPTIONS__H
#define SOL_EXCEPTIONS__H
#pragma once

 #include <lem/cstring.h>
 #include <lem/conversions.h>
 #include <string>

 #if defined LEM_BORLAND && !defined LEM_CODEGEAR
  #include <exception.h>
 #endif

 namespace Solarix
 {
  using lem::to_wstr;
  using lem::E_BaseException;
  using lem::E_ParserError;
  using lem::UCString;

  // *******************************************************************
  // ������� ����� ��� ����������, ����������� ����� SOLARIX'�
  // ������� �������� �� ������������ ������� �� ������������
  // ������ exception: ���� � ��������� ������ �������� UNICODE-�������,
  // ��������� ��������� ��������� �������� ����� � �������������
  // ���������, � ������������� � ����� ���������� ��������� (��������,
  // ���������� ��������� � ASCII-���� �� ������� �������� 1251) ���� ��
  // ���������.
  //
  // ����� ����, ���� ������ ������ �������� ������ exception
  // �� ����������������!
  // *******************************************************************
  class E_Solarix : public E_BaseException
  {
   public:
    E_Solarix(void) {};
    explicit E_Solarix( const std::wstring &Text ):E_BaseException(Text) {};
    explicit E_Solarix( const wchar_t *Text ):E_BaseException( to_wstr(Text) ) {};

    // ����������� � ���������� char* ����������� - ����� �� ���������
    // ������ � �������� ������ ������ � �����-���� ASCII-���������,
    // ��� �������� �� ������ ��������������.

    #if defined LEM_GNUC
    virtual ~E_Solarix(void) throw() {};
    #endif
  };

  // *****************************************
  // ������� ����� ��� RUN TIME ������.
  // *****************************************
  class E_RunTime : public E_Solarix
  {
   public:
    E_RunTime(void):E_Solarix(L"run-time error") {};
    explicit E_RunTime( const std::wstring &Text ):E_Solarix(Text) {};
    explicit E_RunTime( const wchar_t *Text ):E_Solarix(Text) {};
  };

  // ********************************************************************
  // 'Form not found' - ��� ��������� ������ ������������ ��� �� �������
  // ��������� ����� � ��������� ������.
  // ********************************************************************
  class E_SG_NoForm : public E_Solarix
  {
   public:
    E_SG_NoForm(void) {};
  };

  // *******************************************
  // �� ������ �������������� �����
  // *******************************************
  class E_SG_NoClass : public E_ParserError
  {
   public:
    UCString class_name;
    E_SG_NoClass( const UCString &Class_name ):class_name(Class_name) {};
  };

  class E_LoadBinError : public E_RunTime
  {
   public:
    E_LoadBinError(void):E_RunTime( L"Loading from binary file error" ) {};
  };

  class E_AA_Error : public E_RunTime
  {
   public:
    E_AA_Error(void):E_RunTime(L"generic AA error") {};
    explicit E_AA_Error( const std::wstring &Text ):E_RunTime(Text) {};
    explicit E_AA_Error( const wchar_t *Text ):E_RunTime(Text) {};
  };

  // *************************************************************
  // Maximum number of variators reached during the AA iterations
  // *************************************************************
  class E_AA_MaxVars : public E_AA_Error
  {
   public:
    E_AA_MaxVars(void)
     :E_AA_Error(L"maximum number of variators has been reached") {};
  };
 }

#endif
