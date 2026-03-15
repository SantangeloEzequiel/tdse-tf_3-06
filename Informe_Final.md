<div align="center">
<img width="365" height="138" alt="Uba_fiuba_ingenieria_logo" src="https://github.com/user-attachments/assets/412e1968-b281-4fce-98f3-e37283acfc46" />

# **Traductor Morse Sonoro: Asistencia de aprendizaje** 

**Autores: Ezequiel Santangelo - 112762, Ramiro Mántaras - 111510**

**Fecha: 8/3/26**

**Cuatrimestre de cursada: 2do 2025**

<div align="justify">

### *Resumen*

Este trabajo es la implementación de un MPV (Mínimo Producto Viable) de un Traductor Morse Sonoro.
El objetivo del producto es la asistencia al aprendizaje para nuevos usuarios de este tipo de comunicación. Cuenta con funciones que permiten a un usuario no experimentado explorar esta codificación conocida a nivel mundial con feedback en tiempo real e intuitivo. Esto lo logra mediante una aplicación móvil sencilla que trabaja en conjunto con el dispositivo en modos de traducción Morse-Letra y Letra-Morse.

El proyecto fue desarrollado utilizando firmware Bare-Metal sobre un microcontrolador STM32F410RBT6 de arquitectura Cortex-M3. El sistema utiliza máquinas de estados modulares y jerárquicas por motivos de escalabilidad y organización.

El presente documento detalla las características técnicas del producto, modelos utilizados y evaluaciones de funcionamiento varias.

# Índice
1. [Introducción general](#introducción-general)
    - [1.1 Objetivos](#11-objetivos)
    - [1.2 Análisis de mercado](#12-análisis-de-mercado)
2. [Introducción específica](#introducción-específica)
    - [2.1 Requisitos del proyecto](#21-requisitos-del-proyecto)
    - [2.2 Casos de uso](#22-casos-de-uso)
    - [2.3 Elementos del hardware](#23-elementos-del-hardware)
        - [2.3.1 Pulsador](#231-pulsador)
        - [2.3.2 DIP Switch](#232-dip-switch)
        - [2.3.3 LEDs (Diodos Emisores de Luz)](#233-leds-diodos-emisores-de-luz)
        - [2.3.4 Micrófono Electret](#234-micrófono-electret)
        - [2.3.5 Amplificador](#235-amplificador)
        - [2.3.6 Buzzer 3,3kHz](#236-buzzer-33khz)
        - [2.3.7 Memoria E2PROM externa](#237-memoria-e2prom-externa)
        - [2.3.8 HC-05](#238-hc-05)
        - [2.3.9 Placa de desarrollo](#239-placa-de-desarrollo)
3. [Diseño e implementación](#diseño-e-implementación)
    - [3.1 Esquema general](#31-esquema-general)
    - [3.2 Esquemático Eléctrico](#32-esquemático-eléctrico)
        - [3.2.1 Preamplificador](#321-preamplificador)
        - [3.2.2 Memoria E2PROM](#322-memoria-e2prom)
        - [3.2.3 Buzzer](#323-buzzer)
        - [3.2.4 GPIO](#324-gpio)
    - [3.3 PCB](#33-pcb)
        - [3.3.1 Pinout](#331-pinout)
    - [3.4 Descripción de comportamiento](#34-descripción-de-comportamiento)
        - [3.4.1 Modo de Espera](#341-modo-de-espera)
        - [3.4.2 Modo de Recepción](#342-modo-de-recepción)
        - [3.4.3 Modo de Transmisión](#343-modo-de-transmisión)
    - [3.5 Firmware](#35-firmware)
        - [3.5.1 Task system](#351-task-system)
        - [3.5.2 Task sensor](#352-task-sensor)
        - [3.5.3 Task GPIO Output](#353-gpio-output)
        - [3.5.4 Task mic](#354-task-mic)
        - [3.5.5 Task HC-05](#355-task-hc-05)
        - [3.5.6 Memoria Morse](#356-memoria-morse)
    - [3.6 Aplicación Móvil](#36-aplicación-móvil)
4. [Ensayos y resultados](#ensayos-y-resultados)
    - [4.1 Medición y análisis de consumo](#41-medición-y-análisis-de-consumo)
    - [4.2 Medición y análisis de tiempos de ejecución (WCET)](#42-medición-y-análisis-de-tiempos-de-ejecución-wcet)
    - [4.3 Cálculo del factor de uso (U) de la CPU](#43-cálculo-del-factor-de-uso-u-de-la-cpu)
    - [4.4 Cumplimiento de requisitos](#44-cumplimiento-de-requisitos)
    - [4.5 Reporte de uso](#45-reporte-de-uso)
    - [4.6 Prueba de integración](#46-prueba-de-integración)
5. [Conclusiones](#conclusiones)
    - [5.1 Resultados obtenidos](#51-resultados-obtenidos)
    - [5.2 Próximos pasos](#52-próximos-pasos)
6. [Bibliografía](#bibliografía)


# **Introducción General**
## 1.1 Objetivos

El objetivo de este proyecto es diseñar e implementar un sistema de comunicacion entre dos usuarios a travez de MORSE, facilitandole el acceso a quienes no conozcan el codigo, funcionando asi como "traductor". Se busca además garantizar una comunicación clara y confiable entre los dispositivos, implementando detección precisa de señales sonoras, y una correcta decodificación y visualización de los mensajes en texto.

## 1.2 Análisis de Mercado

Existen varios productos similares y app de celulares que cumplen la función de "entrenador de MORSE"; pero lo que nosotros queremos implementar también es un traductor (tanto emisor como receptor), algo que por lo menos no es común en el mercado.

Como primer competidor esta la app de Play Store "Learn and Traslate" link: https://play.google.com/store/apps/details?id=com.puzan.morsecode; Pero solo traduce "manualmente" es decir uno tiene que escribir el codigo escuchado; ademas de que es una velocidad fija. Sin embargo, aunque nuestro proyecto requiera hacerlo automaticamente, nos servira para probar y experimentar ciertas funciones

Un segundo competidor es https://www.mercadolibre.com.ar/pantalones-cortos-morse-cw-telegraph-radio-de-onda-corta-par/p/MLA2039077129?matt_tool=38087446&pdp_filters=item_id%3AMLA2469142614&from=gshop&utm_source=chatgpt.com, el cual es el tipico pedal de morse para radio de ondas cortas (entre 3 MHz y 30 MHz).

En resumen, el mercado es escaso (Debido mayormente a que puede llegar a verse como "una comunicacion del pasado"; aunque en ciertos contextos muy eficiente).

# **Introducción Específica**

## 2.1 Requisitos del Proyecto

En la Tabla 2.1 se presentan los requisitos y casos de uso:

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

## 2.2 Casos de uso

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

## 2.3 Elementos del Hardware

### 2.3.1 Pulsador

<table>
  <tr>
    <td width="60%" valign="top">
      <p>Se utiliza un pulsador <strong>Impreso en 3D FDM</strong> como dispositivos de entrada principal.<br>
      Origen: https://www.thingiverse.com/thing:2870754</p>
      <p><strong>Funcionalidad:</strong></p>
      <ul>
        <li>Input de señal morse.</li>
      </ul>
      <p><strong>Implementación Técnica:</strong><br>
      Se utilizaron mecanismos de <em>debounce por software</em> para filtrar el ruido mecánico y garantizar una lectura limpia de la señal.</p>
    </td>
    <td width="40%" align="center">
        <img src="https://github.com/user-attachments/assets/f57e6ff3-a524-4114-a19a-018d5caccd6d" width="250">
      <br><br>
      <em>Figura 2.1: Pulsador impreso utilizado.</em>
    </td>
  </tr>
</table>

### 2.3.2 DIP Switch

<table>
  <tr>
    <td width="60%" valign="top">
      <p>Se utiliza un <strong>DIP Switch de 3 llaves</strong> como controlador de velocidad para el modo emisión.</p>
      <p><strong>Funcionalidad:</strong></p>
      <ul>
        <li>Control de velocidad.</li>
      </ul>
      <p><strong>Implementación Técnica:</strong><br>
      Se utilizaron mecanismos de <em>debounce por software</em> para filtrar el ruido mecánico y garantizar una lectura limpia de la señal.</p>
    </td>
    <td width="40%" align="center">
        <img width="250" alt="dip-switch" src="https://github.com/user-attachments/assets/2b3c45c3-40e8-4b28-9a40-6e6e99716785" />
      <br><br>
      <em>Figura 2.2: Pulsador DIP Switch de tres llaves.</em>
    </td>
  </tr>
</table>


### 2.3.3 LEDs (Diodos Emisores de Luz)

<table>
  <tr>
    <td width="60%" valign="top">
      <p>Se utilizan <strong>LEDs</strong> como indicadores de funcionamiento</p>
      <p><strong>Funcionalidad:</strong></p>
      <ul>
        <li>Indicadores de modo de operación.</li>
        <li>Indicadores de funcionamiento y errores.</li>
      </ul>
      <p><strong>Implementación Técnica:</strong><br>
      Se utilizaron diferentes modos de emisión (<strong>pulso, blink, on, off</strong>) para maximizar la capacidad de transmisión de información.</p>
    </td>
    <td width="40%" align="center">
        <img width="250" alt="dip-switch" src="https://github.com/user-attachments/assets/22028d4d-3719-473d-9a54-6f1f3edc266c" />
      <br><br>
      <em>Figura 2.3: LEDs de colores varios.</em>
    </td>
  </tr>
</table>

### 2.3.4 Micrófono Electret

<table>
  <tr>
    <td width="60%" valign="top">
      <p>Se utiliza un <strong>Micrfófono Electret</strong> como input alternativo, en conjunto con la llave morse.</p>
      <p><strong>Funcionalidad:</strong></p>
      <ul>
        <li>Input de señal sonora morse.</li>
      </ul>
      <p><strong>Implementación Técnica:</strong><br>
      Se utiliza el <i>algoritmo de Goertzel</i> para filtrar la señal y reaccionar solo a bandas de frecuencias específicas. En este caso 3,3kHz.<br>
      Además, cuenta con un preamplificador que adapta la señal para ser recibida de forma segura por el microcontrolador.</p>
    </td>
    <td width="40%" align="center">
        <img width="250" alt="dip-switch" src="https://github.com/user-attachments/assets/8173827e-c2bc-48f7-93ea-56fcd06a528e"/>
      <br><br>
      <em>Figura 2.4: Micrófono electret.</em>
    </td>
  </tr>
</table>

### 2.3.5 Amplificador

<table>
  <tr>
    <td width="60%" valign="top">
      <p>Se utiliza un <strong>Amplificador LM358</strong> para amplificar y desplazar el punto medio de la señal de entrada del micrófono.</p>
      <p><strong>Funcionalidad:</strong></p>
      <ul>
        <li>Preamplificador de entrada de micrófono.</li>
      </ul>
      <p><strong>Implementación Técnica:</strong><br>
      Se utilizan dos amplificadores inversores con referencia en 1,6V en cascada.</p>
    </td>
    <td width="40%" align="center">
        <img width="250" alt="Captura de pantalla 2026-03-08 200018" src="https://github.com/user-attachments/assets/cb178f40-7483-4f59-82b5-e5bf9e8a70c8" />
      <br><br>
      <em>Figura 2.5: IC LM358.</em>
    </td>
  </tr>
</table>

### 2.3.6 Buzzer 3,3kHz

<table>
  <tr>
    <td width="60%" valign="top">
      <p>Se utiliza un <strong>Buzzer con frecuencia de operación 3,3kHz</strong> para generar una señal sonora morse.</p>
      <p><strong>Funcionalidad:</strong></p>
      <ul>
        <li>Output de señal morse en modo emisión.</li>
        <li>Feedback de llave morse en modo recepción.</li>
      </ul>
      <p><strong>Implementación Técnica:</strong><br>
      Se utiliza un circuito amplificador de corriente con un transistor <strong>BC337</strong> como interfaz entre el Buzzer y el microcontrolador.</p>
    </td>
    <td width="40%" align="center">
        <img width="250" alt="Captura de pantalla 2026-03-08 200018" src="https://github.com/user-attachments/assets/458ab863-a26b-4782-89a6-cfe8072146ea" />
      <br><br>
      <em>Figura 2.6: Buzzer estándar.</em>
    </td>
  </tr>
</table>

### 2.3.7 Memoria E2PROM externa

<table>
  <tr>
    <td width="60%" valign="top">
      <p>Se utiliza un <strong>24C02</strong> para almacenar los diccionarios Morse-Letra y Letra-Morse.</p>
      <p><strong>Funcionalidad:</strong></p>
      <ul>
        <li>Almacena el diccionario Morse-Letra.</li>
      </ul>
      <p><strong>Implementación Técnica:</strong><br>
      Se utiliza un <i>árbol binario</i> para almacenar el diccionario Morse-Letra y un <i>vector</i> para almacenar el diccionario Letra-Morse.</p>
    </td>
    <td width="40%" align="center">
        <img width="250"  alt="Captura de pantalla 2026-03-08 202435" src="https://github.com/user-attachments/assets/e5b2d823-5daf-490c-b31d-4838418abdff" />
      <br><br>
      <em>Figura 2.7: IC 24C02.</em>
    </td>
  </tr>
</table>


### 2.3.8 HC-05

<table>
  <tr>
    <td width="60%" valign="top">
      <p>Se utiliza un <strong>HC-05</strong> como interfaz inalámbrica aplicación-dispositivo.</p>
      <p><strong>Funcionalidad:</strong></p>
      <ul>
        <li>Interfaz bidireccional aplicación-dispositivo.</li>
      </ul>
      <p><strong>Implementación Técnica:</strong><br>
        Se implementa un buzzer limitado rx y tx que transmite y recibe de forma periódica y bidireccional, siendo transparente para el dispositivo.<br>
      </p>
    </td>
    <td width="40%" align="center">
        <img width="250" alt="Captura de pantalla 2026-03-08 205942" src="https://github.com/user-attachments/assets/7c936b89-4113-4979-b566-258f468dad36" />
      <br><br>
      <em>Figura 2.8: HC-05.</em>
    </td>
  </tr>
</table>


### 2.3.9 Placa de desarrollo

<table>
  <tr>
    <td width="60%" valign="top">
      <p>Se utiliza una placa de desarrollo <strong>NUCLEO-F103RB</strong> para facilitar la programación, alimentación y coneccionado del microcontrolador, que contiene la lógica del programa.</p>
    </td>
    <td width="40%" align="center">
        <img width="250" alt="Captura de pantalla 2026-03-08 210435" src="https://github.com/user-attachments/assets/98879b1d-1af9-48f7-ac1a-fba764641a44" />
      <br><br>
      <em>Figura 2.9: Placa de desarrollo NUCLEO-F103RB.</em>
    </td>
  </tr>
</table>

# Diseño e implementación
En esta sección se detalla la implementación técnica y específica de los requisitos, comenzando por el hardware utilizado, y ascendiendo hacia el firmware y software.

## 3.1 Esquema general

<img width="2199" height="1080" alt="Diagrama_Bloques" src="https://github.com/user-attachments/assets/ee59740b-8430-4fa7-9ded-29057365209c" />
<p align="center"><em>Imagen 3.1.1: Diagrama en bloques del proyecto.</em></p>

### 3.2 Esquemático eléctrico

A continuación se expone el esquemático eléctrico completo del PCB tipo "Shield" diseñado, y se recorren las diferentes secciones:
<img width="1274" height="827" alt="Esquematico" src="https://github.com/user-attachments/assets/ebe2cb42-8f9c-438b-92c5-7c855c14b5b6" />
<p align="center"><em>Imagen 3.2.1: Esquemático eléctrico completo.</em></p>

### 3.2.1 Preamplificador
<img width="1524" height="690" alt="Esquematico_preamp" src="https://github.com/user-attachments/assets/41ffcb3a-a8f0-4520-9f80-232bb7240f29" />
<p align="center"><em>Imagen 3.2.1.1: Esquemático eléctrico del bloque preamplificador.</em></p>

El bloque preamplificador se divide a su vez en tres partes: El bloque micrófono, y ambos bloques en configuración amplificador inversor con referencia no nula.

El bloque micrófono no es más que la aplicación del circuito sugerido en la datasheet. Alimenta el micrófono electret y filtra la señal contínua con un capacitor de acople.

La señal de entrada, no obstante, se encuentra centrada en 0V y tiene valores pico imperceptibles al microcontrolador de unos 10mV, aproximadamente. Mediante la implementación de los bloques amplificadores se busca obtener una señal centrada en 1,6V, con valores pico cercanos a 1,6V. De este modo, la señal podrá tener máxima excursión simétrica y ser detectada correctamente por la entrada analógica del microcontrolador.

Es bien conocida la amplificación de tensión de este tipo de configuraciones: 

$$A_v = -\left( \frac{R_2}{R_1} \right)$$

En este caso, y aplicando ambas en cascada, se esperará una ganancia de 10000.

(CONTINUAR)

### 3.2.2 Memoria E2PROM
<img width="1246" height="483" alt="Esquematico_Memoria" src="https://github.com/user-attachments/assets/8660efd0-37db-4917-a437-718df6f2dfad" />
<p align="center"><em>Imagen 3.2.1.1: Esquemático eléctrico del bloque memoria.</em></p>

El conexionado de la memoria 24C02 (256B) es muy sencillo. Los terminales E0 , E1 y E2 asignan los últimos 3 dígitos de la dirección del integrado, para la comunicación I2C. Para la aplicación solo necesitamos uno, por lo que sus valores son arbitrarios siempre y cuando el firmware sepa la dirección resultante, en este caso 0x50.

Al tener una resistencia de pull-down interna, la protección contra escritura no se conecta, pues no la utilizaremos.

Finalmente, se analiza el t-rise obtenido con un osciloscopio para determinar la mayor resistencia admisible en la red I2C. En este caso 4K7.
<div align="center">
<img width="500" alt="CBus" src="https://github.com/user-attachments/assets/85878fe3-d6a7-4abc-b0b0-e73a91667231" />
<p align="center"><em>Imagen 3.2.1.2: Relación máx RL en función de la capacidad de la red CBUS.</em></p>
<div align="justify">

### 3.2.3 Buzzer
<div align="center">
<img width="350"  alt="Buzzer" src="https://github.com/user-attachments/assets/d4eb34ca-570f-4399-aecf-980437322201" />
<p align="center"><em>Imagen 3.2.3.1: Esquemático eléctrico del bloque memoria.</em></p>
<div align="justify">
    
El conexionado del buzzer requiere de la aplicación de un amplificador de corriente, pues, a parte de requerir una alimentación de 5V, también tiene una corriente de operación muy superior a la que puede entregar el microcontrolador por un solo pin GPIO.

Para solucionar el problema, se utiliza un simple transistor NPN en modo corte o saturación. La corriente esperada por el pin GPIO será de $$I_b = 2,6mA$$, mientras que la corriente máxima del Buzzer será de $$Ic = h_{fe} I_b \approx 156mA$$ para un $$h_{fe}$$ mínimo de 60.

### 3.2.4 GPIO
<div align="center">
<img width="1300" height="563" alt="gpio" src="https://github.com/user-attachments/assets/06898bf1-d845-4b37-a60c-11d67db9bb4c" />
<img width="303" height="324" alt="gpio2" src="https://github.com/user-attachments/assets/65cf87ac-d8e9-475f-b11b-0e49c88fe2c6" />
<p align="center"><em>Imagen 3.2.4.1: Esquemáticos de GPIOs.</em></p>
<div align="justify">

Se utilizan en este proyecto 9 puertos de uso general (GPIO), 5 entradas y 4 salidas. Aunque su aplicación es muy sencilla, se describe cada uno:

Inputs:
* <strong>Botón Input</strong>: Se utiliza el formato RPU, es decir, normalmente alto.
* <strong>DIP Switch de velocidad</strong>: Se escojen pines con RPD interna disponible, de este modo se elimina este componente de la placa, y el conexionado es directo.
* <strong>Status de HC-05</strong>: El HC-05 cuenta con una salida 0V/3,3V compatible para conexión directa con el microcontrolador, que indica si el dispositivo pudo o no establecer una conexión.

Outputs:
* <strong>LEDs indicadores</strong>: Se utiliza una salida de forma directa para alimentar los LEDs indicadores, con una resistencia de 1K. Esto produce corrientes $$I \approx 1,5mA$$, dependiendo del color.
* <strong>Disparador de Buzzer</strong>: Conectado directamente a la base del transistor de control del Buzzer.

## 3.3 PCB
Una vez diseñados y probados los diferentes bloques circuitales, se los implementó conjuntamente en un PCB que utiliza los pines Morpho de la placa NUCLEO-F103RB para montarse en formato shield.

<div align="center">
<img width="300" alt="placa1" src="https://github.com/user-attachments/assets/d9c0316a-09ce-4f30-a791-8306b42e8534" />
<br>
<img width="300" alt="placa2" src="https://github.com/user-attachments/assets/946c729e-bebc-4739-a669-2ddb47e51256" />
<img width="300" alt="placa3" src="https://github.com/user-attachments/assets/a157a106-970d-4ec2-8c31-e0ecb7047428" />

<p align="center"><em>Imagen 3.3.1.1: Versión final del PCB.</em></p>
<div align="justify">

El PCB final tiene tan solo tres salidas:
* Alimentación independiente (J1) (4,85V - 5,15V)
* Conector para llave morse (J2)
* Conector para HC05 (J4)

### 3.3.1 Pinout

Se utiliza el siguiente listado de conexiones para el conexionado del PCB:

| **Pin** | **Función** | **Implementación** |
| :------ |:------| :-------------- | 
| `PA0`      | ADC1 In | Entrada analógica de micrófono |
| `PA6`      | GPIO Out | Salida para Led_Info |
| `PA7`      | GPIO Out | Salida para Buzzer |
| `PA9`      | UART2 TX | RX de Hc-05 |
| `PA10`      | UART2 RX | TX de Hc-05 |
| `PB0`      | GPIO In | Entrada de Velocidad 3 (DIP Switch) |
| `PB4`      | GPIO Out | Salida para Led_Transmit |
| `PB5`      | GPIO Out | Salida para Led_Receive |
| `PB6`      | SCL | Canal SCL para memoria E2PROM |
| `PB7`      | SDA | Canal SDA para memoria E2PROM |
| `PB10`      | GPIO In | Entrada de STATUS del HC-05 |
| `PC0`      | GPIO In | Entrada de Velocidad 1 (DIP Switch) |
| `PC1`      | GPIO In | Entrada de Velocidad 2 (DIP Switch) |
| `PC7`      | GPIO In | Entrada de llave morse |

<p align="center"><em>Tabla 3.7.1: Tabla de pinouts.</em></p>

<div align="center">
<img width="1200" src="https://github.com/SantangeloEzequiel/tdse-tf_3-06/blob/Presentaci%C3%B3n-Final/images/Pinout.PNG?raw=true"/>
<p align="center"><em>Imagen 3.7.1.1: Distribución de pines de la placa NUCLEO-F103RB.</em></p>
<div align="justify">


## 3.4 Descripción de comportamiento

En esta sección se detalla el comportamiento programado y esperado del dispositivo, así como su forma correcta de uso. Se encuentra intrínsecamente relacionado con la tarea Task System, que se tratará en la sección siguiente.

### 3.4.1 Modo de espera

El dispositivo utiliza el terminal STATE del HC-05 para conocer el estado de la conexión bluetooth. En este modo, se le informa al usuario mediante el parpadeo de los leds EMISIÓN y RECEPCIÓN que el dispositivo se encuentra a la espera de conexión para comenzar a funcionar.

Este modo se activa siempre que no se encuentre una conexión estable, no solo al inicio de la operación.

### 3.4.2 Modo de Recepción

Este es el modo por defecto tras establecer una conexión. Se espera que, ya sea por medio de señales sonoras o utilizando la llave morse, el usuario ingrese una señal. Esta señal es transmitida a la aplicación móvil, que desencripta la información y la muestra en formato letra.

<strong>Importante:</strong> antes de ingresar una secuencia morse, se deben enviar al menos dos símbolos diferentes (PUNTO-LÍNEA) para que el algoritmo pueda aprender la velocidad estimada del usuario.

### 3.4.3 Modo de Transmisión

Si el usuario lo desea, puede cambiar por medio de la aplicación entre los modos Recepción y Transmisión. En este modo, se espera que el usuario escriba en formato LETRA por medio de la aplicación, para convertirlo a señales sonoras.

Se puede controlar la velocidad de transmisión utilizando el DIP Switch (SW1). Estos valores de velocidad van desde el mínimo esperable de un usuario de nivel inicial, hasta el de un profesional (5-40ppm). 

## 3.5 Firmware

El firmware funciona en modo Bare-Metal (sin sistema operativo), y se basa en la ejecución cíclica de diferentes tareas modulares que controlan diferentes etapas del dispositivo.

La base de tiempo es de 1ms (1 tick), fijado por la interrupción `HAL_SYSTICK_Callback`. Por ello, es importante que la ejecución de todas las tareas se efectúe en un tiempo menor a 1 tick.

En caso de sobrepasar dicho límite, se habilita un contador `g_task_xx_tick_cnt` que contabiliza la cantidad de ejecuciones perdidas, y las ejecuta todas en la próxima iteración.

Para el control de tiempos de ejecución, se utiliza la variable `WCET`, contenida en cada tarea, `app.c`.

El gestor de tareas se encuentra en `app.c`, y contiene las siguientes:
* `task_system`: Controla la lógica principal del programa y delega funciones a las demás tareas. Hace uso de la librería `memoria_morse.h`.
* `task_sensor`: Lee las entradas, aplica algoritmos de debounce y devuelve una señal limpia a `task_system`.
* `task_GPIO_output`: Lee las salidas de `task_system` y las aplica a los terminales de la placa. Contiene diferentes estados como ON , OFF , PULSE, etc.
* `task_mic`: Utiliza el lector ADC para capturar la señal de micrófono, aplicar análisis de señales e indicar a `task_system` si detecta audio en las bandas especificadas o no.
* `task_HC05`: Sirve para insertar palabras al buffer de `task_system` que vienen <i>desde</i> la aplicación, y al mismo tiempo tiene un buffer que le permite a `task_system` encolar palabras pendientes para transmitir <i>hacia</i> la aplicación.


<div align="center">
<img width="1200" alt="TPF EMBEBIDOS" src="https://github.com/user-attachments/assets/f427b3a2-51af-447c-8af2-682ad7436e8d" />
<p align="center"><em>Imagen 3.5.1: Flujo de datos entre tareas.</em></p>
<div align="justify">

### 3.5.1 Task system

La tarea más importante del firmware. Se encarga del comportamiento básico del dispositivo, descripto en [3.4 Descripción de comportamiento](#34-descripción-de-comportamiento). A continuación se presenta el diagrama de estados de la tarea:

<div align="center">
<img width="1200" src="https://github.com/SantangeloEzequiel/tdse-tf_3-06/blob/Presentaci%C3%B3n-Final/images/system_Statechart.png?raw=true" />
<p align="center"><em>Imagen 3.5.1.1: Diagrama de estados <i>simplificado</i> de task_system.</em></p>
<div align="justify">

* `ST_SYS_WAITING_CONNECTION`:
  Es el estado por defecto del dispositivo. Se encuentra esperando la conexión bluetooth, mediante la lectura de STATE. Siempre que se pierde la conexión, se vuelve a este estado.
* `ST_SYS_RECEIVING`:
  Observa los eventos `EV_SYS_BTN_INPUT_XX` y `EV_SYS_MIC_INPUT_XX` para enviar los flancos como `1`(ascendente) o `0`(descendiente) hacia la aplicación. La señal evaluada es un resultado de una combinación del micrófono y la llave morse. Es decir, los flancos responden a `MIC OR BTN`.

* `ST_SYS_TRANSMITTING`
  Queda a la espera del búffer rx. Al recibir un caractér, se invoca a `memoria_morse` para obtener una secuencia de puntos o rayas según el caractér. De ser un caractér no listado en el diccionario, se enciende el led ERROR. Este comportamiento no se ve normalmente, por ser la aplicación encargada de enviar solo caractéres válidos.
* `ST_SYS_TRANSMITTING_CHAR`
  Con la secuencia a reproducir obtenida, este modo se encarga de reproducirlo con la asistencia de `task_GPIO_output` y el estado `ST_SYS_TRANSMITTING_WAIT`.
* `ST_SYS_TRANSMITTING_WAIT`
  Decrementa `tick` hasta 0, y vuelve al estado `ST_SYS_TRANSMITTING_CHAR`.

### 3.5.2 Task sensor

La tarea `task_sensor` se encarga de la lectura de todos los puertos GPIO configurados como entrada digital.

<div align="center">
<img width="1200" src="https://github.com/SantangeloEzequiel/tdse-tf_3-06/blob/Presentaci%C3%B3n-Final/images/Sensor_Statechart.png?raw=true" />
<p align="center"><em>Imagen 3.5.2.1: Diagrama de estados de task_sensor.</em></p>
<div align="justify">

Para cada puerto, se implementa un algoritmo de debounce por software mediante los estados `ST_BTN_XX_FALLING` y `ST_BTN_XX_RISING`, que se encarga de filtrar el ruido mecánico de los pulsadores. Para esto, se utiliza `tick`, que se incrementa cada `1ms`. En este caso, se configura el tiempo de debounce en 50ms, es decir, 50 ticks. De este modo, cada vez que se detecta un flanco, se espera a que el puerto se mantenga estable durante 50 ticks para validar el cambio de estado. También, por supuesto, retrasa la entrada, pero de forma imperceptible para el usuario.

### 3.5.3 Task GPIO output

La contraparte de `task_sensor`. Se encarga de aplicar a los puertos GPIO configurados como salida digital el resultado de la lógica del programa. Para esto, se implementan diferentes estados para cada puerto, como `ON`, `OFF`, `PULSE`, `BLINK`, etc. Cada estado tiene un comportamiento diferente, y algunos de ellos hacen uso de `tick` para controlar su duración.

<div align="center">
<img width="1200" src="https://github.com/SantangeloEzequiel/tdse-tf_3-06/blob/Presentaci%C3%B3n-Final/images/GPIO_Output_Statechart.png?raw=true" />
<p align="center"><em>Imagen 3.5.3.1: Diagrama de estados de task_GPIO_output.</em></p>
<div align="justify">

### 3.5.4 Task mic

`task_mic` se encarga de la lectura de n señales analógicas de micrófono, y de la aplicación del algoritmo de Goertzel para detectar la presencia de señales en bandas de frecuencia específicas. En este caso, se configura para detectar señales en la banda de 3,3kHz, que es la frecuencia de operación del buzzer.

<div align="center">
<img width="1200" src="https://github.com/SantangeloEzequiel/tdse-tf_3-06/blob/Presentaci%C3%B3n-Final/images/mic_Statechart.png?raw=true" />
<p align="center"><em>Imagen 3.5.4.1: Flujo de datos entre tareas.</em></p>
<div align="justify">

Aunque el diagrama de estados resulta extremadamente sencillo, la complejidad de la tarea radica en su implementación específica.

Se utiliza la lectura del ADC en formato Batch por medio de DMA e interrupciones por medio de una señal PWM. Esto garantiza que cada `N` muestras separadas `Ts` segundos, se refresque un buffer que contiene valores que reflejan un muestreo de la señal de micrófono.

En este caso, y por cuestiones de efectividad (se debe garantizar un tiempo de ejecución al menos menos a 1ms), se utiliza un Batch de 50 muestras, con una frecuencia de muestreo de 10kHz.

Esto significa que se toman fragmentos de 5ms de la señal de micrófono. Como la frecuencia a detectar es del orden de los 3,3kHz, se cumple el teorema de Nyquist:

$$
f_s > 2 \cdot f_{max}
$$

También debe garantizarse que el tiempo de conversión individual sea inferior a 1/Ts, es decir, 100µs. El tiempo de ejecución será de 12.5 ciclos (SAR) más el tiempo de carga del capacitor Cadc, con valor 8pF.

Para el ADC1, utilizado, se utiliza una señal de clock PCLK2/ADCprescaler = 4MHz. 12,5 ciclos serán 3,125µs.

El tiempo de carga del capacitor será $(R_{in} + R_{adc}) \cdot C_{adc}$, siendo Rin la resistencia de entrada del ADC, y Radc la resistencia interna del ADC, con valor 1k. Al estar conectada la entrada del ADC a la salida de un operacional, ser espera que el tiempo de carga sea aproximadamente $5 \cdot R_{adc} \cdot C_{adc} = 8ns$.

Finalmente, el tiempo total de conversión será de aproximadamente 

$$T_{conv} = 3.125\mu s + 8ns \approx 3.133\mu s < 100\mu s$$

lo que garantiza un correcto funcionamiento del sistema.

### 3.5.5 Task HC05

`task_HC05` se encarga de la comunicación bidireccional con la aplicación móvil por medio del módulo HC-05. Para esto, se implementan dos buffers: uno para almacenar las palabras recibidas desde la aplicación, y otro para almacenar las palabras pendientes de enviar hacia la aplicación.

<div align="center">
<img width="1200"  src="https://github.com/SantangeloEzequiel/tdse-tf_3-06/blob/Presentaci%C3%B3n-Final/images/HC05_Statechart.png?raw=true" />
<p align="center"><em>Imagen 3.5.5.1: Flujo de datos entre tareas.</em></p>
<div align="justify">

Para la implementación específica, se requiere que la transmisión sea de caractéres individuales, debido a la frecuencia máxima fijada por el HC-5:

$$ T_{min} = 8 * T_{bit}  = 8 * \frac{1}{9600} = 0.833ms $$

que se encuentra cerca del límite de 1ms.

Contiene dos estados principales: `ST_HC05_RECEIVING` y `ST_HC05_TRANSMITTING`. En el primero, se espera a recibir un caractér desde la aplicación, que se almacena en el buffer de recepción y se envía a `task_system`, en el segundo, se espera que se reciba un caractér desde alguna tarea, en este caso `task_system`, para transmitirlo a la aplicación.

En ambos casos, el funcionamiento es transparente para la aplicación y las tareas.

### 3.5.6 Memoria Morse

Memoria morse es una librería implementada para separar la comunicación I2C con los diccionarios morse, implementados en la memoria E2PROM, de la lógica principal del programa en `task_system`. 

Contiene una definición de caractér morse en dos bytes:

*Byte 0: bit del caracter en ASCII
*Byte 1: código de secuencia morse: los 3 bits menos significativos representan la cantidad de símbolos, y los 5 bits más significativos representan la secuencia de puntos y rayas, siendo `0` un punto y `1` una raya.

Para la organización de estos datos, se utlizan dos estructuras: una de traducción Morse-Letra, implementada en un árbol binario, y otra de traducción Letra-Morse, implementada en un vector. De este modo, se garantiza una búsqueda eficiente en ambos sentidos.

Para el árbol binario, se utiliza el siguiente esquema:

<div align="center">
<img width="1200" src="https://github.com/SantangeloEzequiel/tdse-tf_3-06/blob/Presentaci%C3%B3n-Final/images/%C3%81rbol%20Binario.jpg?raw=true"/>
<p align="center"><em>Imagen 3.5.6.1: Árbol binario de traducción Morse-Letra.</em></p>
<div align="justify">

esto permite buscar el caracter según el usuario ingresa puntos o rayas en tiempo real.

Para el vector, simplemente se ordenan en orden ascendente por caracter ASCII, mapeando el valor de ASCII a la posición del vector.

En total, el árbol binario contiene 64 posiciones, es decir, 64 caracteres morse, lo que significa un total de 128 bytes. La mitad de la memoria disponible (256 bytes).

Para el vector, se utilizan tan solo 37 caracteres, sin espacios vacíos por implementación, lo cual significa un total de 74 bytes.

Para el grabado de memoria inicial, se utiliza el proyecto `Grabador de Memoria`.

## 3.6 Aplicación móvil
Para la implementación de la comunicación Bluetooth se utilizó como base un repositorio público disponible en GitHub, correspondiente a un terminal Bluetooth (autoría de Kai Morich), el cual permite el envío y recepción de datos mediante comunicacion serie desde dispositivos Android.

Sobre esta base, se desarrolló y añadió lógica propia, integrando el algoritmo de detección de código Morse proveniente de la aplicación MyFriendlyMorse, de autoría propia.
Cabe destacar que únicamente se reutilizó la lógica de detección de Morse, adaptándola al contexto del proyecto, mientras que la aplicación final presenta una implementación y propósito diferentes.



<div align="center">
<img width="1200" src="https://github.com/SantangeloEzequiel/tdse-tf_3-06/blob/Presentaci%C3%B3n-Final/images/Diagrama%20de%20Harel%20Morse-Logic.png?raw=true"/>
<p align="center"> diagrama de estado de HAREL de Logica Morse <em>Imagen 3.5.6.2: </em></p>
<div align="justify">

Aquí se detecta la existencia de símbolos diferentes mediante una función empírica basada en la experimentación. Si bien, en teoría, una raya dura aproximadamente tres veces más que un punto, en la práctica el punto de mayor duración puede ser solo alrededor de 1,5 veces menor que la raya de menor duración.

Por este motivo, se utiliza la condición de que un sonido detectado sea al menos 1,5 veces mayor o menor que el anterior para considerar que se han identificado dos símbolos distintos. Con esta información es posible calcular un umbral inicial entre punto y raya, a partir del cual se promedian las duraciones de los puntos y las rayas detectados, obteniendo así el umbral definitivo.

Para los silencios, en cambio, se emplean los límites teóricos del código Morse: un silencio de tres veces la duración del símbolo corresponde a un separador de letra, mientras que uno de siete veces indica un separador de palabra.

Una vez finalizada la etapa de aprendizaje, los símbolos previamente almacenados se reconstruyen utilizando el umbral definitivo y, a partir de ese momento, el sistema comienza a realizar la traducción en tiempo real.

En conclusión, se puede afirmar que se trata de un sistema que no depende de una duración fija de los símbolos, lo que permite una fácil escalabilidad. Asimismo, contempla ciertas invariancias propias del emisor (no extremas), es decir, mientras los puntos mantengan una duración menor que las rayas, el sistema será capaz de detectarlos correctamente.

Repositorios y enlaces...

Repositorio del terminal Bluetooth (base del proyecto): 

https://github.com/kai-morich/SimpleBluetoothTerminal

Aplicación MyFriendlyMorse en Google Play: 

https://play.google.com/store/apps/details?id=com.santangeloezequiel.myfriendlymorse&hl=es_AR

Repositorio de MyFriendlyMorse: 

https://github.com/SantangeloEzequiel/MyFriendlyMorse

## Ensayos y resultados
### 4.1 Medición y análisis de consumo
### 4.2 Medición y análisis de tiempos de ejecución (WCET)
### 4.3 Cálculo del factor de uso (U) de la CPU
### 4.4 Cumplimiento de requisitos

| Estado | Descripción      |
|-----|---------------------|
| 🟢 | Ya implementado|
| 🟡 | Implementación parcial |
| 🔴 | No implementado |

### *Entrada*

|ID|Descripción|Estado|
| -- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|1.1|El sistema contará con un micrófono para captar señales sonoras.|🟢|
|1.2|El sistema tendrá un botón que permita generar un código Morse manualmente.|🟢|
|1.3|El sistema contará con un teclado matricial (se está considerando la posibilidad   de reemplazarlo por una aplicación Bluetooth en el celular) que permitirá enviar caracteres para ser reproducidos en código Morse.|🟢|

### *Indicadores*

|ID|Descripción|Estado|
| :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|2.1|El sistema contará con un indicador luminoso para indicar si se está recibiendo la señal Morse correctamente.|🟡|
|2.2|El sistema se conectará mediante Bluetooth al celular, para indicar el carácter Morse que se está recibiendo.|🟢|
|2.3|El sistema contará con un indicador luminoso que indique si está en modo "Emisión" o "Recepción".|🟢|
|2.4|El sistema notificará mediante el titileo de LEDs cuando ocurra un error al recibir el código.|🟢|
|2.5|El sistema notificará con audio y luz cuando se deje un código sin introducir completamente.|🔴|

### *Parlante*

|ID|Descripción|Estado|
| :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|3.1|El sistema contará con un parlante (o zumbador, a definir) para reproducir el código Morse que se desea emitir.|🟢|
|3.2|El sistema solo podrá emitir cuando no esté recibiendo, ya que las señales sonoras podrían interferirse.|🟢|

* Se decidió que se tratará de un zumbador para facilitar la detección sonora.

### *Comunicación Bluetooth*

|ID|Descripción|Estado|
| :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|4.1|El sistema se comunicará mediante Bluetooth con la aplicación del celular.|🟢|
|4.2|El sistema deberá enviar a la aplicación los caracteres recibidos (ya habiendo procesado el código Morse).|🟡|
|4.3|El sistema deberá poder recibir caracteres desde la aplicación (en caso de no usar un teclado matricial) para codificarlos en Morse y luego reproducirlos.|🟢|

* La tarea hc_05 se encuentra implementada y funcional. Queda por implementar la lógica del sistema que interpreta los caracteres recibidos y enviados.

### *Aplicación*

|ID|Descripción|Estado|
| :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|5.1|La aplicación deberá permitir al usuario escribir caracteres.|🟢|
|5.2|La aplicación deberá poder mostrar en pantalla los caracteres recibidos.|🟢|


### *Interruptores/Botones*

|ID|Descripción|Estado|
| :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|6.1|El sistema contará con un DIP switch que permitirá controlar la velocidad de emisión.|🟢|



### *ADICIONAL: App de ANDROID*

|ID|Descripción|Estado|
| :-- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |-|
|7.1|La aplicación deberá permitir al usuario escribir caracteres y mostrar en pantalla su traduccion a Morse.|🟢|
|7.2|La aplicación deberá permitir al usuario escribir Morse y mostrar en pantalla su traduccion a texto.|🟢|
|7.3|La aplicación deberá recibir señales de audio Morse de un emisor indeterminado, y poder traducirlo.|🟡|
|7.4|La aplicación deberá tener un apartado "HELP" que guiara al usuario sobre su funcionamiento.|🟠|


### 4.5 Reporte de uso
### 4.6 Prueba de integración
# Conclusiones
## 5.1 Resultados obtenidos
## 5.2 Próximos pasos
# Bibliografía
