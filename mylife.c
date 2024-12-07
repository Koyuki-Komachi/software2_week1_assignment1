#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()関数を使う
//#include "gol.h"

#define height 40
#define width 70

void my_init_cells(int cell[height][width], FILE* fp) {
    if (fp == NULL){
        cell[30][20] = 1;
        cell[30][22] = 1;
        cell[31][22] = 1;
        cell[31][23] = 1;
        cell[32][20] = 1;
    }else {
        int height_point, width_point;
        if (fgetc(fp) == '#'){
            while (fscanf(fp, "%d %d\n", &height_point, &width_point) != EOF) {
                cell[height_point][width_point] = 1;
            }
        }else {
            printf("this is not the correct file");
        }
        }
}

void my_print_cells(FILE* fp, int gen, int cell[height][width]) {
    printf("generation = %d\n", gen);
    cell[0][0] = '+';
    cell[0][width - 1] = '+';
    cell[height - 1][0] = '+';
    cell[height - 1][width - 1] = '+';
    for (int i = 1; i < width - 1; ++i) {
        cell[0][i] = '-';
        cell[height - 1][i] = '-';
    }
    for (int j = 1; j < height; ++j) {
        cell[j][0] = '|';
        cell[j][width - 1] = '|';    
    }
    for (int k = 0; k < width; ++k) {
        for (int l = 0; l < height; ++l) {
            printf("%c ", cell[k][l]);
        }
        printf("\n");
    }
}

void my_update_individual(int k, int l, int cell[height][width], int copy_cell[height][width]) {
    if (cell[k][l] == 1) {
        int count = 0;
        for (int m = k - 1; m <= k + 1; ++m) {
            for (int n = l - 1; n <= l + 1; ++n) {
                if (cell[m][n] == 1) {
                    count++;
                }
            }
        }
        if (count != 3 && count != 4) {
            copy_cell[k][l] = 0;
        }else {
            copy_cell[k][l] = 1; 
        }
    }else if (cell[k][l] == 0) {
        int count = 0;
        for (int m = k - 1; m <= k + 1; ++m) {
            for (int n = l - 1; n <= l + 1; ++n) {
                if (cell[m][n] == 1) {
                    count++;
                }
            }
        }
        if (count == 3) {
            copy_cell[k][l] = 1;
        }else {
            copy_cell[k][l] = 0;
        }
    }
}

void my_update_cells(int cell[height][width]) {
    int copy_cell[height][width] = {0};
    for (int k = 0; k < height; ++k) {
        for (int l = 0; l < width; ++l) {
            my_update_individual(k, l, cell, copy_cell);
        }
    }
    for (int k = 0; k < height; ++k) {
        for (int l = 0; l < width; ++l) {
            cell[k][l] = copy_cell[k][l];
        }
    }
}


int main(int argc, char **argv) {
    FILE *fp = stdout;
    
    int cell[height][width];
    for(int y = 0 ; y < height ; y++){
        for(int x = 0 ; x < width ; x++){
            cell[y][x] = 0;
        }
    }
    
    /* ファイルを引数にとるか、ない場合はデフォルトの初期値を使う */
    if ( argc > 2 ) {
        fprintf(stderr, "usage: %s [filename for init]\n", argv[0]);
        return EXIT_FAILURE;
    }
    else if (argc == 2) {
        FILE *lgfile;
        if ( (lgfile = fopen(argv[1],"r")) != NULL ) {
            my_init_cells(cell,lgfile); // ファイルによる初期化
        }
        else{
            fprintf(stderr,"cannot open file %s\n",argv[1]);
            return EXIT_FAILURE;
        }
        fclose(lgfile);
    }
    else{
        my_init_cells(cell, NULL); // デフォルトの初期値を使う
    }
    
    my_print_cells(fp, 0, cell); // 表示する
    sleep(1); // 1秒休止
    fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)
    
    /* 世代を進める*/
    for (int gen = 1 ;; gen++) {
        my_update_cells(cell); // セルを更新
        my_print_cells(fp, gen, cell);  // 表示する
        sleep(1); //1秒休止する
        fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)
    }
    
    return EXIT_SUCCESS;
}
