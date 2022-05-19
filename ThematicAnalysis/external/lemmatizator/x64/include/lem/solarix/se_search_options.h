#ifndef SE_SEARCH_OPTIONS__H
#define SE_SEARCH_OPTIONS__H
#pragma once

 #include <lem/zeroed.h>
 #include <lem/solarix/tree_link.h>
 #include <lem/solarix/se_morphology.h>

 namespace Solarix
 {
  class Dictionary;

  namespace Search_Engine
  {

   // ********************************************************
   // General search and match options. They describe how to
   // compare the pattern and text.
   // ����� ����� ������ - ���������, ��� ������������ �������
   // � �����.
   // ********************************************************
   struct Search_Options
   {
    /// ���� �������, ������������� �������� -regex, -sample, -logic, -multiword
    enum Query_Type { REGEX, SAMPLE, MULTIWORD };
    Query_Type query_type;

    /// separated lexems are regular expressions
    /// ��������� ����� �������� ����������� ����������� (������� -rx)
    lem::zbool regex;

    // ������� ������ ����������� ��������� - ������ ������ ���������
    // ����������� ����������� * � ?, ������� ����� �������������� ��������
    // ������������� � ���������� �������.
    lem::zbool wildcards;

    lem::zbool rx_suffix; // ��� ��������� ���� -rx -sample ���� ���� �� ��������
                    // ������������ ��������, �� ��������� .+ � ����� ���
                    // �������� ������ ��������������� ������

    lem::zbool case_sensitive; /* Do not ignore case
                            ������ ��������� �������� ���� �������� */

    lem::zbool soundex;          // �������� �����
    Morphology morphology; // ��������� � ������� ����������� ��� ��������� ������������ ����������
    Syntax syntax;         // ��������� �������������� ������ ��� �������� ���������
    lem::zbool strip_accents;    // ������� ������������� ������� � ���� (�������)
    lem::zbool try_translit;     // ��������� ������ � ���������

    lem::zbool ordered;       // ������ �������������� ������� ���� ��������
    int distance;       // ���������� ����� ������������ ������� (� ��������)
    lem::zbool same_sentence; // ����� ������ ���� � �������� ������ �����������
    lem::zbool same_line;     // ����� ������ ���������� � �������� ����� ������ � �����

    int semnet;         // ������ � ���������� - ����. ���-�� ����� �� ����
    std::set<UCString> t_links; // ����� ���������� �� ��������� ������
    lem::MCollect< Solarix::Tree_Link > t_ilinks; // ������� ���� ������ �� �������
    lem::zbool l_synonyms, l_grammars, l_translations, l_semantics; // ���� ������ ������ ������

    float minbound;     // ����������� ������������� ��������
    int n_max_miss;     // ������������ ������� ������ ��� �������� ��������� ����
    lem::zbool rooting;       // ��������� ���������� � ����� ��� ��������� ���������

    lem::zbool once_per_file; // ������ ������� ���� ��� � �����
    lem::zbool allow_partial; // ��������� ��������� ������������� ���� (�� ��� �����)

    lem::zbool target_content; // ������ � ��������� �������� ���������
    lem::zbool target_filename; // ������ � ����� �����

    void Init( const Search_Options &x );
 
    Search_Options(void);
    Search_Options( const Search_Options &x );

    void operator=( const Search_Options &x );

    void Clear(void);

    void AllowSynonyms(void);
    void AllowGrammar(void);
    void AllowSemantics(void);
    void AllowTranslations(void);

    void UnrollAllowedLinks( const Solarix::Dictionary *sol_id );

    bool SpecifiedLinkGroupsOnly(void) const;
   };

  } // namespace Search_Engine
 } // namespace Solarix

#endif
