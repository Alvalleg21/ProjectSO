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
	
	//Inicializar bbdd
	MYSQL *conn;
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexin
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "bd" ,0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	
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
	// establecemos el puerto de escucha
	serv_adr.sin_port = htons(9000);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	int i;
	// Bucle infinito
	for (;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		
		int terminar =0;
		// Entramos en un bucle para atender todas las peticiones de este cliente
		//hasta que se desconecte
		while (terminar ==0)
		{
			// Ahora recibimos la petici?n
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
			
			// Tenemos que a?adirle la marca de fin de string 
			// para que no escriba lo que hay despues en el buffer
			peticion[ret]='\0';
			
			
			printf ("Peticion: %s\n",peticion);
			
			// vamos a ver que quieren
			char *p = strtok( peticion, "/");
			int codigo =  atoi (p);
			// Ya tenemos el c?digo de la petici?n
			char nombre[20];
			
			if (codigo !=0)
			{
				p = strtok( NULL, "/");

				strcpy (nombre, p);
				// Ya tenemos el nombre
				printf ("Codigo: %d, Nombre: %s\n", codigo, nombre);
			}
			
			if (codigo ==0) //petici?n de desconexi?n
				terminar=1;
			else if (codigo ==1) 
			{
				//char jugador[20];
				// Pregunto los nombre de un jugador
				//printf ("Dame el nombre de un jugador (Juanito o Pepito)\n"); 
				//scanf ("%s", jugador);
			
				char consulta [80];
				strcpy (consulta,"SELECT PARTIDA.GANADOR FROM PARTIDA WHERE PARTIDA.GANADOR ='");
				strcat (consulta, nombre);
				strcat (consulta,"';");
			
				err=mysql_query (conn, consulta);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				//recogemos el resultado de la consulta. El resultado de la
				//consulta se devuelve en una variable del tipo puntero a
				//MYSQL_RES tal y como hemos declarado anteriormente.
				//Se trata de una tabla virtual en memoria que es la copia
				//de la tabla real en disco.
				resultado = mysql_store_result (conn);
				// El resultado es una estructura matricial en memoria
				// en la que cada fila contiene los datos de una persona.
				
				// Ahora obtenemos la primera fila que se almacena en una
				// variable de tipo MYSQL_ROW
				row = mysql_fetch_row (resultado);
				int i=0;
				if (row == NULL)
					printf ("No se han obtenido datos en la consulta\n");
				else
				{
					while (row !=NULL) {
						i=i+1;
						row = mysql_fetch_row (resultado);
					}
					sprintf (respuesta, "Ha ganado %i veces\n", i);
					
				}
					
					//strcpy(respuesta,i);
					//sprintf (respuesta,"%d",strlen (nombre));
			}
			
			else if (codigo ==2)
			{
				int puntos1;
				int puntos2;
				MYSQL_RES *resultado1;
				MYSQL_RES *resultado2;
				MYSQL_ROW row1;
				MYSQL_ROW row2;
				char consulta [80];
				strcpy (consulta, "SELECT SUM(PUNTUACION.PUNTOS) FROM (JUGADOR,PARTIDA,PUNTUACION)"
						" WHERE (PUNTUACION.ID_J=JUGADOR.IDJ) AND (PUNTUACION.ID_P=PARTIDA.ID)" 
						"AND (JUGADOR.USERNAME='PEPITO')");
				
				err=mysql_query (conn, consulta);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				
				resultado1 = mysql_store_result (conn);
				row1 = mysql_fetch_row (resultado1);
				
				if (row1== NULL)
					printf("No se han obtenido datos en la consulta\n");
				else
					puntos1 = atoi (row1[0]);
				
				strcpy (consulta, "SELECT SUM(PUNTUACION.PUNTOS) FROM (JUGADOR,PARTIDA,PUNTUACION)"
						" WHERE (PUNTUACION.ID_J=JUGADOR.IDJ) AND (PUNTUACION.ID_P=PARTIDA.ID)" 
						"AND (JUGADOR.USERNAME='JUANITO')");
				
				err=mysql_query (conn, consulta);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				
				resultado2 = mysql_store_result (conn);
				row2 = mysql_fetch_row (resultado2);
				
				if (row2== NULL)
					printf("No se han obtenido datos en la consulta\n");
				else
					puntos2 = atoi (row2[0]);
				
				if (puntos1<puntos2)
					
					sprintf(respuesta, "Juanito es quien mas puntos tiene con %d puntos\n", puntos2);
				else
					sprintf(respuesta, "Pepito es quien mas puntos tiene con %d puntos\n", puntos1);
			}
			else //quiere saber si es alto
			{
				MYSQL_RES *resultado9;
				MYSQL_ROW row9;
				int IDJ;
				char ganador [20];
				//En primer lugar, preguntamos el nombre de un ganador. 
				printf("Dame el nombre del ganador\n");
				scanf("%s",ganador);
				printf("esta bien");
				//Ahora, realizamos la consulta. En mi caso, queremos el ID del ganador. 
				char consulta [80];
				strcpy (consulta,"SELECT JUGADOR.IDJ FROM (JUGADOR, PARTIDA, PUNTUACION) WHERE PARTIDA.GANADOR = ' AND	 PARTIDA.IDP=PUNTUACION.ID_P AND	PUNTUACION.ID_J=JUGADOR.IDJ");
				strcat (consulta, ganador);
				strcat (consulta,"';");
				
				printf("esta bien");
				
				err=mysql_query (conn, IDJ);
				if (err!=0) {
					printf ("Error al consultar datos de la base %u %s\n",
							mysql_errno(conn), mysql_error(conn));
					exit (1);
				}
				//recogemos el resultado de la consulta
				resultado9 = mysql_store_result (conn);
				row9 = mysql_fetch_row (resultado9);
				if (row9 == NULL)
					printf ("No se han obtenido datos en la consulta\n");
				else
					while (row9 !=NULL) {
						// la columna 0 contiene tiene el ID del ganador
						sprintf (respuesta, "IDJ: %i", row9[0]);
						// obtenemos la siguiente fila
						row9 = mysql_fetch_row (resultado9);
				}
			}
				
			if (codigo !=0)
			{
				
				printf ("Respuesta: %s\n", respuesta);
				// Enviamos respuesta
				write (sock_conn,respuesta, strlen(respuesta));
			}
		}
		// Se acabo el servicio para este cliente
		close(sock_conn); 
		mysql_close (conn);
		
	}
}
