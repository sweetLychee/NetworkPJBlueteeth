#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h> 
#include <bluetooth/hci.h> 
#include <bluetooth/hci_lib.h>

int main(int argc, char **argv)
{
    inquiry_info *devices = NULL;
    int max_rsp, num_rsp;
    int adapter_id, sock, len, flags;
    int i;

    charaddr[19] = {0};
    charname[248] = {0};

    adapter_id= hci get route(NULL);
    sock = hciopendev(adapter_id);

    if (adapter_id < 0 || sock < 0) 
    {
        perror( "opening socket" );
        exit(1); 
    }

    len = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    
    devices = (inquiry_info *)malloc(max_rsp * sizeof(inquiry_info));
    num_rsp = hci_inquiry(adapter_id, len, max_rsp, NULL, &devices, flags);
    if(num_rsp < 0) perror("hci_inquiry");
    for(i= 0;i<num_rsp;i++) 
    { 
        ba2str(&(devices+i)−>bdaddr, addr);
        memset(name, 0 , sizeof(name)) ;
        if(0 != hci read remote name (sock, &(devices+i)−>bdaddr, sizeof(name) , name, 0))
        {
            strcpy(name, "[unknown]");
        }
        printf( "%s %s\n" , addr, name);
    }
    
    free(devices);
    close(sock);
    return 0;
}