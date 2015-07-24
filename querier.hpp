#ifndef QUERIER_HPP
#define QUERIER_HPP
#include <string>
#include <sys/time.h>
#include <ldns/ldns.h>

#include "logger.hpp"

typedef unsigned int uint32_t;

struct Measurement {
	std::string domain_name;
	struct timeval timestamp;
	uint32_t querytime;
};

typedef struct Measurement Measurement;

class DNS_Querier {

public:
	
	DNS_Querier (uint32_t nb_domains, std::vector<std::string> domains,
				 uint32_t period, uint32_t random_prefix_size, 
				 DNS_Logger &logger);

	std::string generate_random_dn (std::string dn);
	void run();
	void query_all();

private:
	Measurement query_domain (ldns_resolver *res, std::string dn);

	std::vector<std::string> m_domains;
	uint32_t m_nbDomains; 
	uint32_t m_period;
	uint32_t m_prefSize;
	DNS_Logger m_logger;
};


#endif
