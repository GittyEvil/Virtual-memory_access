#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define nbrframes 256

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
    frame size nbrframes bytes
    physical memory = <frames> x nbrframes bytes

*/

/*
filhantering
read

behöver kunna läsa av filerna, storlek å läsa in de rätt för hantering
*/

typedef struct {
    int algo; //algoritmen du väljer i kommando
    int nbrFrames; //antal frames
    int* pageTable;
    int pageFaults; //missar
    int pageHits; //träffar
    int pageChange; //sido byte, replacement
    int* pMemory; //fysiskt minne
    int currentTime; //för lru
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
    sim ->currentTime = 0;

    sim -> pageTable = (int*)malloc(sizeof(int) * nbrFrames);
    sim -> pMemory = (int*)malloc(sizeof(int) * nbrFrames);

    //lägger de tomma sidorna som -1 i minne
    for(int i = 0; i < nbrFrames; i++) {
        sim ->pMemory[i]= -1;
    }
    
    return sim;
}

int pageHitorMiss(vsim *sim, int page) {
    //funktion returnar 1 om pageHit annars 0 om fault
    for(int i = 0; i < sim -> nbrFrames;i++) {
        if(sim ->pMemory[i] == page) {
            return 1; 
        }
    }
    return 0;
}

void fifo(vsim *sim, int page) {
    /*
    hantering av first in first out
    */
    static int nextFrame = 0;
    
    //om sidträff
    if(pageHitorMiss(sim,page)) {
        sim -> pageHits++;
        return;
    } 
    sim -> pageFaults++;

    //om sida var tom så läggs in en ny adress
    for(int i = 0; i < sim -> nbrFrames;i++) {
        if(sim ->pMemory[i] == -1) {
            sim -> pMemory[i] = page;
            return;
        }
    }
    //cirkulär fifo, sidbyte sker vid fulla ramar
    sim -> pMemory[nextFrame] = page;
    nextFrame = (nextFrame + 1) % sim -> nbrFrames; 
    sim ->pageChange++;
}

void upt(vsim *sim, int page, int* adresses, int currentIndex, int nbrAdresses) {
    /*
    beladys algoritm, kolla framtid å tar bort det som least used
    */
    if (pageHitorMiss(sim, page)) {
        sim -> pageHits++;
        return;
    }
    sim -> pageFaults++;

    //om tom lägg till adress i minne
    for (int i = 0; i < sim -> nbrFrames; i++) {
        if (sim -> pMemory[i] == -1) {
            sim -> pMemory[i] = page;
            //sim -> pageChange++;
            return;
        }
    }

    int furthest = -1;
    int replaceIndex = -1;

    //de som är använt senast byts sedan ut eller inte använd alls
    for (int i = 0; i < sim -> nbrFrames; i++) {
        int j;
        for (j = currentIndex + 1; j < nbrAdresses; j++) {
            if (adresses[j] == sim -> pMemory[i]) {
                if (j > furthest) {
                    furthest = j;
                    replaceIndex = i;
                }
                break;
            }
        }
        if (j == nbrAdresses) {
            replaceIndex = i;
            break;
        }
    }
    //byter ut adress och ökar sidbyte
    sim -> pMemory[replaceIndex] = page;
    sim -> pageChange++;
}

void lru(vsim *sim,int page) {
    /*
    least read ues time eller nått, senast läst nyligen bort först
    */
    int leastUsed = INT_MAX;
    int lUsedIndex = -1;

    //checkar alla adresser i minne och ökar tiden de varit använda
    for(int i = 0; i < sim -> nbrFrames;i++) {
       if(sim ->pMemory[i] == page) {
           sim ->pageTable[i] = sim ->currentTime++;
           sim ->pageHits++;
           return;
       }  
    }
    sim -> pageFaults++;
    //om tom lägg in och öka tiden för addresen
    for(int i = 0; i < sim -> nbrFrames;i++) {
       if(sim ->pMemory[i] == -1) {
           sim ->pMemory[i] = page;
           sim ->pageTable[i] = sim -> currentTime++;
           return;
       }  
    }
    //lägger in adress som är använd minst för att sen byta ut
    for(int i = 0; i < sim -> nbrFrames;i++) {
        if(sim -> pageTable[i] < leastUsed) {
            leastUsed = sim -> pageTable[i];
            lUsedIndex = i;
        }
    }
    //byter ut om allt gått rätt
    if(lUsedIndex != -1) {
        sim -> pMemory[lUsedIndex] = page;
        sim -> pageTable[lUsedIndex] = sim -> currentTime++;  
        sim -> pageChange++;
    }
}

//för att kunna läsa antalet adresser i filen som används
int countLines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Kunde inte öppna filen");
        exit(1);
    }

    int count = 0;
    char line[nbrframes]; 

    // Läs filen rad för rad
    while (fgets(line, sizeof(line), file) != NULL) {
        count++;
    }

    fclose(file);
    return count;
}

//för att kunna läsa .dat filer
int* readFile(const char* filename,int* nbrAdresses) {
    *nbrAdresses = countLines(filename);
    int *adresses = (int*) malloc(*nbrAdresses * sizeof(int));
    FILE *file = fopen(filename,"r");

    if(file == NULL) {
        printf("kunde ej läsa fil");
        exit(0);
    }

    char line[nbrframes];
    int count = 0;
    
    //konvetering från hexa till heltal
    while (fgets(line, sizeof(line), file) != NULL) {
        //konverting av hex till heltal
        int adress;
        if (sscanf(line, "%x", &adress) == 1) {
            //delar adress mha ramarna(sidstorleken)
            adress = adress / nbrframes; 
            adresses[count] = adress;
            printf("Läst address: 0x%x -> Sida: %d\n", adress * nbrframes, adress);  
            count++;
        } else {
            printf("Ogiltig adress: %s\n", line);
        }
    }

    fclose(file);
    return adresses;
}

vsim simulation(vsim *sim, int address,int* adresses,int currentIndex, int nbrAdresses) {
    int page = address;
    switch(sim -> algo) {
        //fifo
        case 0:
            fifo(sim, page);
            break;
        //opt
        case 1:
            upt(sim, page, adresses,currentIndex, nbrAdresses);
            break;
        //lru
        case 2:
            lru(sim, page);
            break;
        default:
            printf("hittar inte algoritm");
            exit(1); 
    }
}

void summary(vsim *sim) {
    printf("antal frames: %d\n",sim -> nbrFrames);
    printf("antal träffar: %d\n",sim -> pageHits);
    printf("antal missar: %d\n",sim -> pageFaults);
    printf("antal byten: %d\n",sim -> pageChange);
}

int main(int args, char *argc[]) {
    //ser till att kommandot skrivs in rätt
    if (args < 5) {
        printf("kommando: vmsim -a <fifo | upt | lru> -n <number of frames> -f <trace file>\n");
        return 1;
    }

    const char* filename = NULL;
    int algo = -1;
    int nbrFrames = -1;
    
    //checkar input användare ger i kommandot vid körning
    for (int i = 1; i < args; i++) {
        if (strcmp(argc[i], "-a") == 0) {
            if (strcmp(argc[i + 1], "fifo") == 0) {
                algo = 0;
            } else if (strcmp(argc[i + 1], "upt") == 0) {
                algo = 1;
            } else if (strcmp(argc[i + 1], "lru") == 0) {
                algo = 2;
            } else {
                printf("felaktig algoritm försök igen");
                return 1;
            }
        } else if (strcmp(argc[i], "-n") == 0) {
            nbrFrames = atoi(argc[i + 1]);
        } else if (strcmp(argc[i], "-f") == 0) {
            filename = argc[i+1];
        }
    }

    if (algo == -1 || filename == NULL || nbrFrames == -1) {
        printf("Fel, alla parametrar måste anges.\n");
        return 1;
    }
    //initering av simulator
    vsim *sim = create_sim(algo, nbrFrames);

    //Läs fil och adresser
    int nbrAdresses;
    int* adresses = readFile(filename, &nbrAdresses);

    //Simulering
    for (int i = 0; i < nbrAdresses; i++) {
        simulation(sim, adresses[i],adresses, i, nbrAdresses);
    }
    //Summering
    summary(sim);

    //frigör upp alla adresser för å kunna reset:a
    free(adresses);
    free(sim->pageTable);
    free(sim->pMemory);
    free(sim);

    return 0;
}



