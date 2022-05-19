// -----------------------------------------------------------------------------
// File MACRO_PARSER.H
//
// (c) by Koziev Elijah     all rights reserved 
//
// SOLARIX Intellectronix Project http://www.solarix.ru
//                                http://sourceforge.net/projects/solarix  
//
// You must not eliminate, delete or supress these copyright strings
// from the file!
//
// Content:
// ���������� Macro_Parser - ������ ������ ������ �� ��������� ������ �
// ����������� ������������ ������������� ����� C.
//
// 27.11.2009 - ������� ��������� ������� �����
// -----------------------------------------------------------------------------
//
// CD->31.01.1997
// LC->10.10.2015
// --------------

#ifndef IR_READER__H
#define IR_READER__H
#pragma once

 #include <lem/config.h>
 #include <lem/macros.h>
 #include <lem/containers.h>
 #include <lem/stack.h>
 #include <lem/fstring.h>
 #include <lem/lexers.h>
 #include <lem/sol_io.h>
 #include <lem/ir_parser.h>

 namespace lem
 {
  namespace Iridium
  {

   typedef UTextParser UTS;

   // ���������� ������� ��������� ������������ �������.
   const int SRC_BETH_ECHO_PRINT       = 0x00000001; // ���-������ �� ��������, �������������� ����������
   const int SRC_BETH_JOURNALIZE       = 0x00000002; // ������������ �������� ����������.
   const int SRC_BETH_PRINT_SOURCE     = 0x00000004; // ������ � ������ �������� ������.
   const int SRC_BETH_PRINT_LINE_NUM   = 0x00000008; // �������� � ������ ������ ������������ �����.

   const int BETH_NO_CASE = 0x00010000; // ������ �������������� � ��������

   const int B_EOF_REACHED = 666;


 // ***********************************************************************
 // ����� BSourceState - ���������� � ��������� ������� ������ ��� ������
 // BethSource. ��������� ��������� ������������ �������� ������, �����
 // ������ ���������� ������ � �������� �������.
 // ***********************************************************************
 class BSourceState
 {
  private:
   lem::int16_t ifile;  // ����� ����� � ������.
   int itoken; // ����� ������ � ����� ������ ����������� ������� �����.

  public:
   BSourceState(void)
   {
    ifile=UNKNOWN;
    itoken=UNKNOWN;
    return;
   }

   BSourceState( const BSourceState &bs )
   {
    ifile=bs.ifile;
    itoken=bs.itoken;
    return;
   }

   BSourceState( lem::int16_t iFile, int iToken )
   {
    ifile=iFile;
    itoken=iToken;
    return;
   }

   inline void operator=( const BSourceState &bs )
   { ifile=bs.ifile; itoken=bs.itoken; }

   inline lem::int16_t GetFile(void)  const { return ifile;  }
   inline int GetIndex(void) const { return itoken; }

   inline bool operator==( const BSourceState &bs ) const
   { return itoken==bs.itoken && ifile==bs.ifile; }

   inline bool empty(void) const { return itoken==UNKNOWN; }
 };

 const lem::uint32_t UNKNOWN_TOKEN = 0xffffffffu;

 class BethToken : public UToken
 {
  private:
   const wchar_t *full_str;
   lem::int16_t ifile;  // ������ �����, �� �������� ����� ��� ��������.
   lem::uint32_t itoken; // ������ ������ � ������ �������, ����������� �� ���������� �����.

  public:
   BethToken(void):UToken(),full_str(NULL),ifile(UNKNOWN),itoken(UNKNOWN_TOKEN)
   {}

   BethToken( const UToken &t, lem::int16_t iFile, lem::uint32_t iToken )
   :UToken(t), full_str(NULL), ifile(iFile), itoken(iToken)
   {}

   BethToken( const UToken &t, const wchar_t *FullStr, lem::int16_t iFile, lem::uint32_t iToken )
   :UToken(t), full_str(FullStr), ifile(iFile), itoken(iToken)
   {}

   BethToken( const BethToken &bt )
   :UToken(bt)
   {
    full_str = bt.full_str;
    ifile=bt.ifile;
    itoken=bt.itoken;
    return;
   }

   inline const BethToken& operator=( const BethToken& bt )
   {
    UToken::operator=(bt);
    full_str = bt.full_str;
    ifile=bt.ifile;
    itoken=bt.itoken;
    return *this;
   }

   /**************************************************************
    ���������� ��������� ������� ������� ������ � �����, ������ ��
    ����� ���������.
   ***************************************************************/
   inline const BSourceState GetBegin(void) const
   { return BSourceState(GetFile(),GetIndex()); }

   inline lem::int16_t GetFile(void) const  { return ifile; }
   inline lem::uint32_t GetIndex(void) const { return itoken; }
   int GetLine(void) const { return UToken::GetBegin().GetLine(); }
   int GetSymbolPos(void) const { return UToken::GetBegin().GetSymbolPos(); }

   inline bool operator==( int T ) const { return GetToken()==T; }

   const wchar_t* GetFullStr(void) const { return full_str==NULL ? UToken::c_str() : full_str; }
 };



 class Macro_Parser
 {
  public:
   // ����������� ����� - ��� ������������� ���������� � �������� Base_Parser
   typedef BSourceState pos_type;
   typedef BethToken    token_type;
   typedef UCString     str_type;

   // ������ ����������� ��������, ��� ��� ������ ��������� �� ������
   // ������ ������.
   typedef MCollect<BethToken>::const_iterator const_iterator;
   typedef MCollect<BethToken>::const_reference const_reference;
   typedef MCollect<BethToken>::size_type size_type;

   static UCString t_Diez;
   static UCString t_Double_Diez;
   static UCString t_Define;
   static UCString t_Undefine;
   static UCString t_If;
   static UCString t_Elif;
   static UCString t_Endif;
   static UCString t_Include;
   static UCString t_Error;
   static UCString t_Print;
   static UCString t_Stop;
   static UCString t_File;
   static UCString t_Line;
   static UCString t_Enum;
   static UCString t_Else;
   static UCString t_Pragma;
   static UCString t_OroundParen;
   static UCString t_CroundParen;
   static UCString t_Comma;
   static UCString t_Backslash;
   static UCString t_End;
   static UCString t_Equal;
   static UCString t_Begin;
   static UCString t_Floating;
   static UCString t_Lisp;
   static UCString t_Prolog;
   static UCString t_Pascal;
   static UCString t_Cpp;
   static UCString t_C;
   static UCString t_Comment;
   static UCString t_On;
   static UCString t_Seq;
   static UCString t_Cast;
   static UCString t_LanguageMix;
   static UCString t_Region;
   static UCString t_EndRegion;

  protected:
   Stack<int> block;  // ���� ����������� #if ... #else ... #endif
   IR_Macro_Table macro; // ������ ����������� ��������.
   IR_Var_Table global_var_table; // ������ ���������� ���������� - ��� �������� ���� #define A B.
   int include_level; // ������� ����������� ����������: ������ ������� include ����� 2.

   int line_count; // Total number of parsed lines in source files
                   // (includes are also counted)

   int n_error, n_warning; // �������� ������ � ��������������

   bool do_check_mixage;

   lem::MCollect<BethToken> text; // ������ ����������� �������
   lem::PtrCollect<lem::UFString> full_str; // ���������� ������� �����, ���� ��������� ���� BethToken::full_str

   size_type next; // ������ ���������� ��������� ������ �� ������.
   lem::uint32_t options; // ����������� �����: SRC_BETH_xxx (��. "lem_src.h")
   int i_last_tok; // ������ ���������� ���������� ������.
   const CodeConverter *code_page; // ������� ������� ������� ��� ������ �� ������
   lem::Stack<int> cp_stack; // ���� �������� ������� ������ - ��� ��������� #pragma CodeConverter

   lem::uint32_t src_set_flags; // ����� ��� ���������� ��������� ������ UTS.
   lem::uint32_t src_clr_flags;

   MCollect<UCString> tokens;
   MCollect<UCString> compl_delims;
   lem::Collect<lem::Path> flist; // ������ ���� ������, �� ������� ������� �����

   UFString root_dir; // ������� � ��������� �����������

   lem::Sol_IO ios; // ������ ��� ������ ���������.

   static int iseq; // ������� ��� ������ �������������� #seq, ������� ����
                    // ���������������� �������� ����� �����  

   bool ProbeOption( lem::uint32_t Option ) const;

   /***********************************************************
    �������� �� ������� ������ ������ �� ������������ ��������?
   ************************************************************/
   inline bool IsMacro( const UCString &s ) const
   { return macro.Find(s)!=UNKNOWN; }

   void AddMacro( UTS &txtfile );
   void DelMacro( UTS &txtfile );
   void DeclareIf( UTS &txtfile );
   void ProcessElse( UTS &txtfile, const UToken&);
   void ProcessEndif( UTS &txtfile, const UToken&);
   void ErrorMessage( UTS &txtfile );
   void PrintMessage( UTS &txtfile );
   void StopReading( UTS &txtfile );
   const UToken CastString( UTS &txtfile );
   void Reach_EOL( UTS &txtfile );
   void Pragma( UTS &txtfile );
   void Sequence( UTS &txtfile );
   void Cast( UTS &txtfile );
   void InsertEnumerated( UTS &txtfile );
   void Postprocess( lem::Collect<UFString> &list ) const;
   void Unroll(
               const UToken &t,
               lem::Collect<UFString> &list,
               UTS &txtfile
              );
   void Merge( UTS &txtfile ); 

   void LoadText( const wchar_t *filename );
   void LoadText( lem::Stream *file );
   void LoadText( UTS &txtfile );

   // ��� ���������� ������� ������ - ��������� ��������� �������� (���
   // ������� ������ ���������� �����). �������� - ������� �������� - �����
   // �� 0 �� 100.
   // ���������������� ����� ����� ����������� ������� ��� ������� �������
   // ����������� ��������� (�������� � GUI).
   virtual void Echo_Progress_Dot( int percent );

  public:
   Macro_Parser(void);
   
   #if defined LEM_UNIX
   virtual ~Macro_Parser(void);
   #endif            

   inline void Set_io( const lem::Sol_IO &IO ) { ios=IO; }
   inline const Sol_IO& Get_io(void) const { return ios; }

   void Open( const wchar_t *filename );
   void Open( lem::Stream *file );

   inline const CodeConverter * GetCP(void) const { return code_page; }
   inline Sol_IO& GetIO(void) { return ios; }

   inline int GetnError(void) const { return n_error; }
   inline int GetnWarning(void) const { return n_warning; }

   inline void RegError(void)   { n_error++; }
   inline void RegWarning(void) { n_warning++; }

   void SetDefines( const MCollect<UCString> &defines );
   void SetOptions( lem::uint32_t opt, bool on_off=true );
   void SetSrcOption( lem::uint32_t option, bool flag );
   void SetCodeConverter( const CodeConverter *CP );
  
   inline void SetRootDir( const UFString& s ) { root_dir=s; }

   void SetComplDelims( const MCollect<UCString> &Delims );
   void SetTokens( const MCollect<UCString> &Tokens );

   void SetTokens( int n, const UCString *list );
   void SetComplDelims( int n, const UCString *list );

   inline const MCollect<UCString>& GetTokens(void) const { return tokens; }

   virtual const BethToken& read(void); // ������� ����� � ������ ������������ �� ���������
   virtual const BethToken& pick(void); // ������� ����� ��� ����������� �������
   virtual void read_it( const int token );
   virtual void read_it( const UCString &token );
   virtual int read_int(void);
   virtual int read_signed_int(void);

   // ��� ������ ��� ������ ������, ���� ������� - ������������ true � ������
   // �������������, ����� ������ �������� �� ������� ����� � ������������ false;
   virtual bool probe( const int token );
   virtual bool probe( const UCString &token );

   #if !defined LEM_NOREAL
   virtual double read_real(void);
   #endif

   inline const BethToken& get( const BSourceState &s ) const
   { return text[s.GetIndex()]; }

   inline const BethToken& get( int itok ) const { return text[itok]; }
   inline size_type GetNext(void) const { return next; }
   inline void backward(void) { if(next) next--; }

   void ToLineBegin(void);
   inline bool eof(void) { return next>=text.size()-1; }

   const BSourceState tellp(void) const;
   void seekp( const BethToken &bt );
   void seekp( const BSourceState &ss );

   friend void Print_Error( const BethToken& t, Macro_Parser& src, bool Warn_Only=false );
   friend void Print_Error( const BSourceState& s, Macro_Parser& src, bool Warn_Only=false );
   friend void Print_Error( Macro_Parser& src, bool Warn_Only=false );

   inline int GetnFile(void) const { return CastSizeToInt(flist.size()); }

   inline const lem::Path& GetFileName( int i ) const
   { return flist[i]; }

   inline size_type GetiNext(void) const { return next; }

   inline int GetLineCount(void) const { return line_count; }

   inline const_iterator begin(void) const { return text.begin(); }
   inline const_iterator end(void)   const { return text.end();   }
   inline const MCollect<BethToken>& lexems(void) const { return text; }
 };


 /****************************************************************
  ������������� ������ ������� �������, ������� ����� �� ���������
  ����������� ��� ���������� ANSI-�������� � UNICODE.
 *****************************************************************/
 inline void Macro_Parser::SetCodeConverter( const CodeConverter *CP )
 { code_page=CP; }


  class BethSourceList : public Macro_Parser
  {
   public:
    BethSourceList( const MCollect<UToken> &List, int ifrom );

    LEM_GNUC_VIRTUAL ~BethSourceList(void) {}
  };

    extern void Print_Error( Sol_IO &io, UTextParser &txtfile );
    extern void Print_Error( Sol_IO &io, const UToken &t, UTextParser &txtfile );

    #if defined LEM_GNU
     extern void Print_Error( const BethToken& t, Macro_Parser& src, bool Warn_Only=false );
     extern void Print_Error( const BSourceState& s, Macro_Parser& src, bool Warn_Only=false );
     extern void Print_Error( Macro_Parser& src, bool Warn_Only=false );
     extern void Print_Error( Sol_IO &io, const BethToken& t, UTextParser &txtfile, bool Warn_Only=false );
    #else
     extern void Print_Error( const BethToken& t, Macro_Parser& src, bool Warn_Only );
     extern void Print_Error( const BSourceState& s, Macro_Parser& src, bool Warn_Only );
     extern void Print_Error( Macro_Parser& src, bool Warn_Only );
     extern void Print_Error( Sol_IO &io, const BethToken& t, UTextParser &txtfile, bool Warn_Only );
    #endif

   } // namespace 'Iridium'
 } // namespace 'lem'

#endif

