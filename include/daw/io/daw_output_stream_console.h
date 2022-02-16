// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_io
//

#pragma once

#include "daw_output_stream.h"

#include <cstdio>
#include <memory>
#include <string>

namespace daw::io {
	struct console_output_stream final : output_stream {
		console_output_stream( ) = default;
		~console_output_stream( ) override = default;
		console_output_stream( console_output_stream const & ) = delete;
		console_output_stream( console_output_stream && ) = delete;
		console_output_stream &operator=( console_output_stream const & ) = delete;
		console_output_stream &operator=( console_output_stream && ) = delete;

		void write( std::byte byte ) override;
		void write( std::span<std::byte const> data ) override;

		[[nodiscard]] inline bool can_write( ) const noexcept override {
			return true;
		}
	};

	static constinit console_output_stream con_out{ };

	struct console_error_stream final : output_stream {
		console_error_stream( ) = default;
		~console_error_stream( ) override = default;
		console_error_stream( console_error_stream const & ) = delete;
		console_error_stream( console_error_stream && ) = delete;
		console_error_stream &operator=( console_error_stream const & ) = delete;
		console_error_stream &operator=( console_error_stream && ) = delete;

		void write( std::byte byte ) override;
		void write( std::span<std::byte const> data ) override;

		[[nodiscard]] inline bool can_write( ) const noexcept override {
			return true;
		}
	};

	static constinit console_output_stream con_err{ };
} // namespace daw::io
