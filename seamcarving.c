// Prerequisites

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "seamcarving.h"

// struct rgb_img{
//     uint8_t *raster;
//     size_t height;
//     size_t width;
// };

// void create_img(struct rgb_img **im, size_t height, size_t width){
//     *im = (struct rgb_img *)malloc(sizeof(struct rgb_img));
//     (*im)->height = height;
//     (*im)->width = width;
//     (*im)->raster = (uint8_t *)malloc(3 * height * width);
// }

// int read_2bytes(FILE *fp){
//     uint8_t bytes[2];
//     fread(bytes, sizeof(uint8_t), 1, fp);
//     fread(bytes+1, sizeof(uint8_t), 1, fp);
//     return (  ((int)bytes[0]) << 8)  + (int)bytes[1];
// }

// void write_2bytes(FILE *fp, int num){
//     uint8_t bytes[2];
//     bytes[0] = (uint8_t)((num & 0XFFFF) >> 8);
//     bytes[1] = (uint8_t)(num & 0XFF);
//     fwrite(bytes, 1, 1, fp);
//     fwrite(bytes+1, 1, 1, fp);
// }

// void read_in_img(struct rgb_img **im, char *filename){
//     FILE *fp = fopen(filename, "rb");
//     size_t height = read_2bytes(fp);
//     size_t width = read_2bytes(fp);
//     create_img(im, height, width);
//     fread((*im)->raster, 1, 3*width*height, fp);
//     fclose(fp);
// }

// void write_img(struct rgb_img *im, char *filename){
//     FILE *fp = fopen(filename, "wb");
//     write_2bytes(fp, im->height);
//     write_2bytes(fp, im->width);
//     fwrite(im->raster, 1, im->height * im->width * 3, fp);
//     fclose(fp);
// }

// uint8_t get_pixel(struct rgb_img *im, int y, int x, int col){
//     return im->raster[3 * (y*(im->width) + x) + col];
// }

// void set_pixel(struct rgb_img *im, int y, int x, int r, int g, int b){
//     im->raster[3 * (y*(im->width) + x) + 0] = r;
//     im->raster[3 * (y*(im->width) + x) + 1] = g;
//     im->raster[3 * (y*(im->width) + x) + 2] = b;
// }

// void destroy_image(struct rgb_img *im)
// {
//     free(im->raster);
//     free(im);
// }


// void print_grad(struct rgb_img *grad){
//     int height = grad->height;
//     int width = grad->width;
//     for(int i = 0; i < height; i++){
//         for(int j = 0; j < width; j++){
//             printf("%d\t", get_pixel(grad, i, j, 0));
//         }
//     printf("\n");    
//     }
// }

// Part 1

int R_x(struct rgb_img *im, int y, int x)
{
    uint8_t prev = get_pixel(im, y, (x-1 + im->width) % im->width, 0);
    uint8_t next = get_pixel(im, y, (x+1) % im->width, 0);
    return (next - prev);
}
int G_x(struct rgb_img *im, int y, int x)
{
    uint8_t prev = get_pixel(im, y, (x-1 + im->width) % im->width, 1);
    uint8_t next = get_pixel(im, y, (x+1) % im->width, 1);
    return (next - prev);
}
int B_x(struct rgb_img *im, int y, int x)
{
    uint8_t prev = get_pixel(im, y, (x-1 + im->width) % im->width, 2);
    uint8_t next = get_pixel(im, y, (x+1) % im->width, 2);
    return (next - prev);
}
int R_y(struct rgb_img *im, int y, int x)
{
    uint8_t prev = get_pixel(im, (y-1 + im->height) % im->height, x, 0);
    uint8_t next = get_pixel(im, (y+1) % im->height, x, 0);
    return (next - prev);
}
int G_y(struct rgb_img *im, int y, int x)
{
    uint8_t prev = get_pixel(im, (y-1 + im->height) % im->height, x, 1);
    uint8_t next = get_pixel(im, (y+1) % im->height, x, 1);
    return (next - prev);
}
int B_y(struct rgb_img *im, int y, int x)
{
    uint8_t prev = get_pixel(im, (y-1 + im->height) % im->height, x, 2);
    uint8_t next = get_pixel(im, (y+1) % im->height, x, 2);
    return (next - prev);
}
int delta_x(struct rgb_img *im, int y, int x)
{
    int R = pow(R_x(im, y, x), 2);
    int G = pow(G_x(im, y, x), 2);
    int B = pow(B_x(im, y, x), 2);
    return (R + G + B);
}
int delta_y(struct rgb_img *im, int y, int x)
{
    int R = pow(R_y(im, y, x), 2);
    int G = pow(G_y(im, y, x), 2);
    int B = pow(B_y(im, y, x), 2);
    return (R + G + B);
}
int calc_pixel_energy(struct rgb_img *im, int y, int x)
{
    return sqrt(delta_x(im, y, x) + delta_y(im, y, x));
}
void calc_energy(struct rgb_img *im, struct rgb_img **grad) // right - left in the x direction, bottom - top in the y direction; next - prev in both directions
{
    *grad = (struct rgb_img *)malloc(sizeof(struct rgb_img));
    (*grad)->height = im->height;
    (*grad)->width = im->width;
    (*grad)->raster = (uint8_t *)malloc(3 * im->height * im->width);
    for (int y = 0; y < im->height; y++)
    {
        for (int x = 0; x < im->width; x++)
        {
            int pixel_energy = calc_pixel_energy(im, y, x) / 10;
            set_pixel(*grad, y, x, (uint8_t)pixel_energy, 0, 0);
        }
    }
}

// Part 2

/* void loop_list_maker(struct rgb_img *grad, int x, int *p_list[])
{
    if (x == 0) {memcpy(p_list, (int[]){1,2}, 2 * sizeof(int));}
    else if (x == grad->width) {memcpy(p_list, (int[]){0, 1}, 2 * sizeof(int));}
    else {memcpy(p_list, (int[]){0,1,2}, 3 * sizeof(int));}
}

int best_prev_index(struct rgb_img *grad, int y, int x)
{
    int prev_energies[3];
    int index[3];
    loop_list_maker(grad, x, &index);
    int start = index[0];
    int end = (sizeof(index)/sizeof(index[0]));
    for (int i = start; i < end; i++)
    {
        prev_energies[i] = get_pixel(grad, y - 1, x - 1 + i, 0);
    }
    int lowest_energy_index = 0;
    int lowest_energy = INFINITY;
    for (int j = start; j < end; j++)
    {
        if (prev_energies[j] < lowest_energy)
        {
            lowest_energy = prev_energies[j];
            lowest_energy_index = j;
        }
    }
    return lowest_energy_index;
}
int lowest_prev_cost(struct rgb_img *grad, int y, int x)
{
    int prev_energies[3];
    int index[3];
    loop_list_maker(grad, x, &index);
    int start = index[0];
    int end = (sizeof(index)/sizeof(index[0]));
    for (int i = start; i < end; i++)
    {
        prev_energies[i] = get_pixel(grad, y - 1, x - 1 + i, 0);
    }
    return fmin(fmin(prev_energies[0], prev_energies[1]), prev_energies[2]);
}

int best_prev_cost(struct rgb_img *grad, int *best_arr, int y, int x)
{
    int prev_costs[3];
    int index[3];
    loop_list_maker(grad, x, &index);
    int start = index[0];
    int end = (sizeof(index)/sizeof(index[0]));
    for (int i = start; i < end; i++)
    {
        prev_costs[i] = best_arr[(y - 1) * grad->width + (x - 1 + i)];
    }
    return fmin(fmin(prev_costs[0], prev_costs[1]), prev_costs[2]);
}

int opt_path_cost(struct rgb_img *grad, int* best_arr, int y, int x)
{
    int cur_cost = get_pixel(grad, y, x, 0);
    if ((y - 1) < 0)
    {
        return cur_cost;
    }
    int optimal_prev_index = best_prev_index(grad, y, x);
    int optimal_prev_cost = lowest_prev_cost(grad, y, x);
    return cur_cost + opt_path_cost(grad, best_arr, y - 1, x - 1 + optimal_prev_index);
}
void dynamic_seam(struct rgb_img *grad, double **best_arr)
{
    int height = grad->height;
    int width = grad->width;
    (*best_arr) = malloc(height * width);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            (*best_arr)[y * grad->width + x] = opt_path_cost(grad, best_arr, y, x);
        }
    }
} */

// Recursive approach did not work out, loops seem easier at this point


void initialize_first_row(struct rgb_img *grad, double **best_arr, int width)
{
    for (int x = 0; x < width; x++)
    {
        (*best_arr)[x] = get_pixel(grad, 0, x, 0);
    }
}

double min_prev_cost(int height, int width, int y, int x, double **best_arr, int *index)
{
    double min_prev_cost = INFINITY;
    for (int i = -1; i < 2; i++)
    {
        if (x + i >= 0 && x + i < width)
        {
            double cur_path_cost = (*best_arr)[(y - 1) * width + (x + i)];
            if (cur_path_cost < min_prev_cost)
            {
                min_prev_cost = cur_path_cost;
                *index = i;
            }
        }
    }
    return min_prev_cost;
}

void fill_best_arr(struct rgb_img *grad, double **best_arr, int height, int width)
{
    for (int y = 1; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index; // useless here but the index parameter is used in part 3
            double best_prev_cost = min_prev_cost(height, width, y, x, best_arr, &index);
            double cur_cost = (double)get_pixel(grad, y, x, 0);
            (*best_arr)[y * width + x] = cur_cost + best_prev_cost;
        }
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr)
{
    int height = grad->height;
    int width = grad->width;
    (*best_arr) = (double *)malloc(height * width * sizeof(double));
    initialize_first_row(grad, best_arr, width);
    fill_best_arr(grad, best_arr, height, width);
}

void print_best_arr(double *best_arr, int height, int width) // needed for debugging
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            printf("%f ", best_arr[y * width + x]);
        }
        printf("\n");
    }
}

// Part 3

int find_lowest_cost(double *best_arr, int height, int width)
{
    double min_cost = 2147483646; // whenever I tried infinity, it defaulted to this number
    int index = -1;
    for (int i = 0; i < width; i++)
    {
        double cur_cost = best_arr[(height - 1) * width + i];
        if (cur_cost < min_cost)
        {
            min_cost = cur_cost;
            index = i;
        }
    }
    return index;
}

void reverse_list(int *p_list, int size)
{
    int start = 0;
    int end = size - 1;
    while (start < end) 
    {
        int temp = p_list[start];
        p_list[start] = p_list[end];
        p_list[end] = temp;
        start++;
        end--;
    }

}

void recover_path(double *best_arr, int height, int width, int **path)
{
    (*path) = (int *)malloc(height * sizeof(int));
    (*path)[0] = find_lowest_cost(best_arr, height, width);
    int x = (*path)[0];
    int index;
    for (int y = height - 1; y > 0; y--)
    {
        int prev_cost = min_prev_cost(height, width, y, x, &best_arr, &index); // the cost itself is useless, we are only looking for the index here
        (*path)[height - y] = x + index;
        x += index;
    }
    reverse_list(*path, height);
}

void print_path(int *path, int height) //needed for debugging
{
    for (int i = 0; i < height; i++)
    {
        printf("%d ", path[i]);
    }
}

// Part 4

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path)
{
    int width = src->width - 1;
    int height = src->height;
    create_img(dest, height, width);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int seam_x = path[y];
            if (x >= seam_x)
            {
                set_pixel(*dest, y, x, get_pixel(src, y, x + 1, 0), get_pixel(src, y, x + 1, 1), get_pixel(src, y, x + 1, 2));
            }
            else
            {
                set_pixel(*dest, y, x, get_pixel(src, y, x, 0), get_pixel(src, y, x, 1), get_pixel(src, y, x, 2));
            }
        }
    }

}

// int main()
// {
//     // // debug tests:
//     // struct rgb_img *grad;
//     // struct rgb_img *im;
//     // read_in_img(&im, "6x5.bin");
  
//     // calc_energy(im,  &grad);
  
//     // print_grad(grad);
//     // double *best_arr;
//     // dynamic_seam(grad, &best_arr);
//     // print_best_arr(best_arr, grad->height, grad->width);

//     // int *path;
//     // recover_path(best_arr, grad->height, grad->width, &path);
//     // print_path(path, grad->height);

//     // Guerzhoy tests aka part 5:
//      struct rgb_img *im;
//     struct rgb_img *cur_im;
//     struct rgb_img *grad;
//     double *best;
//     int *path;

//     read_in_img(&im, "HJoceanSmall.bin");
//     int i;
//     for(i = 0; i < 150; i++){
//         printf("i = %d\n", i);
//         calc_energy(im,  &grad);
//         dynamic_seam(grad, &best);
//         recover_path(best, grad->height, grad->width, &path);
//         remove_seam(im, &cur_im, path);

//         // char filename[200];
//         // sprintf(filename, "img%d.bin", i);
//         // write_img(cur_im, filename);


//         destroy_image(im);
//         destroy_image(grad);
//         free(best);
//         free(path);
//         im = cur_im;
//     }
//     char filename[200];
//     sprintf(filename, "img%d.bin", i);
//     write_img(cur_im, filename);
//     destroy_image(im);
// }
