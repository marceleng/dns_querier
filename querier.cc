#include <iostream>
#include <ldns/ldns.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

#include "querier.h"
#include "conf.h"

DNS_Querier::DNS_Querier (int nb_domains, std::string domains[]) : m_nb_domains(nb_domains){
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
							
	Measurement result = { ldns_pkt_timestamp(p), ldns_pkt_querytime(p) };
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

int main() {
	DNS_Querier dq = DNS_Querier(NB_DOMAINS,DOMAINS);
	dq.query_all();
 }
