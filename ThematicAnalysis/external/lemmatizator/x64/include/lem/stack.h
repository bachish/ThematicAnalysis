// -----------------------------------------------------------------------------
// File STACK.H
//
// (c) Koziev Elijah
//
// Content:
// ������ Stack - ��������� ���� '����' ��� ������� � ������������
// ������������� �����������.
//
// ����������� �� ��������� � STL stack ���������������� - ���������
// ��������� ��������, �� ���� �������� �� ������ ������ Collect'� (�������
// �������, ��� �� ������� STL).
// -----------------------------------------------------------------------------
//
// CD->06.10.1994
// LC->25.04.2007
// --------------

#ifndef LEM_STACK__H
#define LEM_STACK__H
#pragma once

 #include <lem/containers.h>

 namespace lem
 {
 
  template<class ITEM> class Stack : public Collect<ITEM>
  {
   public:
    Stack(void) {}

    #if defined __MINGW__ || defined LEM_GNUC
     inline typename Collect<ITEM>::size_type size(void) const { return Collect<ITEM>::size(); }
     inline const typename Collect<ITEM>::value_type& back(void) const { return Collect<ITEM>::back(); }
     inline typename       Collect<ITEM>::value_type& back(void)       { return Collect<ITEM>::back(); }
    #endif 
  
    // ************************************************************
    // ������ ����� ����������� �� 1.
    // � ������� �� ������� std::stack::pop(), ���������� ������
    // � ������� ����� �������.
    // ************************************************************
    inline const ITEM pop(void)
    {
     LEM_CHECKIT_Z(size()>0);
     const ITEM x = back();
     #if defined LEM_MSC && _MSC_VER<=1200
     ReduceBy(1);
     #else 
     lem::Collect<ITEM>::ReduceBy(1);
     #endif
     return x;
    }

    // ***********************************************
    // ����� ������� ND ��������� ��������� �� �����.
    // ***********************************************
    inline void drop( int ND )
    {
     LEM_CHECKIT_Z(ND>=0);
     #if defined LEM_MSC && _MSC_VER<=1200
     ReduceBy(ND);
     #else
     lem::Collect<ITEM>::ReduceBy(ND);
     #endif
     return;
    }

    // ***********************************************
    // ���������� �������� �� ������� �����.
    // ***********************************************
    inline void push( const ITEM& item )
    {
     #if defined LEM_MSC && _MSC_VER<=1200
     push_back(item);
     #else
     lem::Collect<ITEM>::push_back(item);
     #endif
     return;
    }

    //***********************************************************
    // ���������� ������ �� �������, ��������� �� depth �������
    // �� ��������. ���� depth=0, �� - ������ �� ��������.
    //***********************************************************
    inline ITEM& top( int depth=0 )
    {
     LEM_CHECKIT_Z( int(size())-1-depth>=0);
     return lem::Collect<ITEM>::at(size()-1-depth);
    }

    inline const ITEM& top( int depth=0 ) const
    {
     LEM_CHECKIT_Z( int(size())-1-depth>=0);
     return lem::Collect<ITEM>::at(size()-1-depth);
    }

//    inline const ITEM& get( int i ) const { return operator[](i); }
//    inline       ITEM& get( int i )       { return operator[](i); }
  };

 } // end of namespace 'lem'

#endif
