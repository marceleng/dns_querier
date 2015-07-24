DNS QUERIER
================================

A simple DNS querier that periodically sends dns queries to specified
domains and register the querying times in a MySQL database


DEPENDENCIES
================================
You need to have libmysql++ and libldns on your machine. The headers have to be
in a canonical location (e.g., /usr/include/{mysql,mysql++}/) and the libraries
must be in ld path (e.g., in /usr/lib)


RUNNING THE QUERIER
================================
(1) Compile the source code:

	cd /path/to/source
	make
	
(2) Prepare the MySQL setup:
 + Create a specific database for the querier (e.g., 'dns_perf')
 + Create a specific username and password and grant access to the database
 + Update 'conf' file with the corresponding credentials and informations
 + The program will create the corresponding tables if necessary
	
(3) Specify domains names random prefix size in 'conf' file:
 + List domain names ' DOMAINS' field
 + Specify the size for the random prefix (ie if RANDOM_PREFIX_SIZE=3,
	  the program will send query for xyz.domain.ext where x,y and z are 
	  randomly generated)

(4) Run program:

	./dns_query <i>
	
`i` is an integer representing the number of minutes between consecutives queries


MySQL INFRASTRUCTURE
================================
 + Table "domains": contains the domain names and their the up-to-date statistics
 + Table "time_series": contains the actual time series for all domain names


CONTENTS
================================
conf -------------> The configuration
conf.{h,c}pp -----> A parser for "conf"
querier.{h,c}pp --> the DNS querier and the main() function
logger.{h,c}pp ---> an API for MySQL
Makefile ---------> standard makefile
README.md --------> this README file
