#if !defined WordCoord___H
 #define WordCoord___H


 #undef GetForm
 namespace Solarix
 {

 /*************************************************************************
  ���������, ������� ���������� ���������� ���������� ��� ���������� �
  ������ ���������� (�������� ��� �������). ���� ����� - ������ ������ �
  ������ ����� � ������ ���� ������ �������� ��� ����������� ����������
  �� ��� ���, ���� ������ ������������ ��� ���������� (����� SolWordForm)
  �� ���������� �����-���� ��������.

  This record contains all necessary information to determine a wordform
  or a charform as the element of Alphabet or Lexicon. A pair of numbers -
  index of entry and index of form in the entry's list - are the valid
  pointer to a wordform or a charform unless the list of coordinates in
  SolWordForm class clone has not changed.
 **************************************************************************/
 class Word_Coord
 {
  private:
   int ientry; // ����� ��������� ������ (external index, not an entrykey!)
   int iform;  // ����� ���������� � �������� ��������� ������
               // ���� iform==UNKNOWN, �� ������� ������������ ���
               // ��������� ������, �� ��������� �� ������ ��� ����.

  public:
   Word_Coord( void )
   { ientry=UNKNOWN; iform=UNKNOWN; }

   Word_Coord( int iEntry, int iForm )
   { ientry=iEntry; iform=iForm; }

   inline bool operator==( const Word_Coord& x ) const
   { return ientry==x.ientry && iform==x.iform; }

   inline bool operator!=( const Word_Coord& x ) const
   { return ientry!=x.ientry || iform!=x.iform; }

   inline int GetEntry(void) const { return ientry; }
   inline int GetForm(void)  const { return iform;  }
   inline bool IsUnknown(void) const { return ientry==UNKNOWN; }
 };

 }
#endif
