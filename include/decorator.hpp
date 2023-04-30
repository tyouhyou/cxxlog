/* *
 * Decorate a function.
 *
 * @author  tyouhyou    github.com/tyouhyou
 * @license GPL
 * */

#pragma once

#include <functional>

namespace zb
{
    /*
     * reference:
     * https://stackoverflow.com/questions/28498852/c-function-decorator
     */
    template <class TR>
    struct deco
    {
        template <class TF, class... TARG>
        deco(TF &&fun, TARG &&...args)
            : val(std::forward<TF>(fun)(std::forward<TARG>(args)...))
        {
        }

        TR &&value()
        {
            return static_cast<TR &&>(val);
        }

    private:
        TR val;
    };

    template <>
    struct deco<void>
    {
        template <class TF, class... TARG>
        deco(TF &&fun, TARG &&...args)
        {
            std::forward<TF>(fun)(std::forward<TARG>(args)...);
        }

        void value()
        {
        }
    };

    template <class TF, class TB, class TA>
    auto deco_beforeafter(TF &&fun, TB &&before, TA &&after)
    {
        return
            [fun = std::forward<TF>(fun),
             before = std::forward<TB>(before),
             after = std::forward<TA>(after)](auto &&...args) -> decltype(auto)
        {
            before();
            deco<std::result_of_t<TF(decltype(args)...)>> ret(
                fun, std::forward<decltype(args)>(args)...);
            after();
            return ret.value();
        };
    }

} // namespace zb