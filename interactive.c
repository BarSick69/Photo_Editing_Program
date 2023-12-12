#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#include "bmp.h"
#include "string.h"

#define SIZE 1000
#define MAX_IMAGINI 1000
float **create_filter(int size, float *values);
typedef struct {
    int N;
    int M;
    int ***pixeli;
} Imagine;

typedef struct {
    int size;
    float **values;
} Filter;

int main() {
    Imagine images[MAX_IMAGINI];
    Filter filters[MAX_IMAGINI];
    int N = 0, M = 0, index = 0, x = 0, y = 0, w = 0, h = 0, cols = 0;
    int rows = 0, r = 0, g = 0, b = 0, index_dst = 0, index_src = 0, filter_size = 0;
    int im_in = 0;
    int fil_in = 0;
    char path[SIZE];
    char comanda[SIZE];
    int images_size = 0;
    int filters_count = 0;
    while (1) {
        scanf(" %s", comanda);
        size_t len = strlen(comanda);

        if (comanda[len - 1] == '\n') {
            comanda[len - 1] = '\0';
        }

        if (strcmp(comanda, "e") == 0) {
            for (int s = 0; s < images_size; s++) {
                for (int i = 0; i < images[s].N; i++) {
                    for (int j = 0; j < images[s].M; j++) {
                        free(images[s].pixeli[i][j]);
                    }
                    free(images[s].pixeli[i]);
                }
                free(images[s].pixeli);
            }
            for (int s = 0; s< filters_count; s++) {
                for (int i= 0; i< filters[s].size; i++) {
                    free(filters[s].values[i]);
                }
                free(filters[s].values);
            }

            exit(0);
        } else if (strcmp(comanda, "l") == 0) {
            scanf("%d %d %s", &N, &M, path);
            images[images_size].pixeli = (int ***)malloc(N * sizeof(int **));
            for (int i = 0; i < N; i++) {
                images[images_size].pixeli[i] = (int **)malloc(M * sizeof(int *));
                for (int j = 0; j < M; j++) {
                    images[images_size].pixeli[i][j] = (int *)malloc(3 * sizeof(int));
                }
            }
            read_from_bmp(images[images_size].pixeli, N, M, path);
            images[images_size].N = N;
            images[images_size].M = M;
            images_size++;
        } else if (strcmp(comanda, "s") == 0) {
            scanf("%d %s", &index, path);
            write_to_bmp(images[index].pixeli, images[index].N, images[index].M, path);
        } else if (strcmp(comanda, "ah") == 0) {
            scanf("%d", &index);
            flip_horizontal(images[index].pixeli, images[index].N, images[index].M);
        } else if (strcmp(comanda, "ar") == 0) {
            scanf("%d", &index);
            int temp = images[index].N;
            images[index].pixeli = rotate_left(images[index].pixeli, images[index].N, images[index].M);
            images[index].N = images[index].M;
            images[index].M = temp;
        } else if (strcmp(comanda, "ac") == 0) {
            scanf("%d %d %d %d %d", &index, &x, &y, &w, &h);
            images[index].pixeli = crop(images[index].pixeli, images[index].N, images[index].M, x, y, h, w);
            images[index].N = h;
            images[index].M = w;
        } else if (strcmp(comanda, "ae") == 0) {
            scanf("%d %d %d %d %d %d", &index, &rows, &cols, &r, &g, &b);
            images[index].pixeli = extend(images[index].pixeli, images[index].N, images[index].M, rows, cols, r, g, b);
            images[index].M = images[index].M + 2 * cols;
            images[index].N = images[index].N + 2 * rows;
        } else if (strcmp(comanda, "ap") == 0) {
            scanf("%d %d %d %d", &index_dst, &index_src, &x, &y);
            int M_dst = images[index_dst].M;
            int N_dst = images[index_dst].N;
            int N_src = images[index_src].N;
            int M_src = images[index_src].M;
            paste(images[index_dst].pixeli, N_dst, M_dst, images[index_src].pixeli, N_src, M_src, x, y);
        } else if (strcmp(comanda, "cf") == 0) {
            scanf("%d", &filter_size);
            float filter[filter_size][filter_size];
            for (int i = 0; i < filter_size; i++) {
                for (int j = 0; j < filter_size; j++) {
                    scanf("%f", &filter[i][j]);
                }
            }
            filters[filters_count].values = (float **)malloc(sizeof(float *) * filter_size);
            for (int i = 0; i < filter_size; i++) {
                filters[filters_count].values[i] = (float *)malloc(sizeof(float) * filter_size);
            }
            for (int i = 0; i < filter_size; i++) {
                for (int j = 0; j < filter_size; j++) {
                    filters[filters_count].values[i][j] = filter[i][j];
                }
            }
            filters[filters_count].size = filter_size;
            filters_count++;
        } else if (strcmp(comanda, "af") == 0) {
            scanf("%d %d", &im_in, &fil_in);
            int N_img = images[im_in].N;
            int M_img = images[im_in].M;
            int fil_s = filters[fil_in].size;
            images[im_in].pixeli = apply_filter(images[im_in].pixeli, N_img, M_img, filters[fil_in].values, fil_s);
        } else if (strcmp(comanda, "di") == 0) {
            scanf("%d", &im_in);
            for (int i = 0; i < images[im_in].N; i++) {
                for (int j = 0; j < images[im_in].M; j++) {
                    free(images[im_in].pixeli[i][j]);
                }
                free(images[im_in].pixeli[i]);
            }
            free(images[im_in].pixeli);
            for (int i = im_in; i < images_size-1; i++) {
                images[i] = images[i+1];
            }
            images_size = images_size-1;
        } else if (strcmp(comanda, "df") == 0) {
            scanf("%d", &fil_in);
            for (int i = 0; i < filters[fil_in].size; i++) {
                free(filters[fil_in].values[i]);
            }
            free(filters[fil_in].values);
            for (int i = fil_in; i < filters_count-1; i++) {
                filters[i] = filters[i+1];
            }
            filters_count = filters_count-1;
        }
    }

    return 0;
}
