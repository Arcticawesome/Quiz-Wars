import time
import serial
import threading
import pyttsx3  # Import text-to-speech library 

# Initialize the serial connection (ensure the correct COM port)
ser = serial.Serial('COM5', 9600, timeout=1)

# Initialize the text-to-speech engine
engine = pyttsx3.init()

# Flags to manage the question display and buzz handling
stop_printing = False
buzzed_in = False  # Flag to detect if a team buzzed in during retyping

# Function to display the question letter-by-letter
def display_question(question_text):
    global stop_printing, buzzed_in
    print("\nQuestion: ", end="", flush=True)

    for letter in question_text:
        if stop_printing:  # Stop typing immediately if buzzed in
            break
        print(letter, end="", flush=True)
        time.sleep(0.1)  # Delay between letters for smoother effect
        
        # Check for buzz-in during typing
        if ser.in_waiting > 0:  # Check if there's data from the serial port
            line = ser.readline().decode('utf-8').strip()
            if line in ["TeamA buzzed in", "TeamB buzzed in"]:
                buzzed_in = True
                stop_printing = True  # Stop typing
                print(f"\n{line}!")  # Print which team buzzed in
                return line  # Return the team that buzzed in

# Function to detect the team that buzzed in and prompt for an answer
def wait_for_buzz(question_answer, question_text, team_answer_result):
    global stop_printing, buzzed_in

    while True:
        line = ser.readline().decode('utf-8').strip()
        if line in ["TeamA buzzed in", "TeamB buzzed in"]:
            stop_printing = True  # Stop the question from printing
            buzzed_in = True  # Indicate a buzz-in
            print(f"\n{line}!")
            team_name = line.split()[0]  # Extract the team name ("TeamA" or "TeamB")
            team_answer_result["team"] = team_name
            team_answer_result["answer"] = handle_answer(team_name, question_answer)
            if team_answer_result["answer"] == 0:  # If the team answered incorrectly
                print(f"{team_name} got it wrong! Switching turns.")
                other_team = "TeamB" if team_name == "TeamA" else "TeamA"
                retype_question_for_team(other_team, question_text, question_answer, team_answer_result)
            break

# Function to retype the question for the other team
def retype_question_for_team(team_name, question_text, question_answer, team_answer_result):
    global stop_printing, buzzed_in
    stop_printing = False  # Reset stop flag
    buzzed_in = False  # Reset buzz-in flag

    # Start a thread to display the question again, allowing interruption
    question_thread = threading.Thread(target=display_question, args=(question_text,))
    question_thread.start()

    # Monitor for buzz-in during retyping
    while question_thread.is_alive():
        if ser.in_waiting > 0:  # Continuously check for data from the serial port
            line = ser.readline().decode('utf-8').strip()
            if line in ["TeamA buzzed in", "TeamB buzzed in"]:
                stop_printing = True  # Stop the question thread
                buzzed_in = True
                print(f"\n{line}!")
                team_answer_result["team"] = line.split()[0]  # Extract the team name
                team_answer_result["answer"] = handle_answer(team_answer_result["team"], question_answer)
                break  # Exit the loop to stop retyping

    question_thread.join()  # Ensure the question thread completes

# Function to handle the team's answer
def handle_answer(team, correct_answer):
    print(f"{team}, please type your answer: ", end="")
    team_answer = input()  # Wait for the team to type their answer

    # Send the answer to the serial port for Arduino
    ser.write(team_answer.encode())

    # Check if the answer is correct
    if correct_answer.lower() in team_answer.strip().lower():
        print("Correct answer!")
        return 20  # Return points
    else:
        print("Wrong answer!")
        return 0  # Return no points

# Function to declare the winner
def declare_winner(team_a_score, team_b_score):
    if team_a_score > team_b_score:
        winner = "Team A won"
    elif team_b_score > team_a_score:
        winner = "Team B won"
    else:
        winner = "It's a tie"  # Adjusted message
    
    # Print and read the winner aloud
    print(f"{winner}!")
    engine.say(f"{winner}")
    engine.runAndWait()

# Simulating a game loop with questions
questions = [
    {
        "question": "In this structure, lysozyme hydrolyzes the bond between NAM and NAG. Transport through this structure occurs in channels called plasmodesmata. Gram staining is used to determine the thickness of this structure’s peptidoglycan in bacteria. In fungi, this structure is made of chitin instead of cellulose, which is common in this structure in plants. For 10 points, name this protective cellular structure that surrounds the cell membrane.                                                                                      ",
        "answer": "Cell Wall"
    },
    {
        "question": "One side in this battle was hurt from a victory at the Battle of Guilford Courthouse. The winning side in this battle may not have won without the intervention of the Comte de Grasse’s ships. One side’s army was led by leaders such as the Comte de Rochambeau, and escape was prevented due to a blockade by the French navy. In the first stage of this battle, the winning side was led by the Marquis de Lafayette. George Washington defeated General Cornwallis at, for 10 points, which decisive battle of the American Revolution?                                                                                      ",
        "answer": "Yorktown"
    },
    {
        "question": "One work by this man considers the possibility of being deceived by an evil demon, and this man who believed that the soul resides in the pineal gland names a mind-body dualism. This author of Meditations on First Philosophy wrote a work concerned with principles of skepticism that identified the one statement he knew without doubt, Discourse on Method. For 10 points, name this philosopher who gives his name to the rectangular coordinate system and penned the phrase “Cogito ergo sum.                                                                                      ”                                                                                      ",
        "answer": "Descartes"
    },
   # Add more/Change questions as you see fit.
]


team_a_score = 0
team_b_score = 0

# Simulating a game loop where we present questions
for i, q in enumerate(questions):
    print(f"\nCurrent Score - Team A: {team_a_score} | Team B: {team_b_score}")

    team_answer_result = {"team": "", "answer": 0}
    stop_printing = False
    buzzed_in = False  # Reset buzz-in flag for each question

    # Prevent early buzz-ins
    time.sleep(0.5)  # Small delay before enabling buzz detection

    # Start a thread to display the question letter-by-letter
    question_thread = threading.Thread(target=display_question, args=(q["question"],))
    question_thread.start()

    # Wait for buzz-in (in the main thread)-+++
    wait_for_buzz(q["answer"], q["question"], team_answer_result)

    # Wait for the question thread to finish before continuing to the next question
    question_thread.join()

    # Update team scores based on the answer
    if team_answer_result["team"] == "TeamA":
        team_a_score += team_answer_result["answer"]
    elif team_answer_result["team"] == "TeamB":
        team_b_score += team_answer_result["answer"]

    # Print updated score after each question
    print(f"\nUpdated Score - Team A: {team_a_score} | Team B: {team_b_score}")

    if i == len(questions) - 1:
        print("\nGame Over!")
        engine.say("Game Over!")
        engine.runAndWait()
    else:
        print("\nNext Question...")

    # Wait a moment before proceeding to the next question
    time.sleep(5)

# Declare the winner at the end of the game
declare_winner(team_a_score, team_b_score)
