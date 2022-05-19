#if !defined WordSplitResults__H
 #define WordSplitResults__H

namespace Solarix
{
 // ���������� ���������� ������ ������ ������� �������.
 struct WordSplitResults : lem::NonCopyable
 {
  bool greedy; // true, ���� �������� ����� ����� ������, false ���� �������� ����� ���� �������� ��������� �����������.
  lem::PtrCollect< lem::MCollect<lem::UCString> > words; // �������� ������� ����, ������� ���� ���������� ������ ���������.

  WordSplitResults() : greedy(false) {}
 };
}

#endif
