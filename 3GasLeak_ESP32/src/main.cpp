//Prueba del sensor MQ2:

//Librerias utilizadas:
#include <WiFi.h>
#include "FirebaseESP32.h"
#include "time.h"

//----------------WIFI configuracion-------------------------------
// const char* ssid     = "TP-LINK_4106";
// const char* password = "54281264";
// const char* ssid     = "Totalplay-3BA3";
// const char* password = "3BA3EC89zu2C57wF";
 const char* ssid     = "Dark_Side";
 const char* password = "112233445566ax";
//------------CONEXION CON LA BASE DE DATOS DESEADA---------------------------
#define FIREBASE_HOST "https://cisterna-tinaco-iot-default-rtdb.firebaseio.com/" //Link de Manu
#define FIREBASE_AUTH "fucx9kG1YkhDPdbI22iULkC3EbsT1zEnlsz1msMm" //*****************
//Firebase Data object--------------------------------------------------------
FirebaseData firebaseData;
//Pines
int pMQ2 = 4; //Pin Sensor MQ2
int pbzz = 25; //Pin de buzzer

//-------------------Funciones-------------------------------------------------

void ActBzz();

//--------------------Varbles Globales----------------------------------------

// int GasThresh=600; //limite de Valor Threshold
bool EstadoAlarma=false; //Variable para controlar el buzzer.
// int GasPPM = 0;                   // Conversion of Value to PPM
// int GasMQ2Value = 0;              // Value of Sensor Reading
int GasMQ2digital = 0;

int contconexion = 0;
//Si deseamos una ruta especifica:
String nodo = "/gas";


void setup() {
  Serial.begin(115200);
//---------------------------------pines a ocupar-----------------
  pinMode(pbzz, OUTPUT);
  pinMode(pMQ2, INPUT);
  Serial.println("Listo Setup");
// -----------------------Conexión WIFI--------------------------
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) { 
      Serial.println("");
      Serial.println("WiFi conectado");
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
//---------------------BASE DE DATOS-----------------------------
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  
  //----------Sensor magnetico para la ventana1------------
  // GasMQ2Value = analogRead(pMQ2);
  GasMQ2digital = digitalRead(pMQ2);
  digitalWrite(pbzz, LOW);
  Serial.print("Sensor Value: ");
	// Serial.println(GasMQ2Value);
  Serial.println(GasMQ2digital);
  Firebase.setInt(firebaseData, nodo + "/GasPPM", GasMQ2digital);

  if (GasMQ2digital == 0 ) {
    EstadoAlarma= true;
  }
  else{
    EstadoAlarma= false;
    Firebase.setBool(firebaseData,nodo + "/GasEstado" , false);
  }

  while (EstadoAlarma==true)
  {
    Firebase.setBool(firebaseData, nodo + "/GasEstado" , true);
    GasMQ2digital = digitalRead(pMQ2);
    Firebase.setInt(firebaseData,nodo + "/GasPPM", GasMQ2digital);
    Serial.print("Sensor Value: ");
    Serial.println(GasMQ2digital);
	  ActBzz();
    
    if (GasMQ2digital == 1) {
        EstadoAlarma= false;
        Firebase.setBool(firebaseData,nodo + "/GasEstado" , false);
    } 
  
  }
  delay(2000); // wait 2s for next reading

}

void ActBzz(){   //En esta funciona se activa el buzzer.
    Serial.print("Gas Detected!");
    Serial.print('\n');
    //digitalWrite(pbzz, HIGH);
    tone (pbzz, 2000,100);
    delay(250);
    digitalWrite(pbzz, LOW);
    delay(250);
}
