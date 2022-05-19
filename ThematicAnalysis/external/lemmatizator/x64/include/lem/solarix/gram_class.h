// -----------------------------------------------------------------------------
// File GRAM_CLASS.H
//
// (c) Koziev Elijah
//
// Content:
// ����� GramClass - �������������� ����� ��� ���������. ������ ����������
// ���� ��� ���������, ������� ���-�� ����� ��� ������ ���������, �������������
// � ������� ������.
// ��������� ����� ������� ������ ���������, ���������������, ��������.
//
// 07.08.2008 - ��������� ����������� ������� ������ ����� ������ ���
//              ��������������� ������������.
// 13.12.2008 - ��������� ����������� �������� ������ ������� ���������.
// -----------------------------------------------------------------------------
//
// CD->06.10.1995
// LC->29.10.2010
// --------------

#ifndef SOL_CLASS__H
#define SOL_CLASS__H
#pragma once

 #include <lem/noncopyable.h>
 #include <lem/solarix/coord_pair.h>
 #include <lem/solarix/string_set.h>

 namespace lem
 {
  namespace Iridium
  {
   class BethToken;
  }
 }

 namespace Solarix
 {
  using lem::UCString;
  using lem::IntCollect;

  class Grammar;

  /******************************************************
   �������������� ����� ������������ ��� �������������
   �������� ����������� ������ ����������. � ���������,
   ����� �������� ������ �������� ���������, �������
   ��������� ��� ������� ������ ���������� � �����������.
  *******************************************************/
  class GramClass : lem::NonCopyable
  {
   public:
    lem::UCString name; // ��� ��������������� ������.
    Solarix::UCStringSet aliases; // �������� �����
    CA_Array dim_index;  // ������� ���������-���������.
    CA_Array attr_index; // ���������� � �����������-���������.
    CA_Array tag_index; // ���������� � �����������-�����.

    IntCollect parent; // ������ �������� ������� (������������) �������.
    bool is_parent;    // �������� �� ��� ����� ������������.

    #if defined SOL_LOADTXT && defined SOL_COMPILER
    bool is_realized; // false, ���� ���� extern ����������, true ���� ���� ����������.
    UCString c_name;  // ��� ��������� ��� ���������� � C/C++-����
    #endif

    #if defined SOL_LOADTXT && defined SOL_COMPILER
    void LoadAttributesTxt( Macro_Parser &file, const Grammar& gram );
    void LoadDimentionsTxt( Macro_Parser &file, const Grammar& gram );
    void LoadTagsTxt( Macro_Parser &file, const Grammar& gram );
    #endif

    #if defined SOL_SAVETXT
    void SaveAttributesTxt( OFormatter& txtfile, const Grammar& gram ) const;
    void SaveDimentionsTxt( OFormatter& txtfile, const Grammar& gram ) const;
    void SaveTagsTxt( OFormatter& txtfile, const Grammar& gram ) const;
    #endif

   protected:
    #if defined SOL_LOADTXT && defined SOL_COMPILER
    virtual bool Load_Private_Data( 
                                   const lem::Iridium::BethToken &t,
                                   lem::Iridium::Macro_Parser &txtfile,
                                   Grammar& gram
                                  );
    #endif

    #if defined SOL_SAVETXT
    virtual void Save_Private_Txt( OFormatter& txtfile, const Grammar& gram ) const;
    #endif

   public:
    lem::MCollect< std::pair<GramCoordAdr/*attr*/, int/*default_state*/> > attr_defaults; // �������� ��������� �� ���������

   public:
    GramClass(void);

    #if defined SOL_LOADBIN
    GramClass( lem::Stream &bin );
    #endif

    virtual ~GramClass(void);

    #if defined SOL_LOADTXT && defined SOL_COMPILER
    virtual void LoadTxt(
                         Macro_Parser &txtfile,
                         Grammar& gram,
                         bool IsRealized
                        );
    #endif

    virtual void LoadBin( lem::Stream &bin );

    #if defined SOL_SAVEBIN
    virtual void SaveBin( lem::Stream &bin ) const;
    #endif

    inline const UCString& GetName(void) const { return name; }
    const UCStringSet& GetAliases(void) const { return aliases; }
    bool MatchName( const lem::UCString &name ) const;

    #if defined SOL_LOADTXT && defined SOL_COMPILER
    inline bool IsRealized(void) const { return is_realized; }

    inline bool IsEmpty(void) const
    { return dim_index.empty() && attr_index.empty(); }
    #endif

    inline bool IsParent(void) const { return is_parent; }

    inline const IntCollect& parents(void) const { return parent; }

    inline const CA_Array& attrs(void) const { return attr_index; }
    inline const CA_Array& dims(void)  const { return dim_index;  }
    inline const CA_Array& tags(void)  const { return tag_index;  }

    bool FindLocCoordIndex( const GramCoordAdr iglobal, bool strict=false ) const;

    #if defined SOL_LOADTXT && defined SOL_COMPILER
    bool operator==( const GramClass & c ) const;
    bool operator!=( const GramClass & c ) const;

    inline const UCString& get_C_Name(void) const { return c_name; }
    #endif

    #if defined SOL_SAVETXT
    // ���������� � ��������� ����� ��������� ���� ����������� ������.
    // �������� ��� ����������������� �� ���������� ��������� ��������. 
    void SaveTxt( OFormatter& txtfile, Grammar& gram ) const;
    #endif
  };

  const int SOL_EQUIL_INDEX = -4; // ��� ������� ��� ��������� ���������,
                                  // ������� ������� ���������� � ��������
                                  // ������ ��������� ���������.


 } // namespace 'Solarix'

#endif
