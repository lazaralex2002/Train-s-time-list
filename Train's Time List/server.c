/* servTCPConcTh2.c - Exemplu de server TCP concurent care deserveste clientii
   prin crearea unui thread pentru fiecare client.
   Asteapta un numar de la clienti si intoarce clientilor numarul incrementat.
	Intoarce corect identificatorul din program al thread-ului.
  
   
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <mysql.h>

/* portul folosit */
#define PORT 2909

/* codul de eroare returnat de anumite apeluri */
int errno;

typedef struct thData{
	int idThread; //id-ul thread-ului tinut in evidenta de acest program
	int cl; //descriptorul intors de accept
}thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);

int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;	
  int nr;		//mesajul primit de trimis la client 
  int sd;		//descriptorul de socket 
  int pid;
  pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
	int i=0;
  

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
  /* utilizarea optiunii SO_REUSEADDR */
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 2) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
    {
      int client;
      thData * td; //parametru functia executata de thread     
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      // client= malloc(sizeof(int));
      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}
	
        /* s-a realizat conexiunea, se astepta mesajul */
    
	// int idThread; //id-ul threadului
	// int cl; //descriptorul intors de accept

	td=(struct thData*)malloc(sizeof(struct thData));	
	td->idThread=i++;
	td->cl=client;

	pthread_create(&th[i], NULL, &treat, td);	      
				
	}//while    
};				
static void *treat(void * arg)
{		
		struct thData tdL; 
		tdL= *((struct thData*)arg);	
		printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
		fflush (stdout);		 
		pthread_detach(pthread_self());		
		raspunde((struct thData*)arg);
		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);
		return(NULL);	
  		
};

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

int command_handler( char* response, char* buf, int nr)
{
    response[0] = 0;
    bool ok = true;
    MYSQL *con = mysql_init(NULL);
    if (con == NULL)
    {
        strcpy(response, "mysql_init() failed.");
        ok = false;
    }
    if (mysql_real_connect(con, "localhost", "retele", "retele",
          "cfr_calatori", 0, NULL, 0) == NULL)
    {
        strcpy(response, "mysql_real_connect() failed.");
        ok = false;
    }
    if ( ok )
    {
        switch (nr)
        {
            case 0:
            {
                break;
            }
            case 1:
            {
                strcat(response,"route_short_name|| route_id || stop_name || stop_id || arrival_time || departure_time || delay time \n");
                char sql[1000] = "select route_short_name, routes.route_id, stop_name, stop_id, arrival_time, departure_time, delay_time from stops natural join stop_times join trips on stop_times.trip_id = trips.trip_id join routes on routes.route_id = trips.route_id join delay on routes.route_id = delay.route_id where stop_id = ";
                strcat( sql, buf);
                strcat( sql, " order by departure_time");
                if (mysql_query(con, sql) )
                {
                    finish_with_error(con);
                }
                break;
            }
            case 2:
            {
                strcat(response,"route_short_name|| route_id || stop_name || stop_id || arrival_time || departure_time || delay time \n");
                char sql[1000] = "select route_short_name, routes.route_id, stop_name, stop_id, arrival_time, departure_time, delay_time from stops natural join stop_times join trips on stop_times.trip_id = trips.trip_id join routes on routes.route_id = trips.route_id join delay on routes.route_id = delay.route_id where stop_id = ";
                strcat( sql, buf);
                strcat( sql, " and TIMESTAMPDIFF(MINUTE,departure_time ,sysdate()) < 60 order by departure_time");
                if (mysql_query(con, sql) )
                {
                    finish_with_error(con);
                }
                break;
            }
            case 3:
            {   
                strcat(response,"route_short_name|| route_id || stop_name || stop_id || arrival_time || departure_time || delay time \n");
                char sql[1000] = "select route_short_name, routes.route_id, stop_name, stop_id, arrival_time, departure_time, delay_time from stops natural join stop_times join trips on stop_times.trip_id = trips.trip_id join routes on routes.route_id = trips.route_id join delay on routes.route_id = delay.route_id where stop_id = ";
                strcat( sql, buf);
                strcat( sql, " and TIMESTAMPDIFF(MINUTE,arrival_time ,sysdate()) < 60 order by departure_time");
                if (mysql_query(con, sql) )
                {
                    finish_with_error(con);
                }
                break;
            }
            case 4:
            {
                char str1[20];
                char str2[20];
                char sql[1000]="update delay set delay_time = ";
                int i = 0;
                while(i < strlen(buf) && buf[i] >= '0' && buf[i] <= '9')
                {
                    str1[i]=buf[i];
                    ++i;
                }
                str1[i] = 0;
                ++i;
                int i1 = i;
                while(i < strlen(buf) && buf[i] >= '0' && buf[i] <= '9')
                {
                    str2[i -i1]=buf[i];
                    ++i;
                }
                str2[i-i1] = 0;
                strcat(sql,str2);
                strcat(sql," where route_id = ");
                strcat(sql, str1);
                strcat(response,"delay set");
                if (mysql_query(con, sql) )
                {
                    finish_with_error(con);
                }
                return 0;
                break;
            }
            case 5:
            {
                strcat(response,"route_short_name|| route_id || stop_name || stop_id || arrival_time || departure_time || delay time \n");
                char sql[1000] = "select route_short_name, routes.route_id, stop_name, stop_id, arrival_time, departure_time, delay_time from stops natural join stop_times join trips on stop_times.trip_id = trips.trip_id join routes on routes.route_id = trips.route_id join delay on routes.route_id = delay.route_id where stop_name like '";
                strcat( sql, buf);
                strcat( sql, "' order by departure_time");
                if (mysql_query(con, sql) )
                {
                    finish_with_error(con);
                }
                break;
            }
            case 6:
            {
                strcat(response,"route_short_name|| route_id || stop_name || stop_id || arrival_time || departure_time || delay time \n");
                char sql[1000] = "select route_short_name, routes.route_id, stop_name, stop_id, arrival_time, departure_time, delay_time from stops natural join stop_times join trips on stop_times.trip_id = trips.trip_id join routes on routes.route_id = trips.route_id join delay on routes.route_id = delay.route_id where stop_name like '";
                strcat( sql, buf);
                strcat( sql, "' and TIMESTAMPDIFF(MINUTE,departure_time ,sysdate()) < 60 order by departure_time");
                if (mysql_query(con, sql) )
                {
                    finish_with_error(con);
                }
                break;
                break;
            }
            case 7:
            {
                if (mysql_query(con, "SELECT stop_id, stop_name FROM stops order by stop_name"))
                {
                    finish_with_error(con);
                }
                break;
            }
            case 8:
            {
                strcat(response,"route_short_name|| route_id || stop_name || stop_id || arrival_time || departure_time || delay time \n");
                char sql[1000] = "select route_short_name, routes.route_id, stop_name, stop_id, arrival_time, departure_time, delay_time from stops natural join stop_times join trips on stop_times.trip_id = trips.trip_id join routes on routes.route_id = trips.route_id join delay on routes.route_id = delay.route_id where stop_name like '";
                strcat( sql, buf);
                strcat( sql, "' and TIMESTAMPDIFF(MINUTE,arrival_time ,sysdate()) < 60 order by departure_time");
                if (mysql_query(con, sql) )
                {
                    finish_with_error(con);
                }
                break;
                break;
            }
            default:
                break;
            }
        }
        MYSQL_RES *result = mysql_store_result(con);
        int num_fields = mysql_num_fields(result);

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)))
        {
            for(int i = 0; i < num_fields; i++)
            {
                strcat( response , row[i] ? row[i] : "NULL" );
                strcat( response, " ");
            }
            strcat( response, "\n");
        }
        mysql_free_result(result);
        //strcpy(response, "no rows were found.");
        mysql_close(con);
}

void raspunde(void *arg)
{
    int nr, i=0;
    char buf[50];
    char response[2000000];
	struct thData tdL; 
	tdL= *((struct thData*)arg);
	if (read (tdL.cl, &nr,sizeof(int)) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.\n");
			
			}
    if (read (tdL.cl, &buf,sizeof(buf)) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.\n");
			
			}
    if(nr == -1)strcpy(response,"Command does not repect protocol");
    else if ( nr == 0)strcpy(response,"show-protocol\nhelp\nget-todays-train-list:<station name|id>\nget-next-departures:<station name|id>\nget-next-arrivals<station name|id>\nsend-train-delay<route_id, integer>\nget-stations\n");
	else command_handler(response, buf, nr);
	int l = strlen(response) * sizeof(char);
    if (write (tdL.cl, &l, sizeof(l)) <= 0)
		{
		 printf("[Thread %d] ",tdL.idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		}
		 /* returnam mesajul clientului */
	 if (write (tdL.cl, &response, l) <= 0)
		{
		 printf("[Thread %d] ",tdL.idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		}
	else
		printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",tdL.idThread);	

}
