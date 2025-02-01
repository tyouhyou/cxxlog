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
    class Deco
    {
    public:
        auto wrap(std::function<TR(TARGS...)> fn) -> Deco<TR, TARGS...> &
        {
            fun = std::forward<std::function<TR(TARGS...)>>(fn);
            return *this;
        }

        auto wrap_before(std::function<bool(TARGS...)> fn) -> Deco<TR, TARGS...> &
        {
            fn_b = std::forward<std::function<bool(TARGS...)>>(fn);
            return *this;
        }

        auto wrap_after(std::function<bool(TARGS...)> fn) -> Deco<TR, TARGS...> &
        {
            fn_a = std::forward<std::function<bool(TARGS...)>>(fn);
            return *this;
        }

        virtual TR operator()(TARGS... args) = 0;

        auto call(TARGS... args) -> Deco<TR, TARGS...> &
        {
            if (Deco<TR, TARGS...>::fn_b)
            {
                fn_b_rst = Deco<TR, TARGS...>::fn_b(std::forward<TARGS>(args)...);
                if (!fn_b_rst)
                {
                    return *this;
                }
            }
            _call(std::forward<TARGS>(args)...);
            if (Deco<TR, TARGS...>::fn_a)
            {
                fn_a_rst = Deco<TR, TARGS...>::fn_a(std::forward<TARGS>(args)...);
            }
            return *this;
        }

        TR result(bool &rst_fn_before, bool &rst_fn_after)
        {
            rst_fn_before = fn_b_rst;
            rst_fn_after = fn_a_rst;
            return _result();
        }

        virtual std::shared_ptr<TR> presult(bool &rst_fn_before, bool &rst_fn_after)
        {
            rst_fn_before = fn_b_rst;
            rst_fn_after = fn_a_rst;
            return _presult();
        }

    protected:
        std::function<TR(TARGS...)> fun;
        std::function<bool(TARGS...)> fn_b;
        std::function<bool(TARGS...)> fn_a;

        bool fn_b_rst = false;
        bool fn_a_rst = false;

        virtual void _call(TARGS... args) = 0;
        virtual TR _result() = 0;
        virtual std::shared_ptr<TR> _presult() = 0;
    };

    template <class TR, class... TARGS>
    class Deco_func : public Deco<TR, TARGS...>
    {
    public:
        Deco_func<TR, TARGS...>() = default;

        TR operator()(TARGS... args) override
        {
            Deco<TR, TARGS...>::call(std::forward<TARGS>(args)...);
            return _result();
        }

    protected:
        TR _result() override { return rst; }
        std::shared_ptr<TR> _presult() override { return std::make_shared<TR>(rst); }
        void _call(TARGS... args) override
        {
            rst = Deco<TR, TARGS...>::fun(std::forward<TARGS>(args)...);
        }

    private:
        TR rst;
    };

    template <class... TARGS>
    class Deco_action : public Deco<void, TARGS...>
    {
    public:
        Deco_action<TARGS...>() = default;
        void operator()(TARGS... args) override
        {
            Deco<void, TARGS...>::call(std::forward<TARGS>(args)...);
        }

    protected:
        void _result() override {}
        std::shared_ptr<void> _presult() override { return nullptr; }
        void _call(TARGS... args) override
        {
            Deco<void, TARGS...>::fun(std::forward<TARGS>(args)...);
        }
    };

} // namespace zb