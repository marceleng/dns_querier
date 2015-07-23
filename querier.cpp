#include <stdio.h> //printf()
#include <stdlib.h> //rand()
#include <cstdlib>

#include "querier.hpp"
#include "conf.hpp"

#define MINUTE_IN_SEC 60


DNS_Querier::DNS_Querier (uint32_t nb_domains, std::vector<std::string> domains, uint32_t period,
							uint32_t random_prefix_size, DNS_Logger &logger) : 
							m_nbDomains(nb_domains),m_period(period),
							m_prefSize(random_prefix_size), m_logger(logger)
{
	//TODO: add to preprocessing
	this->m_domains = new std::string[nb_domains];
	for (uint32_t i=0; i<nb_domains; i++) {
		this->m_domains[i] = domains[i];
	}
}

DNS_Querier::~DNS_Querier() 
{
	delete[] this->m_domains;	
}

std::string DNS_Querier::generate_random_dn (std::string dn) {
	
	std::string output = std::string(this->m_prefSize,'a');
	
	for(uint32_t i=0; i<this->m_prefSize; i++) {
		char rand_char = (char) (rand() % 36);
		output[i] = (rand_char > 25) ? '0' + rand_char-26 :
						'a'+rand_char;
	}
	output += "."+dn;
	return output;
}

//Returns query time
Measurement DNS_Querier::query_domain (ldns_resolver *res, std::string dn) {
	ldns_rdf *domain;
	ldns_pkt *p;
	
	domain = ldns_dname_new_frm_str(dn.c_str());
	
	p = ldns_resolver_query(res, domain, LDNS_RR_TYPE_A,
							LDNS_RR_CLASS_IN, LDNS_RD);
							
	Measurement result = { dn, ldns_pkt_timestamp(p), ldns_pkt_querytime(p) };
	return result;
}

void DNS_Querier::query_all() {
	
	Measurement results[this->m_nbDomains];
	ldns_resolver *res;

	ldns_resolver_new_frm_file(&res, NULL);
	
	for(uint32_t i=0; i<this->m_nbDomains; i++) {
		std::string dn = generate_random_dn(this->m_domains[i]);
		results[i] = query_domain(res, dn);
	}

	(this->m_logger).log_queries(results,this->m_nbDomains);
}

void DNS_Querier::run() {
	//Used to compensate for the DNS and MySQL overhead
	timeval *start_time = (timeval*) malloc(sizeof(timeval));
	timeval *end_time = (timeval*) malloc(sizeof(timeval));
	
	while(1) {
		gettimeofday(start_time,NULL);
	
		this->query_all();

		gettimeofday(end_time,NULL);
		//We substract the time spent in query_all() to make it really periodic
		sleep(this->m_period*MINUTE_IN_SEC - end_time->tv_sec + start_time->tv_sec);
	}
}

int main(int argc, char* argv[]) {

	uint32_t period;

	if ((argc < 2) || !(period=atoi(argv[1]))) {
		printf("Usage: ./dns_querier <i>\n\ti is the querying period in minutes\n");
		return -1;
	}

	setbuf(stdout, NULL);
	
	ConfParser cp;
	cp.parse_file("conf");
	DNS_Logger logger(cp.get_mysql_user(),cp.get_mysql_pass(),cp.get_mysql_db(),
						cp.get_mysql_addr(),cp.get_mysql_port(),
						cp.get_random_prefix_size()+1);
	
	DNS_Querier dq((cp.get_domains()).size(), cp.get_domains(), period,
					cp.get_random_prefix_size(), logger);
	dq.run();
 }
