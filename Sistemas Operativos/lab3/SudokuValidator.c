#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <pthread.h>

int i = 0;
int j = 0;
int boardSize = 9;
int sudokuBoard[9][9];

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)


void *checkColumns(){
	omp_set_nested(1);
	omp_set_num_threads(9);
	int columnContainsDigits = 0;
	pid_t tid;

	// Revisamos columnas individuales
	#pragma omp parallel for private(tid) schedule(dynamic)
	for (i = 0; i<boardSize; i++){
		tid = syscall(SYS_gettid);
		printf("En la revision de columnas el siguiente es un thread en ejecucion: %d\n", tid);

		// Por cada iteracion creamos una lista vacia para almacenar
		// los elementos de la columna.
		int colNums[9] = {0};
		
		// Revisamos cada fila de la columna (cada elemento)
		#pragma omp parallel for schedule(dynamic)
		for (j = 0; j<boardSize; j++){
			int num = sudokuBoard[j][i];
			if (num < 1 || num > 9 || colNums[num - 1] == 1){
				//despliegue de error o salir de thread
				printf("Error en numero leido en columna");
				columnContainsDigits = 0;
				pthread_exit(NULL);
			}else{
				colNums[num - 1] = 1;
				columnContainsDigits = 1;
			}
		}
	}

	return (void*) columnContainsDigits;
	//pthread_exit(NULL);
}

void *checkRows(){
	omp_set_nested(1);
	omp_set_num_threads(9);
	int rowContainsDigits = 0;

	// Revisamos columnas individuales
	#pragma omp parallel for schedule(dynamic)
	for (i = 0; i<boardSize; i++){
		// Por cada iteracion creamos una lista vacia para almacenar
		// los elementos de la columna.
		int rowNums[9] = {0};
		
		// Revisamos cada fila de la columna (cada elemento)
		#pragma omp parallel for schedule(dynamic)
		for (j = 0; j<boardSize; j++){
			int num = sudokuBoard[i][j];
			if (num < 1 || num > 9 || rowNums[num - 1] == 1){
				//despliegue de error o salir de thread
				printf("Error en numero leido en fila");
				rowContainsDigits = 0;
				pthread_exit(NULL);
			}else{
				rowNums[num - 1] = 1;
				rowContainsDigits = 1;
			}
		}
	}

	return (void*) rowContainsDigits;
}


int checkSubgrid(int gridRowStart, int gridColStart){
	int subgridContainsDigits = 0; 	
	
	for (i = 0; i < (gridRowStart+3); i++){
		int subgridNums[9] = {0};
		for (j = 0; j < (gridColStart+3); j++){
			int num = sudokuBoard[i][j];

			if (num < 1 || num > 9 || subgridNums[num - 1] == 1){
				subgridContainsDigits = 0;
				break;
			}else{
				subgridNums[num - 1] = 1;
				subgridContainsDigits = 1;			
			}
		}	
	}

	return subgridContainsDigits;
}



int main(int argc, char *argv[]){
	omp_set_nested(1);
	omp_set_num_threads(1);
	char *addr;	
	int fd, fo, size;
	struct stat sb;
	size_t length;
	ssize_t s;
	int j = 0, k = 0;

	fd = open(argv[1], O_RDONLY);

	// Verificar si hubo algun error o no
	if (fd == -1)
		handle_error("open");

	// Obtenemos el tamano del archivo
	if (fstat(fd, &sb) == -1)
		handle_error("fstat");

	size = sb.st_size;

	addr = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (addr == MAP_FAILED)
		handle_error("mmap");

	int counter = 0;
	for (j = 0; j < boardSize; j++){
		for(k = 0; k < boardSize; k++){
			sudokuBoard[j][k] = addr[counter] - '0';
			counter += 1;			
		}
	}

	int sg = 0, col_res = 0, row_res = 0;
	for (j = 0; j < boardSize; j+=3){
		for(k = 0; k < boardSize; k+=3){
			sg = checkSubgrid(j, k);
			if (sg != 1)
				break;
		}
	}

	// Conseguimos el no de este proceso (no el del thread)
	pid_t parent_pidt = getpid();

	char pid_ref[10];
	sprintf(pid_ref, "%d", parent_pidt);

	printf("El thread en el que se ejecuta main es: %s\n", pid_ref);

	pid_t ft = fork();

	// Proceso hijo empieza
	if (ft == 0) {
		pid_t pf = getppid();
		// hijo
		execlp("ps", "ps", "-p", pid_ref, "-lLf", NULL);
	} 
	// Proceso padre empieza	
	else {
		//int col_res, row_res;
		void *valid_cols, *valid_rows;
		pid_t thread_id;

		// padre
		pthread_t thread_id_c;
		pthread_create(&thread_id_c, NULL, checkColumns, NULL);
		pthread_join(thread_id_c, &valid_cols);
		col_res = (int *)valid_cols;

		thread_id = syscall(SYS_gettid);
		
		printf("El thread que ejecuta revision de columnas es: %d\n", thread_id);

		wait(0);

		pthread_t thread_id_r;
		pthread_create(&thread_id_r, NULL, checkRows, NULL);
		pthread_join(thread_id_r, &valid_rows);
		row_res = (int *)valid_rows;
		
		// Desplegar si la solucion del sudoku es valida
		if (sg == 1 && col_res == 1 && row_res == 1){
			printf("La solucion al sudoku SI es valida\n");
		}else {
			printf("La solucion al sudoku NO es valida\n");		
		}
		

		pid_t fp = fork();
		
		if (fp == 0){
			pid_t pt = getppid();
			char pid_ref_t[10];
			sprintf(pid_ref_t, "%d", pt);


			execlp("ps", "ps", "-p", pid_ref_t, "-lLf", NULL);
		}

		wait(0);
	
		return 0;
	}

	munmap(addr, 0);
	close(fd);

	return 0;
}

