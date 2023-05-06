/* *
 * Decorate a function.
 *
 * @author  tyouhyou    github.com/tyouhyou
 * @license GPL
 * */

#pragma once

#include <functional>
#include <memory>
#include <type_traits>

namespace zb
{
    template <class TR, class... TARGS>
    class deco
    {
    public:
        auto wrap(std::function<TR(TARGS...)> fn) -> deco<TR, TARGS...>&
        {
            fun = std::forward<std::function<TR(TARGS...)>>(fn);
            return *this;
        }

        auto wrap_before(std::function<void()> fn) -> deco<TR, TARGS...>&
        {
            fn_b = std::forward<std::function<void()>>(fn);
            return *this;
        }

        auto wrap_after(std::function<void()> fn) -> deco<TR, TARGS...>&
        {
            fn_a = std::forward<std::function<void()>>(fn);
            return *this;
        }

        virtual std::shared_ptr<TR> call(TARGS... args) = 0;

    protected:
        std::function<TR(TARGS...)> fun;
        std::function<void()> fn_b;
        std::function<void()> fn_a;
    };

    template <class TR, class... TARGS>
    class deco_func : public deco<TR, TARGS...> 
    {
    public:
        std::shared_ptr<TR> call(TARGS... args) override
        {
            if (deco<TR, TARGS...>::fn_b) deco<TR, TARGS...>::fn_b();
            auto rst = deco<TR, TARGS...>::fun(std::forward<TARGS>(args)...);
            if (deco<TR, TARGS...>::fn_a) deco<TR, TARGS...>::fn_a();
            return std::make_shared<TR>(rst);
        }
    };

    template <class... TARGS>
    class deco_action : public deco<void, TARGS...>
    {
    public:
        std::shared_ptr<void> call(TARGS... args) override
        {
            if (deco<void, TARGS...>::fn_b) deco<void, TARGS...>::fn_b();
            deco<void, TARGS...>::fun(std::forward<TARGS>(args)...);
            if (deco<void, TARGS...>::fn_a) deco<void, TARGS...>::fn_a();
            return nullptr;
        }
    };

} // namespace zb