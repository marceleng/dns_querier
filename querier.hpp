#ifndef QUERIER_HPP
#define QUERIER_HPP
#include <string>
#include <sys/time.h>
#include <ldns/ldns.h>

#include "logger.hpp"

typedef unsigned int uint32_t;

//Holds the result of a query
struct Measurement {
	std::string url; //The queried url
	struct timeval timestamp; //The timestamp is seconds
	uint32_t querytime; //Time taken by the query in ms
};

typedef struct Measurement Measurement;

// Class to periodically launch DNS queries
class DNS_Querier {

public:
	//Constructor
	DNS_Querier (uint32_t nb_domains, std::vector<std::string> domains,
				 uint32_t period, uint32_t random_prefix_size, 
				 DNS_Logger &logger);


	// From a domain name foo.bar, generates a random URL prefix.foo.bar
	// where the prefix is randomly generated and of size m_prefSize
	std::string generate_random_dn (std::string dn);
	//Handler that calls query_all() every m_period minutes
	void run();
	// Queries all the domains in m_nbDomains and logs the results through m_logger
	void query_all();

private:
	// Launch a DNS query for dn through resolver res
	// Returns the query time and timestamp
	Measurement query_domain (ldns_resolver *res, std::string dn);

	std::vector<std::string> m_domains;
	uint32_t m_nbDomains; 
	uint32_t m_period;
	uint32_t m_prefSize;
	DNS_Logger m_logger;
};


#endif
