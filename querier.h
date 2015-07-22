#ifndef QUERIER_H
#define QUERIER_H
#include <string>
#include <sys/time.h>

struct Measurement {
	struct timeval timestamp;
	 uint32_t querytime;
};

typedef struct Measurement Measurement;

class DNS_Querier {

public:
	DNS_Querier(int nb_domains, std::string domain_names[]);
	std::string generate_random_dn (std::string dn);
	Measurement query_domain (ldns_resolver *res, std::string dn);
	void query_all();

private:
	std::string *m_domains;
	int m_nb_domains; 
};


#endif
