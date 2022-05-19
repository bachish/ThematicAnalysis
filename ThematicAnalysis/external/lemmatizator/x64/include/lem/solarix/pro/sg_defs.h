// -----------------------------------------------------------------------------
// File SG_DEFS.H
//
// (c) Koziev Elijah, Obninsk-Cherepovets, Russia, 1995-2005
//
// Content:
// ������� ��� ������������ ������������ ������� ��������������
// ���������� � � ������-����������.
//
// ��������� ��� ������ � ��������� ������� (�������� ��� ����� SG_Entry::flags
// � SG_Entry::flags_2
// -----------------------------------------------------------------------------
//
// CD->31.03.1998
// LC->18.01.2005
// --------------

#if !defined SOL_SYNGRAM_DEFS__H
 #define SOL_SYNGRAM_DEFS__H

 // �������� ���� ��� ������, ������� ������ ������������ �� ������.
 #define SOL_SG_NONTERMINAL_ENTRY 0x0001

 // �������� ���� ��� ������, ������� �� ������������ �� ���������, �� ����
 // �� ����� ���������� �������������.
 #define SOL_SG_TERMINAL_ENTRY    0x0000

 // ������ �� ����� ��������� �������������, ���� ��� ����� �����
 // ��������� �������������.
 #define SOL_SG_SILENT_ENTRY     0x0002

 // ������ ����� �������� �������������.
 #define SOL_SG_VOICED_ENTRY       0x0000


 // ������ ������������ ����� �������������� ������� ��� ����������� �����
 // ��������������� �������.
 #define SOL_SG_QUANTOR_ENTRY     0x0004


 #define nonterminal_entry       0x0001 | 0x0000 // SOL_SG_NONTERMINAL_ENTRY | SOL_SG_VOICED_ENTRY
 #define terminal_entry          0x0000 | 0x0000 // SOL_SG_TERMINAL_ENTRY | SOL_SG_VOICED_ENTRY
 #define terminal_silent_entry   0x0000 | 0x0002 // SOL_SG_TERMINAL_ENTRY | SOL_SG_SILENT_ENTRY
 #define shadow_entry            0x0001 | 0x0002 // SOL_SG_NONTERMINAL_ENTRY | SOL_SG_SILENT_ENTRY
 #define quantor_entry           0x0004 | 0x0001 // SOL_SG_QUANTOR_ENTRY | SOL_SG_NONTERMINAL_ENTRY 

#endif
// ------------------------- End Of File [SG_DEFS.H] ---------------------------
