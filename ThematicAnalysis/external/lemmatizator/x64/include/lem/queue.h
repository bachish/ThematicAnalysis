// -----------------------------------------------------------------------------
// File QUEUE.H
//
// (c) Koziev Elijah
//
// Content:
// ����� Queue - ������������� ������-��������� ��� ���������, �������� ���
// �������. ����� ������, �������, ���������� ������, ������ �� � �����������
// (FIRST IN - FIRST OUT, FIFO). ��� ��� ������ ������ �������� �� ������ ������
// �������������� ����������, �� ����� ���������� � ����� ��������� �������.
// -----------------------------------------------------------------------------
//
// CD->24.10.1997
// LC->16.08.2007
// --------------

#ifndef LEM_QUEUE__H
#define LEM_QUEUE__H
#pragma once

 #include <lem/containers.h>

 namespace lem
 {
  class Stream;

  template <class ITEM> class Queue : public Collect<ITEM>
  {
   public:
    #if defined LEM_GNUC
    typedef typename Collect<ITEM>::size_type size_type; 
    #endif 
  
   public:
    Queue( int n0=0 ):Collect<ITEM>(n0) {}

    /******************************************************
     ��������� �������� � ����� �������.
    *******************************************************/
    inline void push( const ITEM &to_add )
    { Collect<ITEM>::push_back(to_add); }

    /********************************************************************
     ���������� ������ �������� �� ������ �������, �� ���� �����������
     � ��� ������ ����. ������� ����� ���������� ����������, �����
     �������� ����������� �� 1, �� ����� ������ ����������� ����� ������
     �� ����������.
    *********************************************************************/
    const ITEM pop(void)
    {
     LEM_CHECKIT_Z(! Collect<ITEM>::empty() );
     const ITEM res = Collect<ITEM>::front();
     
     for( size_type i=0; i< Collect<ITEM>::size()-1; i++ )
      Collect<ITEM>::operator[](i) = Collect<ITEM>::operator[](i+1);

     Collect<ITEM>::resize( Collect<ITEM>::size()-1 );
     return res;
    }
  };

 } // end of namespace 'lem'

#endif
