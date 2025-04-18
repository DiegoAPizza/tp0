#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	log_info(logger,"mensaje");
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();


	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	if(config==NULL){
		log_info(logger,"No existe el Config");
	}

	ip=config_get_string_value(config,"IP");
	puerto=config_get_string_value(config,"PUERTO");
	valor=config_get_string_value(config,"CLAVE");


	// Loggeamos el valor de config
	log_info(logger,"%s",ip);
	log_info(logger,"%s",puerto);
	

	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);


	// Enviamos al servidor el valor de CLAVE como mensaje
	t_pcb pcb=crear_pcb();;
	
	enviar_mensaje(valor,conexion);
	enviar_pcb(conexion,pcb);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}



t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
		nuevo_logger=log_create("tp0.log","tp0",true,LOG_LEVEL_INFO);
		return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config=config_create("./cliente.config");
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	
	
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(1){
		// La primera te la dejo de yapa
		log_info(logger,"Salir con exit");
		leido = readline("> ");
		if(strcmp(leido,"exit")==0){
			break;
		}
		log_info(logger,"%s",leido);
		free(leido);
	}

	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	t_log* logger=iniciar_logger();
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete=crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while(1){
		// La primera te la dejo de yapa
		log_info(logger,"Salir con exit");
		leido = readline("> ");
		agregar_a_paquete(paquete,leido,strlen(leido)+1);
		if(strcmp(leido,"exit")==0){
			break;
		}
		log_info(logger,"%s",leido);
		free(leido);
	}
	

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	enviar_paquete(paquete,conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */


}
