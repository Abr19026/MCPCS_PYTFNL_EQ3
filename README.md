# Código proyecto final equipo 3 - Sensor cardiaco

- El código final (todavía está en progreso) está en la carpeta **/CodigoProyecto**
- Además, el código final usa 2 librerias, el código de las librerías está en la carpeta **/Include**
- El diagrama del circuito en fritzing es **Diagrama.fzz**
- La carpeta **Pruebas_Modulos** tiene los códigos antiguos para probar el sensor, pantalla de segmentos y el Bluetooth individialmente

## Pasos de compilación
1. Para poder compilar e instalar el código es necesario instalar el [arduino-cli](https://arduino.github.io/arduino-cli/0.31/installation/)
  
2. Si tienes **Make** (incluido en linux), compilar y subir es muy fácil  [(Make para Windows)](https://gnuwin32.sourceforge.net/downlinks/make.php)
    * Para compilar, escribir en la linea de comandos:  
    `make compile`
    * Para subir el código al arduino escribir en la linea de comandos:  
     `make upload`  
  
    __NOTA 1__: Probablemente tendras que configurar el puerto conectado al arduino, en el archivo **Makefile** cambiar el valor de la variable SERIAL_PORT al que esté conectado a tu arduino  

    __NOTA 2__: Para cambiar el Sketch a compilar, cambiar el valor de la variable SKETCH en **Makefile** 
    a la dirección de la carpeta que tiene al sketch .ino dentro, por ejemplo, si quiero compilar PruebaSegmentos, cambiaría sketch así:  
    `SKETCH ?= Pruebas_Modulos/PruebaSegmentos`

    Si no quieres instalar Make, seguir las siguientes instrucciones
3. Para compilar se necesita abrir una terminal en esta carpeta y escribir:  
    `arduino-cli compile "$<" -b arduino:avr:uno --build-path="$@" --build-cache-path="$@" --libraries ./Include`  
    donde
    - `$<` es {Nombre_del_Sketch}/{Nombre_del_sketch}.ino
    - `$@` es el lugar donde se va a guardar el resultado de la compilación

4. Para subir el código al arduino ejecutar el siguiente comando:  
    `arduino-cli upload --input-dir "$@" --fqbn arduino:avr:uno -p $(SERIAL_PORT)`  
    donde
    - $@ es el lugar donde guardaste el resultado de la compilación
    - $(SERIAL_PORT) es el nombre del puerto conectado al arduino