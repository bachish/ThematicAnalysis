// -----------------------------------------------------------------------------
// File BASE_INDEXER.H
//
// (c) 2005 by Koziev Elijah     all rights reserved 
//
// SOLARIX Intellectronix Project http://www.solarix.ru
//                                http://sourceforge.net/projects/solarix  
//
// Licensed under the terms of GNU Lesser GPL
// You must not eliminate, delete or supress these copyright strings
// from the file!
//
// Content:
// Search engine FAIND.
// Base class for document index database.
// -----------------------------------------------------------------------------
//
// CD->04.09.2002
// LC->28.03.2005
// --------------

#ifndef BASE_INDEXER__H
#define BASE_INDEXER__H
#pragma once

 #include <utility>

 #include <lem/qarray.h>
 #include <lem/mqueue.h>
 #include <lem/mset.h>
 #include <lem/stl.h>
 #include <lem/integer_math.h>

 namespace Solarix
 {
  namespace Search_Engine
  { 

   using namespace lem;
//   using namespace lem::File_Factory;
   using namespace Solarix;

 const int W_QUEUE_LEN = 4; // ����� ������� ������� ������������ ����.
 const int NCHAR     = 33;  // ����� �������� � ��������

 const int NSUBLIST  = LEM_CSTRING_LEN;   // ������� ������ ���������� ���� ��� ���� 1, 2,
                            // 3, ..., ������. �� ����� ������ ��������� �����
                            // 10. �� ����� ��������� ������� �����.

 const int NCSL=256; // ���-�� ���������� ��� ���������� ����������

 // ****************************************************************
 // ��������� ��� ������� ��������� ���������� ���� ������������ �
 // ����� �����: ����� ��������� � ����� �������� � ������.
 // ****************************************************************
 inline int e_block( int key ) { return (key>>26) & 0x4fUL; }
 inline int e_hash( int key )  { return (key>>18) & 0xffUL; }
 inline int e_item( int key )  { return key & 0x4ffffUL; }

 // *****************************************************************
 // �������� ���� �� ���� �����������.
 // *****************************************************************
 inline int e_key( int iblock, lem::uint8_t hash_key )
 {  return hash_key | (iblock<<26); }

 inline int hash_to_key( lem::uint8_t hash, int item )
 {  return item | (hash<<18); }


 // ****************************
 // ������ ��� �����-�������
 // ****************************

 // ********************************************************
 // List of words that have equal hash code and length
 // ********************************************************
 class w_hash_list : public MCollect< std::pair<UCString,int> >
 {
  public:
   w_hash_list(void):MCollect< std::pair<UCString,int> >(0,1024) {};

   w_hash_list( const w_hash_list &x ) : MCollect< std::pair<UCString,int> >(x) {};

   inline void operator=( const w_hash_list &x )
   {
    MCollect< std::pair<UCString,int> >::operator=(x);
   }
 };


 // ************************************************************
 // Sectainer - the list of lists of words. All words in this
 // sectainer have equal length (but different hash codes).
 // ************************************************************
 class w_list : public FxCollect< w_hash_list, 256 >
 {
  public:
   w_list(void) : FxCollect< w_hash_list, 256 >() {};

   inline int add_lexem( const UCString &Word )
   {
    int ifound=UNKNOWN;

    w_hash_list &words_n = operator[]( static_cast<unsigned>(Word.GetHash()) );

    // ���������� ����� �����...
    LEM_LOOP_A( j, words_n )
     if( words_n[j].first==Word )
      {
       words_n[ifound=j].second++;
       break;
      }

    // ����� ��� � ������ - ���������.
    if( ifound==UNKNOWN )
     {
      ifound = words_n.size();
      words_n.push_back( std::make_pair( Word, 1 ) );
     }

    return hash_to_key( static_cast<unsigned>(Word.GetHash()), ifound );
   }
 };


 // **************************************************************
 // �������� ����� ���������� ���������� �� ������.
 // **************************************************************
 class Base_Indexer
 {
  protected:
   // ����������� ������ ��� �����-�������
   // ��� ��������� �������� ������� ������ ���� ������ 1 ������,
   // 2 ������� � �����, ���� ��������� ������ - ��� ���� ������������
   // ����� ������ �������� (NSUBLIST-1).
   w_list words[NSUBLIST]; // ������ 1,2,3,4, � ������
   int total_wlen; // ����� ���� ���� - ��� ���������� ������� �����
   int nlex; // ������� ���� ����������

  public:
   Base_Indexer(void);

   int operator << ( const UCString &w );

   inline const std::pair<UCString,int>& get_by_key( int key ) const
   { return words[ e_block(key) ] [ e_hash(key) ] [ e_item(key) ]; }

   void Build_Whole_List( lem::MCollect<UCString> &words_n ) ;

   void Build_Whole_List(
                         w_hash_list &words_n,
                         IntCollect &sort_index
                        );
 };

 } // namespace Search_Engine
 } // namespace Solarix

#endif
// ----------------------- End Of File [BASE_INDEXER.H] ------------------------
