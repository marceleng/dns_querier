#ifndef CONF_HPP
#define CONF_HPP

#define RANDOM_PREFIX_SIZE 10
#define NB_DOMAINS 10

std::string DOMAINS[NB_DOMAINS] = {
	".google.com",
	".facebook.com",
	".youtube.com",
	".yahoo.com",
	".live.com",
	".wikipedia.org",
	".baidu.com",
	".blogger.com",
	".msn.com",
	".qq.com"
};

std::string MYSQL_USER = "dnsq";
std::string MYSQL_PASSWD = "asdffh(rgjrgkljj#/djdjfd";
std::string MYSQL_ADDRESS = "localhost";
std::string MYSQL_DB = "dns";
#ifndef MYSQL_PORT //Usually def in libmysql
#define MYSQL_PORT = 3306;
#endif

#endif
