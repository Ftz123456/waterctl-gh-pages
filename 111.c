#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265
#define WIDTH 800
#define HEIGHT 600

typedef struct {
    int x;
    int y;
    int r;
    int g;
    int b;
} Heart;

void draw_heart(int fbfd, struct fb_var_screeninfo vinfo, struct fb_fix_screeninfo finfo, Heart heart) {
    long int screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    char *fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    for (int y = -heart.r; y <= heart.r; y++) {
        for (int x = -heart.r; x <= heart.r; x++) {
            if (x * x + y * y <= heart.r * heart.r) {
                int px = heart.x + x;
                int py = heart.y - y;
                if (px >= 0 && px < vinfo.xres && py >= 0 && py < vinfo.yres) {
                    long int location = (px + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                                       (py + vinfo.yoffset) * finfo.line_length;
                    *(fbp + location) = heart.b; // blue
                    *(fbp + location + 1) = heart.g; // green
                    *(fbp + location + 2) = heart.r; // red
                }
            }
        }
    }

    munmap(fbp, screensize);
}

int main() {
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;

    // 打开framebuffer设备文件
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        return 1;
    }

    // 获取framebuffer的可变参数信息
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error: reading variable screen info");
        return 1;
    }

    // 获取framebuffer的固定参数信息
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error: reading fixed screen info");
        return 1;
    }

    // 计算framebuffer的总大小
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // 将framebuffer映射到内存中
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    vinfo.bits_per_pixel = 32;
    vinfo.xres = WIDTH;
    vinfo.yres = HEIGHT;

    // 创建一个爱心结构
    Heart heart = {400, 300, 50, 255, 0, 0};

    int blinking = 0;  // 用于控制闪烁
    int num_blinks = 5;  // 闪烁次数
    int blink_delay = 500000;  // 闪烁的延迟时间，单位为微秒

    while (num_blinks > 0) {
        if (blinking) {
            draw_heart(fbfd, vinfo, finfo, heart);
        } else {
            // 清空屏幕
            memset(fbp, 0, screensize);
        }

        blinking = !blinking;
        usleep(blink_delay);
        num_blinks--;
    }

    // 关闭framebuffer设备文件
    close(fbfd);

    return 0;
}