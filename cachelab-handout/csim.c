#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "cachelab.h"
#include "assert.h"

long getlowbits(long number, long n);
void getaddrfromline(char* buff, char* result);
long getlownnumber(long addr, long n);
bool isequalbits();

int main(long argc, char* argv[])
{
    long tagbits = 0;
    long setsbits = 0;
    long linesperset = 0;
    long blockbits = 0;
    char* filename = NULL;
    bool infoflag = false;
    FILE* fp = NULL;

    for (long i=1; i<argc; i++){
        size_t size = strlen(argv[i]);
        assert(size == 2);
        switch (argv[i][1])
        {
        case 'v':
            infoflag = true;
            break;
        
        case 's':
            setsbits = atoi(argv[++i]);
            break;
        
        case 'E':
            linesperset = atoi(argv[++i]);
            break;
        
        case 'b':
            blockbits = atoi(argv[++i]);
            break;
        
        case 't':
            filename = argv[++i];
            break;

        default:
            printf("Parameter error, you should use -v -s -E -b -t");
            return -1;
        }

    }

    tagbits = sizeof(long) - setsbits - blockbits;

    long count = pow(2, setsbits) * linesperset;
    long* tagptr = malloc(sizeof(long) * count);
    long* usedptr = malloc(sizeof(long) * count);

    if (usedptr == NULL || tagptr == NULL){
        printf("malloc error.");
        return -1;
    }

    memset(tagptr, 0, sizeof(long) * count);
    memset(usedptr, 0, sizeof(long) * count);

    printf("s: %ld\n", setsbits);
    printf("E: %ld\n", linesperset);
    printf("b: %ld\n", blockbits);
    printf("filename: %s\n", filename);
    printf(infoflag ? "true" : "false");

    char file[1024];
    strcpy(file, getcwd(NULL, 0));
    strcat(file, "/");
    strcat(file, filename);
    puts(file);

    fp = fopen(file, "r");

    char buff[1024];
    char number[1024];
    unsigned long addr;
    long n;

    long set;
    long block;
    long tag;
    puts("fuck\n");

    while (fgets(buff, 1024, fp) != NULL) {
        memset(number, 0, 1024);
        if(*(buff+1) == 'L')continue;

        printf("line: %s", buff);
        getaddrfromline(buff, number);
        printf("stri: %s\n", number);
        sscanf(number, "%x", (unsigned int*)&addr);
        printf("addr: %x\n", (unsigned int)addr);

        tag = getlownnumber(addr >> (blockbits + setsbits), tagbits);
        set = getlownnumber(addr >> blockbits, setsbits);
        block = getlownnumber(addr, blockbits);

        printf("tag: %lx\n", tag);
        printf("set: %lx\n", set);
        printf("block: %lx\n\n", block);

        long* nowptr = usedptr + set * count;

        long* this = nowptr;

        bool allone = true;
        for (int i=0; i < linesperset; i++) {
            if (*this != 0) {
                if (*(tagptr + set * count + i) == tag) {
                    if (*(buff+1) == 'M') printf("hit hit");
                    else printf("hit");
                    (*this)++;
                    break;
                }
            }
            else allone = false;
            this++;
        }

        long index = 0;

        if (allone == false) {
            this = nowptr;
            for (int i=0; i < linesperset; i++) {
                if (this == 0)break;
                this++;
            }
            *this = 1;
            *(tagptr + set * count) = tag;
            if (*(buff+1) == 'M') printf("miss hit");
            else printf("miss");
        }

        else {
            long min = 0x8FFFFFFFFFFFFFFF;
            this = nowptr;
            for (int i=0; i < linesperset; i++) {
                if (*this < min) {
                    min = *this;
                    index = i;
                }
                this++;
            }
            *(tagptr + set *count + index) = tag;
            *this = 1;
            if (*(buff+1) == 'M') printf("miss eviction hit");
            else printf("miss eviction");
        }
    }
    // prlongSummary(0, 0, 0);
    return 0;
}

void getaddrfromline(char* buff, char* result) {
    long start = 0;
    long end = 0;

    for (long i=0; ; i++) {
        if ((buff[i] == ' ') && (i != 0) &&(buff[i+1] != ' ')) {
            start = i + 1;
        }
        else if (buff[i] == ',') {
            end = i;
            break;
        }
    }
    strncpy(result, buff + start, end - start);
}

long getlownnumber(long addr, long n) {
    long x = 1;
    for (long i=1; i<n; i++)x =x | (x << 1);
    return x & addr;
}
