#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <string.h> 
#include <stdbool.h>
#include <net/if.h>
#include <sys/ioctl.h>

#define MAX_MESSAGE_SIZE 1000


/*When we were making this program there were many errors occured in Windows regarding the libraries <arpa/inet.h>,<net/if.h>,<sys/iocls.h>..We did our best to solve that problems,but unfortunatelly we couldn't find any solution.So our team members talked with the lecture.After that we dicided to do our program in linux based operating system because of the linux has all libraries and compilers as an inbuild.So this c programs are nicely working in linux operating system.if the any kind of error occured please kindly contact us,0763261558*/




char*socket0(){

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char message[8000];
    char clientName[10];

    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed");
        exit(EXIT_FAILURE);
    }

    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Failed:8080 is using..");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    
    if (listen(server_socket, 5) == -1) {
        perror("Failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for other.....\n");

        
            
    socklen_t addr_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
    if (client_socket == -1) {
        perror("Failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    
     printf("connected.\n");
     
    char recieverName[10];
    printf("Enter reciever name : ");
    scanf("%s",&recieverName);
    
    while (1) {
        
        if (recv(client_socket, message, sizeof(message), 0) <= 0) {
            perror("Failed");
            break;
        }

        
        printf("\033[1;34m");
        printf("%s's Message : %s",recieverName ,message);
        printf("\033[0m");
       
  
        fgets(message, sizeof(message), stdin);
        send(client_socket, message, strlen(message), 0);
 
 
    }
       
    
  
}






char* getIpAddress() {
    int fd;
    struct ifreq ifr;

    
    const char* networkInterface = "ens33";

    
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        return NULL;
    }

    
    strncpy(ifr.ifr_name, networkInterface, IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';

    if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
        perror("ioctl");
        close(fd);
        return NULL;
    }

    close(fd);

    
    char* ipAddress = malloc(INET_ADDRSTRLEN);
    if (ipAddress == NULL) {
        perror("malloc");
        return NULL;
    }

    inet_ntop(AF_INET, &(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), ipAddress, INET_ADDRSTRLEN);

    return ipAddress;
}

int ipBreaker(char ipAddress[13], int start, int last) {
    int num = 0;
    for (int i = start; i <= last; i++) {
        num = num * 10 + (ipAddress[i] - '0');
    }
    return num;
}

bool isValidIp(char ipAddress[13]) {
    
    if (strlen(ipAddress) != 13)
        return false;

    int ipArray[4];
    ipArray[0] = ipBreaker(ipAddress, 0, 2);
    ipArray[1] = ipBreaker(ipAddress, 4, 6);
    ipArray[2] = ipBreaker(ipAddress, 8, 8);
    ipArray[3] = ipBreaker(ipAddress, 10, 12);

    for (int i = 0; i < sizeof(ipArray) / sizeof(ipArray[0]); i++) {
        if (ipArray[i] < 0 || ipArray[i] > 255) {
            return false;
        }
    }
    return true;
}

void connectPeer() {
    
    char ipAddress[16]; 
    printf("\n\t\t\t\tEnter Peer IP address to connect : ");
    scanf("%13s", ipAddress); 

    if (!isValidIp(ipAddress)) {
        char ch;
        printf("\t\t\t\tInvalid IP address.\n\n");
        printf("\t\t\t\tDo you want to continue ? (Y/N) : ");
        scanf(" %c", &ch); 
        if (ch == 'Y' || ch == 'y') {
            connectPeer();
        } else if (ch == 'N' || ch == 'n') {
            //mainMenu();
        }
    } else {
        printf("Connecting\n");
        socket0();
    }
}

void mainMenu(){
    printf("\n\n \t\t\t\t\t  _____    _____  _             _ \n");
    printf("\t\t\t\t\t /  ___|  / ____|| |           | | \n");
    printf("\t\t\t\t\t| |      | |     | | ___   __ _| |_ \n");
    printf("\t\t\t\t\t| |      | |     | |/ _ \\ / _' |  _| \n");
    printf("\t\t\t\t\t| |____  | |____ |  /  | | |_| | |_ \n");
    printf("\t\t\t\t\t \\ ____|  \\_____||_|   |_|\\__,_|\\__| \n\n");
    printf("\t\t\t\t\t====================================\n\n\n");
    
    char ipAddress[16] = "0.0.0.0";
    char connectionStatus[] = "Not Connected";

    char* ipResult = getIpAddress(); 

    if (ipResult != NULL) {
        strncpy(ipAddress, ipResult, sizeof(ipAddress)-1); 
        strcpy(connectionStatus, "Connected"); 
    }
    
    printf("\t\t\t\tConnection status : %s\t\t IP Address : %s \n\n",connectionStatus,ipAddress);


    printf("\t\t\t\t[01] Connect to a Peer\n\n");
    printf("\t\t\t\t[02] Exit \n\n");
    int option;
    printf("\t\t\t\tEnter an option to continue  : ");
    scanf("%d",&option);
    switch(option){
        case 1:connectPeer();break;
        case 2:exit(0);break;
        default:printf("\t\t\t\tInvalid input.\n \t\t\t\tTry again. \n\n ");
                //mainMenu();
    }
}

int main() {
    mainMenu();
    return 0;
}

