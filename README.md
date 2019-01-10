# Überprachtige-KBS-game
A Bomberman game running on arduino's with multiplayer functionality.
## Game Description
The game supports two players, referred to as peeps. Peep 1 is the master and Peep 2 is the slave.
When the game is started, the level select screen will be opened. Peep 1 will be able to choose a level and Peep 2 will wait for Peep 1 to select a level. The game has three levels, but a random level can also be chosen. Level 1 & 2 are levels made by us. Level 3 is an empty level.
When selecting a level, Peep 1 will send the level number. If the selected level is random, a seed byte to generate the level will also be sent.

Afterwards, the game will be loaded. The layout of the screen can be seen in the image below
![Game screen layout](https://i.imgur.com/Zxvom9d.jpg)

The level can be seen on the left of the screen. It shows the peeps (Peep 1 is blue, Peep 2 is red), the barrels and the unbreakable blocks. On the right, important values are shown.
The values are, from top to bottom:
* Peep 1's lives
* Peep 2's lives
* The countdown timer
* Peep 1's score
* Peep 2's score

The peeps can move indepentently from eachother. The new location of the peeps after they have moved will be sent over infrared. The peeps can also place bombs, but this currently is broken. If it would work though, the bomb location would be sent over and would explode for both players. If a peep gets hit, a life is removed and the new lives will be sent over. Peeps will also get scorepoints for hitting eachother.
When a player is dead, or the timer has ran out, the game will be over. If the game ended on someone losing all their lives, the winner will be the one who is still alive. If the timer has ran out, the winner will be based on score. Players get a bonus when the game has ended.

# Installation
## Environment setup
### Linux
To set up the environment in Linux, execute `./setup_envirenment`.
This will dowload the Aruino subsystem and the Arduino-Makefile files in the home directory of the current user.
### Windows
Cry

_**For some reason, you can't build the project in Windows**_
## Building

To easily specify for which player you would like to compile, specify `PEEP` in your command:

`make PEEP=1`

or

`make PEEP=2`

If you want to specify a device, use the option `DEVICE_PATH`

Your command would look like
`make DEVICE_PATH=/dev/ttyACM1`

And thus to build code for peep 2 and for another device, and monitor it:
`make clean upload monitor DEVICE_PATH=/dev/ttyACM1 PEEP=2`

# Instructions for playing the game
## Level selection screen
* Use the joystick on the nunchuk to change selected button
* Then press the Z button to accept the selection
## The game
* Use the joystick to move the peep
* Press the Z button to place a bomb
* Run away to avoid the bomb
* The peep who dies first loses.
* If the timer expires, the peep with the least amount of lives left, wins.
* If both peeps have the save score, then it's a draw.
## Endscreen
* Press the reset button on the arduino to go to the start.



_Have fun playing!_



```
Frans Bauer FTW
                                                                                
                                           ~+:                                  
                                       =~:+=+$=?+~+=                            
                                      IIZZO7I~7IO$==?~                          
                                    ~:I$ZZ$7$OOZZ7ZZ$IZ7                        
                                    Z7=I$Z$Z$$$$$ZO88OO8                        
                                  :+$+I??????IIII77ZD88D                        
                                  :7+I????????III7I$88D8:                       
                                  ~?????++++++II7II7OOD8                        
                                  :?I?++=+===?I7III7Z8D8~                       
                                   II?++++===??IIII7Z88D7                       
                                  ~$?++===+==+??III7888Z7                       
                                  =IO7=====~=?II?II7O8O$?                       
                                  +??D8$??7777??7??IZ87+                        
                                  ?+????=+=??77I=+?IZZ??                        
                                  ++=~+?=++~=====+I777=~                        
                                  ++==?+~+I+=~~=+?77I7                          
                                  ~??+$++II~+==+??I77$                          
                                   I+7I?++?+????III$$Z                          
                                   :?????+???I+??I$ZO7                          
                            :   :~=+II?I$II??+II7ZZZ77+ ~                       
                       :~~====?I7I7I=$I???I++?I:~~$7$7ZZ?=                      
                     ~?I$$$$7$$I77$7=+77II??77~=~+I7?ZZZZ+~:                    
                   :I$ZZZZZZZZ$$7$$$$?$88OO88$=~=77OZZOZZZ77~:=                 
                  +7ZZZOZZZZZZ$$$ZZZZZ=?7II7?+=~=7ZZZZZOZZZ$7=~::+              
                 +I$ZZZZZZZZZZ$Z$ZZZZZZZZZZZZZ~~==ZZZZOOZ~=::::::::+            
                =$ZZZZZZZZZZZZ$ZZZZZZZZZZZZ~===+~~~~IOO?=?+==+~~~Z=~+           
               $$ZZZZZZZZZZZZZZZZZZZZZZZZ~=~~=+==~=7Z7I$+$$77$ZZZZZ~~          
               $ZZZZZZZZZZZZZZZZZZZZZZZZZZO$$?++=~~=?ZZ$+Z$$7$ZOZ==7?=:         
              =ZZZOZZ$ZZZZZZZZZZZZZZZZZZZZ~===~~~~=:?7$$Z$Z$ZOZOZZZOZZ+~        
             ~$ZZZOZOZZZZZZZZZZZ==ZZZZZZZZZ7+I7=~~~~=77ZZZZZZOOZZZOZOZZ$~       
            ?7ZZZOZZOZZZZZZZZZZ==+OZZZZZZZ$==~===~~~=7$ZZZ$ZZZOZZOOOOOZZ$       
           :?ZZZZOOOOZ$Z=~:   ==?NNZZZZZZZZZ?$77?=~=Z7$+OOZOZZOOOOOOOOOOO?      
           :ZZZOOOOOOZZ?====~==7DNOZZZZZZZZ$+==~=:=77$Z?7 8ZOZOZZZOOOOOOOZ      
           ZZOOOOOOOOZ+======++IIZZZZZZZZZZZZO7??I7O$$7?ZMDDOOOOOOOOZOOOOO::    
          ?ZZZOOOOOOOZ7+===~~=+?IDZZZZZZZZZZZDN$?7$$Z$= NDDNZOOOOOOOOZOOOO      
         IZZZZOOOOOOO77==+=  =+IIIZZZZZZZZZZZONNNNI+:D8DNDD8I7OOOOOOOOOOOZ      
        ?78ODD8888Z7+7$?++7???7?7NNZZZZZZZZZZODNNDNDDDDDDM87$I7ZOOOOOOOOOZ      
       =:~=7$$ZZZ77II7II+++==++=+IOOZZZZZZZZZZ8NNNNNDDDDD8I777?7OOOOOOOO8       
       I?I???I$$7$7I?$77$77ZZ$ODNNNOOZZZZZOZZZO8DNN7+=+??IIIII7I~+?I$OOOO       
      77$7I7$7$777IIII$Z$Z8MMMMNMNOOOZZZZOZZOOOZZO8DI++++?I?III77=+I77OOZ       
      $$$$$$$$$$77I??I7OMMMNO8OOOOOOZZOZOOOOOOOOOOO87I?I++?IIII7$$+~77$OO       
      7Z$Z$$$$$777I?I7OMOOOOOOOOOOOOOZOOOOOOOOOOOOOO8I???+II?II777$7=?$$        
      $ZZZ$$$$7$77$$Z8N8O8O8OOOOOOOOOOOOOOOOOOOOOOOOO7I??++III7I777Z7?$$        
      $$$Z$$$$7$$7ZOO 88OO8OOOOOOOOOOOOOOOOOOOOOOOOOOOII?????III77$$$$7+        
       $$$$$$$$$$O$   88O8OOOOOOOOOOOOOOOOOOOOOOOOOOO8$I?????IIIII$$ZZZ=        
        ZZOZZ?:       88OOOOOOOOOOOOOOOOOOOOO88OOOOOOO8$7I?I??I?I77$$ZZ+        
                     IO8OOOOOOOOOOOOOOOOOOOOOOO8OO888O8Z$I???IIII777$ZO+        
                     ZOOOOOOOOOOO88OOOOOO8OOO8888888888DZ7777IIIIII7$ZO:        
                     ZOOOOOOOO88888OO8888888888888888888N8$$77IIIII7$ZZ::       
                     ZO8OOOO8888888O888888888888888888888NNZ$7777777ZZO         
```

