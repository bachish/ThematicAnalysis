// -----------------------------------------------------------------------------
// File QARRAY.H
//
// (c) Koziev Elijah
//
// Content:
// ����� BaseArray - ������-������, ������ �������� ��� ������ � ���������������
// ��������� �����. ��������� �������� ��������� �������� ������� ���� (� ��
// �������� ��� ������ 0, ��� ��� ������ �����������).
//
// ����� QCArray - ��������� � ����������� ���������� �������� ���������.
// -----------------------------------------------------------------------------
//
// CD->21.05.1996
// LC->19.04.2007
// --------------

#ifndef LEM_BaseArray__H
#define LEM_BaseArray__H
#pragma once

 #include <lem/containers.h>

 namespace lem
 {

 /****************************************************************************
  ����� � �������� �������������� ���������� �� ���������� � ������������
  �������, � ���� �������� �������� ����������� ������, ����� ������������
  ����� BaseArray (��� ����������� �� ����), ������� ������������� ��� �����
  � ���������� �����. ��� ����� LemBaseArray �������� ����������� MCollect �
  ������������� ����� ������� �������� ����� ��������� ������� ��
  ������������� ��������� ������ �������.

  !!! �� ������������� ������� ���� ITEM ��� �� ���������� ���� ������������
      ������. ����� ����, ���������, ��� ������� �������� ���������� �������
      ���������� ���� ����� ���� ITEM. ��������� �������� ����� MCollect
      � ����� [lem_marr.h].
 ****************************************************************************/
 template <class ITEM>
 class BaseArray : public MCollect<ITEM>
 {
  protected:
   int i0,in; // ����� � ������ ��������� �������. ���� �������� i0 � in
              // ����� �������� ����������� ���������.

   inline void Init( int I0, int In, const ITEM *A=NULL )
   {
    LEM_CHECKIT_Z( I0>=0 && In>=0 );

    if( I0>In )
     {
      // ������� ��������� ������ ������ - �� �� ��������
      // �� ������ ��������. ������� ��������, ��� ��� ������
      // Init( 0, 0, NULL ) ������ ����� �������� �� ������ ��������.
      // ����� �������� ������ ������, ���� ������ ����� Init( 0, -1, NULL )
      i0=I0;
      in=I0-1;
     }
    else
     {
      // ����� ����� ��������� � �������.
      const int N = (in=In) - (i0=I0) + 1;

      // �������� ����������� �����.
      AddAll(N);

      if( A!=NULL )
       {
//        LEM_LOOP( i, N )
        ::memcpy( (void*) MCollect<ITEM>::get_Container(), (const void*)A, (size_t)sizeof(ITEM)*N );
       }
      else
       {
        // �������� ���.
        MCollect<ITEM>::Nullify();
       }
     }

    return;
   }

/*
   inline void Init( const BaseArray<ITEM>& q )
   {
    MCollect<ITEM>::Init(q);

    i0 = q.i0;
    in = q.in;

    return;
   }
*/

   inline int norm( int i ) const
   {
    LEM_CHECKIT_Z( i>=i0 && i<=in );
    return i-i0;
   }

  
  public:
   #if defined LEM_GNUC
   typedef typename MCollect<ITEM>::size_type size_type;
   #else
   typedef MCollect<ITEM>::size_type size_type;
   #endif  

   #if defined __MINGW__ || defined LEM_GNUC
   inline size_type size(void) const { return MCollect<ITEM>::size(); }
   #endif    
  
  
  public:
   BaseArray(void):MCollect<ITEM>()
   {
    Init(0,0,NULL);
    Again();
    return;
   }

   /***************************************************************************
    ������ �������� �� ���� �����������: �� ��������� ����� ������
    ��������� �������� ������� ��-�� ������������. ����, ����� �����
    ��������� ������������ � ��������� ������� ����� ����������.

    ����������� ������ ������ ���������, ������� ������ ���������� � From
    � ���� �� To ������������. ��������� �� �������� i<From � i>To �����
    ��������� ������������ � �������� ������� ������ �����������. �������
    �������� A ��������� �� ������ ���������������� ��������. ���� A=NULL,
    �� ������������� �� ������������. ������� ��������, ��� ��� �������������
    ������� � �������� From ����� ���������� ��� A[0], ������� From+1 ���
    A[1], � ��� �����. ������� ��� ��������� ��������� �������� ��� ����������
    A, � ����� ���������� ���. ����� ����, A ����� ��������� �� ������
    ����������� ������.
   ****************************************************************************/
   BaseArray( int I0, int In, const ITEM *A=NULL )
   :MCollect<ITEM>()
   { Init(I0,In,A); }

   // *************************************************************************
   // ������������ �������� �� ����������� ������������ �������� ����������
   // ��������� ��������� - �� 0 �� NI, ��� ��� NI ������ ���������� ���������
   // � �������.
   // *************************************************************************
   BaseArray( int NI, const ITEM *A=NULL )
   :MCollect<ITEM>()
   { Init(0,NI-1,A); }

   // ******************************************************************
   // ��������� ������ ��������� � �������� ���������� �������� � ������
   // ������� �������� ��������� �������� BY.
   // ******************************************************************
   BaseArray( int I0, int In, const ITEM& BY )
   :MCollect<ITEM>()
   {
    Init(I0,In,NULL);

    for( int i=0; i<size(); i++ )
     MCollect<ITEM>::operator[](i) = BY;

    return;
   }

   BaseArray( const BaseArray<ITEM>& q )
   :MCollect<ITEM>(q)
   {
    i0 = q.i0;
    in = q.in;
   }

   BaseArray<ITEM>& operator=( const BaseArray<ITEM>& q )
   {
    MCollect<ITEM>::operator=(q); 
    i0 = q.i0;
    in = q.in;
    return *this;
   }

   /***************************************************
    ���� ��������� ������� ������������� �������� k.
   ****************************************************/
   BaseArray<ITEM>& operator=( const ITEM& a )
   {
    #if defined LEM_GNUC
    typedef typename BaseArray<ITEM>::size_type SZ;
    #else
    typedef BaseArray<ITEM>::size_type SZ;
    #endif  
   
    for( SZ i=0; i<size(); i++ )
     operator[](i)=a;

    return *this;
   }

  inline int GetFrom(void) const { return i0; }
  inline int GetTo(void)   const { return in; }

  // �������� ���������� �������� ��������������, ��� ��� � ��� ���� ����
  // ���� ���������� �������� ������� 'in'.
  inline void Add( const ITEM& a )
  {
   MCollect<ITEM>::push_back(a);
   in++;
   return;
  }

//  inline BaseArray<ITEM>& operator<<( const ITEM& a )
//  {
//   Add(a);
//   return *this;
//  }

//  inline BaseArray<ITEM>& operator<<( const BaseArray<ITEM>& a )
//  {
//   ReshapeBy(a.size() );
//   LEM_LOOP( i, a.size() )
//    Add(a(i));
//
//   return *this;
//  }

  inline void Remove( int i )
  {
   MCollect<ITEM>::Remove(i-i0);
   in--;
   return;
  }

  // ***********************************************************************
  // � ������� ����� ������ ����� ������� ������ ������, ������ �� ����������
  // ���������. ������� ��������, ��� �� ��������� ����������� ������� ������
  // � ����� ���������!
  // ***********************************************************************
  inline void Again(void)
  {
   MCollect<ITEM>::clear();
   in=i0-1;
   return;
  }

  inline void AddAll( int N )
  {
   MCollect<ITEM>::resize( size() + N );
   // �������, ��� i0 �� ��������.
   in=i0+N-1;
   return;
  }

  inline ITEM& operator[]( int i ) { return MCollect<ITEM>::operator[](norm(i)); }
  inline const ITEM& operator[]( int i ) const { return MCollect<ITEM>::operator[](norm(i)); }

  inline const ITEM& get( int i ) const { return MCollect<ITEM>::operator[](norm(i)); }
  inline const ITEM& operator()( int i ) const { return MCollect<ITEM>::operator[](norm(i)); }

  inline bool operator==( const BaseArray<ITEM> &x ) const
  {
   if( i0!=x.i0 || in!=x.in )
    return false;

   for( int i=0; i<size(); i++ )
    if( MCollect<ITEM>::operator[](i) != static_cast<const MCollect<ITEM>&>(x)[i] )
     return false;

   return true;
  }

  inline bool operator!=( const BaseArray<ITEM> &x ) const
  {
   if( in!=x.in || i0!=x.i0 )
    return true;

   for( int i=0; i<size(); i++ )
    if( MCollect<ITEM>::operator[](i) != static_cast<const MCollect<ITEM>&>(x)[i] )
     return true;

   return false;
  }

  /***************************************************
   ��� �������� ������� ����������� �� ����������� k.
  ****************************************************/
  inline BaseArray<ITEM>& operator*=( const ITEM& k )
  {
   for( int i=0; i<size(); i++ )
    MCollect<ITEM>::operator[](i) *= k;

   return *this;
  }

  /***************************************************
   ��� �������� ������� ������� �� ����������� k.
  ****************************************************/
  inline BaseArray<ITEM>& operator/=( const ITEM& k )
  {
   for( int i=0; i<size(); i++ )
    MCollect<ITEM>::operator[](i) /= k;

   return *this;
  }

  /***************************************************
   � ������� �������� ������� ������������ k.
  ****************************************************/
  inline BaseArray<ITEM>& operator+=( const ITEM& k )
  {
   for( int i=0; i<size(); i++ )
    MCollect<ITEM>::operator[](i) += k;

   return *this;
  }

  /***************************************************
   �� ������� �������� ������� ���������� k.
  ****************************************************/
  BaseArray<ITEM>& operator-=( const ITEM& k )
  {
   for( int i=0; i<size(); i++ )
    MCollect<ITEM>::operator[](i) -= k;

   return *this;
  }

  /***************************************************************************
   �������� ������� s ������������ � ��������������� ��������� ������ �������.
   ������������ ��������� ������ �������� ��� ����� ��������.
  ****************************************************************************/
  BaseArray<ITEM>& operator+=( const BaseArray<ITEM> &s )
  {
   LEM_CHECKIT_Z( s.i0==i0 && s.in==in );

   for( int i=0; i<size(); i++ )
    MCollect<ITEM>::operator[](i) += static_cast<const MCollect<ITEM>&>(s)[i];

   return *this;
  }

  /****************************************************************************
   �������� ������� s ���������� �� ��������������� ��������� ������ �������.
   ������������ ��������� ������ �������� ��� ����� ��������.
  ****************************************************************************/
  BaseArray<ITEM>& operator-=( const BaseArray<ITEM> &s )
  {
   LEM_CHECKIT_Z( s.i0==i0 && s.in==in );

   for( int i=0; i<size(); i++ )
    MCollect<ITEM>::operator[](i) -= static_cast<const MCollect<ITEM>&>(s)[i];

   return *this;
  }

  inline const BaseArray<ITEM> operator*( const ITEM& k ) const
  {
   BaseArray<ITEM> b(*this);

   for( int i=0; i<b.size(); i++ )
    static_cast<const MCollect<ITEM>&>(b)[i] *= k;

   return b;
  }


  /*************************************************************
   ��������� ������������ ���� �������� - ����������� �������
   �����. ������������ ������������ ������ �������� ��� �����
   ��������.
  **************************************************************/
  const ITEM operator*( const BaseArray<ITEM> &b ) const
  {
   LEM_CHECKIT_Z(b.GetFrom()==GetFrom());
   LEM_CHECKIT_Z(b.GetTo()==GetTo());

   ITEM res(0);

   for( int i=0; i<size(); i++ )
    res += MCollect<ITEM>::operator[](i) * static_cast<const MCollect<ITEM>&>(b)[i];

   return res;
  }



  const BaseArray<ITEM> operator/( const ITEM& k ) const
  {
   BaseArray<ITEM> b(*this);

   for( int i=0; i<size(); i++ )
    static_cast<const MCollect<ITEM>&>(b)[i] /= k;

   return b;
  }
 };

  /***********************************************************
   ��������� � ���������� ����� ���� ��������� ������� a. ����
   ������ a ����, �� ����������� ����� 0.
  ************************************************************/
  template<class ITEM>
  inline const ITEM sum_up( const BaseArray<ITEM> &a )
  {
   ITEM s(0);

   for( int i=0; i<a.size(); i++ )
    s += static_cast<const MCollect<ITEM>&>(a)[i];

   return s;
  }


  #if !defined LEM_NOREAL
  typedef BaseArray<double> RArray;
  #endif // !defined SOL_NOREAL

  typedef BaseArray<int>  IntArray;
  typedef BaseArray<bool> BoolArray;

 } // namespace 'lem'

#endif
