#include "Stepper.h"
#include "Servo.h"

// Zone Manette
int bouton1r = 9;
int bouton2r = 47;
int bouton3r = 53;
int bouton4r = 45;

// Zone Capteurs
int FDC_treuil = 8; // capteur Fin De Course pour le Treuil
int FDC_rechargement = 7; // capteur Fin De Course pour le impact
int FDC_embrayage = 6; // capteur Fin De Course pour l'embrayage

// Zone Moteurs
// TREUIL
  // Moteur Embrayage
const int Pas = 200; // 200 pas sont nécessaires pour effectuer une révolution complète du moteur pas à pas
Stepper myStepper(Pas, 2, 4, 3, 5); // on détermine les pins rattachés au moteur (IN1, IN2, IN3, IN4)

  // Moteur Enroulement
#define borneENA 31
#define borneIN1 33
#define borneIN2 35
#define borneIN3 37
#define borneIN4 39
#define borneENB 41

// Impact
  // Moteur Embrayage
Stepper myStepper1(Pas, 10, 12, 11, 13 ); // on détermine les pins rattachés au moteur (IN1, IN2, IN3, IN4)(10 , 12, 11, 13)

  // Moteur Impact
#define borneENAi 30
#define borneIN1i 32
#define borneIN2i 34
#define borneIN3i 36
#define borneIN4i 38
#define borneENBi 40

// Arrimage
  // Servomoteur
Servo servo;
Servo servo2;

// Variables d'états de fonctionnement du robot
int ouverture = 0; // ouverture = 0 inactivité ouverture = 1 ouverture en cours ouverture = 2 ouverture finis
int fermeture = 0; // fermeture = 0 inactivité fermeture = 1 fermeture en cours fermeture = 2 fermeture finis 
int treuil = 0; // treuil = 0 minirobot rentré treuil = 1 minirobot sorti
int embrayage = 0; // embrayage = 0 désembrayé embrayage = 1 embrayé
int embrayage2 = 0; // embrayage = 0 désembrayé embrayage = 1 embrayé
int impact = 0; // impact = 0 rechargé; impact = 1 non rechargé
int porte = 0; // porte = 0 fermé; porte = 1 ouverte
int arrimage = 0; // arrimage = 0 verrouillé; arrimage = 1 déverrouillé

int var = 0; //permet le fonctionnement du bouton
int var2 = 1; //permet le fonctionnement du bouton
int var3 = 0; //permet le fonctionnement du bouton
int var4 = 0; //permet le fonctionnement du bouton

int m = 90; //permet fonctionnement des servomoteurs

// Fonction d'action du robot
void Fermeture() {
  fermeture = 1;
  Serial.println("Procédure de docking et de verrouillage du mini-robot dans la station hôte en cours...");
  while (fermeture == 1) {
    Treuil(1);
    Porte(0);
    Impact(0);
    if (treuil == 0 && porte == 0 && impact == 0) {
      fermeture = 2;
    }
  }
  Arrimage(0);
  fermeture = 0;
  Serial.print("finis! ");
  Serial.println("le mini-robot a été sécurisé avec succès");
}

void Ouverture() {
  ouverture = 1;
  Serial.println("Procédure de undocking et de déverrouillage du mini-robot dans la station hôte en cours...");
  while (ouverture == 1) {
    Arrimage(1);
    Porte(1);
    Treuil(0);
    Impact(1);
    if (treuil == 1 && porte == 1 && impact == 0) {
      ouverture = 2;
    }
  }
  ouverture = 0;
  Serial.print("finis! ");
  Serial.println("le mini-robot a été expulsé avec succès");
}

void Maj() { // permet de faire un état des lieux du robot en cas de panne et/ou bug
  Serial.print("Affichage des variables d'état du système :");
  Serial.print("ouverture =");
  Serial.println(ouverture);
  Serial.println("ouverture = 0 inactivité ouverture = 1 ouverture en cours ouverture = 2 ouverture finis");
  Serial.print("fermeture =");
  Serial.println(fermeture);
  Serial.println("fermeture = 0 inactivité fermeture = 1 fermeture en cours fermeture = 2 fermeture finis ");
  Serial.print("treuil =");
  Serial.println(treuil);
  Serial.println("treuil = 0 minirobot rentré treuil = 1 minirobot sorti");
  Serial.print("embrayage =");
  Serial.println(embrayage);
  Serial.println("embrayage = 0 désembrayé embrayage = 1 embrayé");
  Serial.print("impact =");
  Serial.println(impact);
  Serial.println("impact = 0 rechargé; impact = 1 non rechargé");
  Serial.print("porte =");
  Serial.println(porte);
  Serial.println("porte = 0 fermé; porte = 1 ouverte");
}

// Déclaration des fonctions secondaires
// Fonction Treuil
void Treuil(int etat) { // état = 0 roue libre état = 1 rapatriement
  if (etat == 1 && treuil == 1) {
    Serial.println("rapatriement minirobot...");
    tirage_treuil();
    delay(1000);
    Serial.print("finis!");
    treuil = 0;
  }
  if (etat == 1 && treuil == 0) {
    Serial.print("Treuil déjà rapatrié...");
  }
  if (etat == 0 && treuil == 0) {
    Serial.print("déroulement du Treuil...");
    deroulement_treuil();
    delay(1000);
    Serial.println("finis!");
    treuil = 1;
  }
  if (etat == 0 && treuil == 1) {
    Serial.println("Treuil déjà déployé...");
  }
}

// Fonctions locales Treuil
void tirage_treuil() {
  embrayage_treuil(1);
  moteur_treuil(1);
}

void deroulement_treuil() {
  embrayage_treuil(0);
}

// Fonction secondaire Treuil (Embrayage)
void embrayage_treuil(int etat) { // état = 0 désembrayé état = 1 embrayé
  if (etat == 1) {
    myStepper.step(1000); // le moteur tourne dans un sens 
    delay(500); // on applique un temps de délai
    Serial.print("embrayé = OK");
    embrayage = 1;
  } else if (etat == 0) {
    myStepper.step(-1000); // le moteur tourne dans l'autre sens
    delay(500); // on met un temps de délai
    Serial.print("désembrayé = OK");
    embrayage = 0;
  }
}

void moteur_treuil(int etat) { // état == 0 rien état == 1 moteur en marche
  if (etat == 1) {
    if (embrayage == 0) {
      while (digitalRead(FDC_treuil) != 0) {
        digitalWrite(borneIN1, HIGH); // On active la sortie branchée sur IN1 et le moteur tourne dans un sens
        digitalWrite(borneIN2, LOW); // On désactive la sortie branchée sur IN2 et le moteur ne tourne pas
        digitalWrite(borneENA, HIGH); // On active la sortie branchée sur ENA pour alimenter le moteur (driver)
      }
      digitalWrite(borneENA, LOW); // On désactive la sortie branchée sur ENA et on arrête l'alimentation du moteur  
    }
  }
}

// Fonction Impact
void Impact(int etat) { // état = 0 rechargement état = 1 impact
  if (etat == 1 /*&& porte == 1*/) {
    Serial.print("impact en cours...");
    impact_minirobot();
    Serial.println("finis!");
    Impact(0);
  } else if (etat == 0) {
    Serial.print("rechargement impact en cours...");
    impact_rechargement();
  }
}

// Fonctions locales Impact
void impact_rechargement() {
  while (digitalRead(FDC_treuil) != 0) {
    digitalWrite(borneIN1i, HIGH); // On active la sortie branchée sur IN1 et le moteur tourne dans un sens
    digitalWrite(borneIN2i, LOW); // On désactive la sortie branchée sur IN2 et le moteur ne tourne pas
    digitalWrite(borneENAi, HIGH); // On active la sortie branchée sur ENA pour alimenter le moteur (driver)
  }
  digitalWrite(borneENAi, LOW); // On désactive la sortie branchée sur ENA et on arrête l'alimentation du moteur
}

void impact_minirobot(){
  embrayage_impact(0);
}

// Fonction secondaire Impact (Embrayage)
void embrayage_impact(int etat) { // état = 0 désembrayé état = 1 embrayé
  if (etat == 1) {
    myStepper1.step(1000); // le moteur tourne dans un sens
    delay(500); // on applique un temps de délai
    Serial.print("embrayé = OK");
    embrayage = 1;
  } else if (etat == 0) {
    myStepper1.step(-1000); // le moteur tourne dans l'autre sens
    delay(500); // on met un temps de délai
    Serial.print("désembrayé = OK");
    embrayage = 0;
  }
}

void moteur_impact(int etat) { // état == 0 rien état == 1 moteur en marche
  if (etat == 1) {
    if (embrayage2 == 0) {
      while (digitalRead(FDC_rechargement) != 0) {
        digitalWrite(borneIN1i, HIGH); // On active la sortie branchée sur IN1 et le moteur tourne dans un sens
        digitalWrite(borneIN2i, LOW); // On désactive la sortie branchée sur IN2 et le moteur ne tourne pas
        digitalWrite(borneENAi, HIGH); // On active la sortie branchée sur ENA pour alimenter le moteur (driver)
      }
      digitalWrite(borneENAi, LOW); // On désactive la sortie branchée sur ENA et on arrête l'alimentation du moteur
    }
  }
}

// Fonction Arrimage
void Arrimage(int etat) { // état = 0 désarrimage état = 1 arrimage
  if (etat == 1 and m == 90) {
    m = 10;
    Serial.print("Arrimage du minirobot en cours...");
    servo.write(m);                  // on envoie la position m au servomoteur et il se déplace à celle-ci
    servo2.write(m);
    delay(1000);
    Serial.println("finis!");
    arrimage = 1;
  } else if (etat == 0 and m == 10) {
    m = 90;
    Serial.print("Désarrimage du minirobot en cours...");
    servo.write(m);                  // on envoie la position m au servomoteur et il se déplace à celle-ci
    servo2.write(m);
    delay(1000);
    Serial.println("finis!");
    arrimage = 0;
  }
}


// Fonction Porte
void Porte(int etat) { // état = 0 fermeture état = 1 ouverture
  if (etat == 1) {
    ouverture_porte();
    Serial.println("ouverture de la porte en cours...");
    porte = 1;
  }
  if (etat == 0 && treuil == 0) {
    fermeture_porte();
    Serial.println("fermeture de la porte en cours...");
    porte = 0;
  }
}

// Fonctions locales Porte
void ouverture_porte() {
  return;
}

void fermeture_porte() {
  return;
}

// Setup
void setup() {
  Serial.begin(9600);
  delay(300); // added delay to give wireless ps2 module some time to startup, before configuring it
  
  // Zone Capteurs
  
  pinMode(FDC_treuil, INPUT_PULLUP);
  pinMode(FDC_rechargement, INPUT_PULLUP);
  pinMode(FDC_embrayage, INPUT_PULLUP);

  // Zone Moteurs
// Treuil
  // Embrayage
  myStepper.setSpeed(60);
  myStepper1.setSpeed(60);
  // Enroulement
  pinMode(borneENA, OUTPUT); // Configuration de la broche: "borne ENA(13)" comme une sortie
  pinMode(borneIN1, OUTPUT); // Configuration de la broche: "borne IN1(12)" comme une sortie
  pinMode(borneIN2, OUTPUT); // Configuration de la broche: "borne IN2(11)" comme une sortie
  pinMode(borneIN3, OUTPUT); // Configuration de la broche: "borne IN3(10)" comme une sortie
  pinMode(borneIN4, OUTPUT); // Configuration de la broche: "borne IN4(9)" comme une sortie
  pinMode(borneENB, OUTPUT); // Configuration de la broche: "borne ENB(8)" comme une sortie

// Impact
  // Moteur rechargement
  pinMode(borneENAi, OUTPUT); // Configuration de la broche: "borne ENA(13)" comme une sortie
  pinMode(borneIN1i, OUTPUT); // Configuration de la broche: "borne IN1(12)" comme une sortie
  pinMode(borneIN2i, OUTPUT); // Configuration de la broche: "borne IN2(11)" comme une sortie
  pinMode(borneIN3i, OUTPUT); // Configuration de la broche: "borne IN3(10)" comme une sortie
  pinMode(borneIN4i, OUTPUT); // Configuration de la broche: "borne IN4(9)" comme une sortie
  pinMode(borneENBi, OUTPUT); // Configuration de la broche: "borne ENB(8)" comme une sortie
// Arrimage
  // Servomoteur verrouillage
  servo.attach(23);
  servo2.attach(22);
  servo.write(m);              
  servo2.write(m);
}

// Loop
void loop() {
  int bouton1; 
  int bouton2;
  int bouton3;
  int bouton4;
  bouton1 = digitalRead(bouton1r); // pin 9 jaune rouge
  bouton2 = digitalRead(bouton2r); //pin 25 bleu
  bouton3 = digitalRead(bouton3r); //pin 49 bleu rouge
  bouton4 = digitalRead(bouton4r); //pin 51 jaune
  Serial.print(bouton1);
  Serial.print(var);
  Serial.print(bouton2);
  Serial.print(var2);
  Serial.print(bouton3);
  Serial.print(var3);
  Serial.print(bouton4);
  Serial.println(var4);
  if (var == 0) {
    if (bouton1 == 1) {//rouge pin 9 jaune rouge
      Arrimage(1);
      var = 1;
    }
  } else {
    if (bouton1 == 1) {
      Arrimage(0);
      var = 0;
    }
  }
  if (var2 == 0) {
    if (bouton2 == 1) {// pas rouge pin 25 jaune
      embrayage_treuil(1);
      moteur_treuil(1);
      var2 = 1;
    }
  } else {
    if (bouton2 == 1) {
      embrayage_treuil(0);
      var2 = 0;
    }
  }
  if (var3 == 0) {
    if (bouton3 == 1) {// pas rouge pin 49 bleu 
      embrayage_impact(1);
      moteur_impact(1);
      var3 = 1;
    }
  } else {
    if (bouton3 == 1) {
      embrayage_impact(0);
      var3 = 0;
    }
  }
  if (var4 == 0) {
    if (bouton4 == 1) {// pas rouge pin 49 bleu 
      moteur_treuil(1);
      var4 = 1;
    }
  } else {
    if (bouton4 == 1) {
      moteur_treuil(1);
      var4 = 0;
    }
  }
  delay(1000);
}
