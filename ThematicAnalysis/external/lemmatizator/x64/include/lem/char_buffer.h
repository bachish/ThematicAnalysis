// CD->24.01.2007
// LC->04.09.2007
// --------------

#ifndef LEM_CHAR_BUFFER__H
#define LEM_CHAR_BUFFER__H
#pragma once

 #include <lem/config.h>

 #if LEM_DEBUGGING==1
  #define ASSERTIT Assert()
 #else
  #define ASSERTIT 
 #endif

 #include <boost/noncopyable.hpp>
 #include <lem/runtime_checks.h>

 namespace lem
 {
  // *********************************************************************
  // ��������� ����� StaticBuffer<T>(n) ������������� ������������ T[n],
  // �� ���������� ����� ��������� �����������:
  // 1) ������ ���������� �� �� �����, � � ������������ ����;
  // 2) ���������� ��������� ������ �� memory overrun
  // 3) ����������� �������������� ������ ������
  // *********************************************************************
  template < class T >
  class StaticBuffer : boost::noncopyable
  {
   private:
    T* ptr; // ���������� ���� � ������������ ����
    int n;  // ������� ��������� ����������� � ������.

   public:
    // ����������� �������� ����� ������� ������� � ��������������
    // ��� ������.
    explicit StaticBuffer( int N ) : ptr( new T[N+1] ), n(N)
    {
     LEM_CHECKIT_Z(N>=0);
     memset( ptr, 0, sizeof(T)*(N+1) );
     ptr[n] = (T)0xa5; // GUARDIAN ITEM
     return;
    }

    ~StaticBuffer(void)
    {
     ASSERTIT;
     delete[] ptr;
     #if LEM_DEBUGGING==1
      ptr=NULL;
     #endif
    }

    inline       T* get(void)       { ASSERTIT; return ptr; }
    inline const T* get(void) const { ASSERTIT; return ptr; }

    inline       operator T*(void)       { ASSERTIT; return ptr; }
    inline operator const T*(void) const { ASSERTIT; return ptr; }

    inline T* c_str(void)       { ASSERTIT; return ptr; }
    inline const T* c_str(void) const { ASSERTIT; return ptr; }

    inline       T& operator[]( int i )       { ASSERTIT; LEM_CHECKIT_Z(i<n); return ptr[i]; }
    inline const T& operator[]( int i ) const { ASSERTIT; LEM_CHECKIT_Z(i<n); return ptr[i]; }

    inline size_t Size(void) const { return n; }
    inline void Clear(void) { ASSERTIT; memset( ptr, 0, sizeof(T)*n ); }

    #if LEM_DEBUGGING==1
    inline void Assert(void) const
    {
     const unsigned int guardian = 0x000000ffU & ptr[n];
     if( guardian!=0xa5 )
      LEM_STOPIT;
    }
    #else
    inline void Assert(void) const {}
    #endif
  };


  typedef StaticBuffer<char>    CharBuffer;
  typedef StaticBuffer<wchar_t> WCharBuffer;
 }

#endif
