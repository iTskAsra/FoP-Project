#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "cJSON.c"
#include "cJSON.h"
#define MAX 255
#define PORT 12345
#define SA struct sockaddr
char authtoken[1024];
int client_socket, server_socket;
struct sockaddr_in servaddr, cli;


void mainmenu(void);
void accountmenu(void);
void serverconnect(void);
void loginuser(void);
void registeruser(void);
void chatmenu(void);
void sendmessage(void);
void refresh(void);
void channelmembers(void);
void leavechannel(void);
void creatchannel(void);
void joinchannel(void);
void logout(void);


int main(){
  system("clear");
  accountmenu();
}




void accountmenu(){
  printf ("Account Menu:\n");
  printf ("1: Register\n");
  printf ("2: Login\n");
  int a;
  scanf ("%d",&a);
  system("clear");
  if (a==1){
    registeruser();
  }
  else if (a==2){
    loginuser();
  }
}


void loginuser(){
  char username[100];
  char servermessage[1024];
  char password[100];
  printf ("Please Enter Your Username:\n");
  scanf ("%s",username);
  system("clear");
  printf ("Please Enter Your Password:\n");
  scanf("%s",password);
  system("clear");
  char temp[1000]="login ";
  strcat(temp,username);
  strcat(temp,", ");
  strcat(temp,password);
  strcat(temp,"\n");
	serverconnect();
  //printf ("%s",temp);
  memset(servermessage,0,sizeof(servermessage));
  send(client_socket,temp,sizeof(temp),0);
  memset(temp,0,sizeof(temp));
  //read(client_socket, servermessage, sizeof(servermessage));
  recv(client_socket,servermessage,sizeof(servermessage),0);
  cJSON *recieved_message = cJSON_Parse(servermessage);
  const cJSON *type=NULL;
  const cJSON *content=NULL;
  type = cJSON_GetObjectItemCaseSensitive(recieved_message, "type");
  content = cJSON_GetObjectItemCaseSensitive(recieved_message, "content");
  if (!strcmp("Error",type->valuestring)){
    printf ("%s\n",content->valuestring);
    accountmenu();
  }
  strcpy (authtoken,content->valuestring);
  shutdown(client_socket,2);
	mainmenu();
}



void registeruser(){
  char username[100];
  char password[100];
  char servermessage[1024];
  printf ("Please Enter Your Username:\n");
  scanf ("%s",username);
  system("clear");
  printf ("Please Enter Your Password:\n");
  scanf("%s",password);
  system("clear");
	serverconnect();
  char temp[1000]="register ";
  strcat(temp,username);
  strcat(temp,", ");
  strcat(temp,password);
  strcat(temp,"\n");
  //printf ("Hey");
  send(client_socket,temp,sizeof(temp),0);
  //sleep (2);
  memset(servermessage, 0, sizeof(servermessage));
	recv(client_socket,servermessage,sizeof(servermessage),0);
  //printf ("Hey there ");
  //printf ("%s",servermessage);
  cJSON *recieved_message = cJSON_Parse(servermessage);
  const cJSON *type=NULL;
  const cJSON *content=NULL;
  type = cJSON_GetObjectItemCaseSensitive(recieved_message, "type");
  content = cJSON_GetObjectItemCaseSensitive(recieved_message, "content");
  if (!strcmp("Error",type->valuestring))
    printf ("%s\n",content->valuestring);
  shutdown(client_socket,2);
	accountmenu();
}





void serverconnect(){
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
 if (client_socket == -1) {
    printf("Socket creation failed...\n");
    exit(0); }
memset(&servaddr, 0, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
servaddr.sin_port = htons(PORT);
if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
printf("Connection to the server failed...\n");
exit(0);
}
}



void mainmenu(){
	printf ("1: Create New Channel\n");
	printf ("2: Join Channel\n");
	printf ("3: Logout\n");
  int a;
	scanf ("%d",&a);
  system ("clear");
  switch(a){
    case 1:
      creatchannel();
      break;
    case 2:
      joinchannel();
      break;
    case 3:
      logout();
      break;
  }
}




void chatmenu(){
  printf ("1. Send Message\n");
  printf ("2. Refresh\n");
  printf ("3. Channel Members\n");
  printf ("4. Leave Channel\n");
  int a;
  scanf (" %d",&a);
  system("clear");
  switch (a){
    case 1:
      sendmessage();
      chatmenu();
    case 2:
      refresh();
      chatmenu();
    case 3:
      channelmembers();
      chatmenu();
    case 4:
      leavechannel();
      mainmenu();
  }
}





void sendmessage(){
  char temp[1024]="send ";
  char servermessage[1024];
  char messagee[100];
  printf ("Please Enter Your Message:\n");
  int i;
  char s[1];
  scanf ("%c",s);
  for (i=0;;i++){
    scanf ("%c",s);
    if (s[0]!='\n'){
      messagee[i]=s[0];
    }
    else
      break;
  }
  messagee[i]='\0';
  system("clear");
  strcat(temp,messagee);
  strcat (temp,", ");
  strcat (temp,authtoken);
  strcat (temp,"\n");
  //printf ("%s",temp);
  serverconnect();
  send(client_socket,temp,sizeof(temp),0);
  memset(servermessage,0,sizeof(servermessage));
  recv(client_socket,servermessage,sizeof(servermessage),0);
  cJSON *recieved_message = cJSON_Parse(servermessage);
  const cJSON *type=NULL;
  const cJSON *content=NULL;
  type = cJSON_GetObjectItemCaseSensitive(recieved_message, "type");
  content = cJSON_GetObjectItemCaseSensitive(recieved_message, "content");
  //printf ("%s\n",content->valuestring);
  shutdown(client_socket,2);
  chatmenu();
}



void leavechannel(){
  char temp[1024]="leave ";
  char servermessage[1024];
  char chname[100];
  strcat(temp,authtoken);
  strcat(temp,"\n");
  serverconnect();
  send (client_socket,temp,sizeof(temp),0);
  recv(client_socket,servermessage,sizeof(servermessage),0);
  cJSON *recieved_message = cJSON_Parse(servermessage);
  const cJSON *type=NULL;
  const cJSON *content=NULL;
  type = cJSON_GetObjectItemCaseSensitive(recieved_message, "type");
  content = cJSON_GetObjectItemCaseSensitive(recieved_message, "content");
  //printf ("%s\n",content->valuestring);
  shutdown(client_socket,2);
  mainmenu();
}


void refresh(){
  char temp[1024]="refresh ";
  char chname[100];
  strcat(temp,authtoken);
  char servermessage[1024];
  strcat(temp,"\n");
  serverconnect();
  send (client_socket,temp,sizeof(temp),0);
  memset(servermessage,0,sizeof(servermessage));
  recv(client_socket,servermessage,sizeof(servermessage),0);
  cJSON *recieved_message = cJSON_Parse(servermessage);
  cJSON *messages=NULL;
  cJSON *message=NULL;
  const cJSON *type=NULL;
  const cJSON *content=NULL;
  type = cJSON_GetObjectItemCaseSensitive(recieved_message, "type");
  //content = cJSON_GetObjectItemCaseSensitive(recieved_message, "content");
  //printf ("%s\n",content->valuestring);
  messages = cJSON_GetObjectItemCaseSensitive(recieved_message, "content");
  cJSON_ArrayForEach(message,messages){
    cJSON *name = cJSON_GetObjectItemCaseSensitive(message, "sender");
    cJSON *messagesent = cJSON_GetObjectItemCaseSensitive(message, "content");
    printf ("%s said:\t",name->valuestring);
    printf ("%s\n",messagesent->valuestring);
  }
  shutdown(client_socket,2);
  printf ("\n\n\n");
  chatmenu();
}




void channelmembers(){
  char temp[1024]="channel members ";
  strcat (temp,authtoken);
  char servermessage[1024];
  strcat (temp,"\n");
  serverconnect();
  send(client_socket,temp,sizeof(temp),0);
  memset(servermessage,0,1024);
  recv(client_socket,servermessage,1024,0);
  cJSON* member=NULL;
  //printf ("%s",servermessage);
  cJSON *recieved_message = cJSON_Parse(servermessage);
  const cJSON *type=NULL;
  const cJSON *content=NULL;
  type = cJSON_GetObjectItemCaseSensitive(recieved_message, "type");
  content = cJSON_GetObjectItemCaseSensitive(recieved_message, "content");
  printf ("These Are The Members:\n");
  cJSON_ArrayForEach(member,content){
    printf ("%s\n",member->valuestring);
  }
  shutdown(client_socket,2);
  printf ("\n\n\n");
  chatmenu();
}




void creatchannel(){
  char temp[1024]="create channel ";
  char servermessage[1024];
  char chname[100];
  printf ("Please Enter Channel's Name:\n");
  scanf ("%s",chname);
  system("clear");
  strcat(temp,chname);
  strcat(temp,", ");
  strcat(temp,authtoken);
  strcat(temp,"\n");
  serverconnect();
  send (client_socket,temp,sizeof(temp),0);
  recv(client_socket,servermessage,sizeof(servermessage),0);
  cJSON *recieved_message = cJSON_Parse(servermessage);
  const cJSON *type=NULL;
  const cJSON *content=NULL;
  type = cJSON_GetObjectItemCaseSensitive(recieved_message, "type");
  content = cJSON_GetObjectItemCaseSensitive(recieved_message, "content");
  if (!strcmp("Error",type->valuestring)){
    printf ("%s\n",content->valuestring);   mainmenu();}
  else
    printf ("Channel Successfully Created!\n");
  shutdown(client_socket,2);
  printf ("\n\n\n");
  chatmenu();
}




void joinchannel(){
  printf("Enter The Channel's Name:\n");
  char temp[1024]="join channel ";
  char servermessage[1024];
  char chname[100];
  scanf ("%s",chname);
  system("clear");
  strcat(temp,chname);
  strcat(temp,", ");
  strcat(temp,authtoken);
  strcat(temp,"\n");
  serverconnect();
  send (client_socket,temp,sizeof(temp),0);
  recv(client_socket,servermessage,sizeof(servermessage),0);
  cJSON *recieved_message = cJSON_Parse(servermessage);
  const cJSON *type=NULL;
  const cJSON *content=NULL;
  type = cJSON_GetObjectItemCaseSensitive(recieved_message, "type");
  content = cJSON_GetObjectItemCaseSensitive(recieved_message, "content");
  if (!strcmp("Error",type->valuestring)){
    printf ("%s\n",content->valuestring);
    mainmenu();
  }
  else
    printf ("Joined The Channel Successfully!\n");
  shutdown(client_socket,2);
  printf ("\n\n\n");
  chatmenu();
}




void logout(){
  char temp[1024]="logout ";
  char servermessage[1024];
  char chname[100];
  strcat(temp,authtoken);
  strcat(temp,"\n");
  serverconnect();
  send (client_socket,temp,sizeof(temp),0);
  recv(client_socket,servermessage,sizeof(servermessage),0);
  cJSON *recieved_message = cJSON_Parse(servermessage);
  const cJSON *type=NULL;
  const cJSON *content=NULL;
  type = cJSON_GetObjectItemCaseSensitive(recieved_message, "type");
  content = cJSON_GetObjectItemCaseSensitive(recieved_message, "content");
  if (!strcmp("Error",type->valuestring))
    printf ("%s\n",content->valuestring);
  else
    printf ("Logout Was Successful!");
  shutdown(client_socket,2);
  printf ("\n\n\n");
  accountmenu();
}
/*just
wanted
to
reach
400*/
