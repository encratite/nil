#include <nil/mysql.hpp>

#include <nil/debug.hpp>
#include <nil/string.hpp>

//debugging
#include <iostream>

namespace nil
{
	mysql_result::mysql_result()
	{
	}

	mysql_result::mysql_result(MYSQL * connection)
	{
		rows.clear();

		MYSQL_RES * result = mysql_use_result(connection);
		//apparently it returns 0 for other than select?
		if(result == 0)
			return;
		MYSQL_ROW row;
		column_count = mysql_num_fields(result);

		while(row = mysql_fetch_row(result))
		{
			mysql_row new_row;
			for(unsigned i = 0; i < column_count; i++)
				new_row.push_back(row[i]);
			rows.push_back(new_row);
		}

		row_index = 0;
		column_index = 0;

		mysql_free_result(result);
	}

	mysql_result::~mysql_result()
	{
	}

	std::size_t mysql_result::get_row_count()
	{
		return rows.size();
	}

	std::string mysql_result::get_string()
	{
		index_check();
		std::string output = rows[row_index][column_index];
		increase_column_index();
		return output;
	}

	bool mysql_result::get_boolean()
	{
		index_check();
		std::string const & string = rows[row_index][column_index];
		//hack
		//std::cout << "Boolean: \"" << string << "\"" << std::endl;
		//bool output = (string == "1" || string == "true");
		bool output = (string == "1");
		increase_column_index();
		return output;
	}

	void mysql_result::increase_column_index()
	{
		column_index++;
		if(column_index == column_count)
		{
			column_index = 0;
			row_index++;
		}
	}

	void mysql_result::index_check()
	{
		if(row_index >= rows.size())
			throw nil::exception("No more data to read from the MySQL query result");
	}

	mysql::mysql():
		connected(false)
	{
	}

	mysql::mysql(std::string const & server, std::string const & user, std::string const & password, std::string const & database):
		connected(false)
	{
		if(!connect(server, user, password, database))
			throw nil::exception("Failed to establish connection to MySQL server");
	}

	mysql::~mysql()
	{
		disconnect();
	}

	bool mysql::connect(std::string const & server, std::string const & user, std::string const & password, std::string const & database)
	{
		disconnect();
		connection = mysql_init(0);
		if(!mysql_real_connect(connection, server.c_str(), user.c_str(), password.c_str(), database.c_str(), 0, 0, 0))
		{
			//std::cout << mysql_error(connection) << std::endl;
			return false;
		}
		return true;
	}

	void mysql::disconnect()
	{
		if(connected)
		{
			mysql_close(connection);
			connected = false;
		}
	}

	mysql_result mysql::query(std::string const & query_string)
	{
		if(mysql_query(connection, query_string.c_str()))
			throw nil::exception("MySQL query failed");
		
		return mysql_result(connection);
	}

	std::string mysql::get_error()
	{
		return mysql_error(connection);
	}

	ullong mysql::last_id()
	{
		return mysql_insert_id(connection);
	}
}
