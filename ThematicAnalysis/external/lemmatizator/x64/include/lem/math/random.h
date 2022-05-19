// -----------------------------------------------------------------------------
// File RANDOM.H
//
// (c) Koziev Elijah
//
// Content:
// ���������� ��������� ��������������� ����� � �������������� ���������� ��
// ���������. ����� ���������� �� ������ ��������� � ���������� ��������
// �������������.
//
// 25.04.2005 - ��������� ���������������� - ��� ������������� � Boost.Random
// 18.09.2005 - ����� �������� �� Boost 1.32 ����� ������� ������� min � max,
//              ������� �������� �� undef'���   
// -----------------------------------------------------------------------------
//
// CD->18.01.1996
// LC->13.09.2007
// --------------

#ifndef LEM_RANDU__H
#define LEM_RANDU__H
#pragma once

 #include <lem/integer.h>
 #include <lem/streams.h>
 #include <lem/qarray.h>
 #include <lem/math/math_constants.h>
 
 #undef min
 #undef max

 namespace lem
 {
  namespace Math
  {
 /*
   ���������� ���������������� ���������� - ����������� �������� ���������
   ���������� ���������� ����������� � ���������. ���, ����������, �����
   ��������� �������� ���������, �� ���� ��������� ����������� ������������
   INLINE �������, �� ��� ������ �������� ���������.
 */

 // ������������ ����������������� (�������� ������������ �����) ���������.
 // ��������� - ���������� �������������� ����� �����.
 class RandU
 {
  protected:
   static int Number; // ������� ����������� �����������. ������ ���������
                      // ���������������� ��-������.

   lem::uint32_t HALFM,IA,IC; // ��������� �������������� ��������� ����������.
   lem::uint32_t LASTY;       // ��������� ������������ ��������� �����.

   
   #if !defined LEM_NOdouble
   double S; // ��������� ��� ����������� � �������������� �����
   #endif

  public:
   typedef lem::uint32_t result_type;

//   static result_type min_value;
//   static result_type max_value;
   enum { max_value=0xffffffffUL, min_value=0UL };

  public:
   // *********************************************************************
   // �������� ������������ ��� �������� ����������������� ����������� (���
   // ������� n ����� ���� ������������������.
   // *********************************************************************
   RandU( int n=-1 );

   // ��������� ����������
   inline void seed( lem::uint32_t X ) { LASTY=X; }

   // ��������� ��������� ��������������� ����� ������ ����.
   inline RandU::result_type operator()(void) { return (LASTY=LASTY*IA+IC); }

   #if !defined LEM_NOdouble
   // ��������� ��������� ��������������� ����� ������������� ����.
   inline double aget(void) { return S*(LASTY=LASTY*IA+IC); }

   void Generate( int NR, double *dest );
   #endif

   // ������������ ��������� ����� � ��������� 0...bound_max-1
   inline RandU::result_type iget( int bound_max ) 
   {
    LEM_CHECKIT_Z( double( result_type(max_value) ) > 0 );
    return result_type( aget()*bound_max );
   } 

   inline RandU::result_type min(void) const { return min_value; }
   inline RandU::result_type max(void) const { return max_value; }

   void SaveBin( Stream& bin ) const;
   void LoadBin( Stream& bin );
 };


 #if !defined LEM_NOdouble
 /*
   ��� ��������� ��������� �������������� ����� � ��������������
   ��������� �� ����������� ���������� �������: ����� �������� ����������
   ���������� (� ������ �����������) �������������� ����� �����������
   ����������� ������ �������������.
 
   ������ 6 ����������� ������ �������.
 */
 class GaussianRandU : protected RandU
 {
  private:
   double middle, sigma, A, n2;
   int n;

  public:
   typedef double result_type;

  public:
   GaussianRandU(void);
   GaussianRandU( double M, double Sigma, int N=6 );
   result_type aget(void);
   inline result_type operator()(void) { return aget(); }

   void SaveBin( Stream& bin ) const;
   void LoadBin( Stream& bin );
 };

 //**************************************************************************

 class RandErmacov
 {
  private:
   double M,P,N,LASTY;

  public:
   typedef double result_type;

  public:
   RandErmacov( int igen=-1 );
   result_type aget(void);
   inline result_type operator()(void) { return aget(); }

   inline RandU::result_type min(void) const { return 0; }
   inline RandU::result_type max(void) const { return 1; }

   void SaveBin( Stream& bin ) const;
   void LoadBin( Stream& bin );
 };

 inline double RandErmacov::aget(void)
 {
  LASTY = (LASTY*M)/P;
  LASTY = (LASTY-floor(LASTY))*P;
  return LASTY/N;
 }

 class GaussianRnd : protected RandErmacov
 {
  private:
   double middle, sigma, A, n2;
   int n;

  public:
   typedef double result_type;

  public:
   GaussianRnd(void) { middle=sigma=A=n2=n=0; }
   GaussianRnd(
               double M,
               double Sigma,
               int igen=-1,
               int N=6
              );
   result_type aget(void);
   inline result_type operator()(void) { return aget(); }

   void SaveBin( Stream& bin ) const;
   void LoadBin( Stream& bin );
 };

 class Chi2RndU : public RandU
 {
  public:
   typedef double result_type;

  public:
   Chi2RndU(void);

   result_type aget(void);
   inline result_type operator()(void) { return aget(); }
 };


 // ***************************************************
 // ����� ��� �������, ������������� ����� � ��������
 // �������� ��������������.
 // ***************************************************
 class AnyRnd : public RandU
 {
  private:
   RArray F;
   int i0,in;
   double dx, a, b, ab;

  public:
   typedef double result_type;

  public:
   AnyRnd(void);
   AnyRnd( const RArray& P, double A=0., double B=1. );
   AnyRnd( const AnyRnd& ar );

   void operator=( const AnyRnd& ar );

   result_type aget(void);
   inline result_type operator()(void) { return aget(); }
 };
 #endif

 // ----------------------------------------------------------------

 #if !defined LEM_NOdouble
 class Rnd1
 {
  private:
   unsigned long ix1, ix2, ix3;
   unsigned long M1, IA1, IC1, M2, IA2, IC2, M3, IA3, IC3;
   double RM1, RM2;
   double r[98];

  public:
   typedef double result_type;

  public:
   Rnd1( int seed=-1 );

   result_type aget(void);
   inline result_type operator()(void) { return aget(); }

   inline RandU::result_type min(void) const { return 0; }
   inline RandU::result_type max(void) const { return 1; }

   void SaveBin( Stream& bin ) const;
   void LoadBin( Stream& bin );
 };

 class GaussianRnd1 : protected Rnd1
 {
  private:
   double middle, sigma, A, n2;
   int n;

  public:
   typedef double result_type;

  public:
   GaussianRnd1(void) { middle=sigma=A=n2=n=0; }
   GaussianRnd1( double M, double Sigma, int N=6 );

   result_type aget(void);
   inline result_type operator()(void) { return aget(); }

   void SaveBin( Stream& bin ) const;
   void LoadBin( Stream& bin );
 };


 class SubRnd
 {
  private:
   long MBIG;
   double FAC;
   int inext,inextp;
   long ma[56];

  public:
   typedef long result_type;

   BOOST_STATIC_CONSTANT( result_type, min_value = lem::int32_min );
//   static const result_type min_value = boost::integer_traits<long>::const_min;

   BOOST_STATIC_CONSTANT( result_type, max_value = lem::int32_max );
//   static const result_type max_value = boost::integer_traits<long>::const_max;

  public:
   SubRnd( long idum=1 );

   // ���������� ��������� ��������������� �����.
   inline double aget(void)
   {
    if( ++inext==56)
     inext=1;

    if( ++inextp==56 )
     inextp=1;

    long mj = ma[inext] - ma[inextp];
    if( mj<0 )
     mj += MBIG;

    ma[inext] = mj;
    return FAC*mj;
   }

   inline long iget(void)
   {
    if( ++inext==56)
     inext=1;

    if( ++inextp==56 )
     inextp=1;

    long mj = ma[inext] - ma[inextp];
    if( mj<0)
     mj += MBIG;

    ma[inext] = mj;
    return mj;
   }

  inline result_type operator()(void) { return iget(); }

  inline RandU::result_type min(void) const { return min_value; }
  inline RandU::result_type max(void) const { return max_value; }

  void SaveBin( Stream& bin ) const;
  void LoadBin( Stream& bin );
 };

 class GaussianSubRnd : protected SubRnd
 {
  private:
   double middle, sigma, A, n2;
   int n;

  public:
   typedef long result_type;

  public:
   GaussianSubRnd(void) { middle=sigma=A=n2=n=0; }
   GaussianSubRnd(
                  double M,
                  double Sigma,
                  long idum=1,
                  int N=6
                 );
   double aget(void);

   void SaveBin( Stream& bin ) const;
   void LoadBin( Stream& bin );
 };

 class SubRnd2
 {
  private:
   double MBIG;
   double FAC;
   int inext,inextp;
   double ma[56];

  public:
   typedef double result_type;

  public:
   SubRnd2( double idum=3.1415926 );

   // ���������� ��������� ��������������� �����.
   inline double aget(void)
   {
    if( ++inext==56)
     inext=1;

    if( ++inextp==56 )
     inextp=1;

    double mj = ma[inext] - ma[inextp];
    if( mj<0 )
     mj += MBIG;

    ma[inext] = mj;
    return mj*FAC;
   }

  inline result_type operator()(void) { return aget(); }
  
  inline RandU::result_type min(void) const { return 0; }
  inline RandU::result_type max(void) const { return 1; }

  void SaveBin( Stream& bin ) const;
  void LoadBin( Stream& bin );
 };
 #endif

  } // namespace Math
 } // namespace lem

#endif
