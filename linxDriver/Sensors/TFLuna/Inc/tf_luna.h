#ifndef LIBRERIA_TF_LUNA_H
#define LIBRERIA_TF_LUNA_H


#include <stdint.h>
#include <stdbool.h>


#define TF_LUNA_FRAME_SIZE  9
#define TF_LUNA_HEADER      0x59

/*Tipo de dato que contiene las tres mediciones del Sensor TF-Luna*/
typedef struct
{
    uint16_t distance_cm;
    uint16_t signal_strength;
    float temperature_c;
} TFLuna_t;

/*Función que se encarga de verificar el Buffer y parsear los datos divididos*/
bool parse_tfluna_frame(const uint8_t *frame, TFLuna_t *output_data);

#endif /* LIBRERIA_TF_LUNA_H */