#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>

typedef enum
{
	MENSAJE,
	PAQUETE,
	PCB
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


typedef struct {
	uint8_t ax,bx,cx,dx;
	uint32_t pc,eax,ebx,ecx,edx,si,di;
}t_cpu;
typedef struct 
{
	int pid,pc,quantum;
	t_cpu registro;
}t_pcb;
void enviar_pcb(int socket_cliente, t_pcb pcb);
t_cpu iniciar_t_cpu();
t_pcb crear_pcb();
int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void* serializar_paquete_pcbs(t_paquete* paquete, int bytes);

#endif /* UTILS_H_ */
