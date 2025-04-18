#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>

#define PUERTO "4444"

typedef enum
{
	MENSAJE,
	PAQUETE,
	PCB
}op_code;

extern t_log* logger;
typedef struct {
	uint8_t ax,bx,cx,dx;
	uint32_t pc,eax,ebx,ecx,edx,si,di;
}t_cpu;
typedef struct 
{
	int pid,pc,quantum;
	t_cpu registro;
}t_pcb;
void mostrar_cpu(t_cpu,t_log*);
void mostrar_pcb(t_pcb*,t_log*);
void* recibir_buffer(int*, int);
t_pcb* recibir_pcb(int);
int iniciar_servidor(void);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);

#endif /* UTILS_H_ */
