#ifndef SYNTAX_ANALYZER_TIMEOUT__H
#define SYNTAX_ANALYZER_TIMEOUT__H
#pragma once

 namespace Solarix
 {
  // ���������, �������������� ��������� ����������� ��������
  // ��� ���������� ��������������� �������.
  struct SyntaxAnalyzerTimeout
  {
   enum { MAX_VARIATORS=4096 }; // �������� ������������� ����� ���������� � ����
                                // �������������� �������� �� ���������

   int max_tokenization_paths; // ����. ����� ��������������� ������ ����� � ����� �����������
   int max_variators;
   int max_elapsed_millisecs;
   int max_alt; // ����. ����� ������������ ����� ������� � ���������� �������
   int max_bottomup_trees; // ����. ����� ��������������� ��������� ���������� ����� � ���������� �������
   int max_recursion_depth;

   SyntaxAnalyzerTimeout(void) : 
    max_variators(SyntaxAnalyzerTimeout::MAX_VARIATORS), max_elapsed_millisecs(3600000), max_alt(0), max_tokenization_paths(1000), max_bottomup_trees(1000), max_recursion_depth(10000) {}
  };

 }

#endif
