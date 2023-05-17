#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <ncurses.h>

int main(void) {
    char cadena[100];
    int sockfd;
    struct sockaddr_in servaddr;
    FILE *myf = fopen("conversacion_cliente.txt", "a");
    time_t t;
    struct tm *tm;
    char hora[100];
    char *tmp;
    char sendline[100] = "Usando el puerto 2200 \n";

    initscr(); // Inicializar la pantalla de ncurses
    clear(); // Limpiar la pantalla
    noecho(); // No mostrar las teclas pulsadas en la pantalla

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(22000);

    inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    printw("\n\n\t\t----Se inicio el chat-----\n\n");
    fprintf(myf, "\n\n\t\t----Se inicio el chat-----\n\n");

    refresh(); // Actualizar la pantalla

    while(!strstr(cadena, "adios") && !strstr(sendline, "adios")) {
        bzero(cadena, 100);
        t = time(NULL);
        tm = localtime(&t);
        strftime(hora, 100, "\n yo (%H:%M) -> ", tm);
        printw("%s", hora);
        refresh();

        echo(); // Mostrar las teclas pulsadas en la pantalla
        getstr(sendline);
        noecho(); // No mostrar las teclas pulsadas en la pantalla nuevamente

        tmp = strcat(hora, sendline);
        fprintf(myf, "%s", tmp);
        write(sockfd, sendline, strlen(sendline) + 1);
        refresh();

        if (!strstr(cadena, "adios")) {
            strftime(hora, 100, "\n otro usuario (%H:%M) -> ", tm);
            read(sockfd, cadena, 100);
            tmp = strcat(hora, cadena);
            printw("%s", hora);
            fprintf(myf, "%s", tmp);
            refresh();
        }
    }

    printw("\n\n Conversacion Finalizada \n");
    printw("\n Se genero el archivo -> conversacion_cliente.txt");
    fprintf(myf, "\n\n Conversacion Finalizada \n");
    fclose(myf);

    refresh();
    getch(); // Esperar a que el usuario presione una tecla para salir
    endwin(); // Finalizar el modo ncurses

    return 0;
}