#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "conf.hpp"

void ConfParser::parse_file(std::string filename)
{
	std::ifstream infile (filename.c_str());
	
	bool in_domains = false;
	
	std::string line;
	std::string key;
	std::string value;
	
	while( std::getline(infile, line) ) {
		if (line.size() != 0) {
			//Remove comments if necessary
			int eol = line.find("#");
			if (eol != -1) {
				line = line.substr(eol,line.size()-eol);
			}
		
			std::istringstream is_line(line);
			
			//Handle the DOMAINS array
			if (in_domains) {
				this->m_domains.push_back(this->handle_array_line(line));
				if (line.find("]") != std::string::npos) {
					in_domains = false;
				}
			}
			//Else look for the key=value structure
			else if( std::getline(is_line, key, '=') ) {
				if( std::getline(is_line, value) ) {
					if (key.compare("DOMAINS")==0) {
						in_domains = true;
						this->m_domains.push_back(this->handle_array_line(value));
					}
					else {
						store_line(key, value);
					}
				}
			}
			else {
				std::cerr << "Non-valid line in conf file: "<<line<<std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
}

std::string ConfParser::handle_array_line(std::string line)
{
	int offset = line.find_first_not_of(" \t\f\v\n\r[");
	std::string result = line.substr(offset,line.size()-offset);
	result = result.substr(0,result.find_last_of(",]"));
	return result;
}

void ConfParser::store_line(std::string key, std::string value)
{
	std::istringstream temp(value);
	if (key.compare("RANDOM_PREFIX_SIZE")==0) {
		temp >> (this->m_randomPrefixSize);
	}
	else if (key.compare("MYSQL_USER")==0) {
		this->m_mysqlUser = value;
	}
	else if (key.compare("MYSQL_PASSWORD")==0) {
		this->m_mysqlPass = value;
	}
	else if (key.compare("MYSQL_DB")==0) {
		this->m_mysqlDB = value;
	}
	else if (key.compare("MYSQL_ADDRESS")==0) {
		this->m_mysqlAddr = value;
	}
	else if (key.compare("MYSQL_PORT")==0) {
		temp >> this->m_mysqlPort;
	}
	else if (key.compare("DOMAINS")==0) {
		
	}
	else {
		std::cerr << "Unknown field in conf file: " << key << std::endl;
		exit(EXIT_FAILURE);
	}
}


/*
 * GETTERS BLOCK
 */
const std::vector<std::string>& ConfParser::get_domains() const
{
	return m_domains;
}

const std::string& ConfParser::get_mysql_addr() const
{
	return m_mysqlAddr;
}

const std::string& ConfParser::get_mysql_db() const 
{
	return m_mysqlDB;
}

const std::string& ConfParser::get_mysql_pass() const 
{
	return m_mysqlPass;
}

uint32_t ConfParser::get_mysql_port() const 
{
	return m_mysqlPort;
}

const std::string& ConfParser::get_mysql_user() const 
{
	return m_mysqlUser;
}

uint32_t ConfParser::get_random_prefix_size() const 
{
	return m_randomPrefixSize;
}
