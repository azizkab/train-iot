#define s0 2
#define s1 3
#define s2 7
#define s3 5
#define out 6

#define ENA 9 // Broche pour contrôler la vitesse via PWM
#define IN1 8 // Broche pour contrôler la direction du moteur
#define IN2 10 // Broche pour contrôler la direction du moteur

#define TRIG 11 // Broche pour le pin trig du capteur ultrason
#define ECHO 12 // Broche pour le pin echo du capteur ultrason

int red = 0;
int green = 0;
int blue = 0;

// Valeurs de vitesse pour différentes couleurs entre 0 et 255  toutes les valeurs a changer en fonction du test 
int normalSpeed = 100; // Exemple de valeur pour la vitesse normale 
int increasedSpeed = 190; //  vitesse augmentée entre  
int maxSpeed = 255; //  vitesse maximaleentre  

void setup() {
   // Configurer les broches du capteur de couleur
   pinMode(s0, OUTPUT);
   pinMode(s1, OUTPUT);
   pinMode(s2, OUTPUT);
   pinMode(s3, OUTPUT);
   pinMode(out, INPUT);

   // Configurer les broches du capteur ultrason
   pinMode(TRIG, OUTPUT);
   pinMode(ECHO, INPUT);

   // Initialiser la communication série pour le débogage
   Serial.begin(9600);

   // Définir les facteurs de mise à l'échelle pour le capteur (HIGH, HIGH signifie mise à l'échelle maximale)
   digitalWrite(s0, HIGH);
   digitalWrite(s1, HIGH);

   // Configurer les broches de contrôle du moteur
   pinMode(ENA, OUTPUT);
   pinMode(IN1, OUTPUT);
   pinMode(IN2, OUTPUT);
   
   // Définir la direction du moteur
   digitalWrite(IN1, HIGH);
   digitalWrite(IN2, LOW);
}

void loop() {
   // Lire la valeur rouge
   digitalWrite(s2, LOW);
   digitalWrite(s3, LOW);
   red = GetData();

   // Lire la valeur bleue
   digitalWrite(s2, LOW);
   digitalWrite(s3, HIGH);
   blue = GetData();

   // Lire la valeur verte
   digitalWrite(s2, HIGH);
   digitalWrite(s3, HIGH);
   green = GetData();

   // Déterminer et afficher la couleur dominante
   DisplayDominantColor(red, green, blue);

   // Ajuster la vitesse du moteur en fonction de la couleur
   AdjustMotorSpeed(red, green, blue);

   // Vérifier la présence d'obstacles
   if (ObstacleDetected()) {
      // Arrêter le moteur si un obstacle est détecté
      analogWrite(ENA, 0);
   }

   // Courte pause avant la prochaine lecture
   delay(100);
}

int GetData() {
   // Mesurer la durée de l'impulsion sur la broche de sortie du capteur
   return pulseIn(out, LOW); // Ajuster si nécessaire en fonction de la sortie haute ou basse du capteur
}

void DisplayDominantColor(int r, int g, int b) {
   int minColor = min(r, min(g, b));

   // Afficher la couleur avec la durée d'impulsion minimale, qui représente la plus haute intensité
   if (minColor == r) {
      Serial.println("Rouge détecté");
   } else if (minColor == g) {
      Serial.println("Vert détecté");
   } else if (minColor == b) {
      Serial.println("Bleu détecté");
   } else {
      Serial.println("Couleur incertaine");
   }
}

void AdjustMotorSpeed(int r, int g, int b) {
   int minColor = min(r, min(g, b));
   int speed;

   // Déterminer la vitesse en fonction de la couleur détectée
   if (minColor == r) {
      speed = normalSpeed;
   } else if (minColor == g) {
      speed = increasedSpeed;
   } else if (minColor == b) {
      speed = maxSpeed;
   } else {
      speed = 0; // Arrêter le moteur si la couleur est incertaine voir avec les autres membres si on change ca 
   }

   // Régler la vitesse du moteur
   analogWrite(ENA, speed);
}

bool ObstacleDetected() {
   // Envoyer une impulsion au pin TRIG
   digitalWrite(TRIG, LOW);
   delayMicroseconds(2);
   digitalWrite(TRIG, HIGH);
   delayMicroseconds(10);
   digitalWrite(TRIG, LOW);

   // Lire la durée de l'impulsion du pin ECHO
   long duration = pulseIn(ECHO, HIGH);
   // Calculer la distance (la vitesse du son est d'environ 343 mètres par seconde)
   float distance = (duration / 2.0) * 0.0343;

   // Retourner vrai si un obstacle est détecté dans une certaine distance (on va la mettre a  20 cm au debut en attendant le test )
   return distance < 20.0; 
}
