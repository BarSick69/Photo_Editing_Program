#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"

// TODO(maxim.durnea): Task 1
int ***flip_horizontal(int ***image, int N, int M) {
    for (int i = 0; i < N; i++) {
        int y = 1;
        for (int j = 0; j < M / 2; j++) {
            int R = image[i][j][0];
            int G = image[i][j][1];
            int B = image[i][j][2];
            int pixel[3] = {R, G, B};
            for (int k = 0; k < 3; k++) {
                image[i][j][k] = image[i][M - y][k];
            }
            for (int k = 0; k < 3; k++) {
                image[i][M - y][k] = pixel[k];
            }
            y++;
        }
    }
    return image;
}

// TODO(maxim.durnea): Task 2
int ***rotate_left(int ***image, int N, int M) {
    int ***rotated = (int ***)malloc(M * sizeof(int **));
    for (int i = 0; i < M; i++) {
        rotated[i] = (int **)malloc(N * sizeof(int *));
        for (int j = 0; j < N; j++) {
            rotated[i][j] = malloc(3 * sizeof(int));
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            rotated[j][i][0] = image[i][M - j - 1][0];
            rotated[j][i][1] = image[i][M - j - 1][1];
            rotated[j][i][2] = image[i][M - j - 1][2];
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
    return rotated;
}

// TODO(maxim.durnea): Task 3
int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
    int ***cropped = (int ***)malloc(h * sizeof(int **));
    for (int i = 0; i < h; i++) {
        cropped[i] = (int **)malloc(w * sizeof(int *));
        for (int j = 0; j < w; j++) {
            cropped[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            cropped[i][j][0] = image[y + i][x + j][0];
            cropped[i][j][1] = image[y + i][x + j][1];
            cropped[i][j][2] = image[y + i][x + j][2];
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
    return cropped;
}

// TODO(maxim.durnea): Task 4
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    int extended_N = N + 2 * rows;
    int extended_M = M + 2 * cols;
    int ***extended = (int ***)malloc(sizeof(int **) * (extended_N));
    for (int i = 0; i < extended_N; i++) {
        extended[i] = (int **)malloc(sizeof(int *) * extended_M);
        for (int j = 0; j < extended_M; j++) {
            extended[i][j] = (int *)malloc(sizeof(int) * 3);
        }
    }
    for (int i = 0; i < extended_N; i++) {
        for (int j = 0; j < extended_M; j++) {
            if (i < rows || i >= N + rows || j < cols || j >= M + cols) {
                extended[i][j][0] = new_R;
                extended[i][j][1] = new_G;
                extended[i][j][2] = new_B;
            } else {
                extended[i][j][0] = image[i - rows][j - cols][0];
                extended[i][j][1] = image[i - rows][j - cols][1];
                extended[i][j][2] = image[i - rows][j - cols][2];
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return extended;
}
// TODO(maxim.durnea): Task 5
int ***paste(int ***image_dst, int N_dst, int M_dst, int ***image_src, int N_src, int M_src, int x, int y) {
    int k = 0;
    int l = 0;
    for (int i = y; i < N_dst && k < N_src; i++) {
        for (int j = x; j < M_dst && l < M_src; j++) {
            image_dst[i][j][0] = image_src[k][l][0];
            image_dst[i][j][1] = image_src[k][l][1];
            image_dst[i][j][2] = image_src[k][l][2];
            l++;
        }
        l = 0;
        k++;
    }
    return image_dst;
}

// TODO(maxim.durnea): Task 6
int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    const int MAX_VALUE = 255;
    int ***filtered = (int ***)malloc(sizeof(int **) * N);
    for (int i = 0; i < N; i++) {
        filtered[i] = (int **)malloc(sizeof(int *) * M);
        for (int j = 0; j < M; j++) {
            filtered[i][j] = (int *)malloc(sizeof(int) * 3);
        }
    }
    int filter_offset = filter_size / 2;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            float sum_r = 0.0f;
            float sum_g = 0.0f;
            float sum_b = 0.0f;
            for (int a = -filter_offset; a <= filter_offset; a++) {
                for (int b = -filter_offset; b <= filter_offset; b++) {
                    if (i + a >= 0 && i + a < N && j + b >= 0 && j + b < M) {
                        sum_r += (float)image[i + a][j + b][0] * filter[a + filter_offset][b + filter_offset];
                        sum_g += (float)image[i + a][j + b][1] * filter[a + filter_offset][b + filter_offset];
                        sum_b += (float)image[i + a][j + b][2] * filter[a + filter_offset][b + filter_offset];
                    }
                }
            }
            filtered[i][j][0] = ((int)sum_r > (int)MAX_VALUE) ? MAX_VALUE : (sum_r < 0) ? 0 : (int)sum_r;
            filtered[i][j][1] = ((int)sum_g > (int)MAX_VALUE) ? MAX_VALUE : (sum_g < 0) ? 0 : (int)sum_g;
            filtered[i][j][2] = ((int)sum_b > (int)MAX_VALUE) ? MAX_VALUE : (sum_b < 0) ? 0 : (int)sum_b;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
    return filtered;
}
