#include <stdio.h>
#include <stdlib.h>

#define nbrFrames 256

//Adrian Stude och Mohamad Muhra


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

typedef struct {
    int algo; //algoritmen du väljer i kommando
    int nbrFrames; //antal frames
    int pageTable;
    int pageFaults; //missar
    int pageHits; //träffar
    int pageChange; //sido byte
    int pMemory; //fysiskt minne
}vsim;

//ska initiera hela simuleringen
vsim* create_sim(int algo, int nbrFrames) {
    /*
    ska 'nollställa' allt å initiera allt så när den faktiskt ska köras
    så är det bara att välja specifikationer å köra(nästa funktion dvs)
    */

    vsim *sim = (vsim*) malloc(sizeof(vsim));
    sim ->algo = algo;
    sim ->nbrFrames = nbrFrames;
    sim ->pageFaults = 0;
    sim ->pageHits = 0;
    sim ->pageChange = 0;

    sim -> pageTable = (int*)malloc(sizeof(int) * nbrFrames);
    sim -> pMemory = (int*)malloc(sizeof(int) * nbrFrames);

    for(int i = 0; i < nbrFrames; i++) {
        sim ->pMemory[i]= -1;
    }
    
    return sim;
}

vsim simulation(vsim *sim, int adresses, int nbrAdresses) {
    for(int i = 0; i < nbrFrames;i++) {
        int page = adresses[i];
    }

    switch(sim -> algo) {
        //fifo
        case 0:
            //fifo();
            break;
        //opt
        case 1:
            //upt();
            break;
        //lru
        case 2:
            //lru();
            break;
        
    }
}

void fifo() {
    /*
    hantering av first in first out
    */
}

void upt() {
    /*
    kräver att du kan se fil för att kunna lista ut vad som ska tas bort först
    */
}

void lru() {
    /*
    least read ues time eller nått, senast läst nyligen bort först
    */
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
void readFile(char filename,int nbrAdresses) {
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
    }
    
    fclose();
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



