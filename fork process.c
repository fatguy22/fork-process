#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
 
int main(){
    
    int row, col, i, j;
    
	printf("matrix size:\n");
	scanf("%d*%d", &row, &col);
	//printf("%d %d", row, col);有成功讀入 
	
	int C[row][col], D[row][col], E[row][col];
	printf("matrix c\n");
	
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			scanf(" %d", &C[i][j]);
		}
	}
	
	/*for(i=0;i<row;i++){有成功讀入 
		for(j=0;j<col;j++){
			printf("%d ", C[i][j]);
		}
		printf("\n");
	}*/
	
	printf("matrix d\n");
	
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			scanf(" %d", &D[i][j]);
		}
	}
	
	for(i=0;i<row;i++){//E矩陣初始化 
		for(j=0;j<col;j++){
			E[i][j] = 0;
		}
	}
	
	//開始進入fork階段
	int shm_fd;//shared memory的file descriptor  
	int pid;
	int z, M, N;//M是列控制，N是行控制 
	long long int *shm_base; 
	
	shm_fd = shm_open("OS", O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, 1024);
	shm_base = mmap(0, 1024, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
	//這個指標能夠拿到記憶體位置 
	
	for(M=0;M<row;M++){
		for(N=0;N<col;N++){
			pid = fork();
			
			if(pid > 0){ 
				z = wait(NULL);//wait( )的回傳值是子process的pid 
				//sscanf(shm_base, "%s", &E[M][N]);從記憶體讀出
				E[M][N] = *shm_base;
			}
			else if(pid == 0){
				for(i=0;i<row;i++){
					E[M][N] = E[M][N] + C[M][i] * D[i][N];
				}
				//sprintf(shm_base, "%s", E[M][N]);寫入記憶體 
				*shm_base = E[M][N];
				exit(0); 
			}
			
		}
	}
	
	munmap(shm_base, 1024);
	shm_unlink("OS");
	
	printf("result:\n");
	
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			printf("%d ", E[i][j]);
		}
		printf("\n");
	}
	
    return 0;
}

