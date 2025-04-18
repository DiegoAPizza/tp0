#include "utils.h"


void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = 0;
	socket_cliente=socket(server_info->ai_family,
                         server_info->ai_socktype,
                         server_info->ai_protocol
						 );
	// Ahora que tenemos el socket, vamos a conectarlo
	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return socket_cliente;
}

t_cpu iniciar_t_cpu(){
	t_cpu cpu;
		cpu.ax=0;
		cpu.bx=0;
		cpu.cx=0;
		cpu.dx=0;
		cpu.eax=0;
		cpu.ebx=0;
		cpu.ecx=0;
		cpu.edx=0;
		cpu.si=0;
		cpu.di=0;
		cpu.pc=0;
	return cpu;
}

t_pcb crear_pcb(){
	t_pcb pcb;

	pcb.pid=0;
	pcb.pc=1;
	pcb.quantum=123;
	pcb.registro= iniciar_t_cpu();


	return pcb;
}


void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}
void enviar_pcb(int socket_cliente, t_pcb pcb) {
    // Crear el paquete
    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->codigo_operacion = PCB;

    // Calcular el tamaño del buffer
    int size_pcb = sizeof(pcb.pc) + sizeof(pcb.pid) + sizeof(pcb.quantum) +
                   sizeof(pcb.registro.ax) + sizeof(pcb.registro.bx) +
                   sizeof(pcb.registro.cx) + sizeof(pcb.registro.eax) +
                   sizeof(pcb.registro.ebx) + sizeof(pcb.registro.ecx) +
                   sizeof(pcb.registro.di) + sizeof(pcb.registro.si);

    paquete->buffer = malloc(sizeof(t_buffer));
    paquete->buffer->size = size_pcb;
    paquete->buffer->stream = malloc(paquete->buffer->size);

    // Copiar los campos del PCB en el buffer con el desplazamiento correspondiente
    int offset = 0;
    memcpy(paquete->buffer->stream + offset, &(pcb.pc), sizeof(pcb.pc));
    offset += sizeof(pcb.pc);
    memcpy(paquete->buffer->stream + offset, &(pcb.pid), sizeof(pcb.pid));
    offset += sizeof(pcb.pid);
    memcpy(paquete->buffer->stream + offset, &(pcb.quantum), sizeof(pcb.quantum));
    offset += sizeof(pcb.quantum);

    memcpy(paquete->buffer->stream + offset, &(pcb.registro.ax), sizeof(pcb.registro.ax));
    offset += sizeof(pcb.registro.ax);
    memcpy(paquete->buffer->stream + offset, &(pcb.registro.bx), sizeof(pcb.registro.bx));
    offset += sizeof(pcb.registro.bx);
    memcpy(paquete->buffer->stream + offset, &(pcb.registro.cx), sizeof(pcb.registro.cx));
    offset += sizeof(pcb.registro.cx);
    memcpy(paquete->buffer->stream + offset, &(pcb.registro.eax), sizeof(pcb.registro.eax));
    offset += sizeof(pcb.registro.eax);
    memcpy(paquete->buffer->stream + offset, &(pcb.registro.ebx), sizeof(pcb.registro.ebx));
    offset += sizeof(pcb.registro.ebx);
    memcpy(paquete->buffer->stream + offset, &(pcb.registro.ecx), sizeof(pcb.registro.ecx));
    offset += sizeof(pcb.registro.ecx);
    memcpy(paquete->buffer->stream + offset, &(pcb.registro.di), sizeof(pcb.registro.di));
    offset += sizeof(pcb.registro.di);
    memcpy(paquete->buffer->stream + offset, &(pcb.registro.si), sizeof(pcb.registro.si));
    offset += sizeof(pcb.registro.si);

    // Calcular el tamaño total a enviar (buffer + 2 * sizeof(int))
    int bytes = paquete->buffer->size + 2 * sizeof(int);

    // Serializar el paquete
    void* a_enviar = serializar_paquete_pcbs(paquete, bytes);

    // Enviar el paquete a través del socket
    send(socket_cliente, a_enviar, bytes, 0);

    // Liberar la memoria
    free(a_enviar);
    eliminar_paquete(paquete);
}
void* serializar_paquete_pcbs(t_paquete* paquete, int bytes) {
    // Reservar memoria para almacenar el paquete serializado
    void* magic = malloc(bytes);
    int desplazamiento = 0;

    // Copiar el código de operación en la memoria serializada
    memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
    desplazamiento += sizeof(int);

    // Copiar el tamaño del buffer en la memoria serializada
    memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
    desplazamiento += sizeof(int);

    // Copiar el contenido del buffer (el stream) en la memoria serializada
    memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
    desplazamiento += paquete->buffer->size;

    // Retornar el puntero a la memoria serializada
    return magic;
}

void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}



void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
