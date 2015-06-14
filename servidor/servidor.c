#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BACKLOG 5

int main() {
	int descrip_socket; //Declaramos el descriptor

	struct addrinfo infoSocket, *infoServidor; //Declaramos las estructuras

	memset(&infoSocket, 0, sizeof(infoSocket)); //Seteamos el valor de la informacion de la direccion a 0
	//Asignamos los valores de info de la conexion
	infoSocket.ai_family = AF_UNSPEC; //Indica a getaddrinfo(3) que la direccion va a ser de tipo IPv4 o IPv6
	infoSocket.ai_flags = AI_PASSIVE; //Solo para cuando se quiere utilizar el socket para un servidor
	infoSocket.ai_socktype = SOCK_STREAM; //Socket de tipo TCP/IP

	char * dirIP = NULL;
	char * puerto = "6666";

	//Obtenemos la direccion y los datos del socket y los mete en infoServidor
	if (getaddrinfo(dirIP, puerto, &infoSocket, &infoServidor) != 0) {
		perror("No se pudo obtener la direccion correctamente.");
		return -1;
	}

	//Creamos el socket
	descrip_socket = socket(infoServidor->ai_family, infoServidor->ai_socktype,
			infoServidor->ai_protocol);
	if (descrip_socket == -1) {
		freeaddrinfo(infoServidor);
		perror("No se pudo crear el socket");
		return -1;
	}

	bind(descrip_socket, infoServidor->ai_addr, infoServidor->ai_addrlen);
	freeaddrinfo(infoServidor);
	listen(descrip_socket, BACKLOG);
	struct sockaddr_in dir_cliente;	// Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc.
	socklen_t addrlenght = sizeof(dir_cliente);

	int socket_cliente = accept(descrip_socket,
			(struct sockaddr *) &dir_cliente, &addrlenght);

	//int32_t tamanio_info = sizeof(char) * 100;
	//char * buffer = (char*) calloc(1,tamanio_info);
	uint32_t numero;
	recv(socket_cliente, &numero, sizeof(uint32_t), 0);

	printf("%d", numero);

	//free(buffer);
	close(socket_cliente);
	close(descrip_socket);
	return 0;
}
