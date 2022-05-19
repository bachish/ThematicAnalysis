#ifndef LEM_PARRAY__H
#define LEM_PARRAY__H
#pragma once

// -----------------------------------------------------------------------------
// File PTR_CONTAINER.H
//
// (c) Koziev Elijah
//
// Content:
// ����� PtrCollect - ������ ��� ����������, ��������� ���������� ��������
// ����� ���������. � ���� �������� ����������� ��� �������� ����� ������
// ��������������, ������ ����, ����� ���������� ���� ��������� �� ��������.
//
// ����� Ptr_NC_Collect - ���������� PtrCollect, �� � ������������
// ���������� �����������, ��� ��� ������ �������� ����� ��������� ��������
// ������ �� ����������� ������������ �����������.
//
// 17.07.2009 - ��� ��� PtrCollect ����� ������� NULL ��������, �� ��� ����������
//              � ����� ��� �� ������������ �������� ��������� ����������
//              ��������, ������� ��� �������� ��������� ���������
//              �����������������.
// -----------------------------------------------------------------------------
//
// CD->01.06.1998
// LC->17.07.2009
// --------------

 #include <lem/containers.h>

 namespace lem
 {
  // ********************************************************************
  // ��� �������� ����������� (��� ������ �������������), �� � ����������
  // ������������, ������� ������� ������� �� �������� ����������.
  // ********************************************************************
  template <class ITEM> class Ptr_NC_Collect : public MCollect<ITEM*>
  {
   protected:
    Ptr_NC_Collect( const Ptr_NC_Collect<ITEM> &x );
    void operator=( const Ptr_NC_Collect<ITEM> &x );

   public:    
    #if defined LEM_GNUC
    typedef typename MCollect<ITEM*>::size_type size_type;  
    typedef typename MCollect<ITEM*>::iterator       iterator;
    typedef typename MCollect<ITEM*>::const_iterator const_iterator;
    #elif _MSC_VER>=1700
	typedef typename MCollect<ITEM*>::size_type size_type;
    typedef typename MCollect<ITEM*>::iterator       iterator;
	typedef typename MCollect<ITEM*>::const_iterator const_iterator;
    #else  
    typedef MCollect<ITEM*>::size_type size_type;  
    typedef MCollect<ITEM*>::iterator       iterator;
    typedef MCollect<ITEM*>::const_iterator const_iterator;
    #endif  


   #if defined __MINGW32__ || defined LEM_GNUC
    inline iterator       begin(void)       { return MCollect<ITEM*>::begin(); }
    inline const_iterator begin(void) const { return MCollect<ITEM*>::begin(); }
    inline iterator       end(void)         { return MCollect<ITEM*>::end(); }
    inline const_iterator end(void)   const { return MCollect<ITEM*>::end(); }

    inline void reserve( size_type n ) { MCollect<ITEM*>::reserve(n); }
    inline bool empty(void) const { return MCollect<ITEM*>::empty(); }

    inline size_type size(void) const { return MCollect<ITEM*>::size(); }

    inline const ITEM* operator[]( size_type i ) const { return MCollect<ITEM*>::operator[](i); }
    inline       ITEM*& operator[]( size_type i )       { return MCollect<ITEM*>::operator[](i); }

    inline const ITEM* get( size_type i ) const { return MCollect<ITEM*>::get(i); }
    inline       ITEM* get( size_type i )       { return MCollect<ITEM*>::get(i); }

   #endif


   public:
    Ptr_NC_Collect(void):MCollect<ITEM*>() {}

    // **************************************
    // �������� - ��������� ����� ���������,
    // ��� ���������������� ��� NULL'�
    // **************************************
    Ptr_NC_Collect( int n0 ):MCollect<ITEM*>(n0)
    {
     LEM_CHECKIT_Z( n0>=0 );
     for( int i=0; i<n0; i++ )
      operator[](i)=NULL;
    }

    Ptr_NC_Collect( lem::Stream& bin )
    { LoadBin(bin); }

    ~Ptr_NC_Collect(void) { clear(); }

     void clear(void)
     {
      // ���������� � ������ �� ���������.
      for( size_type i=0; i<size(); i++ )
       delete get(i);

      MCollect<ITEM*>::clear();
      return;
     }

     // *************************************************************
     // �������������� ����� �� ������ MCollect<...> � ���, �����
     // ���������������� ����� �������� ���������� NULL.
     // *************************************************************
     void AddAll( int to_n )
     {
      LEM_CHECKIT_Z( to_n>=0 );

      const int old_size=size();
      MCollect<ITEM*>::reserve(to_n);

       for( size_type i=old_size; i<size(); i++ )
        MCollect<ITEM*>::push_back(NULL);

      return;
     }

     // ****************************************************************
     // ������� ������� �� ���������� � ������� ����������� ��� ������
     // ��������.
     // ****************************************************************
     void Remove( int k )
     {
      ITEM *ptr = operator[](k);
      delete ptr;
      MCollect<ITEM*>::Remove(k);
      return;
     }

     // �� ���������� ����������� ��������� �������, ��������
     // ��� ��� ���������� � ��������� ���.
     ITEM* Release( int k )
     {
      ITEM *ptr = operator[](k);
      MCollect<ITEM*>::Remove(k);
      return ptr;
     }
   
     inline void resize( size_type n )
     {
      if( n < size() )
       {
        size_type d = size()-n;
        for( size_type k=0; k<d; ++k )
         delete get( size()-1-k );
       }
 
      MCollect<ITEM*>::resize(n);
      return;
     }


     void ReduceTo( int to_n )
     {
      LEM_CHECKIT_Z( to_n>=0 );

      // ���������� ��� ����, ��� ����� ReduceTo ���������� �� ���������
      // ���� ������������ ������. ������� ���� �������� ����� ������� �
      // ������� ������ ��������.
      const int nold = size();
      for( int i=to_n; i<nold; i++ )
       lem_rub_off(get(i));

      MCollect<ITEM*>::ReduceTo(to_n);

      return;
     }

    // **********************************************************
    // ���������� ��������� � ��������� ���������� � ������.
    // ���������������, ��� ����� ��������� ����� ������������
    // ����� SaveBin.
    // **********************************************************
    void SaveBin( lem::Stream& bin ) const
    {
     const size_type ni=size();
     bin.write( &ni, sizeof(ni) );

     for( size_type i=0; i<ni; ++i )
      {
       bin.write_bool( get(i)!=NULL );
       if( get(i)!=NULL )
        get(i)->SaveBin(bin);
      }

     return;
    }


    // ***************************************************************
    // �������� ��������� ���������� �� ������, � ��������� ���������.
    // ���������������, ��� ����� ��������� ����� ������������
    // ����� LoadBin.
    // ***************************************************************
    void LoadBin( lem::Stream& bin )
    {
     size_type ni=0;
     bin.read( &ni, sizeof(ni) );

     if(!ni)
      return;

     reserve(ni); // �������� ������ ��� ��� ��������.

     for( size_type i=0; i<ni; ++i )
      {
       if( bin.read_bool() )
        {
         ITEM *x = new ITEM;
         x->LoadBin(bin);
         push_back(x);
        }
       else
        {
         push_back((ITEM*)NULL);
        }
      }

     return;
    }

   // ******************************************************************
   // ����������� �������� ���������, ��� ��� �� ������ ���������, � �
   // ����������� ������� ��������� ���������� �� ����� ������ - �����
   // ���������� ���� �������.
   // ******************************************************************
   inline bool operator==( const Ptr_NC_Collect<ITEM> &x ) const
   {
    if( size()!=x.size() )
     return false;

    for( size_type i=0; i<size(); ++i )
     if( ( get(i)==NULL && x[i]!=NULL ) || ( get(i)!=NULL && x[i]==NULL ) || ( *get(i) != *x[i]) )
      return false;

    return true;
   }

   inline bool operator!=( const Ptr_NC_Collect<ITEM> &x ) const
   { return !operator==(x); }


   int count( const ITEM &x ) const
   {
    int c=0;

    for( size_type i=0; i<size(); ++i )
     if( get(i)!=NULL && *get(i) == x )
      c++;

    return c;
   }


   int find( const ITEM &x ) const
   {
    size_type c=0;

    for( size_type i=0; i<size(); ++i )
     if( get(i)!=NULL && *get(i) == x )
      return CastSizeToInt(i);

    return UNKNOWN;
   }
  };


  // ******************************************************************
  // ��������� �������� ����������� ����������, ���������� ��-��������
  // ������� �������� ��������.
  // ******************************************************************
  template <class ITEM> class PtrCollect : public Ptr_NC_Collect<ITEM>
  {
   public:
   #if defined __MINGW32__|| defined LEM_GNUC

    typedef typename Ptr_NC_Collect<ITEM>::size_type      size_type;
    typedef typename Ptr_NC_Collect<ITEM>::iterator       iterator;     
    typedef typename Ptr_NC_Collect<ITEM>::const_iterator const_iterator;

    inline iterator       begin(void)       { return Ptr_NC_Collect<ITEM>::begin(); }
    inline const_iterator begin(void) const { return Ptr_NC_Collect<ITEM>::begin(); }
    inline iterator       end(void)         { return Ptr_NC_Collect<ITEM>::end(); }
    inline const_iterator end(void)   const { return Ptr_NC_Collect<ITEM>::end(); }

    inline void resize( size_type n ) { Ptr_NC_Collect<ITEM>::resize(n); }
    inline void reserve( size_type n ) { Ptr_NC_Collect<ITEM>::reserve(n); }
    inline void clear(void) { Ptr_NC_Collect<ITEM>::clear(); }
    inline bool empty(void) const { return Ptr_NC_Collect<ITEM>::empty(); }

    inline size_type size(void) const { return Ptr_NC_Collect<ITEM>::size(); }

    inline const ITEM* operator[]( size_type i ) const { return Ptr_NC_Collect<ITEM>::operator[](i); }
    inline       ITEM*& operator[]( size_type i )       { return Ptr_NC_Collect<ITEM>::operator[](i); }

    inline const ITEM* get( size_type i ) const { return Ptr_NC_Collect<ITEM>::get(i); }
    inline       ITEM* get( size_type i )       { return Ptr_NC_Collect<ITEM>::get(i); }
   #endif


   private:
    void Init( const PtrCollect<ITEM> &x )
    {
     reserve(x.size());
       
     for( size_type i=0; i<x.size(); ++i )
      if( x[i]!=NULL )
       Ptr_NC_Collect<ITEM>::push_back( new ITEM( *x[i] ) );
      else
       Ptr_NC_Collect<ITEM>::push_back( (ITEM*)NULL );

     return;
    }

   public:
    PtrCollect( int n0=0 )
    :Ptr_NC_Collect<ITEM>(n0) {}

    PtrCollect( const PtrCollect<ITEM> &x ) { Init(x); }

    PtrCollect( lem::Stream& bin ) { LoadBin(bin); }

    void SaveBin( lem::Stream& bin ) const
    {
     const size_type ni=size();
     bin.write( &ni, sizeof(ni) );
    
     for( size_type i=0; i<ni; i++ )
      {
       bin.write_bool( get(i)!=NULL );
       if( get(i)!=NULL )
        get(i)->SaveBin(bin);
      }

     return;
    }

    void LoadBin( lem::Stream& bin )
    {
     size_type ni=0;
     bin.read( &ni, sizeof(ni) );

     if(!ni)
      // ��������� ������ ���� ����.
      return;

     reserve(ni); // �������� ������ ��� ��� ��������.

     for( size_type i=0; i<ni; ++i )
      {
       if( bin.read_bool() )
        {  
         ITEM *x = new ITEM;
         x->LoadBin(bin); 
         Ptr_NC_Collect<ITEM>::push_back(x);    
        }
       else
        {
         Ptr_NC_Collect<ITEM>::push_back( (ITEM*)NULL );
        }
      }

     return;
    }

    inline void operator=( const PtrCollect<ITEM> &x )
    {
     if( this==&x )
      return;

     clear();
     Init(x);
     return;
    }
  };


  template < class T>
  inline lem::Stream& operator << ( lem::Stream &bin, const PtrCollect<T> &c )
  {
   c.SaveBin(bin);
   return bin;
  }

  template < class T>
  inline lem::Stream& operator >> ( lem::Stream &bin, PtrCollect<T> &c )
  {
   c.LoadBin(bin);
   return bin;
  }

  // ******************************************************************
  // �������� �������� �������� ��������� ������ �������� ������ �
  // �������������� ����������� �����������, ���������� � ����� �
  // �������� �� �����.
  // ******************************************************************
  template <class ITEM> class CloneCollect : public Ptr_NC_Collect<ITEM>
  {
   public:
   #if defined __MINGW32__|| defined LEM_GNUC
    typedef typename Ptr_NC_Collect<ITEM>::size_type size_type;
    typedef typename Ptr_NC_Collect<ITEM>::iterator       iterator;
    typedef typename Ptr_NC_Collect<ITEM>::const_iterator const_iterator;
   
    inline iterator       begin(void)       { return Ptr_NC_Collect<ITEM>::begin(); }
    inline const_iterator begin(void) const { return Ptr_NC_Collect<ITEM>::begin(); }
    inline iterator       end(void)         { return Ptr_NC_Collect<ITEM>::end(); }
    inline const_iterator end(void)   const { return Ptr_NC_Collect<ITEM>::end(); }

    inline void resize( size_type n ) { Ptr_NC_Collect<ITEM>::resize(n); }
    inline void reserve( size_type n ) { Ptr_NC_Collect<ITEM>::reserve(n); }
    inline void clear(void) { Ptr_NC_Collect<ITEM>::clear(); }
    inline bool empty(void) const { return Ptr_NC_Collect<ITEM>::empty(); }

    inline size_type size(void) const { return Ptr_NC_Collect<ITEM>::size(); }

    inline const ITEM* operator[]( size_type i ) const { return Ptr_NC_Collect<ITEM>::operator[](i); }
    inline       ITEM*& operator[]( size_type i )       { return Ptr_NC_Collect<ITEM>::operator[](i); }

    inline const ITEM* get( typename Ptr_NC_Collect<ITEM>::size_type i ) const { return Ptr_NC_Collect<ITEM>::get(i); }
    inline       ITEM* get( typename Ptr_NC_Collect<ITEM>::size_type i )       { return Ptr_NC_Collect<ITEM>::get(i); }

   #endif


   private:
    void Init( const CloneCollect<ITEM> &x )
    {
     Ptr_NC_Collect<ITEM>::resize( x.size() );

     for( size_type i=0; i<size(); i++ )
      {
       operator[](i) = x[i]==NULL ? NULL : x[i]->clone();
      }

     return;
    }

   public:
    CloneCollect():Ptr_NC_Collect<ITEM>() {}

    CloneCollect( const CloneCollect<ITEM> &x ) { Init(x); }

    CloneCollect( lem::Stream& bin ) { LoadBin(bin); }

    void SaveBin( lem::Stream& bin ) const
    {
     const size_type ni = size();
     bin.write( &ni, sizeof(ni) );

     for( size_type i=0; i<ni; ++i )
      {
       bin.write_bool( get(i)!=NULL );
       if( get(i)!=NULL )
        get(i)->SaveBin(bin);
      }

     return;
    }

    void LoadBin( lem::Stream& bin )
    {
     size_type ni=0;
     bin.read( &ni, sizeof(ni) );

     if(!ni)
      // ��������� ������ ���� ����.
      return;

     reserve(ni); // �������� ������ ��� ��� ��������.

     for( size_type i=0; i<ni; ++i )
      {
       if( bin.read_bool() )
        {  
         Ptr_NC_Collect<ITEM>::push_back( ITEM::load_bin(bin) );
        }
       else
        {
         Ptr_NC_Collect<ITEM>::push_back( (ITEM*)NULL );
        }
      }

     return;
    }

    inline void operator=( const CloneCollect<ITEM> &x )
    {
     if( this==&x )
      return;

     clear();
     Init(x);
     return;
    }
  };

 } // namespace 'lem'

#endif
