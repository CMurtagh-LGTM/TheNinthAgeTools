#pragma once

// Borrowed from llvm libc

template <class _Tp, _Tp __v>
struct integral_constant {
  static inline constexpr const _Tp value = __v;
  typedef _Tp value_type;
  typedef integral_constant type;
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <bool __b>
using bool_constant = integral_constant<bool, __b>;

// type_identity
template <typename T> struct type_identity {
  using type = T;
};

template <typename...> inline constexpr bool always_false = false;

// add_rvalue_reference
namespace detail {
template <class T>
auto try_add_rvalue_reference(int) -> type_identity<T &&>;
template <class T> auto try_add_rvalue_reference(...) -> type_identity<T>;
} // namespace detail
template <class T>
struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0)) {
};
template <class T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

// declval
template <typename T> add_rvalue_reference_t<T> declval() {
  static_assert(always_false<T>, "declval not allowed in an evaluated context");
}

// is_void
template <class _Tp>
struct is_void : bool_constant<__is_same(__remove_cv(_Tp), void)> {};
template <class _Tp>
inline constexpr bool is_void_v = __is_same(__remove_cv(_Tp), void);

// is_convertible
namespace detail {
template <class T>
auto test_returnable(int)
    -> decltype(void(static_cast<T (*)()>(nullptr)), true_type{});
template <class> auto test_returnable(...) -> false_type;
template <class From, class To>
auto test_implicitly_convertible(int)
    -> decltype(void(declval<void (&)(To)>()(declval<From>())),
                true_type{});
template <class, class>
auto test_implicitly_convertible(...) -> false_type;
} // namespace detail
template <class From, class To>
struct is_convertible
    : bool_constant<
          (decltype(detail::test_returnable<To>(0))::value &&
           decltype(detail::test_implicitly_convertible<From, To>(0))::value) ||
          (is_void_v<From> && is_void_v<To>)> {};

template <class From, class To>
inline constexpr bool is_convertible_v =
    is_convertible<From, To>::value;

// [concept.convertible]
template <class _From, class _To>
concept convertible_to = is_convertible_v<_From, _To> && requires { static_cast<_To>(declval<_From>()); };

