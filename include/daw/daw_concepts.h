// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_json_link
//

#pragma once

#include <daw/daw_arith_traits.h>
#include <daw/daw_move.h>

#include <ciso646>
#include <concepts>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace daw {
	template<typename From, typename To>
	concept convertible_to = std::is_convertible_v<From, To> and requires( From f ) {
		static_cast<To>( f );
	};

	template<typename Lhs, typename Rhs>
	concept same_as = std::is_same_v<Lhs, Rhs>;

	template<typename>
	inline constexpr bool is_character_type_v = false;

	template<>
	inline constexpr bool is_character_type_v<char> = true;

	template<>
	inline constexpr bool is_character_type_v<char8_t> = true;

	template<typename CharT>
	concept Character = is_character_type_v<std::remove_cvref_t<CharT>>;

	template<typename T>
	using root_type_t = std::remove_cv_t<std::remove_pointer_t<std::decay_t<T>>>;

	template<typename CharPtr>
	concept CharacterPointer = Character<root_type_t<CharPtr>> and
	                           ( (std::is_pointer_v<std::remove_reference_t<CharPtr>> and
	                              not std::is_function_v<std::remove_reference_t<CharPtr>>) or
	                             std::is_array_v<std::remove_reference_t<CharPtr>> );

	static_assert( CharacterPointer<char const ( & )[5]> );
	/// A customizable type trait that allows one to convert to a string_view.  It is a contiguous
	/// range of character types.  It allows for std::data/std::size to query extent of the range
	template<typename T>
	inline constexpr bool is_string_view_like_v = requires( T &&v ) {
		{ std::data( v ) } -> CharacterPointer;
		{ std::size( v ) } -> convertible_to<std::size_t>;
	};

	/// A concept that allows one to convert to a string_view.  It is a contiguous range of character
	/// types.  It allows for std::data/std::size to query extent of the range
	template<typename T>
	concept StringViewable = is_string_view_like_v<T>;

	template<typename T, typename... Args>
	concept constructible_from = std::is_constructible_v<T, Args...>;

	template<typename T>
	concept Containers = requires( T container ) {
		typename std::remove_cvref_t<T>::value_type;
		container.begin( );
		container.end( );
	};

	template<typename T>
	concept StringLike = Containers<std::remove_cvref_t<T>>
	  and same_as<typename std::remove_cvref_t<T>::value_type, char>;

	template<typename T>
	concept EnumType = std::is_enum_v<std::remove_cvref_t<std::remove_cvref_t<T>>>;

	template<typename T>
	concept UnsignedStd = std::is_unsigned_v<std::remove_cvref_t<T>>;

	template<typename T>
	concept SignedStd = std::is_signed_v<std::remove_cvref_t<T>>;

	template<typename T>
	concept IntegralStd = std::is_integral_v<std::remove_cvref_t<T>>;

	template<typename T>
	concept IntegralDaw = daw::is_integral_v<std::remove_cvref_t<T>>;

	template<typename T>
	concept FloatingPointStd = std::is_floating_point_v<std::remove_cvref_t<T>>;

	template<typename T>
	concept FloatingPointDaw = daw::is_floating_point_v<std::remove_cvref_t<T>>;

	template<typename Container>
	inline constexpr bool has_kv_mapping_v = requires {
		typename std::remove_cvref_t<Container>::key_type;
		typename std::remove_cvref_t<Container>::mapped_type;
	};

	template<typename T>
	concept AssociativeContainers =
	  Containers<std::remove_cvref_t<T>> and has_kv_mapping_v<std::remove_cvref_t<T>>;

	template<typename Func, typename... Args>
	concept Invocable = requires( Func &&f, Args &&...args ) {
		std::invoke( DAW_FWD2( Func, f ), DAW_FWD2( Args, args )... );
	};

	template<typename LHS, typename RHS>
	concept assignable_from =
#if defined( __cpp_lib_concepts )
	  std::assignable_from<LHS, RHS>;
#else
	  std::is_lvalue_reference_v<LHS> and
	  /* To much to do common_reference
	  std::common_reference_with<std::remove_reference_t<LHS> const &,
	                             std::remove_reference_t<RHS> &> const &&*/
	  requires( LHS lhs, RHS &&rhs ) {
		{ lhs = std::forward<RHS>( rhs ) } -> std::same_as<LHS>;
	};
#endif

	template<typename T>
	concept move_constructible =
#if defined( __cpp_lib_concepts )
	  std::move_constructible<T>;
#else
	  constructible_from<T, T> and convertible_to<T, T>;
#endif

#if not defined( __cpp_lib_concepts )
	namespace swappable_test {
		using namespace std;
		template<typename T>
		inline constexpr bool swappable_test = requires( T &a, T &b ) {
			swap( a, b );
		};
	} // namespace swappable_test
#endif

	template<typename T>
	concept swappable =
#if defined( __cpp_lib_concepts )
	  std::swappable<T>;
#else
	  swappable_test::swappable_test<T>;
#endif

	template<typename T>
	concept movable =
#if defined( __cpp_lib_concepts )
	  std::movable<T>;
#else
	  std::is_object_v<T> and move_constructible<T> and assignable_from<T &, T> and swappable<T>;
#endif

	template<typename T>
	using iter_difference_t =
#if defined( __cpp_lib_concepts )
	  std::iter_difference_t<T>;
#else
	  typename std::iterator_traits<T>::difference_type;
#endif

	template<typename T>
	using iter_reference_t =
#if defined( __cpp_lib_concepts )
	  std::iter_reference_t<T>;
#else
	  typename std::iterator_traits<T>::reference_type;
#endif

	template<typename I>
	concept weakly_incrementable =
#if defined( __cpp_lib_concepts )
	  std::weakly_incrementable<I>;
#else
	  movable<I> and requires( I i ) {
		typename iter_difference_t<I>;
		requires SignedStd<iter_difference_t<I>>;
		{ ++i } -> same_as<I &>;
		i++;
	};
#endif

	template<typename Result, typename Func, typename... Args>
	concept InvocableResult = requires( Func &&f, Args &&...args ) {
		{ std::invoke( DAW_FWD2( Func, f ), DAW_FWD2( Args, args )... ) } -> convertible_to<Result>;
	};

	template<typename I>
	concept input_or_output_iterator =
#if defined( __cpp_lib_concepts )
	  std::input_or_output_iterator<I>;
#else
	  requires( I i ) {
		{ *i };
	}
	and weakly_incrementable<I>;
#endif

	template<typename Out, typename T>
	concept indirectly_writable =
#if defined( __cpp_lib_concepts )
	  std::indirectly_writable<Out, T>;
#else
	  requires( Out &&o, T &&t ) {
		*o = std::forward<T>( t );
		*std::forward<Out>( o ) = std::forward<T>( t );
		const_cast<const iter_reference_t<Out> &&>( *o ) = std::forward<T>( t );
		const_cast<const iter_reference_t<Out> &&>( *std::forward<Out>( o ) ) = std::forward<T>( t );
	};
#endif

	template<typename I, typename T>
	concept output_iterator = input_or_output_iterator<I> and indirectly_writable<I, T> and
	requires( I i, T &&t ) {
		*i++ = DAW_FWD2( T, t ); // not required to be equality-preserving
	};

	template<typename Container>
	concept BackInsertableContainer = Containers<Container> and
	requires( Container c, typename Container::value_type const &v ) {
		c.push_back( v );
	};

} // namespace daw
