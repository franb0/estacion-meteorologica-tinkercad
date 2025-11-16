/* ============================================================
   Proyecto: Estacion meteorologica basica
   Curso: Informatica I – Arduino
   Alumno: Boito Diaz, Francesco - Legajo: 53136
   ============================================================ */

#include <LiquidCrystal.h>

/* ------------------------------------------------------------
   Definicion del modulo LCD
   Se declara un objeto LiquidCrystal indicando los pines
   utilizados para RS, EN, D4, D5, D6 y D7 del display.
   ------------------------------------------------------------ */
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/* ------------------------------------------------------------
   Declaracion de entradas analogicas
   pinTemp: potenciometro que simula temperatura (A0)
   pinHum : potenciometro que simula humedad (A1)
   ------------------------------------------------------------ */
const int pinTemp = A0;
const int pinHum  = A1;

/* ------------------------------------------------------------
   Variables de control de tiempo
   tCambio : ultimo instante en ms en que se altero el estado
   PERIODO: intervalo de cambio de pantalla (5 segundos)
   ------------------------------------------------------------ */
unsigned long tCambio = 0;
const unsigned long PERIODO = 5000;

/* ------------------------------------------------------------
   Variable de estado
   estado = 0 → se visualiza temperatura
   estado = 1 → se visualiza humedad
   ------------------------------------------------------------ */
int estado = 0;

/* ============================================================
   FUNCION SETUP
   Inicializa el LCD y registra el instante inicial de tiempo.
   ============================================================ */
void setup() {
  lcd.begin(16, 2);     // Configura LCD de 16 columnas × 2 filas
  lcd.clear();          // Limpia el display

  lcd.setCursor(0, 0);
  lcd.print("Estacion meteo");  // Titulo fijo en linea superior

  tCambio = millis();   // Guarda tiempo inicial de referencia
}

/* ============================================================
   FUNCION LOOP
   Ciclo principal del sistema. Realiza:
   1) Lectura analogica
   2) Conversion de rangos
   3) Control de tiempo para alternancia
   4) Visualizacion en LCD
   ============================================================ */
void loop() {

  /* ----------------------------------------------------------
     (1) Lectura de entradas analogicas
     Se obtienen valores en el rango 0–1023.
     ---------------------------------------------------------- */
  int valorTemp = analogRead(pinTemp);
  int valorHum  = analogRead(pinHum);

  /* ----------------------------------------------------------
     (2) Conversion de valores a magnitudes fisicas
     map() escala los valores a rangos arbitrarios:
       - Temperatura: 0 a 50 °C
       - Humedad: 0 a 100 %
     ---------------------------------------------------------- */
  int tempC = map(valorTemp, 0, 1023, 0, 50);
  int humP  = map(valorHum,  0, 1023, 0, 100);

  /* ----------------------------------------------------------
     (3) Alternancia de visualizacion cada 5 segundos
     Se compara el tiempo transcurrido respecto al instante
     guardado en tCambio. Si supera PERIODO, se cambia estado.
     ---------------------------------------------------------- */
  unsigned long ahora = millis();

  if (ahora - tCambio >= PERIODO) {
      estado = (estado == 0 ? 1 : 0);  // Cambio de estado logico
      tCambio = ahora;                // Actualizacion del instante
  }

  /* ----------------------------------------------------------
     (4) Visualizacion en el LCD 16x2 segun el estado
     La primera linea permanece fija; la segunda muestra
     temperatura o humedad segun corresponda.
     ---------------------------------------------------------- */
  lcd.setCursor(0, 0);
  lcd.print("Estacion meteo ");

  lcd.setCursor(0, 1);

  if (estado == 0) {
      lcd.print("Temp: ");
      lcd.print(tempC);
      lcd.print((char)223);   // Codigo ASCII para el simbolo °
      lcd.print("C   ");
  }
  else {
      lcd.print("Hum:  ");
      lcd.print(humP);
      lcd.print("%   ");
  }

  delay(200);  // Pequeño retardo para evitar parpadeos
}
