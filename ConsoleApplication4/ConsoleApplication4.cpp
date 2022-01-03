#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world454d.lib")
#else
#pragma comment(lib, "opencv_world454.lib")
#endif



int main(void)
{
    FILE *fp;
    errno_t error;
    char str[20];
    int configData;
    int ret;

    error = fopen_s(&fp,"C:\\Users\\isoff\\WORK_SPACE\\config.csv", "r");
    
    if (error != 0) {
        printf("not file\n");
        exit(EXIT_FAILURE);
    }
    while (ret=fscanf(fp, "%[^,] , %d", str, &configData) != EOF) {
        
        printf("%d\n",configData);
    }
    fclose(fp);
    
    error = fopen_s(&fp, "C:\\Users\\isoff\\WORK_SPACE\\config.csv", "w");
    if (error != 0) {
        printf("not file\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "Pp , 2");
    fclose(fp);


    return 0;
}