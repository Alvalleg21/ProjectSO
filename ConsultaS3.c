//Incluir esta libreriￂﾭa para poder hacer las llamadas en shiva2.upc.es
//#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
	int main(int argc, char **argv)
{
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
	
	char jugador[20];
	// Pregunto los nombre de un jugador
	printf ("Dame el nombre de un jugador (Juanito o Pepito)\n"); 
	scanf ("%s", jugador);
	
	char consulta [80];
	strcpy (consulta,"SELECT PARTIDA.GANADOR FROM PARTIDA WHERE PARTIDA.GANADOR ='");
	strcat (consulta, jugador);
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
		printf ("Ha ganado %i veces\n", i);

	}
	
	int puntos1;
	int puntos2;
	MYSQL_RES *resultado1;
	MYSQL_RES *resultado2;
	MYSQL_ROW row1;
	MYSQL_ROW row2;
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
		
		printf("Juanito es quien mas puntos tiene con %d puntos\n", puntos2);
	else
		printf("Pepito es quien mas puntos tiene con %d puntos\n", puntos1);
	
	
	MYSQL_RES *resultado9;
	MYSQL_ROW row9;
	int IDJ;
	char ganador [20];
	//En primer lugar, preguntamos el nombre de un ganador. 
	printf("Dame el nombre del ganador\n");
	scanf("%s",ganador);
	printf("esta bien");
	//Ahora, realizamos la consulta. En mi caso, queremos el ID del ganador. 
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
			printf ("IDJ: %i", row9[0]);
			// obtenemos la siguiente fila
			row9 = mysql_fetch_row (resultado9);
	}
			
	mysql_close (conn);
	exit(0);
}


