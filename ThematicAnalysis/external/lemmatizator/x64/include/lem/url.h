// CD->01.10.2005
// LC->17.06.2008
// --------------

#ifndef LEM_URL__H
#define LEM_URL__H
#pragma once

 #include <lem/config.h>
 #include <lem/zeroed.h>
 #include <list>
 #include <lem/fstring.h>
 #include <lem/cstring.h>

 namespace lem
 {
  // ������ ���������� ��� ����������� � ��������-�������, �������
  // ��� ������������ � ������
  class Url
  {
   public:
    enum { MAX_URL_LEN = 1024 };

    UFString full_address; // �������� ������ �����

    // ����������� �� ������ ��������

    UFString login;     // ����� � ������ ��� �����������
    UFString password;

    UCString protocol; // �������� ���������
    UFString adr;      // ����� - ��� ��������� � �����
    UFString path;     // ���� � ���������
    int port;          // ����� �����

    lem::zbool ok;     // ������ ��� �������� ������
  
   private:
    void HttpProtocol(void);
    void Parse( bool exception );
    void Canonize( bool exception );

   public:
    Url(void) { port=0; }

    Url( const UFString &Address, bool exception=true );

    Url( const Url &base, const UFString &doc, bool exception=true );

    Url( const Url &x )
     : full_address(x.full_address),
       login( x.login ),
       password( x.password ),
       protocol( x.protocol ),
       adr( x.adr ),
       path( x.path ),
       port( x.port ), ok(x.ok)
    {}

    inline void operator=( const Url &x )
    {
     full_address = x.full_address;
     login        = x.login;
     password     = x.password;
     protocol     = x.protocol;
     port         = x.port;
     path         = x.path;
     adr          = x.adr;
     ok           = x.ok;
    }

    inline const UFString& string(void) const { return full_address; }

    inline int length(void) const { return full_address.size(); }

    void SaveBin( lem::Stream &bin ) const;
    void LoadBin( lem::Stream &bin );

    // ������ ������ ������, � ������� ������������� �������� � ������ URL'��
    // �� ���� ��� URL "http://solarix.ru/index.shtml ������ "http://solarix.ru"
    // ���� �� URL ��������� �� ����� ��� �� �������, �� ���� ����� ���
    // "http://solarix.ru/for_users", �� ������ �������� ������
    const UFString GetDomainString(void) const;

    const UFString GetDocumentString(void) const;

    static bool IsUrl( const wchar_t *str );
  };


  extern void GetUrlParams(
                           const char *url,
                           std::list< std::pair<std::string/*var*/,std::wstring/*value*/> > &lst
                          );

 } // namespace lem

#endif
