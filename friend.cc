#include "common.h"

using namespace std;

static inline unsigned long rdtscp();
static void get_tsc();
static void printint(uint64_t i);

using uint64_t = __UINT64_TYPE__;

int main() {
    int i, sleepsec = 1;
    char zero[4000];
    F2H f2h;

    puts(f2h, "hello!");
    
    memset((void *)0x310000UL, 0, 0x5000);
    
    uint64_t *pml4t = (uint64_t *)0x310000UL;
    uint64_t *pdpt = (uint64_t *)0x311000UL;
    uint64_t *pd1 = (uint64_t *)0x312000UL;
    uint64_t *pd2 = (uint64_t *)0x313000UL;
    uint64_t *pt1 = (uint64_t *)0x314000UL;
    uint64_t *pt2 = (uint64_t *)0x315000UL;
    
    pml4t[0] = ((uint64_t)pdpt + 0x80000000UL) | (1 << 0) | (1 << 1) | (1 << 2);
    pdpt[0] = ((uint64_t)pd1 + 0x80000000UL) | (1 << 0) | (1 << 1) | (1 << 2);
    pdpt[3] = ((uint64_t)pd2 + 0x80000000UL) | (1 << 0) | (1 << 1) | (1 << 2);
    pd2[0] = ((uint64_t)pt1 + 0x80000000UL) | (1 << 0) | (1 << 1) | (1 << 2);
    pd2[1] = ((uint64_t)pt2 + 0x80000000UL) | (1 << 0) | (1 << 1) | (1 << 2);
    
    for (int i = 0; i < 512; i++) {
        pd1[i] = (0x80000000UL + 0x200000UL * i) | (1 << 0) | (1 << 1) | (1 << 2) | (1 << 7);
    }
    
    asm volatile("movq %0, %%cr3" ::"r"((uint64_t)pml4t + 0x80000000UL));
    
    for (int i = 0; i < 512; i++) {
        pt1[i] = (0x80500000UL + 0x1000UL * i) | (1 << 0) | (1 << 1) | (1 << 2);
    }
    
    pt2[0] = 0x80700000UL | (1 << 0) | (1 << 1) | (1 << 2);
    
    for (i = 0; i < 4001; i++){
        zero[i] = 0;
    }
    
    /*
    puts(f2h, "2MB Memory Writing(Not Huge Page)");
    get_tsc();
    for (i = 0; i < 2097152; i++) {
        pt1[i] = zero[i];
    }
    get_tsc();
     */
    
    for (;;) {
        get_tsc();
    }
    
    /*
    puts(f2h, "2MB Memory Writing(Huge Page)");
    
    pd2[1] = pd2[1] | (1 << 7);
    get_tsc();
    
    for (i = 1; i < 2097152; i++) {
        pt2[i] = zero[i];
    }
    get_tsc();
    */
    
    
    puts(f2h, "bye!");
    
    return_value(f2h, 0);
    
    return 0;
}

static inline uint64_t rdtscp()
{
    uint32_t hi, lo, aux;
    __asm__ volatile("rdtscp" : "=a" (lo), "=d" (hi), "=c" (aux));
    return ((uint64_t)hi << 32) | lo;
}

static void printint(uint64_t num) {
    F2H f2h;
    char buf[10];
    int y = 0;
    int num2 = num;
    while(num2 != 0) {
        y++;
        num2 /= 10;
    }
    for (int x = y - 1; x >= 0; x--) {
        buf[x] = '0' + (num % 10);
        num /= 10;
    }
    buf[y] = '\0';
    puts(f2h, buf);
}

static void get_tsc()
{
#define MAXU64 (0xFFFFFFFFFFFFFFFF)
    static uint64_t prev = MAXU64;
    char send[30], toprint[30];
    int i, c, d, temp, tempb;
    double tempc;
    uint64_t tsc;
    F2H f2h;
    tsc = rdtscp();
    
    printint(tsc);
}

