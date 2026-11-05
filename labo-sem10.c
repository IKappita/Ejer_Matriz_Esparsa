#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estructura para almacenar las celdas esparsas (16 bytes)
typedef struct {
    int fila;
    int col;
    double valor;
} ElementoEsparso;

// Función para generar las matrices en texto con 95% de ceros
void genmat(const char *nombre_archivo, int M, int N) {
    FILE *f = fopen(nombre_archivo, "w");
    if (!f) {
        perror("Error al abrir el archivo");
        return;
    }

    int total_celdas = M * N;
    int nro_no_nulos = (int)(total_celdas * 0.05); // El 5% son elementos no nulos

    // Escribir dimensiones y número de elementos no nulos
    fprintf(f, "%d %d\n", M, N);
    fprintf(f, "%d\n", nro_no_nulos);

    for (int i = 0; i < nro_no_nulos; i++) {
        int fila = rand() % M;
        int col = rand() % N;
        double valor = (double)(rand() % 100 + 1) / 2.5; // Valor decimal aleatorio
        fprintf(f, "%d %d %.2f\n", fila, col, valor);
    }

    fclose(f);
}

// Función para convertir los archivos de texto a formato binario
void convertir_a_binario(const char *txt_path, const char *bin_path) {
    FILE *f_txt = fopen(txt_path, "r");
    FILE *f_bin = fopen(bin_path, "wb");
    if (!f_txt || !f_bin) {
        perror("Error en la conversión a binario");
        return;
    }

    int M, N, nonulos;
    if (fscanf(f_txt, "%d %d", &M, &N) != 2) {
        fclose(f_txt);
        fclose(f_bin);
        return;
    }
    fscanf(f_txt, "%d", &nonulos);

    ElementoEsparso elemento;
    for (int i = 0; i < nonulos; i++) {
        fscanf(f_txt, "%d %d %lf", &elemento.fila, &elemento.col, &elemento.valor);
        // Escritura inmediata a bajo nivel con fwrite
        fwrite(&elemento, sizeof(ElementoEsparso), 1, f_bin);
    }

    fclose(f_txt);
    fclose(f_bin);
}

// UNICO PUNTO DE ENTRADA (MAIN)
int main() {
    srand(time(NULL));

    int M = 1000, N = 1000, P = 1000; // Dimensiones de prueba (hasta 1000x1000)[cite: 1]

    printf("1. Generando matrices de texto...\n");
    genmat("matriz_a.txt", M, N);
    genmat("matriz_b.txt", N, P);
    printf("Matrices de texto generadas exitosamente.\n");

    printf("2. Convirtiendo matrices a formato binario...\n");
    convertir_a_binario("matriz_a.txt", "matriz_a.bin");
    convertir_a_binario("matriz_b.txt", "matriz_b.bin");
    printf("Archivos binarios generados correctamente.\n");

    return 0;
}