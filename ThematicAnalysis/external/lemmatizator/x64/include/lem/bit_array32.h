// -----------------------------------------------------------------------------
// File BIT_ARRAY32.H
//
// (c) Koziev Elijah
//
// Content:
// ����� BitArray32 - ��������� ��� 32� ����������� ������ � ���� ������
// �������������� ����. ��������� ������� �������� 32 ����.
//
// ��. ����� ����� BitVector � ����� lem_bitv.h, ������� ����� �������
// ������������ ���������� �����.
// -----------------------------------------------------------------------------
//
// CD->06.10.1995
// LC->16.10.2009
// --------------

#ifndef LEM_BITA32__H
#define LEM_BITA32__H
#pragma once

 #include <boost/integer/integer_mask.hpp>
 #include <lem/containers.h>
 #include <lem/streams.h>

 namespace lem
 {

  // **************************************************
  // ����. ����� �����, �����. � ���� BitArray32
  // **************************************************
  const int LEM_MAXCATEGORY=32;

  class BitArray32
  {
   protected:
    lem::uint32_t a; // ������� ����.

   public:
    BitArray32(void) { a=0; }

    inline void SetData( lem::uint32_t d ) { a=d; }
    inline lem::uint32_t GetData(void) const { return a; }

    /**************************************
     ���������� ���������� i-�� ����.
    ***************************************/
    inline bool Get( int i ) const
    {
     LEM_CHECKIT_Z( i>=0 && i<LEM_MAXCATEGORY );
     //return !!(a & (boost::high_bit_mask_t<31>::high_bit_fast>>i));
     return (a & (0x80000000U>>i))!=0;
    }

    inline bool operator()( int i ) const { return Get(i); }

    /*******************************************************
     ������������� ���������� i-�� ���� � ��������� do_set.
    ********************************************************/
    inline void Set( int i, bool b=true )
    {
     LEM_CHECKIT_Z( i>=0 && i<LEM_MAXCATEGORY );

     if(b)
      a |= (boost::high_bit_mask_t<31>::high_bit_fast>>i);  // ������������� ���...
     else
      a &= ~(boost::high_bit_mask_t<31>::high_bit_fast>>i); // ������� ���...

     return;
    }

    inline void operator|=( const BitArray32 bits )
    { a |= bits.a; }

    inline bool operator==( const BitArray32 ba ) const
    { return ba.a==a; }

    inline bool operator!=( const BitArray32 ba ) const
    { return ba.a!=a; }

    /**********************************************************
     ���������� ������ �������� ��������� ��� ������� �����,
     � ����������� �� �������� ��������� f
    ***********************************************************/
    const IntCollect GetIndeces( bool f=true ) const
    {
     IntCollect res;

     for( int i=0; i<LEM_MAXCATEGORY; i++ )
      if( Get(i)==f )
       res.push_back( i );

     return res;
    }

    // ���������� ������������ ������� ���������� ���� (������)
    int MaxBit(void) const
    {
     for( int i=LEM_MAXCATEGORY-1; i>=0; i-- )
      if( Get(i)==true )
       return i;

     return -1;
    }
  };
 } // namespace 'lem'

#endif
