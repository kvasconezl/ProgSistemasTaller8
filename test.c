#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <string.h>   // String function definitions
#include <unistd.h>   // para usleep()
#include <getopt.h>
#include <math.h>

#include "arduino-serial-lib.h"

float calculateSD(float data[]);

void error(char* msg)
{
    fprintf(stderr, "%s\n",msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int fd = -1;
	int baudrate = 9600;  // default
	int c = 0;
	char t = 't';
	char h = 'h';
	char s = 'i';
	char on = '1';
	char off = '0';
	int temperatura[12];
	int humedad[12];
	int temporal;
	float medtemperatura;
	float medhumedad;
	

	fd = serialport_init("/dev/ttyACM0", baudrate);

	if( fd==-1 )
	{
		error("couldn't open port");
		return -1;
	}
	
	
	while (1) {
		write(fd, &t, 1);
		usleep(2500000);
		read(fd, &temporal, 1 );
		temperatura[c] = temporal;
		printf("Temperatura: %d\n", temporal);
		
		write(fd, &h, 1);
		usleep(2500000);
		read(fd, &temporal, 1);
		humedad[c] = temporal;
		printf("Humedad: %d\n", temporal);
		if (c == 11) {
			medtemperatura = (float)calculateSD(temperatura);
			medhumedad = (float)calculateSD(humedad);
			printf("La media de la temperatura es: %.2f y su desviación es: %.2f\n", medtemperatura, pow(medtemperatura, 2));
			printf("La media de la humedad es: %.2f y su desviación es: %.2f\n", medhumedad, pow(medhumedad, 2));
			c = -1;
		}
		c++;
	}
	
	
	close( fd );
	return 0;	
}

/* Ejemplo para calcular desviacion estandar y media */
float calculateSD(float data[])
{
    float sum = 0.0, mean, standardDeviation = 0.0;

    int i;

    for(i = 0; i < 10; ++i)
    {
        sum += data[i];
    }

    mean = sum/10;

    for(i = 0; i < 10; ++i)
        standardDeviation += pow(data[i] - mean, 2);

    return sqrt(standardDeviation / 10);
}

