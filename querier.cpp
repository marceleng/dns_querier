#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "querier.hpp"
#include "conf.hpp"

#define MINUTE_IN_SEC 60


DNS_Querier::DNS_Querier (int nb_domains, std::string domains[], uint32_t period) : m_nb_domains(nb_domains),m_period(period),m_logger(MYSQL_USER,MYSQL_PASSWD,MYSQL_DB,MYSQL_ADDRESS,MYSQL_PORT,RANDOM_PREFIX_SIZE+1) {
	this->m_domains = new std::string[nb_domains];
	for (int i=0; i<nb_domains; i++) {
		this->m_domains[i] = domains[i];
	}
}

std::string DNS_Querier::generate_random_dn (std::string dn) {
	
	std::string output = std::string(RANDOM_PREFIX_SIZE,'a');
	
	for(int i=0; i<RANDOM_PREFIX_SIZE; i++) {
		char rand_char = (char) (rand() % 36);
		output[i] = (rand_char > 25) ? '0' + rand_char-26 :
						'a'+rand_char;
	}
	output += dn;
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
	Measurement results[this->m_nb_domains];
	ldns_resolver *res;
	ldns_status s;
	
	char *buffer;
	
	s = ldns_resolver_new_frm_file(&res, NULL);
	
	for(int i=0; i<this->m_nb_domains; i++) {
		std::string dn = generate_random_dn(DOMAINS[i]);
		results[i] = query_domain(res, dn);
	}
	
	for(int i=0; i<this->m_nb_domains; i++) {
		printf("Measurement: %ld,%d\n",(results[i].timestamp).tv_sec,results[i].querytime);
	}
}

void DNS_Querier::run() {
	while(1) {
		this->query_all();
		sleep(this->m_period*MINUTE_IN_SEC);
	}
}

int main() {
	uint32_t period=1;
	DNS_Querier dq(NB_DOMAINS,DOMAINS,period);
	dq.run();
 }
