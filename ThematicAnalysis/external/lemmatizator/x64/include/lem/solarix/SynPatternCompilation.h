#if !defined SynPatternCompilation__H
 #define SynPatternCompilation__H
 
#include <lem/noncopyable.h>
#include <lem/ucstring.h>
#include <lem/containers.h>
#include <lem/solarix/VariableChecker.h>

 namespace Solarix
 {
  #if defined SOL_LOADTXT && defined SOL_COMPILER
  class TrCompilationContext;
  class SynPattern;
  class WordEntrySet;
  class SynGram;

  // �������� ���������� - ��� �������� ������ �� ����������� �����
  class SynPatternCompilation : public VariableChecker
  {
   private:
    const SynPattern *pattern_being_compiled; // ���������� �������������� ��������

    int seq; // ��� ������������ ���������� � ������ ���������
    lem::UCString pattern_name; // ��� �������������� �������� - ��� ����������� ����������� (��������) ��������

    lem::MCollect<lem::UCString> names; // ����� ����������

    lem::UCString root_node_marker; // ��� ����������, ������� �������������� ��� ROOT_NODE.

    WordEntrySet *wordentry_set;
    TrCompilationContext *tr_compilation;

    int ipoint; // ���������������� ������� ������� ����� � ���������

    lem::MCollect<int> export_coords; // ����� ���������� ������ ���� ��������������
    lem::MCollect<lem::UCString> export_nodes; // ����� ������ ������ ���� ��������������

    lem::MCollect<int> null_export_coords; // ��� ���������� �� ����� ��������������, ���� ��������� ��� ��������������
    lem::MCollect<lem::UCString> null_export_nodes; // ���������� ��� ����������� �����

    lem::MCollect<int> actually_exported_coords;
    lem::MCollect<lem::UCString> actually_exported_nodes;

   public:
    SynPatternCompilation( const SynPattern * p );
    ~SynPatternCompilation(void);

    void SetName( const lem::UCString &_pattern_name ) { pattern_name = _pattern_name; }
    const lem::UCString & GetPatternName(void) const { return pattern_name; }

    void Set( WordEntrySet *_sets );

    void RegisterRootNode( const lem::UCString & marker_name );
    const lem::UCString & GetRootNodeMarker() const { return root_node_marker; }

    void AddExportCoord( int id_coord, bool null_export );
    void AddExportNode( const lem::UCString &name, bool null_export );

    bool CoordIsExported( SynGram &sg, int id_coord );
    bool NodeIsExported( const lem::UCString & name );

    const SynPattern * GetPattern(void) const { return pattern_being_compiled; }

    int GetPointNumber(void) const { return ipoint; }
    void BeforeNextPointCompilation(void) { ipoint++; }

    bool IsWordEntrySet( const lem::UCString &name );
    bool IsWordSet( const lem::UCString &name );
    bool IsWordformSet( const lem::UCString &name );
    bool IsCollocationSet( const lem::UCString &name );
    bool IsExportCoord( int id_coord ) const;

    int GetSequenceNumber(void) const { return seq; }

    void CheckThatVariablesTheSame( const SynPatternCompilation &x ) const;
    TrCompilationContext& GetContext(void) const { return *tr_compilation; }
   
    // Store marker name
    void Add( const lem::UCString &name );

    // Is it known marker name?
    virtual int Find( const lem::UCString &name ) const;

    bool IsExportNode( const lem::UCString & node_name ) const;

    bool PatternHasBeenCompiled( SynGram &sg ) const;
  };
  #endif
 }

#endif
