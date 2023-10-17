#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include "cJSON.c"
#include "cJSON.h"
#define MAX 255
#define PORT 12345
#define SA struct sockaddr
int client_socket, server_socket;
struct sockaddr_in server,client;
char buffer[1024];
char temp[128];
char authbuffer[64];


//data structures
typedef struct {
  char tok[64];
  char unme[64];
  int seenmessages;
  int haschannel;
  char channelname[64];
} nuser;


nuser users[32];
int usercounter=0;





//functions
void clconnect(void);
void listentoclient(void);
void createauth(void);
void sregisteruser(void);
void slogin(void);
void screatechannel(void);
void sjoin(void);
void ssend(void);
void srefresh(void);
void schannelmembers(void);
void sleave(void);
void slogout(void);
void existerror(char[]);
void sssmessage(void);
void justlistennigger(void);




//main function
int main(){
  clconnect();
  listentoclient();
  //printf ("Have a Nice Day Kasra!");
  return 0;
}




void clconnect(){
  // Create and verify socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    printf("Socket creation failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully created..\n");

  // Assign IP and port
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(PORT);

  // Bind newly created socket to given IP and verify
  if ((bind(server_socket, (SA*)&server, sizeof(server))) != 0) {
    printf("Socket binding failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully bound..\n");

  // Now server is ready to listen and verify
  /*if ((listen(server_socket, 5)) != 0) {
    printf("Listen failed...\n");
    exit(0);
  }
  else
    printf("Server listening..\n");

  // Accept the data packet from client and verify
  socklen_t len = sizeof(client);
  client_socket = accept(server_socket, (SA*)&client, &len);
  if (client_socket < 0) {
    printf("Server accceptance failed...\n");
    exit(0);
  }
  else
    printf("Server acccepted the client..\n");*/
}






void listentoclient(){
  //clconnect();
  memset(buffer,0,sizeof(buffer));
  justlistennigger();
  //clconnect();
  recv(client_socket,buffer,sizeof(buffer),0);
  //shutdown(server_socket,SHUT_RDWR);
  //close(server_socket);
  printf ("%s",buffer);
  sscanf(buffer,"%s",temp);
  if (!strcmp(temp,"register")){
    //sssmessage();
    sregisteruser();
  }
  if (!strcmp(temp,"login")){
    slogin();
  }
  if (!strcmp(temp,"join")){
    sjoin();
  }
  if (!strcmp(temp,"create")){
    screatechannel();
  }
  if (!strcmp(temp,"send")){
    ssend();
  }
  if (!strcmp(temp,"refresh")){
    srefresh();
  }
  if (!strcmp(temp,"channel")){
    schannelmembers();
  }
  if (!strcmp(temp,"leave")){
    sleave();
  }
  if (!strcmp(temp,"logout")){
    slogout();
  }
  //createauth();
  //printf ("%s\n",authbuffer);
  //char* ss="hey there!";
  //send(client_socket,ss,sizeof(ss),0);
  //printf ("%s\n",buffer);
  //shutdown(server_socket, SHUT_RDWR);
  //sscanf(buffer,"%s",temp);
  //memset(buffer,0,sizeof(buffer));
  //recv(client_socket,buffer,sizeof(buffer),0);
  //shutdown(server_socket, SHUT_RDWR);
  //if (!strcmp(temp,"register")){
  //    printf ("Henlo There!");
  //}
}






void createauth(){
  memset(authbuffer,0,sizeof(authbuffer));
  srand(time(0));
  for (int i=0;i<32;i++){
    //srand(time(0));
    int num=(rand()%59+64);
    //printf ("%d\t",num);
    char b=num;
    //printf ("%c\n",b);
    authbuffer[i]=b;
  }
  authbuffer[32]=0;
}




void sregisteruser(){
  char susername[64];
  char spassword[64];
  sscanf (buffer,"%*s %s %s",susername,spassword);
  for (int i=0;;i++){
    if (susername[i]==','){
      susername[i]='\0';
      break;
    }
  }
  FILE *us;
  char filename[256];
  strcpy (filename,"database/members/");
  strcat (filename,susername);
  strcat (filename,".text");
  //us=fopen(filename,"w+");
  if (!(us=fopen(filename,"r"))){
    //printf ("Gotcha");
    sssmessage();
    fclose(us);
    us=fopen(filename,"w");
    cJSON *user = cJSON_CreateObject();
    cJSON *usernm=NULL;
    cJSON *passwrd=NULL;
    usernm = cJSON_CreateString(susername);
    passwrd = cJSON_CreateString(spassword);
    cJSON_AddItemToObject(user,"username",usernm);
    cJSON_AddItemToObject(user,"password",passwrd);
    char *hey=NULL;
    hey=cJSON_Print(user);
    //printf ("%s",hey);
    cJSON_Delete(user);
    fprintf(us,"%s",hey);
    fclose(us);
    //sssmessage();
  }
  else {
    existerror("user already exists");
  }
  //existerror();
  listentoclient();
}







void slogin(){
  char susername[64];
  char spassword[64];
  char filebuffer[1024];
  sscanf (buffer,"%*s %s %s",susername,spassword);
  for (int i=0;;i++){
    if (susername[i]==','){
      susername[i]='\0';
      break;
    }
  }
  FILE *us;
  char filename[256];
  strcpy (filename,"database/members/");
  strcat (filename,susername);
  strcat (filename,".text");
  if (us=fopen(filename,"r")){
    char line[100];
  while ( fgets( line, 100, us ) != NULL )
    {
      //printf("The line is: %s\n", line);
      strcat(filebuffer,line);
    }
    fclose(us);
    cJSON *user=cJSON_Parse(filebuffer);
    cJSON *username=NULL;
    cJSON *password=NULL;
    username=cJSON_GetObjectItemCaseSensitive(user,"username");
    password=cJSON_GetObjectItemCaseSensitive(user,"password");
    //if (strcmp())
  }
}








void sjoin(){

}






void screatechannel(){

}







void ssend(){

}





void srefresh(){

}






void schannelmembers(){

}






void sleave(){

}






void slogout(){

}




void existerror(char u[]){
  cJSON *message=cJSON_CreateObject();
  cJSON *type=NULL;
  cJSON *content=NULL;
  type =cJSON_CreateString("Error");
  content =cJSON_CreateString(u);
  cJSON_AddItemToObject(message,"Type",type);
  cJSON_AddItemToObject(message,"content",content);
  char *hey=NULL;
  hey=cJSON_Print(message);
  cJSON_Delete(message);
  //clconnect();
  send (client_socket,hey,sizeof(hey),0);
  //shutdown(server_socket,2);
}





void sssmessage(){
  cJSON *message=cJSON_CreateObject();
  cJSON *type=NULL;
  cJSON *content=NULL;
  type =cJSON_CreateString("Successful");
  content =cJSON_CreateString("");
  cJSON_AddItemToObject(message,"Type",type);
  cJSON_AddItemToObject(message,"content",content);
  char *hey=NULL;
  hey=cJSON_Print(message);
  cJSON_Delete(message);
  printf ("%s\n",hey);
  //clconnect();
  send (client_socket,hey,sizeof(hey),0);
  close (client_socket);
  //shutdown (server_socket,SHUT_RDWR);
}





void justlistennigger(){
  if ((listen(server_socket, 5)) != 0) {
    printf("Listen failed...\n");
    exit(0);
  }
  else
    printf("Server listening..\n");

  // Accept the data packet from client and verify
  socklen_t len = sizeof(client);
  client_socket = accept(server_socket, (SA*)&client, &len);
  if (client_socket < 0) {
    printf("Server accceptance failed...\n");
    exit(0);
  }
  else
    printf("Server acccepted the client..\n");
}
