#include <nil/exception.hpp>

namespace nil
{
	exception::exception()
	{
	}

	exception::exception(std::string const & message):
		message(message)
	{
	}

	std::string const & exception::get_message() const
	{
		return message;
	}
}
