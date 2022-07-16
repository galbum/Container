#include "Socket.h"

void errorHandler(const char* errorMessage)
{
    perror(errorMessage);
    exit(EXIT_FAILURE);
}

int establish(unsigned short port)
{
    char myname[BUFFER_LENGTH + 1];
    int s;
    struct sockaddr_in sa{};
    struct hostent *hp;
    //hostnet init
    gethostname(myname, BUFFER_LENGTH);
    hp = gethostbyname(myname);
    if(hp == NULL)
        errorHandler(HOSTNAME_ERR);
    // sockaddr_in init
    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = hp -> h_addrtype;
    // host address
    memcpy(&sa.sin_addr, hp->h_addr, hp->h_length);
    // port number
    sa.sin_port = htons(port);
    /* create socket */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < SUCCESS)
        errorHandler(SOCKET_ERR);
    if (bind(s , (struct sockaddr *)&sa , sizeof(struct sockaddr_in)) != SUCCESS)
    {
        close(s);
        errorHandler(BIND_ERR);
    }
    /* max # of queued connects */
    if (listen(s, MAX_CONNECTIONS) != SUCCESS)
    {
        close(s);
        errorHandler(LISTEN_ERR);
    }
    return(s);
}

int call_socket(char *hostname, unsigned short port)
{
    struct sockaddr_in sa{};
    struct hostent *hp;
    int s;
    if ((hp= gethostbyname (hostname)) == NULL)
        errorHandler(HOSTNAME_ERR);
    memset(&sa,0,sizeof(sa));
    memcpy((char *)&sa.sin_addr , hp->h_addr , hp->h_length);
    sa.sin_family = hp->h_addrtype;
    sa.sin_port = htons((u_short)port);
    if ((s = socket(hp->h_addrtype,SOCK_STREAM,0)) < SUCCESS)
        errorHandler(SOCKET_ERR);
    if (connect(s, (struct sockaddr *)&sa , sizeof(sa)) != SUCCESS)
    {
        close(s);
        errorHandler(CONNECT_ERR);
    }
    return(s);
}

int read_data(int sock, char *buf, int n)
{
    int bcount;
    int br;
    bcount= 0; br= 0;
    while (bcount < n)
    {
        br = read(sock, buf, n - bcount);
        if (br > 0)
        {
            bcount += br;
            buf += br;
        }
        if (br < 1)
            return br;
    }
    return bcount;
}

int write_data(int sock, char* buf, int n)
{
    int bcount = 0;
    int br = 0;
    while (bcount < n) {
        br = write(sock, buf, n - bcount);
        if (br > 0) {
            bcount += br;
            buf += br;
        }
        if (br < 0)
            errorHandler(WRITE_ERR);
    }
    return(bcount);
}

int getConnection(int sock)
{
    struct sockaddr_in isa;
    int i,t;
    i = sizeof(isa);
    if (getsockname(sock, (struct sockaddr *) &isa, (socklen_t *) (&i)) != SUCCESS)
        errorHandler(SOCKNAME_ERR);
    if ((t = accept(sock, (struct sockaddr *) &isa, (socklen_t *) &i)) < SUCCESS)
        errorHandler(ACCEPT_ERR);
    return t;
}

int runServer(unsigned short port)
{
    char buf[BUFFER_LENGTH + 1];
    int sock = establish(port);
    // keep running server indefinitely, listen to clients and run commands.
    while(true)
    {
        int connectedSocket = getConnection(sock);
        read_data(connectedSocket, buf, BUFFER_LENGTH);
        if (system(buf) != SUCCESS)
            errorHandler(RUN_CMD_ERR);
        close(connectedSocket);
        buf[0] = '\0';
    }
}

int runClient(unsigned short port, char *cmdToRun)
{
    char hostname[BUFFER_LENGTH + 1];
    if (gethostname(hostname, BUFFER_LENGTH) != SUCCESS)
        errorHandler(HOSTNAME_ERR);
    int socketNum = call_socket(hostname, port);
    // write command to server and close connection
    write_data(socketNum, cmdToRun, strlen(cmdToRun));
    close(socketNum);
    return SUCCESS;
}

// main function
int main(int argc, char* argv[])
{
    if (argc < SERVER_ARG_NUM)
        errorHandler(USAGE_ERR);
    char *type = argv[1];
    unsigned short port = std::atoi(argv[2]);
    // create a client
    if (argc == CLIENT_ARG_NUM)
    {
        if (strcmp(type, CLIENT_TYPE_ARG) != SUCCESS)
            errorHandler(USAGE_ERR);
        char *cmdToRun = argv[3];
        runClient(port, cmdToRun);
    }
    // create a server
    else if (argc == SERVER_ARG_NUM)
    {
        if (strcmp(type, SERVER_TYPE_ARG) != SUCCESS)
            errorHandler(USAGE_ERR);
        runServer(port);
    }
    else
        errorHandler(USAGE_ERR);
    return SUCCESS;
}
