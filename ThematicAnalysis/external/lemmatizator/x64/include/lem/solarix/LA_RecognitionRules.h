#if !defined LA_RECOGNITION_RULES__H
 #define LA_RECOGNITION_RULES__H

 #include <lem/ptr_container.h>
 #include <lem/solarix/WordCoord.h>
 #include <lem/solarix/LA_RecognitionRule.h>
 #include <lem/solarix/LA_ProjectInfo.h>
 #include <lem/solarix/ProjScore.h>

 namespace Solarix
 {
  class LA_RecognitionTrace;
  class LexiconStorage;

  // ����� ������ ������������� �������������� ���������
  // ��� ������������� �����. ������������� - ��. LA_Recognizer
  struct LA_RecognitionRules : lem::NonCopyable
  {
   int id_language;
   LexiconStorage *storage;

   lem::MCollect<LA_RecognitionRule*> rules; // ����� ������ ������

   lem::MCollect<const LA_RecognitionRule*> rx_rules; // �������, �������� ������� ������� � 
                                                      // �������������� ����������� ���������

   typedef std::multimap<LA_RecognitionRule::HashType,const LA_RecognitionRule*> RULES;
   RULES prefix_rules, affix_rules, syllab_prefix_rules, syllab_affix_rules;

   void AddResult(
                  const Solarix::Word_Coord &wc,
                  float word_score,
                  Solarix::LA_ProjectInfo *inf,
                  lem::MCollect<Solarix::Word_Coord> &found_list,
                  lem::MCollect<ProjScore> & score_list,
                  lem::PtrCollect<Solarix::LA_ProjectInfo> &inf_list
                 ) const;


   LA_RecognitionRules( int _id_language, LexiconStorage *_storage );
   ~LA_RecognitionRules(void);

   void Add( LA_RecognitionRule * rule );

   bool Apply(
              const lem::UCString & normalized_word,
              const lem::UCString & original_word,
              float word_score,
              lem::MCollect<Solarix::Word_Coord> & found_list,
              lem::MCollect<ProjScore> & score_list,
              lem::PtrCollect<Solarix::LA_ProjectInfo> & inf_list,
              bool only_forced,
              LA_RecognitionTrace *trace
             ) const;

   bool ApplyForSyllabs(
                        const lem::UCString &word,
                        float word_score,
                        const lem::MCollect<lem::UCString> & syllabs,
                        lem::MCollect<Solarix::Word_Coord> & found_list,
                        lem::MCollect<ProjScore> & score_list,
                        lem::PtrCollect<Solarix::LA_ProjectInfo> & inf_list,
                        LA_RecognitionTrace *trace
                       ) const;
  };
 }

#endif
