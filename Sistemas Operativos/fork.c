#include <stdio.h>
#include <unistd.h>

int main(){
    int f = fork();
    if(f == 0){
        execl("./helloWorld", (char*)NULL);
    } else {
        printf("%d\n", (int)getpid());
    };
    return(0);
}
