#ifndef SOL_OPTIONS__H
#define SOL_OPTIONS__H
#pragma once

 namespace Solarix
 {
  class Compile_Options
  {
   public:
    bool decompile_rules; // ���������� ����������������� ��� � CPP-�����
                          // ��� ������.

    bool explain_not_compiled; // ��������� � CPP-����� ������� ������
                               // ���������� ����������� �������

    bool explain_correlation; // ��������� ����������� �� ���������� �����
                              // ��������������

    bool explain_context;     // ��������� ����������� �� ���������� �����
                              // �������� � ��������� ���������

    bool explain_predicates; // ��������� ����������� �� �������� ����������

    bool general_explanations; // ����� ����������� � ������������� �����

    Compile_Options(void)
    {
     decompile_rules      = false;
     explain_not_compiled = false;
     explain_correlation  = false;
     explain_context      = false;
     explain_predicates   = false;
     general_explanations = false;
    }

    
    inline void TurnOn(void)
    {
     decompile_rules      = true;
     explain_not_compiled = true;
     explain_correlation  = true;
     explain_predicates   = true;
     explain_context      = true;
     general_explanations = true;
    }
  };
 };

#endif
