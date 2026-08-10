#Creamos una libreria estatica con el archivo fuente .c
add_library(TFLuna_Driver STATIC Src/tf_luna.c)

#Le indicamos a CMake donde encontrar los Headers de la biblioteca
target_include_directories(TFLuna_Driver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Inc)

#Si llegamos a usar otras bibliotecas tambien la incluimos por si acaso
target_include_directories(TFLuna_Driver PRIVATE stm32cubemx)