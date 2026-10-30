# School projects

An overview of the projects in this folder. Each project lives in its own directory; the descriptions are based on the code and documentation currently included.

## HBO projects

- **Central Server** (`centralServer-main`) — A desktop web dashboard for weather stations, with MQTT-related components, sensor views, and history. This is connected to **Minor Smart Things** below.
- **Kickstarter Minor** (`kickstarter-minor-master`) — A React and TypeScript dashboard prototype for monitoring people/devices, with login, device cards, charts, logs, and an alarm interface.
- **Lift** (`Lift-main`) — Arduino lift/elevator prototype code. It reads floor sensor inputs, displays the selected floor, and includes separate I²C master/slave experiments.
- **Minor Smart Things** (`Minor_Smart_Things-main`) — Embedded projects for a motorized shade actuator and a weather station that reads sensors, displays measurements, and communicates wirelessly. It works together with the Central Server project.
- **Automating Sailing Race Signals** (`Project_78-Automatisering-seinen-bij-zeilwedstrijden-main`) — A sailing-race signaling system using Arduino/ESP32 hardware and an e-paper display, with a dashboard prototype and battery-monitoring code.
- **Year 3 Internship** (`StageJaar3-main`) — Internship prototype for a consent-management web application, split into a Flutter mobile app, a React web app, and a plugin/integration component.
- **TI Project 3–4** (`TI-PROJECT3-4-main`) — Java, Arduino, and Node.js experiments and project code, including serial communication, keypad/display input, database access, and a bank API example.
- **Connected Systems (TINCOS01)** (`TINCOS01-main`) — A multi-robot system combining Webots simulation, physical robot controllers, an MQTT central server, and a dashboard for assigning tasks and tracking robot status.
- **Project 56: SCR Sensor Analysis** (`TINPRJ0456-main`) — A system for measuring SCR exhaust sensor data, showing and storing readings, and managing measurement setups for later analysis. The repository includes a Java backend, frontend, and hardware/deployment files.
- **Autonomous TORCS Driver** (`TORCS-main`) — A machine-learning racing bot for the TORCS simulator, with Python client/training code and a bundled TORCS server environment.

## University projects before HBO

- **Tetris** (`Tetris-master`) — A C# Tetris game built with MonoGame, including its own grid, piece, input, and game-world logic.
- **TickTick** (`TickTick-master`) — A C# 2D game built on a reusable game engine, with menus, level selection, gameplay states, and level objects.

## Connected projects

`centralServer-main` and `Minor_Smart_Things-main` are parts of the same connected weather-station system: the latter contains the station/embedded projects, while the former contains the central dashboard.
