#include <stdio.h>
#include <stdlib.h>

#define nbrFrames 256

/*
Krav i uppgift:
tre st page replacement algorithms:
    *FIFO
    *OPT
    *LRU
när programmet körs så används:
    *vmsim -a <fifo | optimal | lru> -n <number of frames> -f <trace file>
    -n : number of frames in physical memory
    -f : the name of the trace file with vr memory

mer grejer som behövs:
    page table (page table hit)
    backing store (page fault)
    page replacement (page out/page in)
    kunna läsa .dat filerna

sist ska en summering skrivas ut

Storlekar:
    storlek på vr minne = 64Kb
    16 bit adresser(0x0000-0xffff)
    frame size 256 bytes
    physical memory = <frames> x 256 bytes

*/

/*
filhantering
read

behöver kunna läsa av filerna, storlek å läsa in de rätt för hantering
*/


void fifo() {

}

void upt() {

}

void lru() {

}
//för att kunna läsa antalet adresser i filen som används
int count_Lines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Kunde inte öppna filen");
        exit(1);
    }

    int count = 0;
    char line[256]; 

    // Läs filen rad för rad
    while (fgets(line, sizeof(line), file) != NULL) {
        count++;
    }

    fclose(file);
    return count;
}

//för att kunna läsa .dat filer
void readFile(char filename,) {
    int capacityAdresses = 10;
    int *adresses = (int*) malloc(capacity * sizeof(int));
    FILE *file = fopen(filename,'r');

    if(file == NULL) {
        printf("kunde ej läsa fil");
        exit(0);
    }

    if(adresses == NULL) {
        printf("problem vid allokering");
    }
    
    //hantering av filadresser
    //konvertera om hexa till heltal
    while(fscanf(file,"%x",%adress) == 1) {
        if(nbrAdresses >= capacity) {
            capacity *=2;
            adresses = (int*)realloc(adresses, capacity * sizeof(int));
        }
        
        if(adresses == NULL) {
            printf("problem vid omvandlingen å allokering");
            exit(0);
        }

        adresses[*nbrAdresses] = adress;
        (*nbrAdresses)++;
    }
    
    fclose();
    return adresses;
}

void main() {
    const char* filename;
    scanf("%s",filename);
    
    //fifo
    
    //UPT
    
    //LRU

    //init simulering
    
    //läsning av fil å hantering av adresser
    int nbrAdresses = count_Lines(filename);
    int* adresses = readFile(filename,nbrAdresses);
    
    //simulering

    //summering

    return 0;
}



