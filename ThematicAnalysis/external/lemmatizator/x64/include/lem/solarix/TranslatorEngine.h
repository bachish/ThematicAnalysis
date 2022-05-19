#if !defined TRANSLATOR_ENGINE__H
 #define TRANSLATOR_ENGINE__H

 #include <lem/solarix/dictionary.h>
 #include <lem/solarix/text_processor.h>
 #include <lem/solarix/Lemmatizator.h>
 #include <lem/solarix/ThTranslator.h>

 namespace Solarix
 {

  // **************************************
  // ������ ��������
  // **************************************
  class TranslatorEngine : lem::NonCopyable
  {
   public:
    lem::Ptr<Solarix::Dictionary> dict;
    lem::Ptr<Solarix::Lemmatizator> lemmatizator; // ����������� ������������
    lem::Ptr<Solarix::TranslatorThesaurus> th_transl; // ���������� ������ ������� ���������

    lem::Path htmlfile;

    int default_yield_format;

    enum { TxtFormat, HtmlFormat, XmlFormat, Html0Format };


   private:
    lem::Ptr<Solarix::Text_Processor> tpu;

    lem::Collect< std::pair<lem::UFString,lem::UFString> > tag_subst; // ������ �������� ����������� ���� <x1> �� ����������� HTML ����
    lem::Collect< std::pair<lem::UFString,lem::UFString> > tag_removal; // �������� ����� ��� ��������� plain text ��������

    lem::OFormatter nul_tty;
    lem::zbool server; 
    lem::FString ip, port;
    lem::zbool use_get_method, use_ngrams;
    lem::UCString scenario;
    lem::UCString language_name;
    Solarix::SyntaxAnalyzerTimeout timeout;
    lem::zbool fulltext; // �������� �� ����������� ������� �������������� ������������

    lem::Ptr<lem::OUFormatter> out_file;

    lem::Ptr<Solarix::SG_TagFilter> transl0_tag; // ����������� ������ ��� ������ ������ ��������� � transl_order=0
    lem::Ptr<Solarix::SG_TagFilter> transl1_tag; // ����������� ������ ��� ������ ������ �������� ���������

    lem::Ptr<Solarix::TrEnvironment> global;

    lem::Path folder;
    lem::Path dict_path; // ���� � ����� dictionary.xml
    lem::Path out_path;
    lem::Path pidfile;

    bool already_loaded;

    int default_language;

    inline bool IsHtml( int yield_format )
    { return yield_format==HtmlFormat || yield_format==Html0Format; }

    bool DoSimple0( OFormatter & html, const lem::UFString &sent_str, int yield_format );
    void DoSimple1( OFormatter & html, const lem::UFString &sent_str, int yield_format );
    void ShowTranslations( OFormatter & html, const lem::PtrCollect<lem::UFString> & rights, int yield_format );

   protected:
    int ExitCode;

   public:
    lem::zbool ok;

   public:
    TranslatorEngine(void);
    virtual ~TranslatorEngine(void);

    virtual void Init( int argc, char *argv[] );

    virtual void LoadModulesFromPath( const lem::Path &p );
    virtual void LoadModules(void);

    static void ProcessSignal( int isig );

    virtual void PingConnection(void);

    virtual void Run(void);

    virtual int GetExitCode(void);

    lem::UFString VarToStr( const Solarix::Variator &var ) const;

    void Do(
            const lem::UFString &str0, // �������� �������������� �����
            lem::Ptr<Solarix::SG_TagFilter> tags_ptr, // ����������� ����
            lem::OFormatter * out, // ��������� ��� ������ ����������� �������������
            int yield_format /*������ ���������� 0-plain text, 1-html, 2-xml*/
           );

    void Do(
            const lem::UFString &str0, // �������� �������������� �����
            lem::Ptr<Solarix::SG_TagFilter> tags_ptr, // ����������� ����
            lem::OFormatter * out, // ��������� ��� ������ ����������� �������������
            int yield_format, /*������ ���������� 0-plain text, 1-html, 2-xml*/
            const char *label 
           );

    void Do(
            const lem::UFString &str0, // �������� �������������� �����
            lem::OFormatter * out, // ��������� ��� ������ ����������� �������������
            int yield_format /*������ ���������� 0-plain text, 1-html, 2-xml*/
           );
  };

 } // namespace Solarix

#endif
