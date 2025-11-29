# **TRADUCTOR MORSE SONORO**
 

**Autores: Mantaras Ramiro - Santangelo Ezequiel**


**Fecha: 2do cuatrimestre 2025**

 



 

### **1\. Selección del proyecto a implementar**

#### **1.1 Objetivo del proyecto y resultados esperados**

El objetivo de este proyecto es diseñar e implementar un sistema de comunicacion entre dos usuarios a travez de MORSE, facilitandole el acceso a quienes no conozcan el codigo, funcionando asi como "traductor". Se busca además garantizar una comunicación clara y confiable entre los dispositivos, implementando detección precisa de señales sonoras, y una correcta decodificación y visualización de los mensajes en texto.

El uso de código Morse puede justificarse en dos contextos concretos, aunque es importante reconocer que no es una tecnología moderna ni eficiente, sino un mecanismo mínimo que se mantiene vigente solo como backup extremo.
En la Educación (Pues se sigue instruyendo en MORSE en escuelas navales y aeronáuticas). Y en situaciones de Emergencia o muy mala calidad de comunicaciones de radio; Donde el MORSE funciona como minimo absoluto; con un canal de un solo bit (Ya sea un boton, un cable, luz, sonido) y cierta cantidad de tiempo, se es capaz de trasmitir cualquier mensaje.

Algunos escenarios reales donde tener un traductor MORSE seria efectivo son...

1. Grupos aislados de Montaña, donde no hay señal; y se quieren comunicar a media distancia a travez de un valle que esta lo suficientmente lejos para que no llegue el sonido.
2. Comunicación silenciosa entre patrullas costeras. Se quiere enviar un mensaje, pero usar radio podria delatar su posición o ser interceptada.
3. Comunicaciones entre dos barcos pesqueros a media distancia, con VHF inhabilitado (bloqueado o saturado).
4. Caso educativo, donde se entrena en comunicaciones de bajo ancho de banda, y obliga a pensar en terminos de señales binarias.

En conclusion, llevar un traductor Morse compacto puede ser útil en emergencias, pero no porque Morse sea moderno, sino porque es el último recurso cuando todo lo demás falla.

#### **1.2 Proyectos similares**

1. Traductor Morse luminoso (óptico). 

2. Traductor Morse por vibración

3. Traductor Morse Sonoro

Para comparar estas alternativas, se tienen en cuenta cuatro aspectos característicos:

1. Disponibilidad del hardware: Se evalúa si el proyecto es fácilmente implementable con hardware disponible en mercados regionales.

4. Tiempo de implementación: Tiempo total que llevará diseñar e implementar el proyecto seleccionado.

5. Costo: El costo total del proyecto.

6. Interés personal

#### **1.3 Selección de proyecto**

En términos de hardware, los sensores lumínicos resultarían difíciles de conseguir y, sobre todo, de adaptar, ya que es muy fácil que sufran interferencias. Un fotodiodo, por ejemplo, si bien no es demasiado costoso, implica un gasto considerable y requiere apuntarlo con mucha precisión; además, para aumentar su alcance sería necesario incorporar una lente. Por estas razones, la solución lumínica se presenta como la más compleja y demandante.

Por otro lado, los sensores de vibración pueden verse afectados por el ruido del entorno, al igual que los sensores de sonido. Sin embargo, la vibración se transmite a través de un sólido, mientras que el sonido lo hace por el aire. Dado que ya hemos trabajado con sensores de sonido y sabemos que podemos filtrar las frecuencias a voluntad, decidimos optar por desarrollar un TRADUCTOR MORSE SONORO.


Los desafíos principales de este proyecto son la integración de las diferentes tecnologías (microfono , teclado matricial para escribir los caracteres que seran traducidos, parlante…), la optimización energética y el eficiente uso de recursos.

###### **1 Diagrama en bloques**

En la Figura 1.3.1 se muestra el diagrama en bloques del sistema con los principales módulos del proyecto

<p align="center">
  <img src="images\diagrama en bloques.png" alt="image1">
</p>

<p align="center"><em>Figura 1: Diagrama en bloques del sistema</em></p>


### **2\. Elicitación de requisitos y casos de uso**

Existen varios productos similares y app de celulares que cumplen la función de "entrenador de MORSE"; pero lo que nosotros queremos implementar también es un traductor (tanto emisor como receptor), algo que por lo menos no es común en el mercado.

Como primer competidor esta la app de Play Store "Learn and Traslate" link: https://play.google.com/store/apps/details?id=com.puzan.morsecode; Pero solo traduce "manualmente" es decir uno tiene que escribir el codigo escuchado; ademas de que es una velocidad fija. Sin embargo, aunque nuestro proyecto requiera hacerlo automaticamente, nos servira para probar y experimentar ciertas funciones

Un segundo competidor es https://www.mercadolibre.com.ar/pantalones-cortos-morse-cw-telegraph-radio-de-onda-corta-par/p/MLA2039077129?matt_tool=38087446&pdp_filters=item_id%3AMLA2469142614&from=gshop&utm_source=chatgpt.com, el cual es el tipico pedal de morse para radio de ondas cortas (entre 3 MHz y 30 MHz).

En resumen, el mercado es escaso (Debido mayormente a que puede llegar a verse como "una comunicacion del pasado"; aunque en ciertos contextos muy eficiente).

En la Tabla 2.1 se presentan los requisitos y casos de uso

| Grupo                   | ID  | Descripción                                                                                                                                                                                                         |
| :---------------------- | :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Entrada                 | 1.1 | El sistema contará con un micrófono para captar señales sonoras.                                                                                                                                                    |
|                         | 1.2 | El sistema tendrá un botón que permita generar un código Morse manualmente.                                                                                                                                         |
|                         | 1.3 | El sistema contará con un teclado matricial (se está considerando la posibilidad de reemplazarlo por una aplicación Bluetooth en el celular) que permitirá enviar caracteres para ser reproducidos en código Morse. |
| Indicadores             | 2.1 | El sistema contará con un indicador luminoso para indicar si se está recibiendo la señal Morse correctamente.                                                                                                       |
|                         | 2.2 | El sistema se conectará mediante Bluetooth al celular, para indicar el carácter Morse que se está recibiendo.                                                                                                       |
|                         | 2.3 | El sistema contará con un indicador luminoso que indique si está en modo "Emisión" o "Recepción".                                                                                                                   |
|                         | 2.4 | El sistema notificará mediante el titileo de LEDs cuando ocurra un error al recibir el código.                                                                                                                      |
|                         | 2.5 | El sistema notificará con audio y luz cuando se deje un código sin introducir completamente.                                                                                                                        |
| Parlante                | 3.1 | El sistema contará con un parlante (o zumbador, a definir) para reproducir el código Morse que se desea emitir.                                                                                                     |
|                         | 3.2 | El sistema solo podrá emitir cuando no esté recibiendo, ya que las señales sonoras podrían interferirse.                                                                                                            |
| Comunicación Bluetooth  | 4.1 | El sistema se comunicará mediante Bluetooth con la aplicación del celular.                                                                                                                                          |
|                         | 4.2 | El sistema deberá enviar a la aplicación los caracteres recibidos (ya habiendo procesado el código Morse).                                                                                                          |
|                         | 4.3 | El sistema deberá poder recibir caracteres desde la aplicación (en caso de no usar un teclado matricial) para codificarlos en Morse y luego reproducirlos.                                                          |
| Aplicación              | 5.1 | La aplicación deberá permitir al usuario escribir caracteres.                                                                                                                                                       |
|                         | 5.2 | La aplicación deberá poder mostrar en pantalla los caracteres recibidos.                                                                                                                                            |
| Interruptores / Botones | 6.1 | El sistema contará con un DIP switch que permitirá controlar la velocidad de emisión.                                                                                                                               |



<p align="center"><em>Tabla 2.1: Requisitos del proyecto</em></p>

En las tablas se presentan 2 casos de uso para el sistema.

| **Elemento**            | **Definición**                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           |
| :---------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Disparador**          | El usuario escribe un texto en la aplicación móvil y solicita enviarlo en código Morse.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  |
| **Precondiciones**      | El sistema está encendido.<br>El modo de operación está en “Emisión”.<br>El parlante (o zumbador) está conectado y funcional.<br>La aplicación móvil está conectada por Bluetooth.                                                                                                                                                                                                                                                                                                                                                                                                                                                                       |
| **Flujo principal**     | 1. El usuario ingresa un texto en la aplicación móvil.<br>2. La aplicación envía el texto al sistema mediante Bluetooth.<br>3. El microcontrolador traduce los caracteres recibidos a código Morse (puntos y rayas).<br>4. El sistema genera las señales sonoras correspondientes a cada carácter Morse a través del parlante.<br>5. Durante la transmisión, se enciende el indicador luminoso de “Emisión”.<br>6. Una vez finalizada la transmisión, el sistema notifica a la aplicación que el envío ha concluido.                                                                                                                                     |
| **Flujos alternativos** | a. **Pérdida de conexión Bluetooth:** el sistema no recibe el texto completo. Se muestra un aviso de error en la aplicación y el LED de error parpadea.<br> b. **Modo incorrecto:** si el sistema está en modo “Recepción”, ignora la orden de transmisión y enciende el indicador correspondiente.<br> c. **Error de codificación:** si el texto contiene caracteres no reconocidos por el traductor Morse, el sistema los omite y notifica en la aplicación.<br> d. **Interrupción del usuario:** si se presiona el botón de cancelación o se apaga el sistema durante la emisión, la transmisión se detiene y se indica mediante un destello del LED. |


<p align="center"><em>Tabla 2.2: Caso de uso 1: El usuario quiere emitir MORSE</em></p>


| **Elemento**            | **Definición**                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   |
| :---------------------- | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Disparador**          | Se desea recibir una señal sonora en código Morse para decodificarla y mostrarla en la aplicación móvil.                                                                                                                                                                                                                                                                                                                                                                                                                                         |
| **Precondiciones**      | El sistema está encendido.<br>El modo de operación está en “Recepción”.<br>El micrófono está conectado y operativo.<br>La aplicación móvil está conectada por Bluetooth y lista para mostrar los caracteres recibidos.                                                                                                                                                                                                                                                                                                                           |
| **Flujo principal**     | 1. El sistema se encuentra en modo “Recepción”.<br>2. El micrófono capta una señal sonora en código Morse.<br>3. El microcontrolador analiza la duración de los pulsos de sonido (puntos y rayas).<br>4. Se decodifica cada secuencia y se traduce a su carácter correspondiente (letra, número o símbolo).<br>5. Los caracteres decodificados se envían a la aplicación móvil mediante Bluetooth.<br>6. La aplicación muestra el texto recibido en pantalla.<br>7. El indicador luminoso de “Recepción” permanece encendido durante el proceso. |
| **Flujos alternativos** | a. **Interferencia o ruido ambiental:** el sistema no puede reconocer correctamente la señal; activa el LED de error.<br> b. **Código incompleto:** si la señal se interrumpe, el sistema notifica con audio y luz que el mensaje quedó incompleto.<br> c. **Modo incorrecto:** si el sistema está en “Emisión”, ignora las señales entrantes y enciende el indicador correspondiente.                                                                                                                                                           |


<p align="center"><em>Tabla 2.3: Caso de uso 2: El usuario quiere recibir MORSE</em></p>