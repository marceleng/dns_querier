#!/bin/sh

user=$(grep -o -P "(?<=MYSQL_USER=).*" conf)
pass=$(grep -o -P "(?<=MYSQL_PASSWD=).*" conf)
address=$(grep -o -P "(?<=MYSQL_ADDRESS=).*" conf)
db=$(grep -o -P "(?<=MYSQL_DB=).*" conf)
port=$(grep -o -P "(?<=MYSQL_PORT=).*" conf)
prefix_size=$(grep -o -P "(?<=RANDOM_PREFIX_SIZE=).*" conf)


############################
# Creates the MySQL schema #
############################
mysql -h $address -P $port -D $db -u $user -p$pass << EOF
CREATE TABLE IF NOT EXISTS domains ( 
id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
name VARCHAR(100) NOT NULL,
first_ts TIMESTAMP NOT NULL,
last_ts TIMESTAMP,
average INT UNSIGNED,
std_dev INT UNSIGNED );
CREATE TABLE IF NOT EXISTS time_series (
id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
domain_id INT UNSIGNED NOT NULL,
timestamp TIMESTAMP NOT NULL,
query_time INT NOT NULL );
EOF

#############################
# Updates the conf.hpp file #
#############################
mv conf.dummy conf.hpp
sed -i s/"\$random_prefix_size"/$prefix_size/
sed -i s/"\$mysql_user"/$user/
sed -i s/"\$mysql_passwd"/$pass/
sed -i s/"\$mysql_address"/$address/
sed -i s/"\$mysql_db"/$db/
sed -i s/"\$mysql_port"/$port/

