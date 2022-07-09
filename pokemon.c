#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<sys/stat.h>
#include<string.h>

#define CantJugadores 100

pthread_t hiloJugador[CantJugadores];

char *nombres[] = {"Allison", "Arthur", "Ana", "Alex", "Arlene", "Alberto", "Barry", "Bertha", "Bill", "Bonnie", "Bret", "Beryl", "Chantal", "Cristobal", "Claudette", "Charley", "Cindy", "Chris", "Dean", "Dolly", "Danny", "Danielle", "Dennis", "Debby", "Erin", "Edouard", "Erika", "Earl", "Emily", "Ernesto", "Felix", "Fay", "Fabian", "Frances", "Franklin", "Florence", "Gabielle", "Gustav", "Grace", "Gaston", "Gert", "Gordon", "Humberto", "Hanna", "Henri", "Hermine", "Harvey", "Helene", "Iris", "Isidore", "Isabel", "Ivan", "Irene", "Isaac", "Jerry", "Josephine", "Juan", "Jeanne", "Jose", "Joyce", "Karen", "Kyle", "Kate", "Karl", "Katrina", "Kirk", "Lorenzo", "Lili", "Larry", "Lisa", "Lee", "Leslie", "Michelle", "Marco", "Mindy", "Maria", "Michael", "Noel", "Nana", "Nicholas", "Nicole", "Nate", "Nadine", "Olga", "Omar", "Odette", "Otto", "Ophelia", "Oscar", "Pablo", "Paloma", "Peter", "Paula", "Philippe", "Patty", "Rebekah", "Rene", "Rose", "Richard", "Rita", "Rafael"};
char *pokemones[] = {"Bulbasaur","Ivysaur","Venusaur","Charmander","Charmeleon","Charizard","Squirtle","Wartortle","Blastoise","Caterpie","Metapod","Butterfree","Weedle","Kakuna","Beedrill","Pidgey","Pidgeotto","Pidgeot","Rattata","Raticate","Spearow","Fearow","Ekans","Arbok","Pikachu","Raichu","Sandshrew","Sandslash","Nidoran","Nidorina","Nidoqueen","Nidoran","Nidorino","Nidoking","Clefairy","Clefable","Vulpix","Ninetales","Jigglypuff","Wigglytuff","Zubat","Golbat","Oddish","Gloom","Vileplume","Paras","Parasect","Venonat","Venomoth","Diglett","Dugtrio","Meowth","Persian","Psyduck","Golduck","Mankey","Primeape","Growlithe","Arcanine","Poliwag","Poliwhirl","Poliwrath","Abra","Kadabra","Alakazam","Machop","Machoke","Machamp","Bellsprout","Weepinbell","Victreebel","Tentacool","Tentacruel","Geodude","Graveler","Golem","Ponyta","Rapidash","Slowpoke","Slowbro","Magnemite","Magneton","Farfetch'd","Doduo","Dodrio","Seel","Dewgong","Grimer","Muk","Shellder","Cloyster","Gastly","Haunter","Gengar","Onix","Drowzee","Hypno","Krabby","Kingler","Voltorb","Electrode","Exeggcute","Exeggutor","Cubone","Marowak","Hitmonlee","Hitmonchan","Lickitung","Koffing","Weezing","Rhyhorn","Rhydon","Chansey","Tangela","Kangaskhan","Horsea","Seadra","Goldeen","Seaking","Staryu","Starmie","Mr. Mime","Scyther","Jynx","Electabuzz","Magmar","Pinsir","Tauros","Magikarp","Gyarados","Lapras","Ditto","Eevee","Vaporeon","Jolteon","Flareon","Porygon","Omanyte","Omastar","Kabuto","Kabutops","Aerodactyl","Snorlax","Articuno","Zapdos","Moltres","Dratini","Dragonair","Dragonite","Mewtwo","Mew"};
int aux=0,entrenadoresMuertos=0,raids=0,rep=0;

FILE *archivo;

/* 
Aux: Auxiliar para crear los 20 jugadores que entrarán en la raid. Los otros 80 se almacenan como un número.
entrenadoresMuertos: Contador de entrenadores cuyos 6 pokemon se hayan debilitado.
raids: Contador de raids. No tiene mucha ciencia.
rep: Contador de repeticiones de opción 1. Para que no siempre trabaje con los jugadores del 0 al 19. (explota cuando rep=5 porque son solo 100 jugadores )
*/
sem_t semRaid;

typedef struct pokemon{
    int id;
    const char *nombre;
    int pc;
    int contadorVida;
}poke;

typedef struct jugador{
    pthread_t tid;
    const char *nombre;
    int nivel;
    int estado;
    poke pokeRaid[6];
}pokemaniaco;

typedef struct raid{
    const char *nombre;
    int pc;
    int contadorVida;
    int victoria;
}raid;

raid pokemonRaid[20];
pokemaniaco jugadorRaid[1000];

void *ingresarJugador(){
    int random = rand() %100;
    pthread_t tid;
	tid = pthread_self();
    sem_wait(&semRaid);
    printf("Entrando en zona critica %ld. AUX = %d\n",tid,aux);
    jugadorRaid[aux].nombre = nombres[random];
    jugadorRaid[aux].tid = tid;
    jugadorRaid[aux].nivel = rand() %30 +10;
    jugadorRaid[aux].estado = 1;
    aux++;
    printf("Saliendo de zona critica %ld. AUX = %d\n",tid,aux);
    sem_post(&semRaid);
}

void *escogerPoke(){
    int idPoke=0;
    int i = rep*20;
    for(i; i<aux;i++){
        fprintf(archivo,"\n\n\nDATOS DEL JUGADOR %d\n\nID: %ld\nNombre: %s\nNivel:%d\n\nDATOS DE SUS POKEMON:\n",i,jugadorRaid[i].tid,jugadorRaid[i].nombre,jugadorRaid[i].nivel);
        printf("\n\n\nDATOS DEL JUGADOR %d\n\nID: %ld\nNombre: %s\nNivel:%d\n\nDATOS DE SUS POKEMON:\n",i,jugadorRaid[i].tid,jugadorRaid[i].nombre,jugadorRaid[i].nivel);
        for(int j=0; j<6; j++){
            fprintf(archivo,"\nPOKEMON %d:",j);
            printf("\nPOKEMON %d:",j);
            int random = rand() %150;
            jugadorRaid[i].pokeRaid[j].nombre = pokemones[random];
            jugadorRaid[i].pokeRaid[j].id = idPoke;
            jugadorRaid[i].pokeRaid[j].pc = rand() %3200 +1000;
            jugadorRaid[i].pokeRaid[j].contadorVida = jugadorRaid[i].pokeRaid[j].pc/10;
            fprintf(archivo,"\nNombre: %s\nId: %d\nPC: %d\n",jugadorRaid[i].pokeRaid[j].nombre,jugadorRaid[i].pokeRaid[j].id,jugadorRaid[i].pokeRaid[j].pc);
            printf("\nNombre: %s\nId: %d\nPC: %d\n",jugadorRaid[i].pokeRaid[j].nombre,jugadorRaid[i].pokeRaid[j].id,jugadorRaid[i].pokeRaid[j].pc);
            idPoke++;
        }
    }
}

int tatekieto(int a,int b){
    if(a == 20){
        int jugadorGolpiao=0;
        int tatekietoSupremo = pokemonRaid[b].pc /1000;
        do{
            for(int i=0;i<6;i++){
                if(jugadorRaid[jugadorGolpiao].pokeRaid[i].contadorVida>0){
                    jugadorRaid[jugadorGolpiao].pokeRaid[i].contadorVida = jugadorRaid[jugadorGolpiao].pokeRaid[i].contadorVida - tatekietoSupremo;
                    if(jugadorRaid[jugadorGolpiao].pokeRaid[i].contadorVida<=0){
                        jugadorRaid[jugadorGolpiao].pokeRaid[i].contadorVida=0;
                        if(i==5){
                            jugadorRaid[jugadorGolpiao].estado = 0; //el jugador murió
                            entrenadoresMuertos++;
                        }
                    }
                    fprintf(archivo,">>%s ATACA A %s DEL JUGADOR %d: -%d. VIDA RESTANTE: %d\n",pokemonRaid[b].nombre,jugadorRaid[jugadorGolpiao].pokeRaid[i].nombre,jugadorGolpiao, tatekietoSupremo, jugadorRaid[jugadorGolpiao].pokeRaid[i].contadorVida);
                    printf(">>%s ATACA A %s DEL JUGADOR %d: -%d. VIDA RESTANTE: %d\n",pokemonRaid[b].nombre,jugadorRaid[jugadorGolpiao].pokeRaid[i].nombre,jugadorGolpiao, tatekietoSupremo, jugadorRaid[jugadorGolpiao].pokeRaid[i].contadorVida);
                    if(jugadorRaid[jugadorGolpiao].pokeRaid[i].contadorVida==0){
                        fprintf(archivo,"El pokemon %s se ha debilitado! El entrenador cambia a %s por %s.\n",jugadorRaid[jugadorGolpiao].pokeRaid[i].nombre,jugadorRaid[jugadorGolpiao].pokeRaid[i].nombre,jugadorRaid[jugadorGolpiao].pokeRaid[i+1].nombre);
                        printf("El pokemon %s se ha debilitado! El entrenador cambia a %s por %s.\n",jugadorRaid[jugadorGolpiao].pokeRaid[i].nombre,jugadorRaid[jugadorGolpiao].pokeRaid[i].nombre,jugadorRaid[jugadorGolpiao].pokeRaid[i+1].nombre);
                    }
                    break;
                }
            }
            jugadorGolpiao++;
        }while(jugadorGolpiao!=20);
    }else{
        for(int i=0;i<6;i++){
            if(jugadorRaid[a].pokeRaid[i].contadorVida>0){
                int tatekieto = jugadorRaid[a].pokeRaid[i].pc /10;
                pokemonRaid[b].contadorVida = pokemonRaid[b].contadorVida - tatekieto;
                if(pokemonRaid[b].contadorVida<0){
                    pokemonRaid[b].contadorVida=0;
                }
                fprintf(archivo,"Jugador %d Ataca con %s.\n%s -%d. Vida restante: %d\n",a,jugadorRaid[a].pokeRaid[i].nombre, pokemonRaid[b].nombre, tatekieto, pokemonRaid[b].contadorVida);
                printf("Jugador %d Ataca con %s.\n%s -%d. Vida restante: %d\n",a,jugadorRaid[a].pokeRaid[i].nombre, pokemonRaid[b].nombre, tatekieto, pokemonRaid[b].contadorVida);
                break;
            }
        }
    }
}

int main(){
    
    srand(getpid());
    int menu = 0,i;
    archivo = fopen("archivo","wt");
    if (archivo == NULL) {
        printf("Error abriendo archivo");
        return 1;
    }
    do{
        fprintf(archivo,"====>BIENVENIDO A POKEMONGO<====\nSeleccione una opción.\n1.-Iniciar Incursión Aleatoria.\n2.-Iniciar Incursion ingresando todos los datos.\n3.-Acceder al registro histórico.\n4.-Salir.\n");
        printf("====>BIENVENIDO A POKEMONGO<====\nSeleccione una opción.\n1.-Iniciar Incursión Aleatoria.\n2.-Iniciar Incursion ingresando todos los datos.\n3.-Acceder al registro histórico.\n4.-Salir.\n");
        scanf("%d",&menu);
        if(menu==1){
            int turno=0,turnoJugador=0,confirmacion;
            sem_t semTatekieto;
            sem_init(&semTatekieto, 0, 1);
            sem_init(&semRaid, 0, 1);
            for(i=rep*20;i<(20+20*rep);i++){
                confirmacion = pthread_create(&hiloJugador[i], NULL, &ingresarJugador, NULL);
		        if(confirmacion != 0){
			        printf("ERROR: %d, al lanzar el hilo\n", confirmacion);

                }
            }
            for(i=rep*20;i<(20+20*rep);i++){
                pthread_join(hiloJugador[i], NULL);
            }
            turnoJugador=0;
            pthread_t elegirPokemon;
            pthread_create(&elegirPokemon, NULL, escogerPoke, NULL);
            pthread_join(elegirPokemon, NULL);
            //Comienzo Batalla
            int random = rand() %150;
            pokemonRaid[raids].nombre = pokemones[random];
            pokemonRaid[raids].pc = rand() %100000 +100000;
            pokemonRaid[raids].contadorVida = pokemonRaid[raids].pc/10;
            do{
        
                if(turnoJugador>20){
                    turnoJugador=0;
                }
                if(turnoJugador==20){
                    fprintf(archivo,"<<<<Turno %d: TURNO DEL POKEMON PULENTO, AGARRENSE LOS CALCETINES>>>>\n",turno);
                    printf("<<<<Turno %d: TURNO DEL POKEMON PULENTO, AGARRENSE LOS CALCETINES>>>>\n",turno);
                }else{
                    fprintf(archivo,"<<<<Turno %d: Turno del jugador %d>>>>\n",turno,turnoJugador);
                    printf("<<<<Turno %d: Turno del jugador %d>>>>\n",turno,turnoJugador);
                    if(jugadorRaid[turnoJugador].estado==0){
                        fprintf(archivo,"Pero este jugador está muerto!\n");
                        printf("Pero este jugador está muerto!\n");
                    }
                }
                sem_wait(&semTatekieto);
                tatekieto(turnoJugador,raids);
                sem_post(&semTatekieto);
                turno++;
                turnoJugador++;
            }while(pokemonRaid[raids].contadorVida>0 && entrenadoresMuertos<20);
            if(entrenadoresMuertos==20){
                fprintf(archivo,"Todos los pokemon de los entrenadores se han debilitado. Han perdido esta incursión.\n");
                printf("Todos los pokemon de los entrenadores se han debilitado. Han perdido esta incursión.\n");
                pokemonRaid[raids].victoria = 0;
            }else if(pokemonRaid[raids].contadorVida==0){
                fprintf(archivo,"Felicidades! Han logrado completar la incursión.\n");
                printf("Felicidades! Han logrado completar la incursión.\n");
                pokemonRaid[raids].victoria = 1;
            }
            raids++;
            rep++;
            menu=0;
        }
        if(menu==2){
            fprintf(archivo,"Lo sentimos, esta función está en mantenimiento.\n");
            printf("Lo sentimos, esta función está en mantenimiento.\n");
            menu=0;
        }
        if(menu==3){
            do{
                fprintf(archivo,"Las últimas incursiones han sido: \n");
                printf("Las últimas incursiones han sido: \n");
                for(i=0;i<raids;i++){
                    fprintf(archivo,"Incursion %d\nNOMBRE DEL POKEMON: %s\nPC: %d\n",i+1,pokemonRaid[i].nombre,pokemonRaid[i].pc);
                    printf("Incursion %d\nNOMBRE DEL POKEMON: %s\nPC: %d\n",i+1,pokemonRaid[i].nombre,pokemonRaid[i].pc);
                    if(pokemonRaid[i].victoria==1){
                        fprintf(archivo,"Incursion %d\nNOMBRE DEL POKEMON: %s\nPC: %d\n",i+1,pokemonRaid[i].nombre,pokemonRaid[i].pc);
                        printf("Incursión ganada por los entrenadores.\n\n");
                    }else{
                        fprintf(archivo,"Incursion ganada por el pokemon.\n\n");
                        printf("Incursion ganada por el pokemon.\n\n");
                    }
                }
                fprintf(archivo,"Desea consultar por alguna en específico?\nIngrese el numero de la incursion\n0 para volver al menu principal\n");
                printf("Desea consultar por alguna en específico?\nIngrese el numero de la incursion\n0 para volver al menu principal\n");
                scanf("%d",&menu);
                if(menu==0){
                    break;
                }
                fprintf(archivo,"Los entrenadores de esta raid fueron:\n");
                printf("Los entrenadores de esta raid fueron:\n");
                menu--;
                int xdd = menu*20;
                int lim = xdd+20;
                for(xdd;xdd<lim;xdd++){
                    fprintf(archivo,"<<<ID: %ld >>> %s\nSus pokemon fueron:\n",jugadorRaid[xdd].tid, jugadorRaid[xdd].nombre);
                    printf("<<<ID: %ld >>> %s\nSus pokemon fueron:\n",jugadorRaid[xdd].tid, jugadorRaid[xdd].nombre);
                    for(int chiCheniol = 0; chiCheniol<6; chiCheniol++){
                        fprintf(archivo,"%s\nID: %d\nPC: %d\n",jugadorRaid[xdd].pokeRaid[chiCheniol].nombre,jugadorRaid[xdd].pokeRaid[chiCheniol].id,jugadorRaid[xdd].pokeRaid[chiCheniol].pc);
                        printf("%s\nID: %d\nPC: %d\n",jugadorRaid[xdd].pokeRaid[chiCheniol].nombre,jugadorRaid[xdd].pokeRaid[chiCheniol].id,jugadorRaid[xdd].pokeRaid[chiCheniol].pc);
                    }
                }
                menu=0;
            }while(menu!=0);
        }
    }while(menu!=4);
    fprintf(archivo,"Adiós!");
    printf("Adiós!");
    fclose(archivo);
}