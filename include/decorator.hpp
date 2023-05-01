/* *
 * Decorate a function.
 *
 * @author  tyouhyou    github.com/tyouhyou
 * @license GPL
 * */

#pragma once

#include <functional>
#include <type_traits>

namespace zb
{
    template <class TR, class... TARGS>
    struct deco_func
    {
        auto wrap(std::function<TR(TARGS...)> fn) -> deco_func<TR, TARGS...>&
        {
            fun = std::forward<std::function<TR(TARGS...)>>(fn);
            return *this;
        }

        auto wrap_before(std::function<void()> fn) -> deco_func<TR, TARGS...>&
        {
            fn_b = std::forward<std::function<void()>>(fn);
            return *this;
        }

        auto wrap_after(std::function<void()> fn) -> deco_func<TR, TARGS...>&
        {
            fn_a = std::forward<std::function<void()>>(fn);
            return *this;
        }

        TR &&call(TARGS... args)
        {
            if (fn_b) fn_b();
            auto rst = fun(std::forward<TARGS>(args)...);
            if (fn_a) fn_a();
            return std::move(static_cast<TR &&>(rst));
        }

    private:
        std::function<TR(TARGS...)> fun;
        std::function<void()> fn_b;
        std::function<void()> fn_a;
    };

    template <class... TARGS>
    struct deco_action
    {
        auto wrap(std::function<void(TARGS...)> fn) -> deco_action<TARGS...>&
        {
            fun = std::forward<std::function<void(TARGS...)>>(fn);
            return *this;
        }

        auto wrap_before(std::function<void()> fn) -> deco_action<TARGS...>&
        {
            fn_b = std::forward<std::function<void()>>(fn);
            return *this;
        }

        auto wrap_after(std::function<void()> fn) -> deco_action<TARGS...>&
        {
            fn_a = std::forward<std::function<void()>>(fn);
            return *this;
        }

        void call(TARGS... args)
        {
            if (fn_b) fn_b();
            fun(std::forward<TARGS>(args)...);
            if (fn_a) fn_a();
        }

    private:
        std::function<void(TARGS...)> fun;
        std::function<void()> fn_b;
        std::function<void()> fn_a;
    };

} // namespace zb