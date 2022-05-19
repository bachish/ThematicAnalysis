// -----------------------------------------------------------------------------
// File CONSOLE_STREAMS.H
//
// (c) 2005 by Elijah Koziev     all rights reserved 
//
// SOLARIX Intellectronix Project http://www.solarix.ru
//                                http://sourceforge.net/projects/solarix  
//
// Licensed under the terms of GNU Lesser GPL
//
// Content:
// ���������� ���� ����������� ������� ������ �� ������� - mout � merr.
// -----------------------------------------------------------------------------
//
// CD->07.01.2005
// LC->13.08.2006
// --------------

#ifndef LEM_CONSOLE_STREAMS__H
#define LEM_CONSOLE_STREAMS__H
#pragma once

 #include <lem/oformatter.h>

 namespace lem
 {
  // ������ �� ��� ����������� ������ ������...
  class OFormatter;
  extern OFormatter *mout;
  extern OFormatter *merr;
 }

#endif
