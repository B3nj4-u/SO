/* 
_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
_/_/              Made in             _/_/
_/_/         Pandabundo's brain       _/_/
_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
*Este código simula muy bien la realidad y a veces algunos clientes vuelven a las cajas por segunda vez a alegar con el cajero*
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include <semaphore.h>

#define CantCajas 4;

pthread_t hiloCliente[20];
pthread_t hiloFuncionario;

typedef struct atencion{
    int nombre;
    int estado;
    pthread_t cliente;
}cajas;

typedef struct afiliado{
    pthread_t id;
    int estado;
}cliente;

int posicion=0,cajasLlenas=0,clientesAtendidos,repetir;
cajas caja[4];
cliente cola[20];


sem_t semIngreso;
sem_t semCola;

void *cajaDisponible(void* arg){
    long id = (long) arg;
    cajasLlenas=0;
    int p=0,b,creoQueYaLoSume=0;
    int pos = (int) id;
    sem_wait(&semIngreso);
    for(p=0;p<4;p++){
        if(cola[pos].estado==1){
            if(caja[p].estado==1){
                printf("Funcionario: La caja %d está ocupada.\n",p);
                cajasLlenas++;
            }else{
                printf("Funcionario: La caja %d está disponible\n",p);
                caja[p].estado = 1;
                caja[p].cliente = cola[pos].id;
                printf("El cliente %d ha ingresado en la caja %d.\n",pos,p);
                sleep(5);
                printf("El cliente %d ha dejado la caja %d\n",pos,p);
                clientesAtendidos--;
                caja[p].estado = 0;
                cola[pos].estado = 2;
                break;
            }
        }
    }
    if(cajasLlenas==4){
        printf("Funcionario: Todas las cajas están llenas. El cliente ha sido agregado a la cola.\n");
    }
    /*if(zonaAcopio[0][0]>5){
        zonaAcopio[0][0]--;
        for(p=0;p<5;p++){
            for(b=0;b<5;b++){
                if(zonaAcopio[p][b]<5){
                    creoQueYaLoSume++;
                    zonaAcopio[p][b]++;
                    printf("HILO CONTROLADOR %ld <%ld> : El container fue almacenado en el sector (%d,%d). Capacidad restante: %d\n\n",id, tid,p,b,5-zonaAcopio[p][b]);
                    break;
                }
            }
            if(creoQueYaLoSume != 0){
                break;
            }
        }
    }else{
        printf("HILO CONTROLADOR %ld <%ld> : El container fue almacenado en el sector (0,0). Capacidad restante: %d\n\n",id, tid,5-zonaAcopio[0][0]);
    }*/
    sem_post(&semIngreso);
}

void *solicitarCaja(){
	pthread_t tid;
    int oo;
	tid = pthread_self();
    sem_wait(&semCola);
    for(oo=0;oo<repetir;oo++){
        if(cola[oo].estado==0){
            cola[oo].estado=1;
            cola[oo].id=tid;
            break;
        }
    }
	printf("FUNCIONARIO: Ha ingresado un cliente a la sucursal. Posicion en la cola: (%d/%d). ID: %ld\n\n",oo,oo,tid);
    posicion++;
    sem_post(&semCola);
}


int main(){
    long i;
    sem_init(&semIngreso, 0, 4);
    sem_init(&semCola,0,1);
    int confirmacion;
    for(int CHIMBARONGO = 0; CHIMBARONGO<4;CHIMBARONGO++){
       caja[CHIMBARONGO].nombre = CHIMBARONGO;
       caja[CHIMBARONGO].estado = 0;
    }
    for(int platano = 0; platano<20; platano++){
        cola[platano].estado=0;
        cola[platano].id=0;
    }
    printf("Cuántos clientes desea ingresar? Max 20\n");
    scanf("%d",&repetir);
    clientesAtendidos = repetir;
    for(i=0;i<repetir;i++){
        confirmacion = pthread_create(&hiloCliente[i], NULL, &solicitarCaja, NULL);
		if(confirmacion != 0){
			printf("ERROR: %d, al lanzar el hilo\n", confirmacion);
        }
    }
    for(i=0;i<repetir;i++){
		pthread_join(hiloCliente[i], NULL);
    }
    do{
        for(i=0;i<repetir;i++){
            confirmacion = pthread_create(&hiloFuncionario, NULL, &cajaDisponible, (void*)i);
            if(confirmacion != 0){
	            printf("ERROR: %d, al lanzar el hilo\n", confirmacion);
                exit(-1);
            }
        }
        for(i=0;i<repetir;i++){
            pthread_join(hiloFuncionario, NULL);
        }
    }while(clientesAtendidos>1);
    return 0;
}