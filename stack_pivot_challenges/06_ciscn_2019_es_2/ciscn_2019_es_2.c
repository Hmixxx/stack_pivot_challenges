#include <stdio.h>
#include <unistd.h>

void hack() {
    system("echo flag");
}

void vul() {
    char s[0x28];
    read(0, s, 0x30);
    printf("%s", s);
    read(0, s, 0x30);
}

int main() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    puts("Welcome, what's your name?");
    vul();
    return 0;
}
