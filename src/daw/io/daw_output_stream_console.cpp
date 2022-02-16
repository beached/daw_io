// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_io
//

#include "daw/io/daw_output_stream_console.h"
#include "daw/io/daw_io_exception.h"

#include <cstddef>
#include <cstdio>

namespace daw::io {
	void console_output_stream::write( std::span<std::byte const> data ) {
		auto const sz = data.size( );
		if( auto ret = std::fwrite( data.data( ), 1, data.size( ), stdout ); ret != sz ) {
			auto err = ferror( stdout );
			throw c_file_io_exception{ err };
		}
	}

	void console_error_stream::write( std::span<std::byte const> data ) {
		auto const sz = data.size( );
		if( auto ret = std::fwrite( data.data( ), 1, data.size( ), stdout ); ret != sz ) {
			auto err = ferror( stderr );
			throw c_file_io_exception{ err };
		}
	}
} // namespace daw::io