#include <stdio.h>
#include "querier.hpp"

DNS_Logger::DNS_Logger(std::string username, std::string password, std::string db_name, 
				std::string db_address, uint32_t port, uint32_t prefix_size) : m_username(username),
											m_password(password),m_dbName(db_name),
											m_dbAddress(db_address),m_port(port),
											m_prefixSize(prefix_size) 
{
	mysqlpp::Connection conn(m_dbName.c_str(), m_dbAddress.c_str(), m_username.c_str(),
							 m_password.c_str(), m_port);
	mysqlpp::Query query = conn.query("CREATE TABLE IF NOT EXISTS domains ( "
		"id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
		"name VARCHAR(100) NOT NULL,"
		"nb_queries INT UNSIGNED NOT NULL,"
		"first_ts TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
		"last_ts TIMESTAMP,"
		"average DOUBLE,"
		"std_dev DOUBLE )");
	query.execute();
	
	query.reset();
	
	query << "CREATE TABLE IF NOT EXISTS time_series ("
		"id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
		"domain_id INT UNSIGNED NOT NULL,"
		"timestamp TIMESTAMP NOT NULL,"
		"query_time INT NOT NULL )";
	query.execute();
}
										  
void DNS_Logger::log_queries(Measurement measurements[], int nb_measurements) {
	mysqlpp::Connection conn(m_dbName.c_str(), m_dbAddress.c_str(), m_username.c_str(),
							 m_password.c_str(), m_port);
	mysqlpp::Query query = conn.query();

	printf("\n################ NEW ITERATION ################\n");

	for(int i=0; i<nb_measurements; i++) {

		Measurement mes = measurements[i];
		//Extract the data
		std::string full_dn = mes.domain_name;
		std::string orig_dn = full_dn.substr(this->m_prefixSize,
											full_dn.size()-this->m_prefixSize);
		uint32_t ts = (mes.timestamp).tv_sec;
		
		uint32_t dn_id = 0;
		
		//First we search for/insert the corresponding domain
		dn_id = this->get_domain_id(orig_dn, query);

		if (!dn_id) {
			printf("Inserting new domain: %s\n",orig_dn.c_str());
			dn_id=this->insert_domain(orig_dn, ts, query);
		}
				
		//Second we insert the data with the right domain id
		this->insert_mes(dn_id,ts,mes.querytime,query);
		//Lastly we update the statistics and the last timestamp
		this->update_stats(dn_id,ts,query);
	}
}

uint32_t DNS_Logger::get_domain_id (std::string domain, mysqlpp::Query query) {
	query.reset();
	query << "select id from domains where name like %0q";
	query.parse();

	mysqlpp::StoreQueryResult res = query.store(domain);
	if (!res.empty())
		return res[0][0];
	else
		return 0;
}

//Returns the id of the inserted domain
uint32_t DNS_Logger::insert_domain (std::string domain, uint32_t ts, mysqlpp::Query query) {
	query.reset();
	query << "insert into domains (name,first_ts) values (%0q,FROM_UNIXTIME(%1))";
	query.parse();
	mysqlpp::SimpleResult simpRes = query.execute(domain,ts);
	return query.insert_id();
}
											
void DNS_Logger::insert_mes (uint32_t dn_id, uint32_t ts, uint32_t query_time, mysqlpp::Query query) {
	query.reset();
	query << "insert into time_series (domain_id,timestamp,query_time)  VALUES (%0,FROM_UNIXTIME(%1),%2)";
	query.parse();
	mysqlpp::SimpleResult simpRes = query.execute(dn_id,ts,query_time);
}

void DNS_Logger::update_stats(uint32_t dn_id, uint32_t ts, mysqlpp::Query query) {
	query.reset();
	query << "select count(*),avg(query_time),std(query_time) from time_series where domain_id=%0";
	query.parse();
	mysqlpp::StoreQueryResult res = query.store(dn_id);
	uint32_t count = res[0][0];
	double avg = res[0][1];
	double std = res[0][2];
	
	query.reset();
	query << "update domains set nb_queries=%0, last_ts=FROM_UNIXTIME(%1), average=%2, std_dev=%3 where id=%4";
	query.parse();
	
	mysqlpp::SimpleResult simRes = query.execute(count,ts,avg,std,dn_id);
}
