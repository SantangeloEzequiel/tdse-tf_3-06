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

#Índice
1. [Introducción general](#introducción-general)
    - [1.1 Objetivos](#11-objetivos)
    - [1.2 Análisis de mercado](#12-análisis-de-mercado)
2. [Introducción específica](#introducción-específica)
    - [2.1 Requisitos del proyecto](#21-requisitos-del-proyecto)
    - [2.2 Casos de uso](#22-casos-de-uso)
        - [2.2.1 Caso de uso 1: El usuario juega una partida en modo clásico](#221-caso-de-uso-1-el-usuario-juega-una-partida-en-modo-clásico)
        - [2.2.2 Caso de uso 2: El usuario cambia la dificultad del juego](#222-caso-de-uso-2-el-usuario-cambia-la-dificultad-del-juego)
        - [2.2.3 Caso de uso 3: El usuario consulta los puntajes máximos](#223-caso-de-uso-3-el-usuario-consulta-los-puntajes-máximos)
    - [2.3 Elementos de hardware](#23-elementos-de-hardware)
        - [2.3.1 Pulsadores](#231-pulsadores)
        - [2.3.2 Leds (Diodos Emisores de Luz)](#232-leds-diodos-emisores-de-luz)
        - [2.3.3 LDR sensor analógico](#233-ldr-sensor-analógico)
        - [2.3.4 Display LCD](#234-display-lcd)
        - [2.3.5 Memoria E2PROM externa](#235-memoria-e2prom-externa)
        - [2.3.6 Placa de desarrollo](#236-placa-de-desarrollo)
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



## 2.2 Casos de uso
