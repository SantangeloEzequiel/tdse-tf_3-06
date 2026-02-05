 # **Traductor Morse Sonoro**
 ## **Informe de Avance**

### **Autores:**

* Ezequiel Santangelo \- Padrón 112762

* Ramiro Mántaras \- Padrón 111510

*Fecha: 01/02/2026*

**2do cuatrimestre 2025**

A continuación se detalla según la siguiente tabla el estado de avance de cada uno de los requerimientos planteados en el documento de especificaciones.

| Estado | Descripción      |
|-----|---------------------|
| 🟢 | Ya implementado|
| 🟡 | En proceso de implementación |
| 🟠 | Se implementará|
| 🔴 | No se implementará |

### *Entrada*

|ID|Descripción|Estado|
| -- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|1.1|El sistema contará con un micrófono para captar señales sonoras.|🟢|
|1.2|El sistema tendrá un botón que permita generar un código Morse manualmente.|🟢|
|1.3|El sistema contará con un teclado matricial (se está considerando la posibilidad   de reemplazarlo por una aplicación Bluetooth en el celular) que permitirá enviar caracteres para ser reproducidos en código Morse.|🟡|

* El módulo micrófono se encuentra implementado y funcional (tarea task_mic).
* No habrá teclado matricial, se implementará mediante Bloetooth.

### *Indicadores*

|ID|Descripción|Estado|
| :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|2.1|El sistema contará con un indicador luminoso para indicar si se está recibiendo la señal Morse correctamente.|🟠|
|2.2|El sistema se conectará mediante Bluetooth al celular, para indicar el carácter Morse que se está recibiendo.|🟡|
|2.3|El sistema contará con un indicador luminoso que indique si está en modo "Emisión" o "Recepción".|🟢|
|2.4|El sistema notificará mediante el titileo de LEDs cuando ocurra un error al recibir el código.|🟠|
|2.5|El sistema notificará con audio y luz cuando se deje un código sin introducir completamente.|🟠|

### *Parlante*

|ID|Descripción|Estado|
| :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|3.1|El sistema contará con un parlante (o zumbador, a definir) para reproducir el código Morse que se desea emitir.|🟠|
|3.2|El sistema solo podrá emitir cuando no esté recibiendo, ya que las señales sonoras podrían interferirse.|🟠|

* Se decidió que se tratará de un zumbador para facilitar la detección sonora.

### *Comunicación Bluetooth*

|ID|Descripción|Estado|
| :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|4.1|El sistema se comunicará mediante Bluetooth con la aplicación del celular.|🟡|
|4.2|El sistema deberá enviar a la aplicación los caracteres recibidos (ya habiendo procesado el código Morse).|🟠|
|4.3|El sistema deberá poder recibir caracteres desde la aplicación (en caso de no usar un teclado matricial) para codificarlos en Morse y luego reproducirlos.|🟡|

* La tarea hc_05 se encuentra implementada y funcional. Queda por implementar la lógica del sistema que interpreta los caracteres recibidos y enviados.

### *Aplicación*

|ID|Descripción|Estado|
| :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|5.1|La aplicación deberá permitir al usuario escribir caracteres.|🟠|
|5.2|La aplicación deberá poder mostrar en pantalla los caracteres recibidos.|🟢|


### *Interruptores/Botones*

|ID|Descripción|Estado|
| :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|6.1|El sistema contará con un DIP switch que permitirá controlar la velocidad de emisión.|🟡|

* Se encuentra implementada la tarea task_sensor, que se encarga del sensado del DIP switch. La funcionalidad de cambio de velocidad, sin embargo, no.



### *ADICIONAL: APP DE ANDROID*

|ID|Descripción|Estado|
| :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|7.1|La aplicación deberá permitir al usuario escribir caracteres y mostrar en pantalla su traduccion a Morse.|🟢|
|7.2|La aplicación deberá permitir al usuario escribir Morse y mostrar en pantalla su traduccion a texto.|🟢|
|7.3|La aplicación deberá recibir señales de audio Morse de un emisor indeterminado, y poder traducirlo.|🟡|
|7.4|La aplicación deberá tener un apartado "HELP" que guiara al usuario sobre su funcionamiento.|🟠|

* Para probar el algoritmo de detección de señales, primero se decidió implementarlo en consola. A partir de eso, y ya teniendo el código funcionando, surgió como posibilidad desarrollar una app de Android de traducción Morse, idea que también estuvo motivada por la falta de aplicaciones en la Play Store que permitan recibir audio como entrada (hay apps de Morse, pero en general no analizan audio).

De esta forma se creó MyFriendlyMorse, dejando la APK incluida en el repositorio. Si bien esta aplicación no entra dentro de los contenidos de la materia, ya que fue programada en otro entorno (Android API 21) y en otro lenguaje (Kotlin), consideramos oportuno mostrarla como una aplicación práctica del algoritmo desarrollado.

El algoritmo de detección y análisis de señales utiliza Goertzel para detectar qué frecuencia está presente en la señal de audio, evitando calcular una FFT completa (ya que realizar una FFT en cada ciclo de detección en tiempo real resulta muy costoso).

Primero se toma un segmento inicial de la señal y se usa Goertzel para ver en qué frecuencia hay mayor energía. Luego, una se vuelve a usar Goertzel para decidir cuándo hay señal y cuándo no (o cuando hay sonido y cuando no). Asi detectamos Morse.

Se asume que el tipo de emisor es indeterminado, y que pueden existir ruidos que alteren la emisión y la recepción. Esto implica que un dash no siempre va a durar tres veces lo que un dot (casi nunca pasa), y lo mismo ocurre con los silencios. Además, a priori no se conoce cuál es el tiempo base del punto.

Para manejar esto, se implementó un período inicial de aprendizaje (Learning Morse), en el cual se espera detectar al menos dos señales diferentes (un punto y una raya). El umbral que usamos fue...

raya ≥ 2 · (punto − 1)

Este criterio fue elegido porque fue el que mejor funcionó con el audio utilizado durante las pruebas.

La aplicación MyFriendlyMorse debería poder traducir correctamente el audio
https://www.youtube.com/watch?v=9A3tBINRdiE

utilizando una velocidad de reproducción de 0.75x.

Actualmente todavía falta mejorar el análisis de los silencios, lo cual será implementado en la brevedad. Con esto, las señales que podrían traducirse no serían tan específicas y el sistema sería más robusto frente a distintas condiciones de emisión.