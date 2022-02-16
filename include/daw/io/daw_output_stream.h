// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_io
//
#pragma once

#include "daw_output_stream_concept.h"

#include <cstddef>
#include <fmt/format.h>
#include <span>
#include <utility>

namespace daw::io {
	struct output_stream {
		output_stream( ) = default;
		virtual ~output_stream( ) = default;
		output_stream( output_stream const & ) noexcept = default;
		output_stream( output_stream && ) noexcept = default;
		output_stream &operator=( output_stream const & ) noexcept = default;
		output_stream &operator=( output_stream && ) noexcept = default;

		/// Write a range of data to the output stream.  The default implementation calls write(
		/// std::byte ) repeatedly
		virtual void write( std::span<std::byte const> data ) = 0;

		[[nodiscard]] virtual bool can_write( ) const noexcept;

		/// Close the output stream.  The default implementation is a no-op
		virtual void close( );

		/// Flush the output stream.  The default implementation is a no-op
		virtual void flush( );

		/// Write a range of data to the output stream.  The default implementation calls write(
		/// std::span<std::byte const> ) with a subset of data specified.
		/// @pre offset + length <= data.size( )
		void write( std::span<std::byte const> data, std::size_t offset, std::size_t length );

		template<typename... Ts>
		void print( fmt::format_string<Ts...> fmt, Ts &&...args ) {
			auto const buff = fmt::format( fmt, std::forward<Ts>( args )... );
			this->write( std::as_bytes( std::span( buff ) ) );
		}

		template<typename... Ts>
		void vprint( std::string_view fmt, Ts &&...args ) {
			auto const buff = fmt::vformat( fmt, std::forward<Ts>( args )... );
			this->write( std::as_bytes( std::span( buff ) ) );
		}
	};
	static_assert( OutputStream<output_stream> );
} // namespace daw::io
