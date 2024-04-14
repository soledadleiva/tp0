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
	

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");
	

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	log_info(logger, "clave:%s", valor); //s de string
	log_info(logger, "puerto:%s", puerto);
	log_info(logger, "ip:%s", ip);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}







t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "TP0 cliente", true, LOG_LEVEL_INFO);
	return nuevo_logger;
}


t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	if (nuevo_config == NULL) {
    // ¡No se pudo crear el config!
    // Terminemos el programa
	perror("Error config.");
	exit(EXIT_FAILURE);
	}
	
	return nuevo_config;
}


void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger, leido);

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido,"")!=0){ //mientras lea algo
		free(leido);
		leido = readline("> ");
		log_info(logger, leido);
	}
	//sale cuando deja de leer 
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
	log_info(logger, "No hay nada mas que leer.");
}


void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	while(strcmp(leido,"")!=0){ //mientras lea algo
		agregar_a_paquete(paquete, leido,  strlen(leido)+1);
	}

	enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(paquete);
	eliminar_paquete(paquete);
}


void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(conexion); //3ro
	log_destroy(logger); //1ro
	config_destroy(config); //2do
}
