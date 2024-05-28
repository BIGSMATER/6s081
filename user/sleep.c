// user/sleep.c（需新建sleep.c文件）
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h" // 一开始做实验，并不清楚需要什么头文件，可以模仿其他程序，尝试

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "Usage: sleep ticks\n");
        exit(1); // 当参数个数不等于所要求的2时，模仿其他程序输出错误提示
    }
    sleep(atoi(argv[1])); // 使用atoi()函数将string参数转化为sleep()所需的int
    exit(0);
}
