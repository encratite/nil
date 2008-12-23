#include <nil/memory.hpp>

namespace nil
{
	bit_reader::bit_reader(std::string const & data):
		offset(0),
		data(data),
		big_endian(false)
	{
	}

	ulong bit_reader::read(std::size_t length)
	{
		ulong output;
		if(big_endian)
			output = nil::read_big_endian(data.c_str(), offset, length);
		else
			output = nil::read_little_endian(data.c_str(), offset, length);
		std::size_t new_offset = offset + length;
		set_offset(new_offset);
		return output;
	}

	bool bit_reader::read_bool()
	{
		return read(1) != 0;
	}

	std::string bit_reader::string()
	{
		std::string output;
		for(std::size_t i = 0; i < 16; i++)
		{
			char letter = static_cast<char>(read(7));
			if(letter == 0)
				return output;
			output.push_back(letter);
		}
		return output;
	}

	void bit_reader::set_offset(std::size_t new_offset)
	{
		std::size_t maximum = data.size() * 8;
		if(new_offset > maximum)
		{
			offset = maximum;
			throw nil::exception("Bit reader error, bit out of range");
		}
		else
			offset = new_offset;
	}

	void write_big_endian(ulong input, char * output, std::size_t size)
	{
		if(size)
		{
			for(char * i = output + size - 1; i >= output; --i)
			{
				*i = (char)input;
				input >>= 8;
			}
		}
	}

	void write_little_endian(ulong input, char * output, std::size_t size)
	{
		if(size)
		{
			for(char * i = output, * end = output + size; i < end; ++i)
			{
				*i = (char)input;
				input >>= 8;
			}
		}
	}

	ulong read_big_endian(char const * input, std::size_t size)
	{
		unsigned char const * unsigned_input = reinterpret_cast<unsigned char const *>(input);
		ulong output = 0;
		if(size)
		{
			for(unsigned char const * i = unsigned_input, * end = unsigned_input + size; i < end; ++i)
			{
				output <<= 8;
				output |= *i;
			}
		}
		return output;
	}

	ulong read_little_endian(char const * input, std::size_t size)
	{
		unsigned char const * unsigned_input = reinterpret_cast<unsigned char const *>(input);
		ulong output = 0;
		if(size)
		{
			for(unsigned char const * i = unsigned_input + size - 1; i >= unsigned_input; --i)
			{
				output <<= 8;
				output |= static_cast<ulong>(*i);
			}
		}
		return output;
	}

	ulong get_bit(char const * data, std::size_t offset)
	{
		std::size_t byte = offset / 8;
		std::size_t bit = offset % 8;
		return (data[byte] >> bit) & 1;
	}

	ulong read_little_endian(char const * data, std::size_t offset, std::size_t count)
	{
		ulong output = 0;
		for(std::size_t i = 0; i < count; i++)
			output |= get_bit(data, offset * 8 + i) << i;
		return output;
	}

	ulong read_big_endian(char const * data, std::size_t offset, std::size_t count)
	{
		ulong output = 0;
		for(std::size_t i = 0; i < count; i++)
		{
			output <<= 1;
			output |= get_bit(data, offset * 8 + i);
		}
		return output;
	}
}
