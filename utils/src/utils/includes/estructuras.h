#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <commons/log.h> 
#include <commons/config.h>
#include <commons/bitarray.h>
#include <commons/temporal.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <assert.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "serializacion.h"



typedef struct{
    int pid;
    t_list*contextos_tids;
    uint32_t base; 
    uint32_t limite;
    int tamanio_proceso; 
}t_contexto_pid;

typedef struct{
    int pid;
    uint32_t base;
    uint32_t limite;
    int tamanio_proceso;
}t_contexto_pid_send;

typedef struct{
    uint32_t AX;
    uint32_t BX;
    uint32_t CX;
    uint32_t DX;
	uint32_t EX;
    uint32_t FX;
    uint32_t GX;
    uint32_t HX;
    uint32_t PC;
}t_registros_cpu; 

typedef struct{
int tid;
int pid;
uint32_t pc;
}t_instruccion_memoria;

typedef struct{
int tid;
int pid;
uint32_t direccionFisica;
uint32_t valor;
}t_write_mem;

typedef struct{
    int tid;
    int pid;
    uint32_t direccionFisica;
}t_read_mem;

typedef struct{
    int pid;
    int tid;
    t_registros_cpu*registros;
}t_contexto_tid;



typedef struct {
    t_contexto_tid* contexto;
	//t_contexto* contexto;
	int quantum_utilizado;
	t_temporal* quantum;
}t_pcb;

typedef enum {
    SUCCESS,
    INVALID_RESOURCE,
    INVALID_INTERFACE,
    OUT_OF_MEMORY,
    INTERRUPTED_BY_USER,
}motivo_exit;

typedef struct {
    t_pcb* pcb;
    motivo_exit motivo;
}t_pcb_exit;

typedef enum // SOLO USARLO CON MEMORIA
{
    ERROR=-1,
    Algo, //hay un case -1 que lo cambie a 1
    //ESTADOS
    NEW,
    READY,
    EXEC,
    BLOCK,
    EXIT_,
    //MENSAJES GENERICOS
    MENSAJE,
	PAQUETE,
    SET,
    READ_MEM,
    WRITE_MEM,
    SUM,
    SUB,
    JNZ,
    LOG,
    DUMP_MEMORY,
    IO,
    PROCESS_CREATE,
    THREAD_CREATE,
    THREAD_JOIN,
    THREAD_CANCEL,
    MUTEX_CREATE,
    MUTEX_LOCK,
    MUTEX_UNLOCK,
    THREAD_EXIT,
    PROCESS_EXIT,
    WAIT,
    SIGNAL,
    EXIT,
    MEM_SEGMENTATION_FAULT,
    //SOLICITUDES DE CPU A OTROS
    PEDIR_INSTRUCCION_MEMORIA,
    //SOLICITUDES DE KERNEL A OTROS
    INICIO_NUEVO_PROCESO,
    FINALIZO_PROCESO,
    FIN_QUANTUM_RR_CPU,
    //COSAS QUE LE LLEGAN EL KERNEL
    IDENTIFICACION,
    OBTENER_VALIDACION,
    //MOTIVOS DE DESALOJO
    TERMINO_PROCESO,
    INTERRUPCION,
    INTERRUPCION_USUARIO,
    LLAMADA_POR_INSTRUCCION,
    //COMUNICACION MEMORIA CON MODULOS
    CREAR_PROCESO,
    ACCESO,
    FINALIZAR_PROCESO,
    ACCESO_TABLA_PAGINAS,
    ACCESO_ESPACIO_USUARIO,
    

    // CONTEXTOS
    CREAR_CONTEXTO_TID,
    ENVIAR_CONTEXTO_PID, //
    ENVIAR_CONTEXTO_TID, //
    CONTEXTO_PID_INEXISTENTE, //
    CONTEXTO_TID_INEXISTENTE,
    OBTENER_CONTEXTO_TID,
    OBTENER_CONTEXTO_PID,
    OBTENER_CONTEXTO_EJECUCION,
    ACTUALIZAR_CONTEXTO_TID,

    OBTENCION_CONTEXTO_TID_OK,
    OBTENCION_CONTEXTO_PID_OK,
    CONTEXTO_EJECUCION_INEXISTENTE,
    OBTENCION_CONTEXTO_EJECUCION_OK,
    ACTUALIZACION_OK,

    //INSTRUCCION FINALIZADA
    PEDIR_TAM_MEMORIA,
    TAMANIO_RECIBIDO,
    TERMINACION_PROCESO,
    OBTENER_INSTRUCCION,
    INSTRUCCION_OBTENIDA,
    ESPACIO_USUARIO,
    WRITE_OK,
    OK_OP_CODE,
    OK_TERMINAR_OP_CODE,
    TERMINAR_EJECUCION_MODULO_OP_CODE
}op_code; // USARLO SOLAMENTE CON MEMORIA


typedef struct{
    char* parametros1;
    char* parametros2;
    char* parametros3;
    char* parametros4;
}t_instruccion;

// Guardamos las instrucciones por pid, tid y program counter
typedef struct{
    int pid;
    int tid;
    uint32_t pc;
    t_instruccion*instrucciones;
}t_instruccion_tid_pid;


typedef struct {
    uint32_t entero1;
    uint32_t entero2;
}t_2_enteros;

typedef struct {
    uint32_t entero;
    bool operacion;
}t_entero_bool;

typedef struct {
    uint32_t entero1;
    uint32_t entero2;
    uint32_t entero3;
}t_3_enteros;

typedef struct{
    int pid;
    int tid;
    uint32_t pc;
}t_tid_pid_pc;

typedef struct {
    uint32_t entero1;
    uint32_t entero2;
    uint32_t entero3;
    uint32_t entero4;
}t_4_enteros;

typedef struct {
    char *string;
    uint32_t entero1;
    uint32_t entero2;
}t_string_2enteros;

typedef struct {
    char *string;
    uint32_t entero1;
    int entero2;
}t_string_2enteros_dato_movOut;

typedef struct {
    char *string;
    uint32_t entero1;
    uint32_t entero2;
	uint32_t entero3;
}t_string_3enteros;

typedef struct {
    char *string;
    uint32_t entero1;
}t_string_mas_entero;

typedef struct{
    t_contexto_pid_send*contexto_pid;
    t_contexto_tid*contexto_tid;
}t_contextos;

typedef struct
{
    op_code codigo_operacion;
    t_buffer* buffer;
} t_paquete; // cpu/memoria

int recibir_operacion(int socket_cliente);

void* recibir_buffer(int* size, int socket_cliente);
void recibir_mensaje(int socket_cliente, t_log* loggs);
t_list* recibir_paquete(int socket_cliente);
void* serializar_paquete(t_paquete* paquete, int bytes);
void enviar_mensaje(char* mensaje, int socket_cliente);
void crear_buffer(t_paquete* paquete);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, uint32_t tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void eliminar_paquete(t_paquete* paquete);

void liberar_conexion(int socket_cliente);
t_config* iniciar_config(char *ruta); 



// Serializacion

void agregar_entero_a_paquete(t_paquete *paquete, uint32_t numero);
void agregar_entero_uint8_a_paquete(t_paquete *paquete, uint8_t numero);

void agregar_string_a_paquete(t_paquete *paquete, char* palabra);







void enviar_entero (int conexion, uint32_t numero, int codop);
void enviar_string (int conexion, char* palabra, int codop);

void enviar_instruccion(int conexion, t_instruccion *instruccion_nueva, op_code codop);
void enviar_2_enteros(int conexion, t_2_enteros* enteros, int codop);




void enviar_codop(int conexion, op_code cod_op);


t_paquete* crear_paquete_op(op_code codop);

// Una vez serializado -> recibimos y leemos estas variables

int leer_entero(char *buffer, int * desplazamiento);


char* leer_string(char *buffer, int * desplazamiento);



char* recibir_string(int socket, t_log* loggs);
//t_contexto* recibir_contexto(int socket);
t_instruccion* recepcionar_instruccion(t_paquete*paquete);

int recibir_entero(int socket);









void recibir_2_string_mas_u32(int socket, char** palabra1,char** palabra2, uint32_t* valor1);
void recibir_2_string_mas_3_u32(int socket, char** palabra1,char** palabra2, uint32_t* valor1, uint32_t* valor2, uint32_t* valor3);

//t_contexto *recibir_contexto_para_thread_execute(int socket,uint32_t tid);

//  NUEVAS FUNCIONES POST CHECKPOINT 2


bool esta_tid_en_lista(int tid,t_list*contextos_tids);


void liberar_contexto_tid(t_contexto_pid *contexto_pid,t_contexto_tid*contexto_tid);
void liberar_contexto_pid(t_contexto_pid *contexto_pid);
void liberar_lisa_contextos();

void remover_contexto_pid_lista(t_contexto_pid* contexto);
void remover_contexto_tid_lista(t_contexto_tid*contexto,t_list*lista);
t_contexto_tid* obtener_tid_en_lista(int tid,t_list*contextos_tids);

void agregar_entero_uint32_a_paquete(t_paquete *paquete, uint32_t numero);



op_code recibir_op_code(int socket_cliente);
t_paquete* recibir_paquete_op_code(int socket_cliente);
int leer_entero(char *buffer, int *desplazamiento);
t_contexto_tid* recepcionar_contexto_tid(t_paquete*paquete);
void enviar_tid_pid_op_code(int conexion,t_tid_pid* info, op_code codop);
void solicitar_contexto_ejecucion(int pid, int tid,int conexion);

void send_paquete_op_code(int socket, t_buffer* buffer, op_code code);

void enviar_registros_a_actualizar(int socket_cliente,t_registros_cpu*registros,int pid, int tid);
void enviar_program_counter_a_actualizar(int socket_cliente,int pc,int pid, int tid);


//t_registros_cpu*recepcionar_registros(t_paquete*paquete,void*stream);
void actualizar_contexto(int pid, int tid, t_registros_cpu* reg);
t_instruccion* obtener_instruccion(int tid, int pid,uint32_t pc);



t_tid_pid* recepcionar_solicitud_contexto_tid(t_paquete* paquete);
void send_solicitud_instruccion_memoria(int tid, int pid, uint32_t pc);
t_instruccion_memoria* recepcionar_solicitud_instruccion_memoria(t_paquete* paquete);




void send_read_mem(int tid, int pid,uint32_t direccionFisica, int socket_memoria);
t_read_mem* recepcionar_read_mem(t_paquete* paquete);
void send_valor_read_mem(uint32_t valor, int socket_cliente, op_code code);

void send_write_mem(int tid, int pid,uint32_t direccionFisica, uint32_t valor, int socket_memoria);
t_write_mem* recepcionar_write_mem(t_paquete* paquete);
uint32_t recepcionar_valor_read_mem(t_paquete* paquete);
void send_terminar_ejecucion_op_code(int socket);
t_tid_pid* recepcionar_solicitud_contexto(t_paquete* paquete);
t_contextos*obtener_contextos(int pid, int tid);
void enviar_contexto_ejecucion(t_contextos*contextos,int socket_cliente);
t_contextos* recepcionar_contextos(t_paquete*paquete);


#endif