#ifndef NIL_EXCEPTION_HPP
#define NIL_EXCEPTION_HPP

#include <stdexcept>

namespace nil
{
	class exception: std::exception
	{
	public:
		exception();
		exception(std::string const & message);

		std::string const & get_message() const;

	private:
		std::string message;
	};
}

#endif
