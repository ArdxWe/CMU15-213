#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

// #define NDEBUG
#define print(x) if (infoflag) printf(x)

#include "cachelab.h"
#include "assert.h"

void getaddrfromline(char* buff, char* result);
long getlownnumber(long addr, long n);

int main(int argc, char* argv[])
{
    // 记录次数
    long hit_count = 0;
    long eviction_count = 0;
    long miss_count = 0;

    // 缓存相关数据
    long tagbits = 0;
    long setsbits = 0;
    long linesperset = 0;
    long blockbits = 0;

    // 文件名
    char* filename = NULL;

    // 是否输出具体信息
    bool infoflag = false;

    // 文件
    FILE* fp = NULL;

    // 实现 LRU 变量
    long max = 1;

    // 解析命令行参数
    for (long i=1; i<argc; i++){
        size_t length = strlen(argv[i]);
        assert(length == 2);
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
            fprintf(stderr, "Parameter error, you should use -v -s number -E number -b number -t filename");
            return -1;
        }

    }

    // 地址剩余位数应该为 tag 的位数
    tagbits = sizeof(void*) * 8 - setsbits - blockbits;

    assert(tagbits > 0);

    // cache块数
    long count = (1 << setsbits) * linesperset;

    // tag 和 used 用来模拟 cache
    // calloc: malloc and memset 0
    long* tagptr = calloc(count, sizeof(long));
    long* usedptr = calloc(count, sizeof(long));

    if (usedptr == NULL || tagptr == NULL){
        fprintf(stderr, "calloc error.");
        return -1;
    }

    fp = fopen(filename, "r");

    char buff[1024];
    char number[1024];
    long addr;

    long set;
    long tag;

    while (fgets(buff, 1024, fp) != NULL) {
        memset(number, 0, 1024);

        // 忽略 I 开头的行
        if(*(buff) == 'I')continue;

        // 去掉行尾回车
        if(buff[strlen(buff)-1] == '\n')buff[strlen(buff)-1] = '\0';

        // 去掉行头空格
        if(infoflag && buff[0] == ' ') {
            printf("%s ", buff + 1);
        }
        else if (infoflag) {
            printf("%s ", buff);
        }

        // 得到地址 存于 number
        getaddrfromline(buff, number);
        sscanf(number, "%lx", &addr);

        // 得到地址对应的 tag set
        tag = getlownnumber(addr >> (blockbits + setsbits), tagbits);
        set = getlownnumber(addr >> blockbits, setsbits);

        // cache 行
        long* nowptr = usedptr + set * linesperset;

        long* this = nowptr;

        bool allone = true;
        
        // 试图找到 tag
        for (int i=0; i < linesperset; i++) {
            if (*this != 0) {
                if (*(tagptr + set * linesperset + i) == tag) {
                    if (*(buff+1) == 'M') {
                        print("hit hit\n");
                        hit_count++;
                    }
                    else {
                        print("hit\n");
                    }
                    (*this) = max++;
                    hit_count++;
                    goto ff;
                }
            }
            else allone = false;
            this++;
        }

        long index = 0;

        // 有空余块
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
                print("miss hit\n");
                hit_count++;
                *this = max++;
            }
            else {
                print("miss\n");
            }
            miss_count++;
        }

        // 驱逐
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
                print("miss eviction hit\n");
                hit_count++;
            }
            else {
                print("miss eviction\n");
            }
            miss_count++;
            eviction_count++;
        }
        ff:;
    }

    printf("hit: %ld ", hit_count);
    printf("miss: %ld ", miss_count);
    printf("eviction: %ld\n", eviction_count);
    printSummary(hit_count, miss_count, eviction_count);
    free(tagptr);
    free(usedptr);
    fclose(fp);
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
    for (long i=1; i<n; i++) x = x | (x << 1);
    return x & addr;
}
