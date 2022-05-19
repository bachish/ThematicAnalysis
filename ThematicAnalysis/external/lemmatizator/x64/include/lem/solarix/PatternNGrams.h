#if !defined PatternNGrams__H
 #define PatternNGrams__H


 #include <lem/ucstring.h>
 #include <lem/ptr_container.h>
 #include <lem/solarix/NGramScore.h>
 
 namespace lem
 {
  namespace Iridium
  {
   class Macro_Parser;
  }
 }


 namespace Solarix
 {
  class Dictionary;
  struct SynPatternResult;
  class KnowledgeBase;
  class TreeMatchingExperience;

  class VariableChecker;

  // ���� �������� � ������ : ngrams { ... }
  class PatternNGram
  {
   private:
    int id_facts;
    lem::MCollect<lem::UCString> backref_name, node_name; // ������ ���������� ��� �������� ����� - ���� ����������.����

   public:
    PatternNGram(void);
    
    PatternNGram( const PatternNGram & x );
    void operator=( const PatternNGram & x );

    bool operator!=( const PatternNGram & x ) const;

    #if defined SOL_LOADTXT && defined SOL_COMPILER
    void LoadTxt( 
                 Dictionary &dict,
                 lem::Iridium::Macro_Parser & txtfile,
                 VariableChecker & compilation_context
                );
    #endif    

    void SaveBin( lem::Stream &bin ) const;
    void LoadBin( lem::Stream &bin );

    #if defined SOL_CAA
    int Match(
              Dictionary & dict,
              const lem::MCollect<int> & PatternSequenceNumber,
              const SynPatternResult * cur_result,
              KnowledgeBase & kbase,
              TreeMatchingExperience &experience
             ) const;
    #endif
  };


  struct SynPatternResultBackTrace;

  // ������ � ������� ������ ������ tree_scorer � ������ : ngrams { ... }
  class PatternNGramTreeScorer
  {
   private:
    lem::UCString root_marker; // ����� ���� ����� �� ������ ������������ ��������� �����
    lem::UCString group_name;
    int id_group;
    int missing_score; // ����� �� ���������������

    #if defined SOL_CAA
    const Solarix::Word_Form* GetRootNode(
                                          Dictionary & dict,
                                          const lem::MCollect<int> & PatternSequenceNumber,
                                          const SynPatternResult * cur_result
                                         ) const;
    #endif

   public:
    PatternNGramTreeScorer();
    
    PatternNGramTreeScorer( const PatternNGramTreeScorer & x );
    void operator=( const PatternNGramTreeScorer & x );

    bool operator!=( const PatternNGramTreeScorer & x ) const;

    #if defined SOL_LOADTXT && defined SOL_COMPILER
    void LoadTxt( 
                 int id_group0,
                 Dictionary &dict,
                 lem::Iridium::Macro_Parser & txtfile,
                 VariableChecker & compilation_context
                );
    #endif    

    void SaveBin( lem::Stream &bin ) const;
    void LoadBin( lem::Stream &bin );

    #if defined SOL_CAA
    int Match(
              Dictionary & dict,
              const lem::MCollect<int> & PatternSequenceNumber,
              const SynPatternResultBackTrace * x_result,
              SynPatternResult * cur_result,
              KnowledgeBase & kbase,
              TreeMatchingExperience &experience,
              const ElapsedTimeConstraint & constraints,
              TrTrace *trace_log
             ) const;
    #endif
  };


  // ��������� ������� �� ������ ���������������� ������� � ������ : ngrams { ... }
  class PatternNGramFunction
  {
   private:
    lem::UCString function_name;
    lem::MCollect<lem::UCString> function_args; // ����� ����������, ������� ���������� � ������� ��� ���������
    lem::MCollect<TrType> arg_types; // ���� ���������� � ��������� �������, ����� ���������� ���� ������ ���������� ��� ������� ������ ��� ����������
    lem::Ptr<TrFunCall> fun; // ���� ���� �������� ������� �������� ������

    #if defined SOL_CAA
    Solarix::Tree_Node* GetTreeByRootName(
                                          const lem::UCString & root_name,
                                          Dictionary & dict,
                                          const lem::MCollect<int> & PatternSequenceNumber,
                                          const SynPatternResult * cur_result,
                                          bool attach_children
                                         ) const;

    Solarix::Word_Form* GetWordform4Tree( const Solarix::Word_Form * src_wf, Dictionary & dict, const SynPatternResult * cur_result ) const;

    void AttachEdges(
                     const Solarix::Word_Form * root_wf,
                     Solarix::Tree_Node * root,
                     Dictionary & dict,
                     const lem::MCollect<int> & PatternSequenceNumber,
                     const SynPatternResult * cur_result
                    ) const;

    #endif 


   public:
    PatternNGramFunction();
    
    PatternNGramFunction( const PatternNGramFunction & x );
    void operator=( const PatternNGramFunction & x );

    bool operator!=( const PatternNGramFunction & x ) const;

    #if defined SOL_LOADTXT && defined SOL_COMPILER
    void LoadTxt( 
                 Dictionary &dict,
                 lem::Iridium::Macro_Parser & txtfile,
                 VariableChecker & compilation_context
                );
    #endif    

    void SaveBin( lem::Stream &bin ) const;
    void LoadBin( lem::Stream &bin );
    void Link( const TrFunctions &funs );

    #if defined SOL_CAA
    int Match(
              Dictionary & dict,
              const lem::MCollect<int> & PatternSequenceNumber,
              const SynPatternResultBackTrace * x_result,
              SynPatternResult * cur_result,
              KnowledgeBase & kbase,
              TreeMatchingExperience &experience,
              const ElapsedTimeConstraint & constraints,
              TrTrace *trace_log
             ) const;
    #endif
  };



  class PatternNGrams
  {
   private:
    lem::PtrCollect<PatternNGram> ngrams;
    lem::PtrCollect<PatternNGramTreeScorer> tree_scorers;
    lem::PtrCollect<PatternNGramFunction> functions;
    int explicit_scores; // ���� ��� ����� ����

   public:
    PatternNGrams(void);
    
    PatternNGrams( const PatternNGrams & x );
    void operator=( const PatternNGrams & x );

    bool operator!=( const PatternNGrams & x ) const;

    #if defined SOL_LOADTXT && defined SOL_COMPILER
    void LoadTxt( 
                 Dictionary &dict,
                 lem::Iridium::Macro_Parser & txtfile,
                 VariableChecker & compilation_context,
                 TrFunctions & user_functions
                );
    #endif    

    void SaveBin( lem::Stream &bin ) const;
    void LoadBin( lem::Stream &bin );
    void Link( const TrFunctions &funs );

    #if defined SOL_CAA
    NGramScore Match(
                     Dictionary & dict,
                     const lem::MCollect<int> & PatternSequenceNumber,
                     const SynPatternResultBackTrace * x_result,
                     SynPatternResult * cur_result,
                     KnowledgeBase & kbase,
                     TreeMatchingExperience &experience,
                     const ElapsedTimeConstraint & constraints,
                     TrTrace *trace_log
                    ) const;
    #endif
  };

 }

#endif
