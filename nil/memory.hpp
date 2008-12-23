#ifndef NIL_MEMORY_HPP
#define NIL_MEMORY_HPP

#include <string>
#include <cstddef>
#include <nil/types.hpp>
#include <nil/exception.hpp>

namespace nil
{
	class bit_reader
	{
	public:
		bit_reader(std::string const & data);
		ulong read(std::size_t length);
		bool read_bool();
		void set_offset(std::size_t new_offset);
		std::string string();

		bool big_endian;

	private:
		std::string data;
		std::size_t offset;
	};

	void write_big_endian(ulong input, char * output, std::size_t size);
	void write_little_endian(ulong input, char * output, std::size_t size);

	ulong read_big_endian(char const * input, std::size_t size);
	ulong read_little_endian(char const * input, std::size_t size);

	ulong read_little_endian(char const * data, std::size_t offset, std::size_t count);
	ulong read_big_endian(char const * data, std::size_t offset, std::size_t count);

	ulong get_bit(char const * data, std::size_t offset);
}

#endif
