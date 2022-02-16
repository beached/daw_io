// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_io
//

#include "daw/io/daw_output_stream.h"

#include <cassert>

namespace daw::io {
	void
	output_stream::write( std::span<std::byte const> data, std::size_t offset, std::size_t length ) {
		assert( offset + length <= data.size( ) );
		this->write(
		  std::span<std::byte const>( data.data( ) + offset, data.data( ) + offset + length ) );
	}

	void output_stream::close( ) {}
	void output_stream::flush( ) {}

	bool output_stream::can_write( ) const noexcept {
		return true;
	}
} // namespace daw::io
