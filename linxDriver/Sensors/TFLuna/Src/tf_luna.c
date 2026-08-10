#include "tf_luna.h"

/*Función que se encarga de verificar el Buffer y parsear los datos divididos*/
bool parse_tfluna_frame(const uint8_t *frame, TFLuna_t *output_data)
{
    /*Hacemos comprobación de los dos Bytes Header o de comprobación*/
    if (frame[0] != 0x59 || frame[1] != 0x59)
    {
        return false;
    }
    /*Hacemos la suma manualmente de los bits del 1-8*/
    uint16_t checksum = 0;
    for (int i = 0; i < 8; i++)
    {
        checksum += frame[i];
    }
    /*Le ponemos una mascara de 8 bits a nuestra variable de 16 bits para tomar en cuenta solo el byte menos significativo*/
    if ((checksum & 0xFF) != frame[8])
    {
        return false;
    }
    /*Hacemos el parseo ya que el resultado de 16 bits nos lo entrega partido en 2 (8 bits y 8 bits) y los pegamos en little endian*/
    output_data->distance_cm = (frame[3] << 8) | frame[2]; //Distancia
    output_data->signal_strength = (frame[5] << 8) | frame[4]; //Saturación de luz

    output_data->temperature_c = ((float)((frame[7] << 8) | frame[6]) / 8.0f) - 256.0f; //Temperatura

    return true;
}