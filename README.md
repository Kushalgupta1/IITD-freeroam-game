### COP290 Task2: A 2 player maze game

by Kushal Kumar Gupta, 2020CS10355
& Divyanshu Agarwal, 2020CS10343

A 2-player game set in the IITD campus, made using SDL in C++. The 2 players can play by  connecting to a common Wi-Fi network. 

See `howtoplay.pdf` for the rules of the game.

#### Installation instructions (for Ubuntu):

- First install the SDL packages using the command terminal using commands-

  ```bash
  apt-get install libsdl2-dev
  apt-get install libsdl2-image-dev
  apt-get install libsdl2-mixer-dev
  apt-get install libsdl2-ttf-dev
  ```

- Then clone the github repo `cop-game` by Kushalgupta1.

- The 2 players should be connected to the same Wi-Fi. The 2 players playing are the server and  the client. The server should delete the `game-client.cpp` and the client should delete the `game-server.cpp file`.

- Enter the server's IP address in the `game-server.cpp` of server and `game-client.cpp` of client in the `serv_ip[]`.

- On the terminal, enter the command `make` to generate the executable `play`.

- Execute the game using `./play`.



#### Resources and references:

1. For SDL,
   - https://lazyfoo.net/tutorials/SDL/index.php
   - YouTube playlist: https://www.youtube.com/watch?v=QQzAHcojEKg&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx
   - https://www.libsdl.org/
2. For making tilemap,
   - Using `Tiled` software, YouTube playlist: https://www.youtube.com/watch?v=ZwaomOYGuYo&list=PLu4oc9P-ABcOXNOyoAvnMyUwn_kkiVA5B
   - Various tilesets obtained from many sources like https://itch.io/ and https://opengameart.org/
3. For socket programming,
   - https://www.geeksforgeeks.org/socket-programming-cc/
   - https://beej.us/guide/bgnet/html/

