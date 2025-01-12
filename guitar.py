import tkinter as tk
import random
import pygame
import time

# Initialize pygame for the metronome tick
pygame.mixer.init()

# Define major and minor chords
major_chords = ['C', 'D', 'E', 'F', 'G', 'A', 'B']
minor_chords = ['Am', 'Bm', 'Cm', 'Dm', 'Em', 'Fm', 'Gm']

timer_running = False
paused = False
metronome_active = False  # Track metronome state separately
chords = []
current_chord_index = 0
current_strum_index = 0
strumming_pattern = []  # Holds the current strumming pattern
beat_count = 1  # Track the 4-count (1, 2, 3, 4 and)
remaining_time = 5 * 60  # 5 minutes in seconds

# Load a tick sound (ensure you have a tick.wav or tick.mp3 in your working directory)
tick_sound = pygame.mixer.Sound("tick.wav")  # Replace with your tick sound file

# Strumming patterns
fixed_pattern = ["D", "D", "U", "D", "U"]
down_up_choices = ["D", "U"]

# Function to generate random chord progression
def generate_progression(scale='major', length=4):
    if scale == 'major':
        return random.sample(major_chords, length)
    elif scale == 'minor':
        return random.sample(minor_chords, length)
    else:
        return random.sample(major_chords + minor_chords, length)

# Timer control functions
def start_timer():
    global timer_running, paused, current_chord_index, remaining_time, metronome_active, beat_count
    if not chords and not strumming_pattern:
        chord_label.config(text="Select a progression or pattern first")
        return
    if not timer_running:
        timer_running = True
        paused = False
        metronome_active = metronome_var.get()  # Check if the metronome is enabled
        remaining_time = 5 * 60  # Reset to 5 minutes
        current_chord_index = 0  # Reset the chord index
        current_strum_index = 0  # Reset the strumming index
        beat_count = 1  # Start the count at 1
        display_strumming_pattern()  # Display the strumming pattern
        countdown()  # Start the countdown
        loop_chords_and_strumming()  # Start looping through chords and strumming
    else:
        chord_label.config(text="Timer already running")

def resume_timer():
    global timer_running, paused, metronome_active
    if paused:
        paused = False
        metronome_active = metronome_var.get()  # Use the checkbox state to decide metronome
        countdown()  # Resume the countdown
        loop_chords_and_strumming()  # Resume the chords and strumming

def pause_timer():
    global paused, metronome_active
    paused = True
    metronome_active = False  # Stop the metronome when paused

def stop_timer():
    global timer_running, paused, remaining_time, metronome_active
    timer_running = False
    paused = False
    metronome_active = False  # Stop the metronome when timer stops
    timer_label.config(text="05:00")
    chord_label.config(text="")
    strumming_label.config(text="")
    remaining_time = 5 * 60  # Reset remaining time

# Function to loop through chords and strumming patterns together with 4-count system
def loop_chords_and_strumming():
    global current_chord_index, current_strum_index, metronome_active, beat_count
    if timer_running and not paused:
        bpm = int(tempo_entry.get())
        beat_interval = 60 / bpm  # Seconds per beat
        half_beat_interval = beat_interval / 2  # Seconds for "and"

        if beat_count in [1, 2, 3, 4]:  # Downstrokes (numbers)
            update_strumming_colors(current_strum_index)
            if strumming_pattern[current_strum_index] == "D":
                if metronome_active:
                    play_metronome_tick()  # Play tick on downbeats (1, 2, 3, 4)

            current_strum_index = (current_strum_index + 1) % len(strumming_pattern)  # Move to next strum
            delay = beat_interval

        elif beat_count in [1.5, 2.5, 3.5, 4.5]:  # Upstrokes ("and")
            update_strumming_colors(current_strum_index)
            if strumming_pattern[current_strum_index] == "U":
                current_strum_index = (current_strum_index + 1) % len(strumming_pattern)  # Move to next strum
            delay = half_beat_interval  # Half the delay for the "and" (upstroke)

        # Increment the beat count and loop back to 1 after 4
        beat_count += 0.5
        if beat_count > 4.5:
            beat_count = 1

        # Handle chord progression
        if chords:
            if current_chord_index >= len(chords):
                current_chord_index = 0  # Restart the chord progression when it ends
            chord_label.config(text=chords[current_chord_index])  # Show the current chord
            current_chord_index += 1

        root.after(int(delay * 1000), loop_chords_and_strumming)  # Delay in milliseconds

# Function to play metronome tick on Down beats only
def play_metronome_tick():
    pygame.mixer.Sound.play(tick_sound)  # Play tick sound

# Function to set the custom strumming pattern
def set_custom_pattern():
    global strumming_pattern
    pattern_input = custom_strum_entry.get().upper()  # Get custom input and ensure it's uppercase
    strumming_pattern = list(pattern_input)  # Set pattern as a list of "D" and "U"
    strumming_label.config(text="Custom pattern set")
    display_strumming_pattern()

# Function to set the fixed strumming pattern
def set_fixed_pattern():
    global strumming_pattern
    strumming_pattern = fixed_pattern  # Set fixed pattern
    strumming_label.config(text="Fixed pattern set")
    display_strumming_pattern()

# Function to generate a random strumming pattern
def set_random_pattern():
    global strumming_pattern
    strumming_pattern = random.choices(down_up_choices, k=5)  # Generate random pattern of length 5
    strumming_label.config(text="Random pattern set")
    display_strumming_pattern()

# Function to display the full strumming pattern on the screen
def display_strumming_pattern():
    pattern_display = " ".join(strumming_pattern)  # Join the pattern with spaces
    strumming_display_label.config(text=pattern_display)  # Display the full pattern

# Function to update the strumming pattern color
def update_strumming_colors(index):
    full_pattern = strumming_pattern.copy()  # Copy the pattern to manipulate the display
    for i in range(len(full_pattern)):
        if i == index:
            full_pattern[i] = f"[{full_pattern[i]}]"  # Highlight the current strum
        else:
            full_pattern[i] = full_pattern[i]  # Keep the others normal

    strumming_display_label.config(text=" ".join(full_pattern))  # Update the display

# Timer countdown function
def countdown():
    global remaining_time, timer_running
    if timer_running and not paused:
        minutes, sec = divmod(remaining_time, 60)
        timer_label.config(text=f"{minutes:02}:{sec:02}")
        if remaining_time > 0:
            remaining_time -= 1
            root.after(1000, countdown)  # Tick every second
        else:
            timer_running = False
            chord_label.config(text="Session complete!")
            strumming_label.config(text="Session complete!")

# Function to start practice with selected chord progression
def start_practice(scale='major'):
    global chords
    chords = generate_progression(scale)
    chord_display_label.config(text=' '.join(chords))
    chord_label.config(text="")

# Main window setup
root = tk.Tk()
root.title("Guitar Practice App")
root.geometry("400x600")

# Timer display
timer_label = tk.Label(root, text="05:00", font=("Helvetica", 18))
timer_label.pack(pady=10)

# Chord display
# Chord display
chord_label = tk.Label(root, text="", font=("Helvetica", 24))
chord_label.pack(pady=10)

# Strumming pattern display (for the full pattern with color changes)
strumming_display_label = tk.Label(root, text="", font=("Helvetica", 24))
strumming_display_label.pack(pady=10)

# Status message for strumming pattern setup
strumming_label = tk.Label(root, text="", font=("Helvetica", 12))
strumming_label.pack(pady=10)

# Full chord progression display
chord_display_label = tk.Label(root, text="", font=("Helvetica", 12))
chord_display_label.pack(pady=10)

# Tempo input
tempo_label = tk.Label(root, text="Enter Tempo (BPM):")
tempo_label.pack()
tempo_entry = tk.Entry(root)
tempo_entry.insert(0, "120")  # Default tempo
tempo_entry.pack(pady=10)

# Custom strumming pattern input
custom_strum_label = tk.Label(root, text="Enter Custom Strumming Pattern (e.g., DDUUD):")
custom_strum_label.pack()
custom_strum_entry = tk.Entry(root)
custom_strum_entry.pack(pady=10)
custom_strum_button = tk.Button(root, text="Set Custom Pattern", command=set_custom_pattern)
custom_strum_button.pack(pady=5)

# Buttons for fixed and random patterns
fixed_strum_button = tk.Button(root, text="Set Fixed Pattern (D D U D U)", command=set_fixed_pattern)
fixed_strum_button.pack(pady=5)

random_strum_button = tk.Button(root, text="Set Random Pattern", command=set_random_pattern)
random_strum_button.pack(pady=5)

# Buttons to start chord progression modes
major_button = tk.Button(root, text="Major Progression", command=lambda: start_practice('major'))
minor_button = tk.Button(root, text="Minor Progression", command=lambda: start_practice('minor'))
random_button = tk.Button(root, text="Random Progression", command=lambda: start_practice('random'))

major_button.pack(side="left", padx=10)
minor_button.pack(side="left", padx=10)
random_button.pack(side="left", padx=10)

# Metronome checkbox
metronome_var = tk.IntVar()  # Variable to store checkbox state
metronome_checkbox = tk.Checkbutton(root, text="Enable Metronome", variable=metronome_var)
metronome_checkbox.pack(pady=5)

# Start Timer button
start_timer_button = tk.Button(root, text="Start Timer", command=start_timer)
start_timer_button.pack(pady=10)

# Resume Timer button
resume_button = tk.Button(root, text="Resume Timer", command=resume_timer)
resume_button.pack(pady=5)

# Timer control buttons
pause_button = tk.Button(root, text="Pause Timer", command=pause_timer)
pause_button.pack(pady=5)

stop_button = tk.Button(root, text="Stop Timer", command=stop_timer)
stop_button.pack(pady=5)

root.mainloop()
