<img width="365" height="138" alt="Uba_fiuba_ingenieria_logo" src="https://github.com/user-attachments/assets/412e1968-b281-4fce-98f3-e37283acfc46" />

# **Traductor Morse Sonoro: Asistencia de aprendizaje** 

**Autores: Ezequiel Santangelo - 112762, Ramiro Mántaras - 111510**

**Fecha: 8/3/26**

**Cuatrimestre de cursada: 2do 2026**

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
    - [3.1 Esquema eléctrico y conexión de placas](#31-esquema-eléctrico-y-conexión-de-placas)
    - [3.2 Descripción del esquema eléctrico](#32-descripción-del-esquema-eléctrico)
    - [3.3 Descripción del comportamiento](#33-descripción-del-comportamiento)
    - [3.4 Firmware del Simon Says](#34-firmware-del-simon-says)
        - [3.4.1 Task actuator](#341-task-actuator)
        - [3.4.2 Task sensor](#342-task-sensor)
        - [3.4.3 Task ADC](#343-task-adc)
        - [3.4.4 Task PWM](#344-task-pwm)
        - [3.4.5 Task gameplay](#345-task-gameplay)
        - [3.4.6 Task storage](#346-task-storage)
        - [3.4.7 Task I2C](#347-task-i2c)
        - [3.4.8 Task display](#348-task-display)
        - [3.4.9 Task menu](#349-task-menu)
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
