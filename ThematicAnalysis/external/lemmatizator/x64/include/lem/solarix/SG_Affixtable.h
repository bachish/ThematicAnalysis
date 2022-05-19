#if !defined Affix_Table__H
 #define Affix_Table__H

 namespace Solarix
 {

 // ***************************************************************
 // ��������� ������� � �������������� ��������� ��� ��������.
 // ��������� ����� rooting - ��������� �������� ��������� ��������
 // � ���������� � �����.
 // ***************************************************************
 class Affix_Table 
 {
  private:
   enum { NLIST=16 };
   lem::MCollect<UCString> list[NLIST];

   struct BaseAffix
   {
    UCString base_afx; // ����� ������ ��������� � ����� ��� ��������� ������� �����
    int id_language;
    int n; // ������� ��� ������� ������������

    BaseAffix(void) { n=0; id_language=UNKNOWN; }
    BaseAffix( const UCString &s, int id_lang ) : base_afx(s), id_language(id_lang), n(1) {}
   };


   #if defined SOL_LOADTXT
   std::multimap< UCString::hash_type, std::pair<int,int> > list1;
   typedef std::multimap< UCString::hash_type, std::pair<int,int> >::const_iterator ITER;
   lem::PtrCollect< MCollect<BaseAffix> > base_afx_list1[NLIST];

   lem::MCollect< lem::triple<UCString /*afx*/, UCString /*base_afx*/, int /*language*/> > rules;
   #endif

   #if defined SOL_LOADTXT && defined SOL_COMPILER
   void GenerateDynformsRules(void);
   #endif

   #if defined SOL_LOADTXT
   void StoreAffix( const UCString &afx, const UCString &base_afx, int id_language );
   #endif

  public:
   Affix_Table(void);

   #if defined SOL_LOADTXT
   Affix_Table( SynGram &sg );
   #endif

   #if defined SOL_SAVEBIN
   void SaveBin( lem::Stream &bin ) const;
   #endif

   #if defined SOL_LOADBIN 
   void LoadBin( lem::Stream &bin );
   #endif

   // ��� ��������� ����� word ������������ ������ ���������
   // ������ - �� ���� ����������� �������� ��������� ��������.
   void GenerateRoots(
                      const UCString &word,
                      float MinBound,
                      lem::MCollect<UCString> &res,
                      lem::MCollect<float>& str_val
                     ) const;

   #if defined SOL_LOADTXT
   //void GenerateDynformsRules( LexicalAutomat &la );
   void PrintInfo( lem::OFormatter &out ) const;
   #endif
 };

 }
#endif
