#pragma once

#include <type_traits>

namespace marlib {

  // struct cf1_matrix_tag{};

  //   template <class T>
  //   struct get_category;
  // 
  //   template <>
  //   struct get_category<> {
  //   using type = constant_value_tag;
  // };
  // 
  // /**
  // * @brief A meta function to get a type as the tag.
  // * The return value is given by 'type' type.
  // * This is an implementation for double.
  // */
  // template <>
  // struct get_category<double> {
  //   using type = constant_value_tag;
  // };
  // 

  template <class T>
  struct double_vector {
    static int size(const T& v) { return v.size(); }
    static const double* ptr(const T& v) { return &v[0]; }
    static double* ptr(T& v) { return &v[0]; }
  };

  // /**
  //  * @brief A meta function of if-then-else. If Cond is true, the return
  //  * value (type) becomes a type given by Then.
  //  */
  // template<bool Cond, typename Then, typename Else>
  // struct if_ {
  //   using type = Then;
  // };
  // 
  // /**
  // * @brief A meta function of if-then-else. If Cond is false, the return
  // * value (type) becomes a type given by Else.
  // */
  // template<typename Then, typename Else>
  // struct if_<false, Then, Else> {
  //   using type = Else;
  // };
  // 
}