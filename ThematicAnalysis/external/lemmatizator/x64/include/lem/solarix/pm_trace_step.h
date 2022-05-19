// -----------------------------------------------------------------------------
// File PM_TRACE_STEP.H
//
// (c) Koziev Elijah, Obninsk-Cherepovets, Russia, 1995-2004
//
// Contens:
// ����� PM_TraceStep - ������� ��� �������� ������������ ������ ������.
// ����� ����� ��������� ����� ���� ���������� ������, ������� �������� �����
// ��������� ������.
// -----------------------------------------------------------------------------
//
// CD->16.04.1998
// LC->07.08.2005
// --------------

#if !defined(SOL_PM_TRACE__H) && !defined SOL_NO_AA
#define SOL_PM_TRACE__H
#pragma once

 #include <lem/quantors.h>

 namespace Solarix
 {
  struct PM_TraceStep
  {
   int actor_key; // ���� ������, ��� �������� �������� ������������.
   int cfg;       // ���� ������������.

   PM_TraceStep(void) { actor_key=cfg=UNKNOWN; }
   PM_TraceStep( const int Key, const int Cfg )
   { actor_key=Key; cfg=Cfg; }
  };
 } // namespace Solarix

#endif
