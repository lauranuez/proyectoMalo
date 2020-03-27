#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>


int main(int argc, char *argv[])
{
	MYSQL *conn;
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta [80];
	conn = mysql_init(NULL);
	
	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	
	
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
		
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// escucharemos en el port 9050
	serv_adr.sin_port = htons(9070);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind\n");
	//La cola de peticiones pendientes no podr? ser superior a 4
	if (listen(sock_listen, 2) < 0)
		printf("Error en el Listen");
	
	int i;
	// Atenderemos solo 5 peticione
	for(;;)
	{
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion");
		
		if (conn==NULL) 
		{
			printf ("Error al crear la conexion: %u %s\n",mysql_errno(conn), mysql_error(conn));
			exit (1);
		}

		
		conn = mysql_real_connect (conn, "localhost","root", "mysql", "partidas",0, NULL, 0);
		if (conn==NULL) 
		{
			printf ("Error al inicializar la conexion: %u %s\n",mysql_errno(conn), mysql_error(conn));
			exit (1);
		}

		
		//sock_conn es el socket que usaremos para este cliente
		int terminar=0;
		while (terminar==0)
		{
			// Ahora recibimos su nombre, que dejamos en buff
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
			
			// Tenemos que a?adirle la marca de fin de string 
			// para que no escriba lo que hay despues en el buffer
			peticion[ret]='\0';
			
			//Escribimos el nombre en la consola
			
			printf ("Se ha conectado: %s\n",peticion);
			
			char *p = strtok( peticion, "/");
			int codigo =  atoi (p);
			printf("codigo= %d\n",codigo);
			
			if (codigo==0)
			{
				terminar=1;
			}

			if(codigo==1)
			{
				char usuario[20];
				char password[20];
				p=strtok(NULL, "/");
				strcpy(usuario, p);
				p=strtok(NULL, "/");
				strcpy(password, p);
				
				err=mysql_query (conn, "SELECT * FROM datos_jugador");
				if (err!=0) 
				{
					printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				if (row == NULL)
					printf ("No se han obtenido datos en la consulta\n");
				else
				{
					while (row != NULL) 
					{
						if (strcmp (row[0],usuario) ==0)
						{
							if(strcmp(row[1],password)==0)
							{
								strcpy(respuesta,"1\0");	
							}
							else 
							{
								strcpy(respuesta,"0\0");
							}
							row = NULL;
						}
						else 
						{
							row = mysql_fetch_row (resultado);
						}	
					}
				}
					
					// Se acabo el servicio para este cliente
				
			}
			
			if (codigo==2)
			{
				char usuario[20];
				char password[20];
				int edad;
				p=strtok(NULL, "/");
				strcpy(usuario, p);
				p=strtok(NULL, "/");
				strcpy(password, p);
				p=strtok(NULL, "/");
				edad= atoi(p);
				
				err=mysql_query (conn, "SELECT * FROM datos_jugador");
				if (err!=0) 
				{
					printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				if (row == NULL)
					printf ("No se han obtenido datos en la consulta\n");
				else
				{
					char name[strlen(row[0])];
					strcpy(name,row[0]);
					while (row !=NULL) 
					{
						strcpy(respuesta, "1\0");
						if (strcmp (name,usuario) ==0)
						{
							strcpy(respuesta,"0\0");
						}
						else row = mysql_fetch_row (resultado);
					}
				}
				if (strcmp(respuesta,"1\0")==0)
				{
					char edads[3];
					strcpy(consulta, "INSERT INTO datos_jugador VALUES('");
					strcat(consulta, usuario);
					strcat (consulta, "','");
					strcat(consulta, password);
					strcat (consulta, "',");
					sprintf(edads, "%d", edad);
					strcat (consulta, edads);
					strcat(consulta, ");");
				}
				
			}
			
			if (codigo==3)
			{
				int Npartida;
				p=strtok(NULL, "/");
				Npartida=atoi(p);
				//Pido al usuario que introduzca el nombre de parti
				//Realizamos la consulta
				strcpy(consulta,"SELECT * FROM partida");
				err=mysql_query(conn,consulta);
				//err=mysql_query(conn,"SELECT relacion.jugador FROM relacion WHERE relacion.partida = 'p1'");
				if (err!=0)
				{
					printf("Error al realizar la consulta %u %s \n", mysql_errno(conn), mysql_error(conn));
					exit(1);
				}
				resultado=mysql_store_result(conn);
				row = mysql_fetch_row(resultado);
				
				if (row == NULL)
					strcpy(respuesta,"Base de datos vacia");
				else
				{
					strcpy(respuesta,"Los jugadores son: ");
					while (row!=NULL)
					{
						int cont = atoi(row[1]);
						if (cont==Npartida)
						{
							strcat (respuesta, row[0]);
							strcat (respuesta, " ");
						}
						row = mysql_fetch_row(resultado);
					}
					if (strcmp(respuesta,"Los jugadores son: ")==0)
						strcpy(respuesta,"Esta ID no corresponde a ninguna partida (prueba 1 o 2)");
					
					
				}	
				respuesta [strlen(respuesta)-1]='\0';
			}
			
		
			
			if(codigo==4)
			{
				err=mysql_query (conn, "SELECT * FROM datos_partida");
				if (err!=0) 
				{
					printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				
				
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				if (row == NULL)
					strcpy(respuesta, "Base de datos vacia");
				else
				{
					strcpy (respuesta, "Los ganadores son :");
					while (row !=NULL) 
					{
						strcat (respuesta, row[4]); 
						strcat (respuesta, " ");
						row = mysql_fetch_row (resultado);
					}
					
				}
				
				respuesta [strlen(respuesta)-1]='\0';
	
				
			}
			
			if (codigo==5)
			{
				int Npartida;
				p=strtok(NULL, "/");
				Npartida=atoi(p);
				char np[20];
			
			
				strcpy (consulta,"SELECT * FROM datos_partida WHERE ID = " );
				sprintf(np, "%d", Npartida);
				strcat (consulta, np);
				strcat (consulta, ";");
				err=mysql_query (conn,consulta );
				if (err!=0) 
				{
					printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
					
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				if (row == NULL)
					strcpy(respuesta, "Base de datos vacia");
				else
				{
					
					char fecha[20];
					char hf[20];
					char dur[20];
					char nme[20];
					
					strcpy(fecha, row[1]);
					strcpy(hf,row[2]);
					strcpy(nme,row[4]);
					sprintf(respuesta, "ID: %d, Fecha: %s, Hora final: %s, Duracion: %dmin, Ganador: %s\0", Npartida,fecha,hf,atoi(row[3]),nme);
				}
			}
			if (codigo !=0)
			{
				printf ("Respuesta: %s\n", respuesta);
				// Enviamos la respuesta
				write (sock_conn,respuesta, strlen(respuesta));
			}
			
			respuesta [strlen(respuesta)-1]='\0';
			
		}
		close(sock_conn);
		mysql_close (conn);
	}
}
