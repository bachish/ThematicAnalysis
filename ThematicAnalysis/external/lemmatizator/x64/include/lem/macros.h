// -----------------------------------------------------------------------------
// File MACROS.H
//
// (c) Koziev Elijah
//
// Content:
// ����� ������������ �������-���������� � ���������. ����������� ������� ���
// ����� ������������ �������������� ����������� C++.
//
// 11.05.2008 - ������ ������ CastSizeToInt ��� �������������� size_t � int,
//              � ��������� ������������ � Debug ��� x64.
// -----------------------------------------------------------------------------
//
// CD->03.10.1996
// LC->11.05.2008
// --------------

#ifndef LEM_DEFS___H
#define LEM_DEFS___H
#pragma once

 #include <lem/config.h> 

 #include <assert.h>


 /************************************************************************
  Useful macro to count elements in an array.
  ������ ��� ����������� ����� ��������� � �������. ������ ��� ��������,
  ��� ������ �������� �����������, for instance: int a[10];
 ************************************************************************/
 #if !defined DIM_A
  #define DIM_A(a) (sizeof(a)/sizeof(a[0]))
 #endif

 // "������" ����, ����������� �� ����������� � ���� �����.
 #if !defined FOREVER
  #define FOREVER while(1)
  #define LEM_FOREVER while(1)
 #endif

 #if !defined(LEM_DEBUGON)
  #if defined LEM_DEBUGGING
   #if LEM_DEBUGGING==0
    // ...
   #else
    #define LEM_DEBUGON

   #undef IFDEBUG
    #define IFDEBUG(A) (void)0
    #endif
  #endif
 #else
  #undef IFDEBUG
  #define IFDEBUG(A) A
 #endif

 #if !defined LEM_DEBUGON
  #define LEM_DEBUGOFF
  #undef IFDEBUG
  #define IFDEBUG(A) (void)0
 #endif

 // Memory deallocation macros. Also they set memory pointer to NULL,
 // that makes the debugging process easier, as we can only do 'free' once.
 #define lem_rub_off(ptr) { delete ptr; ptr=NULL; }
 #define lem_rub_off_array(ptr) { delete[] ptr; ptr=NULL; }
 #define lem_free_z(ptr) { lem_free(ptr); ptr=NULL; }

 // ��� ����� ������� � ���������� ������ ���������� ������ for(...)

 // ***************************************************************
 // GNU C+ compiler often complains about non-virtual destructors
 // within a class having virtual methods.
 // ***************************************************************
 #if defined LEM_GNUC
  #define LEM_GNUC_VIRTUAL virtual
 #else
  #define LEM_GNUC_VIRTUAL
 #endif

 #define free_z(x) { free(x); x=0; }


 #if defined LEM_MSC
  #define LEM_NO_RET(arg) return arg
 #elif defined BOOST_UNREACHABLE_RETURN
  #define LEM_NO_RET(arg) BOOST_UNREACHABLE_RETURN(arg)
 #elif defined LEM_BORLAND
  #define LEM_NO_RET(arg) (void)(arg)
 #else
  #define LEM_NO_RET(arg) return arg
 #endif

 // ������ ������������ ��� ��������� ���������� � �������������������� ��
 // ���������� ����� ���� size_t � int ��� ������� ����������� � �.�.
 #if LEM_DEBUGGING==1 || defined _DEBUG
  inline int CastSizeToInt( size_t x )
  {
   assert( (x & 0x80000000) != 0x80000000 );
   return (int)x;
  }
 #else
  // � �����-������ ������ ������������ ��� ��������� ��������
  #define CastSizeToInt(x) ((int)(x))
 #endif


#endif
