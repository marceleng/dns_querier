#ifndef CONF_HPP
#define CONF_HPP

typedef unsigned int uint32_t; 	

/*
 *	Class to parse a config file and hold the corresponding values
 */
class ConfParser {

public:
	//Main function, read and parse the conf file filename
	void parse_file(std::string filename);
	
	// GETTERS
	const std::vector<std::string>& get_domains() const;
	const std::string& get_mysql_addr() const;
	const std::string& get_mysql_db() const;
	const std::string& get_mysql_pass() const;
	uint32_t get_mysql_port() const;
	const std::string& get_mysql_user() const;
	uint32_t get_random_prefix_size() const;

private:
	// Stores a one_line configuration line in the right holder
	void store_line(std::string key, std::string value);
	// Handle a line from an array in the conf file.
	std::string handle_array_line(std::string line);
	
	uint32_t m_randomPrefixSize;

	//Domain names infos
	std::vector <std::string> m_domains;

	//MySQL infos
	std::string m_mysqlUser;
	std::string m_mysqlPass;
	std::string m_mysqlDB;
	std::string m_mysqlAddr;
	uint32_t m_mysqlPort;
};

#endif
