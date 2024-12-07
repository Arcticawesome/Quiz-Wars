#include <Arduino.h>


const int buttonPinA1 = 9;   // Pins for Button A - Can be changed for your specific Arduino
const int buttonPinA2 = 2;
const int buttonPinA3 = 3;
const int buttonPinA4 = 4;
const int buttonPinB1 = 5;   // Pins for Button B - Can be changed for your specific Arduino
const int buttonPinB2 = 6;  
const int buttonPinB3 = 7; 
const int buttonPinB4 = 8; 


static bool questionAsked = false;


int LEDstateA = LOW;        // Initial state of LED A
int LEDstateB = LOW;        // Initial state of LED B
int lastButtonA = LOW;      // Previous state of Button A
int lastButtonB = LOW;      // Previous state of Button B


bool lockStateA = false;
bool lockStateB = false;
bool lockoutActive = false;


struct Question {
  String text;
  String answer;
};


Question questions[] = {
  {"In this story, a man in Dutch clothing invites this tale's protagonist to play nine-pins. The title man mourns the loss of Wolf but happily discovers that his tyrannical wife has died in his absence. For 10 points, name this short story in which the title man falls asleep for twenty years.", "Rip van Winkle"},
  
  {"This man wrote a collection of pieces in which he utilized the gypsy scale from his native country, and he composed twelve exercises for solo piano that he entitled Transcendental Etudes. For 10 points, name this composer of the Faust Symphony and Hungarian Rhapsodies.", "Liszt"},
  
  {"In fungi, this structure is made of chitin instead of cellulose, which is common in this structure in plants. For 10 points, name this protective cellular structure that surrounds the cell membrane.", "Cell Wall"},
  
  {"In the first stage of this battle, the winning side was led by the Marquis de Lafayette. George Washington defeated General Cornwallis at, for 10 points, which decisive battle of the American Revolution?", "Yorktown"},
  
  {"This author of Meditations on First Philosophy wrote a work concerned with principles of skepticism that identified the one statement he knew without doubt, Discourse on Method. For 10 points, name this philosopher who gives his name to the rectangular coordinate system and penned the phrase 'Cogito ergo sum.'", "Descartes"},

  //Add questions into the python file
};



const int questionCount = sizeof(questions) / sizeof(questions[0]);
int currentQuestionIndex = 0;


void askQuestion();


// Base Team class
class Team {
protected:
    String name;
    int score;


public:
    Team(String teamName) : name(teamName), score(0) {}


    void addPoints(int points) { score += points; }


    int getScore() const { return score; }


    void displayInfo() const {
        Serial.print("Team Name: ");
        Serial.println(name);
        Serial.print("Score: ");
        Serial.println(score);
    }


    virtual void buttonPress(bool& lockoutActive, bool& lockStateOther, int& LEDstate, const int* buttonPins, int* lastButtonStates) = 0;
};


// TeamA Class
class TeamA : public Team {
public:
    TeamA(String teamName) : Team(teamName) {}


    void buttonPress(bool& lockoutActive, bool& lockStateOther, int& LEDstate, const int* buttonPins, int* lastButtonStates) override {
        for (int i = 0; i < 2; i++) {
            int buttonState = digitalRead(buttonPins[i]);


            if (!lockoutActive) {
                delay(10);


                if (buttonState == HIGH && lastButtonStates[i] == LOW) {
                    lockStateOther = true;  // Lock the other team
                    LEDstate = (LEDstate == LOW) ? HIGH : LOW;
                    pinMode(buttonPins[i], OUTPUT);
                    digitalWrite(buttonPins[i], HIGH);


                    Serial.println("TeamA buzzed in");  // Indicate Team A buzzed in
                    lockoutActive = true;  // Activate lockout
                }


                lastButtonStates[i] = buttonState;
            }
        }
    }


    void checkAnswer(String playerAnswer, int& currentQuestionIndex) {
        if (playerAnswer.equalsIgnoreCase(questions[currentQuestionIndex].answer)) {
            Serial.println("Correct answer!");
            addPoints(20);  // Add points to Team A
            Serial.print("Team A Score: ");
            Serial.println(getScore());
            Serial.println();
        } else {
            Serial.println("Wrong answer!");
        }
    }
};


// TeamB Class
class TeamB : public Team {
public:
    TeamB(String teamName) : Team(teamName) {}


    void buttonPress(bool& lockoutActive, bool& lockStateOther, int& LEDstate, const int* buttonPins, int* lastButtonStates) override {
        for (int i = 0; i < 2; i++) {
            int buttonState = digitalRead(buttonPins[i]);


            if (!lockoutActive) {
                delay(10);


                if (buttonState == HIGH && lastButtonStates[i] == LOW) {
                    lockStateOther = true;  // Lock the other team
                    LEDstate = (LEDstate == LOW) ? HIGH : LOW;
                    pinMode(buttonPins[i], OUTPUT);
                    digitalWrite(buttonPins[i], HIGH);


                    Serial.println("TeamB buzzed in");  // Indicate Team B buzzed in
                    lockoutActive = true;  // Activate lockout
                }


                lastButtonStates[i] = buttonState;
            }
        }
    }


    void checkAnswer(String playerAnswer, int& currentQuestionIndex) {
        if (playerAnswer.equalsIgnoreCase(questions[currentQuestionIndex].answer)) {
            Serial.println("Correct answer!");
            addPoints(20);  // Add points to Team B
            Serial.print("Team B Score: ");
            Serial.println(getScore());
            Serial.println();
        } else {
            Serial.println("Wrong answer!");
        }
    }
};


TeamA teamA("Team A");
TeamB teamB("Team B");


const int buttonPinsA[] = {buttonPinA1, buttonPinA2, buttonPinA3, buttonPinA4};
const int buttonPinsB[] = {buttonPinB1, buttonPinB2, buttonPinB3, buttonPinB4};


int lastButtonStatesA[] = {LOW, LOW, LOW, LOW};
int lastButtonStatesB[] = {LOW, LOW, LOW, LOW};


void setup() {
  pinMode(buttonPinA1, INPUT);
  pinMode(buttonPinA2, INPUT);  
  pinMode(buttonPinA3, INPUT);
  pinMode(buttonPinA4, INPUT);
  pinMode(buttonPinB1, INPUT);
  pinMode(buttonPinB2, INPUT);  
  pinMode(buttonPinB3, INPUT);
  pinMode(buttonPinB4, INPUT);


  Serial.begin(9600);
}

void resetBuzzers() {
  digitalWrite(buttonPinsA[0], LOW);
  digitalWrite(buttonPinsA[1], LOW);
  digitalWrite(buttonPinsA[2], LOW);
  digitalWrite(buttonPinsA[3], LOW);
  digitalWrite(buttonPinsB[0], LOW);
  digitalWrite(buttonPinsB[1], LOW);
  digitalWrite(buttonPinsB[2], LOW);
  digitalWrite(buttonPinsB[3], LOW);
}


void resetGame() {
  lockoutActive = false;
  lockStateA = false;
  lockStateB = false;
  questionAsked = false;
  resetBuzzers();
  Serial.println("Lockout reset. Next question!");
}


void askQuestion() {
    Serial.println("Question: ");
    if (currentQuestionIndex < questionCount) {
        Serial.println(questions[currentQuestionIndex].text);
    } else {
        Serial.println("Game Over! No more questions.");
        declareWinner();  // Declare the winner after all questions are asked
    }
}


void declareWinner() {
    if (teamA.getScore() > teamB.getScore()) {
        Serial.println("Team A is the winner!");
    } else if (teamB.getScore() > teamA.getScore()) {
        Serial.println("Team B is the winner!");
    } else {
        Serial.println("It's a tie!");
    }
    resetBuzzers();
}


void loop() {
  if (!lockoutActive) {
    if (!questionAsked && currentQuestionIndex < questionCount) {
      askQuestion();
      questionAsked = true;
    }


    teamA.buttonPress(lockoutActive, lockStateB, LEDstateA, buttonPinsA, lastButtonStatesA);
    teamB.buttonPress(lockoutActive, lockStateA, LEDstateB, buttonPinsB, lastButtonStatesB);
  }


  if (lockoutActive) {
    delay(100); // A short delay to stabilize before answering


    Serial.println("Enter your answer:");


    // Wait for the laptop to send an answer
    while (!Serial.available()) {}


    String playerAnswer = Serial.readStringUntil('\n');
    playerAnswer.trim();  // Remove any extra spaces or newlines


    // Pass both the playerAnswer and currentQuestionIndex to checkAnswer
    if (lockStateB) {
      teamA.checkAnswer(playerAnswer, currentQuestionIndex);  // Team A buzzed in
    } else if (lockStateA) {
      teamB.checkAnswer(playerAnswer, currentQuestionIndex);  // Team B buzzed in
    }


    // Prepare for the next question
    currentQuestionIndex++; // Move to the next question
    if (currentQuestionIndex >= questionCount) {
      // Game Over - All questions answered
      declareWinner(); // Declare winner when all questions are completed
      while (true) {
        // Stay in this loop after the game ends
      }
    }
    resetGame();  // Reset game state for the next round
  }


  delay(100);  // Main loop delay to stabilize readings
}
