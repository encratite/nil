#ifndef NIL_MYSQL_HPP
#define NIL_MYSQL_HPP

#include <string>
#include <vector>

#include <nil/string.hpp>
#include <nil/windows.hpp>
#ifdef NIL_WINDOWS
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif

#include <nil/exception.hpp>
#include <nil/types.hpp>

namespace nil
{
	typedef std::vector<std::string> mysql_row;

	class mysql_result
	{
	public:
		mysql_result();
		mysql_result(MYSQL * connection);
		~mysql_result();

		std::size_t get_row_count();

		ullong get_ullong();
		std::string get_string();
		bool get_boolean();

		template <class number_type>
			number_type get_number()
		{
			index_check();
			std::string const & data = rows[row_index][column_index];
			number_type output = nil::string::string_to_number<number_type>(data);
			increase_column_index();
			return output;
		}

	private:
		std::vector<mysql_row> rows;
		std::size_t row_index;
		std::size_t column_index;
		unsigned column_count;

		void increase_column_index();
		void index_check();

	};

	class mysql
	{
	public:
		mysql();
		mysql(std::string const & server, std::string const & user, std::string const & password, std::string const & database);
		~mysql();
		bool connect(std::string const & server, std::string const & user, std::string const & password, std::string const & database);
		void disconnect();
		mysql_result query(std::string const & query_string);
		std::string get_error();
		ullong last_id();

	private:
		bool connected;
		MYSQL * connection;
	};
}

#endif
