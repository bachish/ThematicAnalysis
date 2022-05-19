// -----------------------------------------------------------------------------
// File VECTOR.H
//
// (c) Koziev Elijah
//
// Content:
// ����������������� ������� � ������������������ ������ (�� ��� �����
// �������� �������).
//
// �������� !!! ������ ����� �� ����� �������� ��������� � ������� �� ��������
// �������, � ���������, ������� Vector2d � Vector3d.
//
// ���������� ��������� ��� ������� ������ �������� �����������.
// -----------------------------------------------------------------------------
//
// CD->03.07.1995
// LC->13.08.2006
// --------------

#pragma once
#ifndef LEM_MATH_VECTOR__H
#define LEM_MATH_VECTOR__H

 #include <lem/math/real.h>

 namespace lem
 {

  template < class T >
  class BaseVector
  {
   private:
    int nitems; // ����� ���������.
    T *elem; // �������� �������.

   public:
    // ---------------------------------------
    // ������ ������, �� ���������� ���������.
    // ---------------------------------------
    BaseVector(void)
    {
     nitems=0;
     elem=NULL;
    }


    // -----------------------------------------------------------------------
    // ������� ������ ��������� ������� � �������������� ��� ��������.
    // ���� c==NULL, �� �������� ��������������� ��������. � ��������� ������,
    // ������������ ���������� ������� ���������, �� ������� ��������� c.
    // -----------------------------------------------------------------------
    BaseVector( int N, const T *c=NULL )
    {
     LEM_CHECKIT_Z(N!=0); 

     elem = new T[nitems=N];

     if( !!c )
      // �� ��������� �������������� ���������� ������.
      memcpy( elem, c, sizeof(T)*nitems );
     else
      memset( elem, 0, sizeof(T)*nitems );

     return;
    }

    // ------------------------
    // ����������� �����������.
    // ------------------------
    BaseVector( const BaseVector& v )
    {
     LEM_CHECKIT_Z(v.nitems>0);
     LEM_CHECKIT_Z(v.elem);

     elem = new T[nitems=v.nitems];
     memcpy( elem, v.elem, sizeof(T)*nitems );
     return;
    }


    BaseVector( int N, T c0, ... )
    {
     va_list argptr;

     LEM_CHECKIT_Z(N>0);

     elem = new T[nitems=N];

     REAL next=c0;
     va_start( argptr, c0 );

     for( int i=0; i<nitems; i++ )
      {
       elem[i]=next;
       next=(REAL)va_arg( argptr, REAL );
      }

     va_end(argptr);
     return;
    }

    ~BaseVector(void)
    {
     delete[] elem;
     nitems=0;
     return;
    }

   // ---------------------------------
   // ���������� �������� ������������.
   // ---------------------------------
   BaseVector& operator=( const BaseVector& p )
   {
    if( &v!=this ) // ����� �� x=x !
     {
      if(nitems!=v.nitems) // ���� ������� ������, ������� �����. ���� ������.
       {
        delete[] elem;
        elem = new T[nitems=v.nitems];
       }

      lem_memcpy( elem, v.elem, sizeof(T)*nitems );
     }

    return *this;
   }


   inline int size(void) const { return nitems; }

   // --------------------------------------
   // �������� ������� � ��������� �������
   // � ��������� ������ ������� �� �������.
   // --------------------------------------
   inline T& operator[]( int i )
   {
    LEM_CHECKIT_Z( i>=0 && i<nitems );
    return elem[i];
   }

   inline const T& operator[]( int i ) const
   {
    LEM_CHECKIT_Z( i>=0 && i<nitems );
    return elem[i];
   }

   inline T get( int i ) const
   { LEM_CHECKIT_Z(i>=0 && i<nitems); return elem[i]; }


   // ---------------------
   // ������������ �������.
   // ---------------------
   void normalize(void)
   {
    LEM_CHECKIT_Z(nitems>0);
    LEM_CHECKIT_Z(elem);

    const REAL l=length();
    for( int i=nitems-1; i>=0; i-- )
     elem[i] /= l;

    return;
   }

   // ------------------------------------
   // ������ ������� - ����������
   // ������ �� ����� ��������� ���������.
   // ------------------------------------
   T length(void) const
   {
    LEM_CHECKIT_Z(nitems>0);
    LEM_CHECKIT_Z(elem);

    REAL s=TOREAL(0.);
    for( int i=nitems-1; i>=0; i-- )
     s += qpow2(elem[i]);

    return qsqrt(s);
   }

   // ---------------------------------------------------------------------------
   // �������� �������� ������ �������.
   //
   // ���� ����� �������� ������, ���������� ��������� ����������� (��� �������).
   // ---------------------------------------------------------------------------
   const BaseVector<T> operator+( const BaseVector<T>& b ) const
   {
    const int aord=a.size();

    LEM_CHECKIT_Z(aord==b.size());
    LEM_CHECKIT_Z(aord>0);
    LEM_CHECKIT_Z(a.elem);
    LEM_CHECKIT_Z(b.elem);

    Vector<T> res(aord);

    for( int i=aord-1; i>=0; i-- )
     res.elem[i] = a.elem[i]+b.elem[i];

    return res;
   }


   // -----------------------------------
   // �������� �������� ���������� �����.
   // -----------------------------------
   const BaseVector<T> operator-( const BaseVector<T>& b ) const
   {
    const int aord=a.size();

    LEM_CHECKIT_Z(aord==b.size());
    LEM_CHECKIT_Z(aord>0);
    LEM_CHECKIT_Z(a.elem);
    LEM_CHECKIT_Z(b.elem);

    Vector<T> res(aord);

    for( int i=aord-1; i>=0; i-- )
     res.elem[i] = a.elem[i]-b.elem[i];

    return res;
   }


   // -------------------------------
   // ��������� ������� �� ���������.
   // -------------------------------
   bool operator==( const BaseVector<T> &b ) const
   {
    const int aord=a.size();

    if( aord!=b.size() )
     return false;

    // ���� ���� ���� ������� �� ���������,
    // �� ������� ��������� ����������.

    for( int i=aord-1; i>=0; i-- )
     if( a.elem[i]!=b.elem[i] )
      return false;

    return true;
   }

   // -------------------------------
   // ��������� ����������� ��������.
   // -------------------------------
   bool operator!=( const BaseVector<T> &b ) const
   {
    const int aord=a.size();
    const int bord=b.size();

    if( aord!=bord )
     return true;

    // ���� ���� ���� ������� �� ���������,
    // �� ������� �� ����� � ������� ���������.
    for( int i=aord-1; i>=0; i-- )
     if( a.elem[i]!=b.elem[i] )
      return true;

    return false;
   }

  };


  typedef BaseVector<lem::Math::REAL> RVector;

 } // namespace lem
#endif
