/* *
* string converter
* 
 * @author  tyouhyou    github.com/tyouhyou
 * @license MIT
 * */

#pragma once

#include <locale>
#include <codecvt>
#include <string>

#define S2WL strcvt::str_to_wstr_w_locale
#define W2SL strcvt::wstr_to_str_w_locale
#define W2U8(wstr) std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr)
#define U82W(u8s) std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(u8s)

namespace zb
{
    class strcvt
    {
    public:
        /*
		* return an empty std::string if convertion failed.
		*/
        static std::string wstr_to_str_w_locale(const std::wstring str, const std::string &sloc = "")
        {
            std::locale loc(sloc);
            auto &facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
            std::string out(str.size() * facet.max_length(), '\0');
            const wchar_t *f_next;
            char *t_next;
            std::mbstate_t mb{};
            auto rst = facet.out(mb, &str[0], &str[str.size()], f_next,
                                 &out[0], &out[out.size()], t_next);
            if (std::codecvt_base::ok != rst)
            {
                return std::string();
            }
            out.resize(t_next - &out[0]);
            return out;
        }

        /*
		* return an empty std::wstring if convertion failed.
		*/
        static std::wstring str_to_wstr_w_locale(const std::string str, const std::string &sloc = "")
        {
            std::locale loc(sloc);
            auto &facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
            std::wstring out(str.size() * facet.max_length(), '\0');
            const char *f_next;
            wchar_t *t_next;
            std::mbstate_t mb{};
            auto rst = facet.in(mb, &str[0], &str[str.size()], f_next,
                                &out[0], &out[out.size()], t_next);
            if (std::codecvt_base::ok != rst)
            {
                return std::wstring();
            }
            out.resize(t_next - &out[0]);
            return out;
        }
    };
} // namespace zb