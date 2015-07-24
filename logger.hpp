#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <mysql++/mysql++.h>

struct Measurement;
typedef struct Measurement Measurement;
typedef unsigned int uint32_t;

// Handle the MySQL storage
class DNS_Logger {

public:
	// Constructor
	// Creates the MySQL schema if necessary
	DNS_Logger(std::string username, std::string password, std::string db_name, 
				std::string db_address, uint32_t port, uint32_t prefix_size,
				bool verbose);

	//Stores all the results in measurements in the MySQL database
	void log_queries (Measurement measurements[], int nb_measurements);

private:
	std::string m_username;
	std::string m_password;
	std::string m_dbName;
	std::string m_dbAddress;
	uint32_t m_port;
	uint32_t m_prefixSize;
	bool m_verbose;
	
	/*
	 * SQL HANDLERS
	 */
	// Returns the id of domain in the 'domains' table if it exists
	// Returns 0 otherwise
	uint32_t get_domain_id(std::string domain, mysqlpp::Query query);
	// Insert a domain in the 'domains' table with ts as first timestamp
	uint32_t insert_domain (std::string domain, uint32_t ts, mysqlpp::Query query);
	// Insert a measurement in the 'time_series' table
	void insert_mes (uint32_t dn_id, uint32_t ts, uint32_t query_time, mysqlpp::Query query);
	// For a given domain id dn_id, updates the last timestamp to ts
	// updates the statistics (count, average and standard dev) from the 'time_series' table
	void update_stats(uint32_t dn_id, std::string domain_name, 
						uint32_t ts, mysqlpp::Query query);
	
	/*
	 * String verifiers
	 */
	//Returns true if c is alphanumeric
	bool is_alphanum (char c);
	// Checks whether dn_name is valid for a database name
	// ie db_name \in [a-zA-Z0-9$_]*
	bool is_valid_name (std::string db_name);
};

#endif
