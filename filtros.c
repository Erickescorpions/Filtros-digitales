#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>

#define PI 3.14159265358979323846
#define NUM_COEFICIENTES_FIR 1001

float* leer_coeficientes_fir(char* file_name) {
  FILE* h1_file = fopen(file_name, "r");
  char buffer[10000];
  while (fgets(buffer, 10000, h1_file) != NULL);

  // ahora los separamos y guardamos en un array de floats
  float* h = (float*) malloc(NUM_COEFICIENTES_FIR * sizeof(float));
  int index = 0;

  // vamos a separar los numeros segun tengan un espacio
  char* token = strtok(buffer, " ");
  do
  {
    h[index++] = atof(token);
  }
  while (token = strtok(NULL, " "));

  return h;
}

float* generar_oscilador(int frecuencia, int frecuencia_muestreo, int N) {
  float w0 = (2 * PI * frecuencia) / frecuencia_muestreo;
  // condiciones iniciales
  float b0 = sin(w0);
  float a1 = 2 * cos(w0);
  float a2 = -1;

  // reservamos espacio en memoria
  float* y = (float*) calloc(N, sizeof(float)); 

  // condiciones iniciales
  y[0] = b0;
  y[1] = a1 * b0;

  // generamos el resto
  for(size_t n = 2; n < N; n++) {
    y[n] = a1 * y[n - 1] - y[n - 2];
  }

  return y;
}

void fn_a_archivo(float* fn, const char* nombre_archivo, int longitud) {
  FILE* archivo = fopen(nombre_archivo, "w");

  for(size_t i = 0; i < longitud; i++) {
    fprintf(archivo, "%.6f\n", fn[i]);
    //printf("%f - %d\n", fn[i], i);
  }

  fclose(archivo);
}

float* leer_datos_audio(char* file_name, int N) {
  FILE* audio = fopen("audio.txt", "r");
  float* x = (float*)malloc(N * sizeof(float));
  // leemos los datos del archivo 
  for (size_t i = 0; i < N; i++) {
    fscanf(audio, "%f", &x[i]);
  }
  fclose(audio);
  return x;
}

void filtrar_fir(float* x, float* y, float* h, int N) {
  for(size_t n = 0; n < N; n++) {
    y[n] = 0;
    for(size_t k = 0; k < NUM_COEFICIENTES_FIR; k++) {
      if (n >= k) {
        y[n] += h[k] * x[n - k];
      }
    }
  }
}

float* filtrado_fir(float* x_con_tonos, int N) {
  float* y_fir1 = calloc(N, sizeof(float));
  float* h1 = leer_coeficientes_fir("h1.dat");
  filtrar_fir(x_con_tonos, y_fir1, h1, N);

  float* y_fir2 = calloc(N, sizeof(float));
  float* h2 = leer_coeficientes_fir("h2.dat");
  filtrar_fir(y_fir1, y_fir2, h2, N);

  free(y_fir1);

  return y_fir2;
}

void filtrar_iir(float* x, float* y, float* b, float* a, int N, int orden) {
    for (int n = 0; n < N; n++) {
        y[n] = b[0] * x[n];
        for (int k = 1; k <= orden; k++) {
            if (n >= k) {
                y[n] += (b[k] * x[n - k]) - (a[k] * y[n - k]);
            }
        }
    }
}

float* filtrado_iir(float* x_con_tonos, int N) {
  float* y_iir1 = calloc(N, sizeof(float));
  // 1 Hz float a1[5] = {1, -3.91535267862329, 5.83027638967055, -3.91100622817273, 0.997781024105975};
  // 20 Hz float a1[5] = {1,	-3.87449923483303,	5.70896727769985,	-3.78936806974068,	0.956543676511205};
  // 2 Hz float a1[5] = {1,	-3.91318066566859,	5.82380756732881,	-3.90449740140184,	0.995566972065976};
  // float a1[5] = {1,	-3.91101107186694,	5.81734818072363,	-3.89800060101838,	0.993357832954140};
  // float a1[5] = {1,	-3.90667914678258,	5.80445765264257,	-3.88504296728877,	0.988954249933127};
  float a1[5] = {1,	-3.90235691321360,	5.79160468137406,	-3.87213310328513,	0.984570188122245};
  // 1 Hz float b1[5] = {0.998889895887223, -3.91317945339799, 5.83027762200205, -3.91317945339799, 0.998889895887223};
  // 20 Hz float b1[5] = {0.978030479206557,	-3.83193365228684,	5.70944999579791,	-3.83193365228684,	0.978030479206557};
  // 2 Hz float b1[5] = {0.997781024102940,	-3.90883903353521,	5.82381249118888,	-3.90883903353521,	0.997781024102940};
  // float b1[5] = {0.996673383272341,	-3.90450583644268,	5.81735924713312,	-3.90450583644268,	0.996673383272341};
  // float b1[5] = {0.994461788958190,	-3.89586105703565,	5.80448832465927,	-3.89586105703565,	0.994461788958190};
  float b1[5] = {0.992255101887975,	-3.88724500824937,	5.79166466572036,	-3.88724500824937,	0.992255101887975};
  filtrar_iir(x_con_tonos, y_iir1, b1, a1, N, 4);

  float* y_iir2 = calloc(N, sizeof(float));
  // 1 Hz float a2[5] = {1, -3.57056106470926, 5.18500612057761, -3.56659736896527, 0.997781024105974};
  // 20 Hz float a2[5] = {1,	-3.53330523420561,	5.07701640350459,	-3.45567084251169,	0.956543676511206};
  // 2 Hz float a2[5] = {1,	-3.56858032235355,	5.17925242263558,	-3.56066171888400,	0.995566972065975};
  // float a2[5] = {1,	-3.56660178611677,	5.17350657358631,	-3.55473703612912,	0.993357832954143};
  //float a2[5] = {1,	-3.56265133661426,	5.16203837416222,	-3.54292047034736,	0.988954249933128};
  float a2[5] = {1,	-3.55870972517827,	5.15060142632686,	-3.53114746762049,	0.984570188122246};
  // 1 Hz float b2[5] = {0.998889895887226, -3.56857921683726, 5.18500735290913, -3.56857921683726, 0.998889895887226};
  //20 Hz float b2[5] = {0.978030479206561,	-3.49448803835865,	5.07749912160267,	-3.49448803835865,	0.978030479206561};
  // 2 Hz float b2[5] = {0.997781024102941,	-3.56462102061877,	5.17925734649567,	-3.56462102061877,	0.997781024102941};
  //float b2[5] = {0.996673383272338,	-3.56066941112295,	5.17351763999579,	-3.56066941112295,	0.996673383272338};
  //float b2[5] = {0.994461788958194,	-3.55278590348080,	5.16206904617895,	-3.55278590348080,	0.994461788958194};
  float b2[5] = {0.992255101887974,	-3.54492859639938,	5.15066141067315,	-3.54492859639938,	0.992255101887974};
  filtrar_iir(y_iir1, y_iir2, b2, a2, N, 4);

  free(y_iir1);

  return y_iir2;
}

void graficar(const char* file_name) {
  if(fork() == 0) {
    // Este es el proceso hijo
    execlp("gnuplot", "gnuplot", "-p", file_name, NULL);
    perror("Error al ejecutar Gnuplot para cuantizacion_qi2.gp");
    exit(1);
  }
}


int main() {

  int f01 = 259;
  int f02 = 594;
  // recuperamos la frecuencia de muestreo del archivo de audio
  int Fs = 8000;
  int duracion = 5;
  int N = Fs * duracion;

  float* x = leer_datos_audio("audio.txt", N);

  // generamos el oscilador de 259hz
  float* os1 = generar_oscilador(f01, Fs, N);
  // generamos el oscilador de 594 hz
  float* os2 = generar_oscilador(f02, Fs, N);

  fn_a_archivo(os1, "os1.dat", N);
  fn_a_archivo(os2, "os2.dat", N);

  float* x_con_tonos = (float*)malloc(N * sizeof(float));

  // Sumar la señal de entrada y los tonos
  for (int i = 0; i < N; i++) {
    x_con_tonos[i] = x[i] + os1[i] + os2[i];
  }

  fn_a_archivo(x, "audio.dat", N);
  fn_a_archivo(x_con_tonos, "x_tonos.dat", N);
  
  float* y_fir = filtrado_fir(x_con_tonos, N);
  fn_a_archivo(y_fir, "y_fir.dat", N);

  float* y_iir = filtrado_iir(x_con_tonos, N);
  fn_a_archivo(y_iir, "y_iir.dat", N);

  // Liberar memoria
  free(x); free(os1); free(os2); free(x_con_tonos); free(y_fir); free(y_iir);

  // graficamos
  graficar("audio.gp");
  graficar("osciladores.gp");
  graficar("seniales_filtradas.gp");

  return 0;
}