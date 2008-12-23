#ifndef NIL_CRC32_HPP
#define NIL_CRC32_HPP

#include <cstddef>
#include <nil/types.hpp>

namespace nil
{
	class crc32
	{
	public:
		crc32();
		crc32(char const * data, std::size_t size);
		ulong hash(char const * data, std::size_t size);
		ulong checksum() const;

	private:
		ulong sum;
	};
}

#endif
