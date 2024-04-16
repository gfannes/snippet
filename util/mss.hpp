#ifndef HEADER_util_mss_hpp_ALREAD_INCLUDED
#define HEADER_util_mss_hpp_ALREAD_INCLUDED

#include <any>
#include <cassert>
#include <expected>
#include <functional>
#include <list>
#include <optional>
#include <type_traits>

namespace mss {

    using Callback = std::function<void(const std::any &expr, const char *file, unsigned int line, const char *function)>;
    using Callbacks = std::list<Callback>;

    inline Callbacks &callbacks()
    {
        static Callbacks s_callbacks;
        return s_callbacks;
    }

    inline void on_failure(Callback callback)
    {
        callbacks().push_back(callback);
    }

    inline bool is_ok(int v) { return v == 0; }
    inline bool is_ok(bool v) { return v; }
    template<typename T>
    bool is_ok(const std::optional<T> &opt)
    {
        return !!opt;
    }
    template<typename T, typename E>
    bool is_ok(const std::expected<T, E> &exp)
    {
        return exp.has_value();
    }

    template<typename Expr>
    void coerce(bool &rc, const Expr &expr)
    {
        rc = is_ok(expr);
    }
    template<typename T, typename Expr>
    void coerce(std::optional<T> &rc, const Expr &expr)
    {
        if (!is_ok(expr))
            rc.reset();
    }
    template<typename T, typename E, typename Expr>
    void coerce(std::expected<T, E> &rc, const Expr &expr)
    {
        if (!is_ok(expr))
        {
            if constexpr (std::is_same_v<Expr, E>)
                rc = std::unexpected(expr);
            else
                rc = std::unexpected(E{});
        }
        else
        {
            if constexpr (std::is_same_v<Expr, T>)
                rc = expr;
        }
    }

    template<typename RC, typename Expr>
    bool coerce(RC &rc, const Expr &expr, const char *file, unsigned int line, const char *function)
    {
        const bool ok = is_ok(expr);
        if (!ok)
            for (const auto &callback : callbacks())
                callback(expr, file, line, function);

        if constexpr (std::is_same_v<RC, Expr>)
            rc = expr;
        else
            coerce(rc, expr);

        return ok;
    }

} // namespace mss

#define MSS_BEGIN(rc) \
    auto l_mss_rc = rc

#define MSS_END() \
    return l_mss_rc

#define MSS(expr)                                                       \
    if (!mss::coerce(l_mss_rc, expr, __FILE__, __LINE__, __FUNCTION__)) \
    {                                                                   \
        return l_mss_rc;                                                \
    }

#endif
