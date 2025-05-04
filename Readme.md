# Reflex Game

This project is a small reflex game in C++ / Arduino created to be loaded in an ESP32 card. This project use Platformio to works.

### About the [simulator](/reflex_game_sim.cpp)

The file reflex_game_sim.cpp is a simulator of the project behavior that works all in the console of your computer. Be free to use it if you want, you'll don't need any other files to make it work.

## How To Play

### Necessary Equipment

- ESP32 card
- OLED Screen SSD1306
- 3 LEDs
- 3 buttons
- wires (to connect them)
- USB câble -> connect the card to your pc

### Instructions To use this code

- make sure to have C++ installed on your computer
- Install Platformio on your IDE
- Download this repository using
    ````bash
    git clone https://github.com/Niviferum/IoT-Reflex-Game.git
    ````
- Open the PLatformio project using Platformio Home
- Build and load the project on the card (you must connect the card to your pc !)
- Get ready to play !

### In-Game

Once the game is launch a random LED will light up in a random timming between 1 and 5 seconds, you have 4 seconds to press the button corresponding to the lit LED as soon as possible.    
Your reaction time will be displayed each time you've pressed the button.

Once you've accomplish this 5 times the game ends giving you your average reaction time on the screen.

If you press the wrong button or don't react in time, the turn won't be counting on your score -> the game ends when you've pressed the correct button 5 times.

### Replay

When the game ends it ask you if you want to do a rematch, to do so just press 3 times on the button of your choice (3 times the same Button).  
The program will end after 10 seconds and put the card under deep sleep. To relaunch the game after this you'll need to use the 'RESET' button of the ESP32 card or unplug/replug it.

## Author

[Vallée Kenny](https://github.com/kenmanga666)
---

Well... Have Fun with this little project !

> [!IMPORTANT]
> This project is a school project, any return positive and negative are welcome, they'll serve to improve work quality.