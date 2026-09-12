
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include "game_html.inc"

static int command_exists(const char *cmd) {
    const char *path = getenv("PATH"); if (!path) return 0;
    char *copy=strdup(path); if(!copy) return 0; char *save=NULL;
    for(char *dir=strtok_r(copy,":",&save);dir;dir=strtok_r(NULL,":",&save)){
        char p[4096]; snprintf(p,sizeof(p),"%s/%s",dir,cmd);
        if(access(p,X_OK)==0){free(copy);return 1;}
    } free(copy); return 0;
}
static void serve_client(int cfd){
    char req[4096]; ssize_t n=read(cfd,req,sizeof(req)-1); if(n<=0)return;
    const char *hdr="HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nCache-Control: no-store\r\nConnection: close\r\n\r\n";
    write(cfd,hdr,strlen(hdr)); write(cfd,game_html,game_html_len);
}
int main(void){
    signal(SIGCHLD,SIG_IGN); int s=socket(AF_INET,SOCK_STREAM,0); if(s<0)return 1;
    int one=1; setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
    struct sockaddr_in a; memset(&a,0,sizeof(a)); a.sin_family=AF_INET; a.sin_addr.s_addr=htonl(INADDR_LOOPBACK); a.sin_port=htons(0);
    if(bind(s,(struct sockaddr*)&a,sizeof(a))<0)return 1; socklen_t alen=sizeof(a); if(getsockname(s,(struct sockaddr*)&a,&alen)<0)return 1; if(listen(s,8)<0)return 1;
    char url[256]; snprintf(url,sizeof(url),"http://127.0.0.1:%d/",ntohs(a.sin_port));
    pid_t child=fork(); if(child<0)return 1;
    if(child==0){ usleep(250000);
        const char *b[]={"chromium","chromium-browser","google-chrome","google-chrome-stable","brave-browser","microsoft-edge",NULL};
        for(int i=0;b[i];++i) if(command_exists(b[i])){ char app[512]; snprintf(app,sizeof(app),"--app=%s",url); execlp(b[i],b[i],app,"--class=BuildTheWall","--disable-session-crashed-bubble","--no-first-run",(char*)NULL);}
        if(command_exists("firefox")) execlp("firefox","firefox","--new-window",url,(char*)NULL);
        fprintf(stderr,"No supported browser found. Open: %s\n",url); _exit(1);
    }
    for(;;){ int c=accept(s,NULL,NULL); if(c<0){if(errno==EINTR)continue;break;} serve_client(c); close(c);}
    close(s); return 0;
}
