#pragma once

#include <memory>
#include <string>
#include <locale>
#include <codecvt>

namespace zqdb {

// utf8转unicode
inline std::wstring utf8_to_unicode(const std::string &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_cvt;
    return utf8_cvt.from_bytes(str);
}

// unicode转utf8
inline std::string unicode_to_utf8(const std::wstring &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_cvt;
    return utf8_cvt.to_bytes(str);
}

// gbk转unicode
inline std::wstring  gbk_to_unicode(const std::string &str)
{
    std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> gbk_cvt(new std::codecvt<wchar_t, char, std::mbstate_t>("chs"));
    return gbk_cvt.from_bytes(str);
}

// unicode转gbk
inline std::string  unicode_to_gbk(const std::wstring &str)
{
    std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> gbk_cvt(new std::codecvt<wchar_t, char, std::mbstate_t>("chs"));
    return gbk_cvt.to_bytes(str);
}

// utf8转gbk
inline std::string utf8_to_gbk(const std::string &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_cvt; // utf8-》unicode转换器
    std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> gbk_cvt(new std::codecvt<wchar_t, char, std::mbstate_t>("chs")); // unicode-》gbk转换器
    std::wstring t = utf8_cvt.from_bytes(str);
    return gbk_cvt.to_bytes(t);
}

// gbk转utf8
inline std::string gbk_to_utf8(const std::string &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_cvt; // utf8-》unicode转换器
    std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> gbk_cvt(new std::codecvt<wchar_t, char, std::mbstate_t>("chs")); // unicode-》gbk转换器
    std::wstring t = gbk_cvt.from_bytes(str);
    return utf8_cvt.to_bytes(t);
}

inline std::string gb2312_to_utf8(const std::string &strGb2312)
{
    std::vector<wchar_t> buff(strGb2312.size());
#ifdef WIN32
    std::locale loc("zh-CN");
#else
    std::locale loc("zh_CN.GB18030");
#endif
    wchar_t *pwszNext = nullptr;
    const char *pszNext = nullptr;
    mbstate_t state = {};
    int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t>>(loc).in(state,
                                                                             strGb2312.data(), strGb2312.data() + strGb2312.size(), pszNext,
                                                                             buff.data(), buff.data() + buff.size(), pwszNext);

    if (std::codecvt_base::ok == res)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
        return cutf8.to_bytes(std::wstring(buff.data(), pwszNext));
    }

    return "";
}

inline std::string utf8_to_gb2312(const std::string &strUtf8)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
    std::wstring wTemp = cutf8.from_bytes(strUtf8);
#ifdef WIN32
    std::locale loc("zh-CN");
#else
    std::locale loc("zh_CN.GB18030");
#endif
    const wchar_t *pwszNext = nullptr;
    char *pszNext = nullptr;
    mbstate_t state = {};

    std::vector<char> buff(wTemp.size() * 2);
    int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t>>(loc).out(state,
                                                                              wTemp.data(), wTemp.data() + wTemp.size(), pwszNext,
                                                                              buff.data(), buff.data() + buff.size(), pszNext);

    if (std::codecvt_base::ok == res)
    {
        return std::string(buff.data(), pszNext);
    }
    return "";
}

}
