#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")
#define PORT 80
#define MESSAGESIZE 1024
#define MAXSOCKETS 10

SOCKET new_socket[MAXSOCKETS];
int qtdsockets = 0;
char *ft[80];

void getDate(char *ft)
{
    time_t rawT;
    struct tm *info;
    time(&rawT);
    info = gmtime(&rawT);
    strftime(ft, 80, "%a, %d %b %Y %X GMT", info);
}

char *mountHeader(int isFallback, long htmlLength){
    char *header;

    header = (char *)malloc(MESSAGESIZE);

    getDate(ft);

    if(isFallback){
        sprintf(header,"HTTP/1.1 404 Not Found\r\nDate: %s\r\nContent-Length: %d\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n",ft,htmlLength);
    }else {
        sprintf(header,"HTTP/1.1 200 OK\r\nDate: %s\r\nContent-Length: %d\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n",ft,htmlLength);
    }

    return header;
}

long getFileLength(FILE *html){
    long htmlLength;

    fseek(html,0,SEEK_END);
    htmlLength = ftell(html);
    fseek(html,0,SEEK_SET);

    return htmlLength;
}


char *mountPage(long length, FILE *html){
    char *htmlRes = (char *)malloc(length + 1);
    fread(htmlRes,1,length,html);
    htmlRes[length] = '\0';
    return htmlRes;
}

char *makeResponse(char *htmlResponse, char *header, long htmlLength){
    char *response = (char *)malloc(htmlLength + strlen(header) + 1);
    strcat(response, header);
    strcat(response, htmlResponse);

    return response;
}

char *processResponse(char *htmlRequest){ // DEU CERTO
    FILE *htmlFile = fopen(htmlRequest,"r");
    char *header;
    char *htmlResponse;
    char *response;
    int isFallback = 0;
    long htmlLength;

    if(htmlFile == NULL){
        htmlFile = fopen("fallback.html","r");
        isFallback = 1;
    }else {
        htmlFile = fopen(htmlRequest,"r");
    }

    htmlLength = getFileLength(htmlFile);

    header = mountHeader(isFallback, htmlLength);

    htmlResponse = mountPage(htmlLength, htmlFile);

    response = makeResponse(htmlResponse, header, htmlLength);

    fclose(htmlFile);
    return response;
}

char *processRequest(char *request)
{
    char path[MESSAGESIZE];
    char *response;

    sscanf(request, "GET /%s ", path);
    if(strcmp(path,"HTTP/1.1") == 0){ // QUANDO VAZIO, RECEBE HTTP/1.1
        response = processResponse("index.html");
    }else {
        strcat(path,".html");
        response = processResponse(path);
    }
    return response;
}



void getdata(int pos)
{
    int len;
    char *ft[80];
    char request[MESSAGESIZE];
    char header[MESSAGESIZE];
    char page[MESSAGESIZE];

    while(1)
    {
        request[0]='\0';
        len=recv(new_socket[pos],request,MESSAGESIZE,0);
        if(len>0)
        {
            len=0;

            char *response = processRequest(request);
            send(new_socket[pos], response, strlen(response)+1, 0);
            closesocket(new_socket[pos]);
            qtdsockets--;
        }
    }
}

int main(int argc, char *argv[])
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server, client;
    int c, pos;
    char errormessage[MESSAGESIZE];

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("\nFalha na inicializacao da biblioteca Winsock: %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    if((s = socket(AF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET)
    {
        printf("\nNao e possivel inicializar o socket: %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );
    if( bind(s,(struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("\nNao e possivel construir o socket: %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    listen(s,3);
    c = sizeof(struct sockaddr_in);
    printf("*** SERVER ***\n\nAguardando conexoes...\n\n");

    while(1)
    {
        pos=qtdsockets;
        new_socket[qtdsockets++] = accept(s, (struct sockaddr *)&client, &c);
        if (new_socket[pos] == INVALID_SOCKET)
        {
            printf("\nConexao nao aceita. Codigo de erro: %d", WSAGetLastError());
        }
        else
        {
            puts("\nConexao aceita.");
            printf("\nDados do cliente - IP: %s -  Porta: %d\n",inet_ntoa(client.sin_addr),htons(client.sin_port));

            if (qtdsockets <= MAXSOCKETS)
            {
                _beginthread(getdata,NULL,pos);
            }
            else
            {
                puts("\nNumero maximo de conexoes excedido!");
                strcpy(errormessage,"HTTP/1.1 429\r\nToo Many Requests\r\nContent-Type: text/html\r\nRetry-After: 60\r\n\r\n<HTML><H1>Numero maximo de conexoes excedido!</H1></HTML>\r\n");
                send(new_socket[pos], errormessage, strlen(errormessage)+1, 0);
                closesocket(new_socket[pos]);
                qtdsockets--;
            }
        }
    }
    closesocket(s);
    WSACleanup();
}
