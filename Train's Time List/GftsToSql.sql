CREATE DATABASE IF NOT EXISTS cfr_calatori;

USE cfr_calatori;

DROP TABLE IF EXISTS agency;

CREATE TABLE agency 
(
    agency_id INT(15),
    agency_name VARCHAR(30),
    agency_url VARCHAR(50),
    agency_timezone varchar(20)
);

DROP TABLE IF EXISTS calendar;

CREATE TABLE calendar
(
    service_id INT(5),
    monday INT(1),
    tuesday INT(1),
    wednesday INT(1),
    thursday INT(1),
    friday INT(1),
    saturday INT(1),
    sunday INT(1),
    start_date date,
    end_date date
);

DROP TABLE IF EXISTS routes; 

CREATE TABLE routes
(
    route_id INT(5),
    agency_id INT(15),
    route_short_name varchar(50),
    route_long_name varchar(850),
    route_type INT(3)
);

DROP TABLE IF EXISTS stop_times;

CREATE TABLE stop_times
(
    trip_id INT(10),
    arrival_time TIME,
    departure_time TIME,
    stop_id INT(10),
    stop_sequence INT(5)
);

DROP TABLE IF EXISTS stops;

CREATE TABLE stops 
(
    stop_id INT(10),
    stop_name char(30),
    stop_lat float(13,10),
    stop_lon float(13,10)
);

DROP TABLE IF EXISTS trips;

CREATE TABLE trips 
(
    route_id INT(5),
    service_id INT(5),
    trip_id INT(10),
    trip_short_name varchar(3)
);

LOAD DATA LOCAL INFILE '/home/alc/Documents/Retele/agency.txt' INTO TABLE agency FIELDS TERMINATED BY ',' IGNORE 1 LINES;
LOAD DATA LOCAL INFILE '/home/alc/Documents/Retele/calendar.txt' INTO TABLE calendar FIELDS TERMINATED BY ',' IGNORE 1 LINES;
LOAD DATA LOCAL INFILE '/home/alc/Documents/Retele/routes.txt' INTO TABLE routes FIELDS TERMINATED BY ',' IGNORE 1 LINES;
LOAD DATA LOCAL INFILE '/home/alc/Documents/Retele/stop_times.txt' INTO TABLE stop_times FIELDS TERMINATED BY ',' IGNORE 1 LINES;
LOAD DATA LOCAL INFILE '/home/alc/Documents/Retele/stops.txt' INTO TABLE stops FIELDS TERMINATED BY ',' IGNORE 1 LINES;
LOAD DATA LOCAL INFILE '/home/alc/Documents/Retele/trips.txt' INTO TABLE trips FIELDS TERMINATED BY ',' IGNORE 1 LINES;
