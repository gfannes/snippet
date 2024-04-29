#include <array>
#include <iostream>

namespace my {
    template<class T>
    struct is_array : std::false_type
    {
    };

    template<class T>
    struct is_array<T[]> : std::true_type
    {
    };

    template<class T, std::size_t N>
    struct is_array<T[N]> : std::true_type
    {
    };

    template<class T>
    struct is_pointer : std::false_type
    {
    };

    template<class T>
    struct is_pointer<T *> : std::true_type
    {
    };

    template<class T>
    struct is_pointer<T *const> : std::true_type
    {
    };

    template<class T>
    struct is_pointer<T *volatile> : std::true_type
    {
    };

    template<class T>
    struct is_pointer<T *const volatile> : std::true_type
    {
    };

    template<class T>
    struct is_reference : std::false_type
    {
    };
    template<class T>
    struct is_reference<T &> : std::true_type
    {
    };
    template<class T>
    struct is_reference<T &&> : std::true_type
    {
    };

    template<class T>
    struct is_member_pointer_helper : std::false_type
    {
    };

    template<class T, class U>
    struct is_member_pointer_helper<T U::*> : std::true_type
    {
    };

    template<class T>
    struct is_member_pointer : is_member_pointer_helper<typename std::remove_cv<T>::type>
    {
    };

    template<class T>
    struct is_function : std::integral_constant<
                             bool,
                             !std::is_const<const T>::value && !std::is_reference<T>::value>
    {
    };

    template<class T, typename = void>
    struct is_union_or_class_helper1 : public std::false_type
    {
    };
    template<class T>
    struct is_union_or_class_helper1<T, decltype((char T::*){}, void())> : public std::true_type
    {
    };

    template<class T>
    struct is_union_or_class : public is_union_or_class_helper1<T>
    {
    };

    template<class T>
    struct is_enum : std::integral_constant<bool, !std::is_void<T>::value && !std::is_integral<T>::value && !std::is_floating_point<T>::value && !is_array<T>::value && !is_pointer<T>::value && !is_reference<T>::value && !is_member_pointer<T>::value && !is_union_or_class<T>::value && !is_function<T>::value>
    {
    };
} // namespace my

enum Color
{
    A
};
struct Test
{
};

int main()
{
    std::cout << "is_enum<int[]>::value: " << my::is_enum<int[]>::value << std::endl;
    std::cout << "is_enum<std::array<int,2>>::value: " << my::is_enum<std::array<int, 2>>::value << std::endl;
    std::cout << "is_enum<int>::value: " << my::is_enum<int>::value << std::endl;
    std::cout << "is_enum<Color>::value: " << my::is_enum<Color>::value << std::endl;
    std::cout << "is_enum<Test>::value: " << my::is_enum<Test>::value << std::endl;
    return 0;
}
