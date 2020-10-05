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

int main(int argc, char* argv[])
{
    long hit_count = 0;
    long eviction_count = 0;
    long miss_count = 0;
    long tagbits = 0;
    long setsbits = 0;
    long linesperset = 0;
    long blockbits = 0;
    char* filename = NULL;
    bool infoflag = false;
    FILE* fp = NULL;
    long max = 1;

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

    tagbits = sizeof(long) * 8 - setsbits - blockbits;

    long count = pow(2, setsbits) * linesperset;
    printf("count: %ld\n", count);
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
    long addr;

    long set;
    long block;
    long tag;
    puts("fuck\n");

    while (fgets(buff, 1024, fp) != NULL) {
        memset(number, 0, 1024);
        if(*(buff) == 'I')continue;

        printf("line: %s", buff);
        getaddrfromline(buff, number);
        printf("stri: %s\n", number);
        sscanf(number, "%lx", &addr);
        printf("addr: %lx\n", addr);

        tag = getlownnumber(addr >> (blockbits + setsbits), tagbits);
        set = getlownnumber(addr >> blockbits, setsbits);
        block = getlownnumber(addr, blockbits);

        printf("tag: %lx\n", tag);
        printf("set: %lx\n", set);
        printf("block: %lx\n\n", block);

        long* nowptr = usedptr + set * linesperset;

        long* this = nowptr;

        bool allone = true;
        for (int i=0; i < linesperset; i++) {
            if (*this != 0) {
                if (*(tagptr + set * linesperset + i) == tag) {
                    if (*(buff+1) == 'M') {
                        printf("hit hit\n");
                        hit_count++;
                    }
                    else printf("hit\n");
                    (*this) = max++;
                    hit_count++;
                    goto ff;
                }
            }
            else allone = false;
            this++;
        }

        long index = 0;

        if (allone == false) {
            this = nowptr;
            for (int i=0; i < linesperset; i++) {
                if (*this == 0) {
                    *(tagptr + set * linesperset+i) = tag;
                    break;
                }
                this++;
            }
            *this = max++;
            if (*(buff+1) == 'M') {
                printf("miss hit\n");
                hit_count++;
                *this = max++;
            }
            else printf("miss\n");
            miss_count++;
        }

        else {
            long min = 0x7FFFFFFFFFFFFFFF;
            this = nowptr;
            for (int i=0; i < linesperset; i++) {
                if (*this != 0 && ((*this) < min)) {
                    min = *this;
                    index = i;
                }
                this++;
            }
            *(tagptr + set *linesperset + index) = tag;
            *(nowptr + index) = max++;
            if (*(buff+1) == 'M') {
                printf("miss eviction hit\n");
                hit_count++;
            }
            else printf("miss eviction\n");
            miss_count++;
            eviction_count++;
        }
        ff:printf("used :");
        long* x = usedptr;
        for (int i=0; i<count; i++){
            printf("%lx ", *x);
            x++;
        }
        printf("\ntags :");
        x = tagptr;
        for (int i=0; i<count; i++){
            printf("%lx ", *x);
            x++;
        }
    }
    printf("hit: %ld\n", hit_count);
    printf("miss: %ld\n", miss_count);
    printf("eviction: %ld\n", eviction_count);
    printSummary(hit_count, miss_count, eviction_count);
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
