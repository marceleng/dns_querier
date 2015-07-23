#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <mysql++/mysql++.h>

struct Measurement;
typedef struct Measurement Measurement;
typedef unsigned int uint32_t;

class DNS_Logger {

public:
	DNS_Logger(std::string username, std::string password, std::string db_name, 
				std::string db_address, uint32_t port, uint32_t prefix_size);
				
	void log_queries (Measurement measurements[], int nb_measurements);

private:
	std::string m_username;
	std::string m_password;
	std::string m_dbName;
	std::string m_dbAddress;
	uint32_t m_port;
	uint32_t m_prefixSize;
	
	/*
	 * SQL HANDLERS
	 */
	uint32_t get_domain_id(std::string domain, mysqlpp::Query query);
	uint32_t insert_domain (std::string domain, uint32_t ts, mysqlpp::Query query);
	void insert_mes (uint32_t dn_id, uint32_t ts, uint32_t query_time, mysqlpp::Query query);
	void update_stats(uint32_t dn_id, uint32_t ts, mysqlpp::Query query);

};

#endif
