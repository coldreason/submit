#include "message_buffer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int shmid;
void *memory_segment=NULL;

int init_buffer(MessageBuffer **buffer) {
    /*---------------------------------------*/
    /* TODO 1 : init buffer*/

    MessageBuffer buf;
    buf.is_empty = 1;
    buf.account_id = 111;
    if((shmid = shmget(KEY,sizeof(MessageBuffer)*BUFFER_SIZE,IPC_CREAT|0666))==-1)return -1;
    printf("shmid : %d\n",shmid);
   if((memory_segment = shmat(shmid,(char *)NULL,0)) == (void*)-1) return -1;
   
   buffer = ((MessageBuffer **)memory_segment);
   for(int i = 0;i<BUFFER_SIZE;i++){
	printf("%d\n",*(buffer+i));
	  *(buffer+i) = &buf;
	   //(*(buffer+i))->is_empty = 0;
	  // printf("%d\n",&buffer[i]);
	  // printf("%d\n",(*(buffer+i)));
	 //  (*buffer)->is_empty = 1;
	 //  (*buffer)->account_id = 0;
	 //  for(int j = 0;j<BUFFER_SIZE;j++){
	 //	(*buffer)->messages.sender_id = 0;
	 //  }
   }
    /* TODO 1 : END                          */
    /*---------------------------------------*/

    printf("init buffer\n");
    return 0;
}

int attach_buffer(MessageBuffer **buffer) {
    /*---------------------------------------*/
    /* TODO 2 : attach buffer                */
    /* do not consider "no buffer situation" */
    	shmid = shmget(KEY,sizeof(MessageBuffer) *BUFFER_SIZE,IPC_CREAT|0666);
    	memory_segment = shmat(shmid(char *)NULL,0);
   	buffer = ((MessageBuffer **)memory_segment); 
    /* TODO 2 : END                          */
    /*---------------------------------------*/

    printf("attach buffer\n");
    printf("\n");
    return 0;
}

int detach_buffer() {
    if (shmdt(memory_segment) == -1) {
        printf("shmdt error!\n\n");
        return -1;
    }

    printf("detach buffer\n\n");
    return 0;
}

int destroy_buffer() {
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
        printf("shmctl error!\n\n");
        return -1;
    }

    printf("destroy shared_memory\n\n");
    return 0;
}

int produce(MessageBuffer **buffer, int sender_id, int data, int account_id) {

    /*---------------------------------------*/
    /* TODO 3 : produce message*/
    
	MessageBuffer * ptr;	
    for(int i = 0;i<BUFFER_SIZE;i++){
    	if(buffer[i].account_id == account_id){
		ptr = &buffer[i];
		break;
	}
	if(buffer[i].is_empty==1){
		ptr = &buffer[i];
		buffer[i].account_id = account_id;
		buffer[i].is_empty=0;
		break;
	}
    }
    for(int i = 0;i<BUFFER_SIZE;i++){
   	if(ptr.messages[i].sender_id==0){
		(*ptr).messages[i].sender_id = sender_id;
		(*ptr).messages[i].data = data;
		break;
	}
    }
    

    /* TODO 3 : END                          */
    /*---------------------------------------*/

    printf("produce message\n");

    return 0;
}

int consume(MessageBuffer **buffer, Message **message) {
    if((*buffer)->is_empty)
	    return -1;

    /*---------------------------------------*/
    /* TODO 4 : consume message
     *              */
	
    static int updatepoint[BUFFER_SIZE] = {0,};
    static int balance[BUFFER_SIZE] = {0,};
    for(int i = 0;i<BUFFER_SIZE;i++){
    	for(int j = 0;j<BUFFER_SIZE;j++){
		if(buffer[i].messages[j].sender_id == 0 && updatepoint[i] < j){
			updatepoint[i]++;
			balance[i]+=buffer[i].messages[j].data;
			*message->sender_id = buffer[i].messages[j].sender_id;
			*message->data = buffer[i].messages[j].data;
			return 0;
		}

	}
    }
    return 1;
    

    /* TODO 4 : END                          */
    /*---------------------------------------*/

    return 0;
}
