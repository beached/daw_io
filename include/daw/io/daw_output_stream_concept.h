// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_io
//
#pragma once

#include "daw/daw_concepts.h"

#include <cstddef>
#include <span>

namespace daw::io {

	template<typename T>
	concept Closable = requires( T &stream ) {
		stream.close( );
	};

	template<typename T>
	concept Flushable = requires( T &stream ) {
		stream.flush( );
	};

	template<typename T>
	concept RangeWriteable = requires( T &stream ) {
		stream.write( std::span<std::byte const>{ } );
	};

	template<typename T>
	concept RangeOffsetWriteable = requires( T &stream ) {
		stream.write( std::span<std::byte const>{ }, std::size_t{ 0 }, std::size_t{ 0 } );
	};

	template<typename T>
	concept MinimalOutputStream = requires( T &stream ) {
		stream.write( std::byte{ 0 } );
	};

	template<typename T>
	concept OutputStream = MinimalOutputStream<T> and RangeWriteable<T>;
} // namespace daw::io
