# Train-s-time-list
The project consists of a concurrent server connected via threads to  multiple clients at the same time. The client sends commands that have to respect a protocol to the server, then the clients receive a response. The client commands are: receiving a list of all trains that arrive in a particular station today; add/ remove delay to a certain train; get a list of all stations; show protocol.  
The project was written in C and it used a SQL database; the data was transformed from XML to GTFS using a community python algorithm; the data was provided by CFR Calatori.
