#include <func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    struct hostent *phost = gethostbyname(argv[1]);
    if(phost){
        printf("h_name:%s\n",phost->h_name);
    }
    for(int i = 0;phost->h_aliases[i] != NULL;i++)
    {
        printf("aligsas = %s\n",phost->h_aliases[i]);
    }
    printf("addtype = %d\n",phost->h_addrtype);
    printf("addrlenth = %d\n",phost->h_length);
    
    char buf[128] = {0};
    
    for(int i = 0;phost->h_addr_list[i] != NULL;i++)
    {
        memset(buf,0,sizeof(buf));
        inet_ntop(phost->h_addrtype,phost->h_addr_list[i],buf,sizeof(buf));
        printf("h_addr_list = %s\n",buf);
    }
    return 0;
}

