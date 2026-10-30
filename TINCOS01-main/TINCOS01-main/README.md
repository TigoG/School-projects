# Connected Systems
Groepsleden: Tigo, Thomas, Khizer, Eli

## Protocol en de topics
Er zijn 5 topics waarop gecommuniceerd wordt:
- `target/{robot_id}`: Wanneer er een target is gezet voor een robot.
- `move/{robot_id}`: Wanneer een robot een stap heeft gezet.
- `stop/{robot_id}`: Wanneer de noodknop voor de robot is ingedrukt (true).
- `obstacle/{obstacle_id}`: Obstakels die er zijn worden verstuurd.
- `goal/{robot_id}`: Wanneer een robot z'n doel heeft bereikt (true).

Zie verder `./documents/communication-scheme.png`.

## Voldaan aan de eisen
- ✅ De unit beweegt zich in de gegeven gesimuleerde ruimte.
- ❌ De unit ontwijkt andere aanwezige units en obstakels.
- ✅ De unit wisselt eigen positiegegevens en posities van obstakels uit met de centrale server.
- ✅ De unit bereikt het opgegeven doel.
- ✅ Een display op de unit geeft de gewenste richting aan voor iedere stap.
- ✅ De gewenste richting wordt ook aangeduid op fysieke hardware, en daar is ook een werkende noodstopknop aanwezig.
- ✅ De routes van de units worden bepaald door middel van een algoritme.
- ✅ Op het dashboard kunnen taken voor de units ingegeven worden en kan een noodstop gegeven worden.
- ✅ Informatie over de positie, de uitgevoerde en de nog openstaande taken van de units wordt afgebeeld op een dashboard.
- ❌ De units verdelen onderling taken afgestemd op de specifieke beperkingen van iedere unit.

## Opzetten van centrale server
1. Installeer Docker.
2. Ga naar `./central-server` en run `compose.sh`.

## Opzetten van Webots
1. Installeer Python 3.12. Zorg dat `python.exe` een command is.
2. Installeer Paho MQTT met `pip install paho-mqtt`.
3. Installeer Webots en open de Webots-omgeving.
    - Open `./webots/worlds/TINCOS_exam2022.wbt`.

## Opzetten van dashboard
1. Installeer Node.
2. Open `./dashboard` en open een Terminal daar.
3. Doe `npm install` om modules te downloaden.
4. Doe `npm run dev` en ga naar http://localhost/.

## Opzetten van hardware unit
1. Set the correct network credentials in secret.h:
    ```cpp
    #define SECRET_SSID "tesla iot"  
    #define SECRET_PASS "fsL6HgjN"
    ```
2. Set the correct robot id in main.cpp on the following lines:  
    ```cpp
    // In this code is the robot id 1
    #define PUBLISHTOPIC "data/BOT1"
    #define SUBSCRIBETOPIC "move/1"
    #define E_STOPTOPIC "stop/1"
    const char* BOT_ID = "1";
    ```

