#include "logger.hpp"
#include "querier.hpp"

DNS_Logger::DNS_Logger(std::string username, std::string password, std::string db_name, 
				std::string db_address, uint32_t port, uint32_t prefix_size) : m_username(username),
											m_password(password),m_dbName(db_name),
											m_dbAddress(db_address),m_port(port),
											m_prefixSize(prefix_size) {}
										  
void DNS_Logger::log_queries(Measurement measurements[], int nb_measurements) {
	mysqlpp::Connection conn(m_dbName.c_str(), m_dbAddress.c_str(), m_username.c_str(),
							 m_password.c_str(), m_port);
	mysqlpp::Query query = conn.query();

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
			dn_id=this->insert_domain(orig_dn, ts, query);
		}
				
		//Second we insert the data with the right domain id
		this->insert_mes(dn_id,ts,mes.querytime,query);
		//Lastly we update the statistics and the last timestamp
		//TODO: Update stats
	}
}

uint32_t DNS_Logger::get_domain_id (std::string domain, mysqlpp::Query query) {
	query.reset();
	query << "select id from domains where name like %0q";
	query.parse();

	mysqlpp::StoreQueryResult res = query.store(domain);
	if (res.empty() > 0)
		return res[0][0];
	else
		return 0;
}

//Returns the id of the inserted domain
uint32_t DNS_Logger::insert_domain (std::string domain, uint32_t ts, mysqlpp::Query query) {
	query.reset();
	query << "insert into domains (name,first_ts) values (%0,%1)";
	query.parse();
	mysqlpp::SimpleResult simpRes = query.execute(domain,ts);
	return query.insert_id();
}
											
void DNS_Logger::insert_mes (uint32_t dn_id, uint32_t ts, uint32_t query_time, mysqlpp::Query query) {
	query.reset();
	query << "insert into time_series (domain_id,timestamp,query_time)  VALUES (%0,%1,%2)";
	query.parse();
	mysqlpp::SimpleResult simpRes = query.execute(dn_id,ts,query_time);
}
