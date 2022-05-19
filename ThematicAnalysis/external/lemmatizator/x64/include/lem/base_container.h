#ifndef LEM_BASE_CONTAINER__H
#define LEM_BASE_CONTAINER__H
#pragma once

// -----------------------------------------------------------------------------
// File BASE_CONTAINERS.H
//
// (c) Koziev Elijah
//
// Content:
// ����� BaseCollect - ������������� ������-��������� ��� ���������� ���������.
// ����� ��� �������� policy classes ��� �������, ������� ���������� �������
// ��������������� ��������� ��������� - ����� ��������� �������� (���� memcpy)
// ��� ��������� ����������� � �.�. ( operator=(...)).
//
// ��� ���� �� ������� ������������ �������, ������ (�� ���) ������ ����������
// �������� ��� ��� ����������. ��� ������� ������ - ����� ����������� ���������
// ������ �������� ���������.
//
// ����� �� ������������ ��������� ������ ������������ ������� - Collect ���
// �������� ��������� � �������������� �������������� �����������, MCollect
// ��� POD'��.
//
// 09.10.2005 - container are based on std::vector for better conformance
//              with STL
// -----------------------------------------------------------------------------
//
// CD->30.08.2006
// LC->30.04.2008
// --------------

 #include <lem/config.h>
 #include <vector>
 #include <lem/runtime_checks.h>
 #include <lem/quantors.h>

 // ����� ������� �� ������ ���������� ����� ������� ��� �����������.
 #undef max
 #undef min

 namespace lem
 {
  // ****************************************
  // Forward declaration.
  // ****************************************

  struct Out_Of_Range
  {
   std::string text;

   Out_Of_Range( const char *Text ) : text(Text) {}
  };

  struct Container
  {
   typedef std::vector<int>::size_type size_type; // ��� ����������� - �������� ��� ����������� �����������
  };

  
 template < class ITEM >
 class BaseCollect : public std::vector<ITEM>
 {
  protected:
   #if LEM_DEBUGGING==1
   void Check_Index( Container::size_type i ) const
   {
    #if defined LEM_GNUC
    if( i>=size() )
    #else
    if( i<0 || i>=size() )
    #endif
     {
      char msg[80];
      std::sprintf( msg, "Index [%d] out of boundaries [0,%d) in BaseCollect", CastSizeToInt(i), CastSizeToInt(size()) );
      throw Out_Of_Range(msg);
     }
   }
   #endif

  public:

   #if defined LEM_BORLAND

    // ������ ��������� �������, �� BCB 6.0 ������� �������� ��� ��������������
    // ����-�����, � MSVC 2003 �������� �� ���. ��� ����� ��������������.
    typedef std::vector<ITEM>::reference       reference;
    typedef std::vector<ITEM>::const_reference const_reference;
    typedef std::vector<ITEM>::iterator        iterator;
    typedef std::vector<ITEM>::const_iterator  const_iterator;
    typedef Container::size_type size_type; 
 
   #elif defined LEM_GNUC || __MINGW32__

    typedef typename std::vector<ITEM>::reference       reference;
    typedef typename std::vector<ITEM>::const_reference const_reference;
    typedef typename std::vector<ITEM>::iterator        iterator;
    typedef typename std::vector<ITEM>::const_iterator  const_iterator;
    typedef typename Container::size_type size_type;  

    inline size_type size(void) const { return std::vector<ITEM>::size(); } 
    inline void clear(void) { return std::vector<ITEM>::clear(); }
    inline void reserve( size_type n ) { std::vector<ITEM>::reserve(n); }
    inline void resize( size_type n ) { std::vector<ITEM>::resize(n); }
    inline bool empty(void) const { return std::vector<ITEM>::empty(); } 
    
    inline const_iterator begin(void) const { return std::vector<ITEM>::begin(); }
    inline const_iterator end(void) const { return std::vector<ITEM>::end(); }

    inline iterator begin(void) { return std::vector<ITEM>::begin(); }
    inline iterator end(void)   { return std::vector<ITEM>::end(); }
   #else     
    typedef Container::size_type size_type;  
   #endif
  

  protected:

   #if defined LEM_MSC && _MSC_VER<=1200
   inline       ITEM* get_Container(void)       { return (ITEM*)&*begin(); }
   inline const ITEM* get_Container(void) const { return (ITEM*)&*begin(); }
   #else
   inline       ITEM* get_Container(void)       { return (ITEM*)&*std::vector<ITEM>::begin(); }
   inline const ITEM* get_Container(void) const { return (ITEM*)&*std::vector<ITEM>::begin(); }
   #endif

  public:
   BaseCollect(void) : std::vector<ITEM>() {}

   BaseCollect( lem::Container::size_type n0 ) : std::vector<ITEM>()
   {
    /*std::vector<ITEM>::*/resize(n0);
   }

   // ********************************
   // ����������� �����������
   // ********************************
   BaseCollect( const BaseCollect<ITEM>& c ) : std::vector<ITEM>(c) {}

   BaseCollect( const ITEM *Begin, const ITEM *End )
    : std::vector<ITEM>( Begin, End )
   {}

   inline void AddList( const BaseCollect<ITEM> &x )
   {
    reserve( size() + x.size() );
    for( size_type i=0; i<x.size(); i++ )
     std::vector<ITEM>::push_back(x[i]);

    return;
   }

   #if defined LEM_GNUC  
   inline reference operator[]( size_type i )
   {
    #if LEM_DEBUGGING==1 
    Check_Index(i);
    return std::vector<ITEM>::at(i);
    #else 
    return std::vector<ITEM>::operator[](i);
    #endif 
   }

   inline const_reference operator[]( size_type i ) const
   {
    #if LEM_DEBUGGING==1 
    Check_Index(i);
    return std::vector<ITEM>::at(i);
    #else 
    return std::vector<ITEM>::operator[](i);
    #endif 
   }
   #elif LEM_DEBUGGING==1 && _MSC_VER>1200
   inline reference operator[]( size_type i )
   {
    Check_Index(i);
    return std::vector<ITEM>::at(i);
   }

   inline const_reference operator[]( size_type i ) const
   {
    Check_Index(i);
    return std::vector<ITEM>::at( i );
   }
   #endif

   inline const_reference get( size_type i ) const { return operator[](i); }
   inline       reference get( size_type i )       { return operator[](i); }

   inline void set( size_type i, const ITEM &x )
   {
    operator[](i) = x;
   }

   #if LEM_DEBUGGING==1 && _MSC_VER>1200
   inline const_reference back(void) const
   { LEM_CHECKIT_Z(size()>0); return std::vector<ITEM>::back(); }

   inline const_reference front(void) const
   { LEM_CHECKIT_Z(size()>0); return std::vector<ITEM>::front(); }

   inline reference back(void)
   { LEM_CHECKIT_Z(size()>0); return std::vector<ITEM>::back(); }

   inline reference front(void)
   { LEM_CHECKIT_Z(size()>0); return std::vector<ITEM>::front(); }
   #endif

   /*********************************************************************
    �������� �������� �� ���������� � ������ ������. ������ �����������
    ����� ������ �� �����������, ������� ���������� ��������� ����� �����
    ����������� ����������!
   **********************************************************************/
   void Remove( size_type k )
   {
    #if defined LEM_GNUC
    LEM_CHECKIT_Z( k<size() );
    #else
    LEM_CHECKIT_Z( k>=0 && k<size() );
    #endif
    std::vector<ITEM>::erase( /*std::vector<ITEM>::*/begin()+k );
    return;
   }

  inline void ReshapeBy( size_type add ) { /*std::vector<ITEM>::*/reserve( size()+add ); }

  /*********************************************************************
   ��������� ����� �������� ��������� �� diminish. ���� ��������������
   ����� ��������� ������ <0, �� ��������� ������ ������ ������.
  **********************************************************************/
  inline void ReduceBy( size_type diminish )
  {
   if(size()<diminish)
    clear();
   else
    resize( size() - diminish );
  }

  inline void ReduceTo( size_type to_n )
  {
   LEM_CHECKIT_Z(to_n>=0);
   if( to_n<size() )
    resize(to_n);
  }

  /*********************************************************************
   � ������ ����������� � ������� i ����� ������� x. ������� �� �������
   i ���������� � i+1, � ��� �����. ������ ������� ������������� �� 1.
  **********************************************************************/
  void Insert( size_type to, const ITEM &x )
  {
   if( empty() )
    {
     // ��������� ���� - ������ ���������.
     LEM_CHECKIT_Z(!to);
     std::vector<ITEM>::push_back(x);
     return;
    }

   #if defined LEM_GNUC
   LEM_CHECKIT_Z(to<size());
   #else
   LEM_CHECKIT_Z(to>=0 && to<size());
   #endif

   std::vector<ITEM>::insert( begin()+to, x );
   return;
  }

   inline int find( const ITEM &x ) const
   {
    for( size_type i=0; i<size(); i++ )
     if( get(i)==x )
      return CastSizeToInt(i);

    return UNKNOWN;
   }

 };


 } //end of namespace 'lem'

#endif
