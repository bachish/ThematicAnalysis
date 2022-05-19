#ifndef FG_SYNONYM_DATA__H
#define FG_SYNONYM_DATA__H
#pragma once

#include <lem/solarix/WordEntry.h> 
#include <lem/ucstring.h>

struct SynonymData
{
 int ie;

 int i_center;
 lem::MCollect<lem::UCString> words;
 lem::Ptr<Solarix::Tree_Node> tree; 
 lem::UFString right;

 float w; // ������������� �������� (0...1)
 int dependency_2gram_hits; // ��������� ������� 2-�����, ��������� ��� ��������� �� ����. ����� ����

 void SubstSynonyms(
                    lem::UFString &result,
                    const lem::MCollect< std::pair<lem::UCString,lem::UCString> >& subst
                   ) const;

 SynonymData(void);
 SynonymData( const SynonymData &x );
 SynonymData( int IE );

 void operator=( const SynonymData &x );

 bool IsSimple(void) const { return words.size()<2; }

 lem::UFString BuildSubstForNoun(
                                 Solarix::Dictionary & dict,
                                 const lem::UCString & old_word,
                                 const lem::UCString &new_word,
                                 const Solarix::CP_Array &dim,
                                 const Solarix::SG_EntryForm& f
                                ) const;

 lem::UFString BuildSubstForAdj(
                                Solarix::Dictionary & dict,
                                const lem::UCString & old_word,
                                const lem::UCString &new_word,
                                const Solarix::CP_Array &dim,
                                const Solarix::SG_EntryForm& f
                               ) const;

 lem::UFString BuildSubstForVerb(
                                 Solarix::Dictionary & dict,
                                 const lem::UCString & old_word,
                                 const lem::UCString &new_word,
                                 const Solarix::CP_Array &dim,
                                 const Solarix::SG_EntryForm& f
                                ) const;

 lem::UFString BuildSubstForOther(
                                  Solarix::Dictionary & dict,
                                  const lem::UCString & old_word,
                                  const lem::UCString &new_word
                                 ) const;

};

#endif
