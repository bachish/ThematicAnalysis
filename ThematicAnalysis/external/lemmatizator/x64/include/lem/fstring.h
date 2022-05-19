#ifndef LEM_FSTRING__H
#define LEM_FSTRING__H
#pragma once

// -----------------------------------------------------------------------------
// File FSTRING.H
//
// (c) by Elijah Koziev   all rights reserved 
//
// SOLARIX Intellectronix Project http://www.solarix.ru
//                                http://sourceforge.net/projects/solarix  
//
// Content:
// BaseFString<T> UNICODE mutable strings template class.
// ������ �������� � �������������� ���������� ������������ ������.
//
// ��. �����: ����� BaseCString ("lem_cstr.h")
//
// 19.08.2004 - ��������� �������������� ������ ��� ���������� ������. ��� ����
// �� ��������� ������������ � ��������� ������������ �������� ������ �������,
// ������� ����� ����� ��� �������� ������ (���� ����������). ������ ���
// ������ ��������� += ���������� ��������� ������ � �������, ����� ���������
// ���������������� ������� �������� ������������� ����������.
//
// ��������� ������������ ������� - �� ���� � �� ���� �����.
//
// 27.09.2004 - ��������� �������� front() � back()
// 01.11.2004 - ���������� ������ ������, ����������� � ������������ ������,
//              � subst(...) 
//
// 13.12.2004 - ������� ������ SaveBin � LoadBin, ����� ���� ������ ��������
//              � ������������ (���������� � �������� Collect<FString>). 
//
// 26.01.2005 - ��������� ��� ���� � insert(...)
//
// 17.09.2005 - ��� ��� ������� realloc � MSVS ������ ��� ��������� NULL �����,
//              ����������� �������� �������� � malloc
//
// 07.01.2006 - ��� ������� ����������� ����� Guardian (��. runtime_checks.h).
//
// 23.03.2008 - �������� ������-�������� StringTypeSelector ��� ������
//              ���������� ���� � ����������� �� ����������� ����� ����������
//              UNICODE/ASCII
// -----------------------------------------------------------------------------
//
// CD->02.07.1996
// LC->26.05.2008
// --------------

#include <lem/config.h>

#if defined LEM_MFC
 #include <afxwin.h>
#endif

#if defined LEM_WINDOWS
 #include <tchar.h>
#endif

#include <lem/afstring.h>
#include <lem/ufstring.h>

namespace lem
{
 template < typename CHAR >
 struct StringTypeSelector
 {
 };

 template <>
 struct StringTypeSelector<char>
 {
  typedef lem::FString string_type;
  typedef char char_type;
 };

 template <>
 struct StringTypeSelector<wchar_t>
 {
  typedef lem::UFString string_type;
  typedef wchar_t char_type;
 };

 #if defined LEM_WINDOWS && defined LEM_MSC && defined _T
  typedef StringTypeSelector<TCHAR>::string_type TString;
 #elif defined UNICODE || defined _UNICODE
  typedef UFString TString;
 #else
  typedef FString TString;
 #endif
}

#endif
