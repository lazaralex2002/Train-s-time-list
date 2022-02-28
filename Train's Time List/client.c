/* cliTCPIt.c - Exemplu de client TCP
   Trimite un numar la server; primeste de la server numarul incrementat.
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
int errno;

/* portul de conectare la server*/
int port;

int date_time(char s[])
{
    int i = 0;
    for ( int i = 0 ; i < 4; ++i) if ( s[i] <'0' || s[i] > '9') return -1;
    if ( s[4] != '-') return -1;
    for ( int i = 5 ; i <= 6; ++i) if ( s[i] <'0' || s[i] > '9') return -1;
    if ( s[7] != '-') return -1;
    for ( int i = 8 ; i <= 11; ++i) if ( s[i] <'0' || s[i] > '9') return -1;
    if ( s[12] != ':') return -1;
    for ( int i = 13 ; i <= 14; ++i) if ( s[i] <'0' || s[i] > '9') return -1;
    if ( s[15] != ':') return -1;
    for ( int i = 16 ; i <= 17; ++i) if ( s[i] <'0' || s[i] > '9') return -1;
    s[18] = 0;
    return 0;
}

int validate(char s[] )
{
	if ( !strcmp(s,"show-protocol")|| !strcmp(s,"help") ) return 0;
	if (!strncmp ( s , "get-todays-train-list:", 22) )
	{
    	int i = 22;
        int l = strlen(s);
        int ok = 1, ok2 = 1;
        while ( i < l )
        {
            if (s[i] < '0' || s[i] > '9') ok = 0;
            ++i;
        }
        while ( i < l )
        {
            if (!( (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <='Z') )) ok2 = 0;
            ++i;
        }
        if( ok == 0 && ok2 == 0) return -1;
        char aux[100];
        strcpy(aux, s+22);
        strcpy(s, aux);
        if(ok)return 1;
        return 5;
	}
    if (!strncmp ( s , "get-next-departures:", 20) )
	{
		int i = 20;
        int l = strlen(s);
        int ok = 1, ok2 = 1;
        while ( i < l )
        {
            if (s[i] < '0' || s[i] > '9') ok = 0;
            ++i;
        }
        while ( i < l )
        {
            if (!( (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <='Z') )) ok2 = 0;
            ++i;
        }
        if( ok == 0 && ok2 == 0) return -1;
        char aux[100];
        strcpy(aux, s+20);
        strcpy(s, aux);
        if(ok)return 2;
        return 6;
	}
    if (!strncmp ( s , "get-next-arrivals:", 18) )
	{
		int i = 18;
        int l = strlen(s);
        int ok = 1, ok2 = 1;
        while ( i < l )
        {
            if (s[i] < '0' || s[i] > '9') ok = 0;
            ++i;
        }
        while ( i < l )
        {
            if (!( (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <='Z') )) ok2 = 0;
            ++i;
        }
        if( ok == 0 && ok2 == 0) return -1;
        char aux[100];
        strcpy(aux, s+18);
        strcpy(s, aux);
        if(ok)return 3;
        return 8;
	}
    if (!strncmp ( s , "send-train-delay:", 17) )
	{
		int i = 17;
        int l = strlen(s);
        int comma = 0;
        int ok1 = 0;
        while ( i < l )
        {
            if(s[i] < '0' || s[i] > '9')
            {
                if ( ok1 == 0 &&s[i] == ':') ok1 = 1;
                else return -1;
            } 
            ++i;
        }
        char aux[100];
        strcpy(aux, s+17);
        strcpy(s, aux);
        return 4;
	}
	if(!strcmp(s,"get-stations"))
	{
		return 7;
	}
	return -1;	
}


void myTruncate( char s[] )
{
    char aux[100];
	int l = strlen(s);
	int i =0; 
    while( s[i] != 0 )
    {
        if( (( s[i] >= '0' && s[i] <= '9') || ( s[i] >= 'a'&& s[i] <='z') || ( s[i] >= 'A'&& s[i] <='Z') || (s[i] =='-') || (s[i] == ':')) != 1 )
        {
            strcpy(aux, s + i + 1);
            strcpy(s + i, aux);
            --i;
        }
        i++;
    }
}

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  		// mesajul trimis
  char buf[50];

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
        if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror ("Eroare la socket().\n");
            return errno;
        }
        if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
        {
            perror ("[client]Eroare la connect().\n");
            return errno;
        }
  /* citirea mesajului */
  printf ("[client]Introduceti o comanda corespunzatoare protocolului\n");
  fflush (stdout);
  fgets(buf, sizeof(buf), stdin);
        myTruncate( buf );
        int nr = validate(buf);
        /* trimiterea mesajului la server */
        if (write (sd,&nr,sizeof(int)) <= 0)
        {
            perror ("[client]Eroare la write() spre server.\n");
            return errno;
        }
        if (write (sd,&buf,sizeof(buf)) <= 0)
        {
            perror ("[client]Eroare la write() spre server.\n");
            return errno;
        }

        /* citirea raspunsului dat de server 
            (apel blocant pina cind serverul raspunde) */
        if (read (sd, &nr,sizeof(int)) < 0)
        {
            perror ("[client]Eroare la read() de la server.\n");
            return errno;
        }
        char response[nr + 1];
        if (read (sd, &response ,nr) < 0)
        {
            perror ("[client]Eroare la read() de la server.\n");
            return errno;
        }
        /* afisam mesajul primit */
        printf ("%s\n", response);
        close (sd);
  //scanf("%d",&nr);

  /* inchidem conexiunea, am terminat */
}
