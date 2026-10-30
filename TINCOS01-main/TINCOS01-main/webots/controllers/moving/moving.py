import paho.mqtt.client as mqtt

from controller import Supervisor

# Create the Supervisor instance
robot = Supervisor()
supervisorNode = robot.getSelf()

def on_connect(client, userdata, flags, rc, properties):
    # Subscribe to the stop topics when connected
    if unitId == "unit0":
        client.subscribe("stop/1")
        client.subscribe("target/1")
        client.publish("goal/1", f"false")
    elif unitId == "unit1":
        client.subscribe("stop/2")
        client.subscribe("target/2")
        client.publish("goal/2", f"false")
    elif unitId == "unit2":
        client.subscribe("stop/3")
        client.subscribe("target/3")
        client.publish("goal/3", f"false")
    elif unitId == "unit3":
        client.subscribe("stop/4")
        client.subscribe("target/4")
        client.publish("goal/4", f"false")

def on_message(client, userdata, msg):
    print(str(msg.payload.decode("utf-8")))
    if msg.payload.decode("utf-8") == "true":
        stop_robot()
    else:
        try:
            newPosX, newPosY = map(int, msg.payload.decode("utf-8").split(","))
            newPosX = (newPosX + 5)
            newPosY = (newPosY + 5)
            targetField = supervisorNode.getField("target")
            if targetField is not None:
                print (f"Moving {unitId} to new position: {newPosX}, {newPosY}")
                targetField.setSFVec2f([newPosX, newPosY])
                #check what id for target
                if unitId == "unit0":
                    client.publish("goal/1", f"false")
                elif unitId == "unit1":
                    client.publish("goal/2", f"false")
                elif unitId == "unit2":
                    client.publish("goal/3", f"false")
                elif unitId == "unit3":
                    client.publish("goal/4", f"false")
        except Exception as e:
            print("An error occurred:", e)

def stop_robot():
    global duration
    duration = -1
    #print("Stopping the robot")
    leds[0].set(1)
    leds[1].set(1)
    leds[2].set(1)
    leds[3].set(1)
    if unitId == "unit0":
        client.publish("goal/1", f"stop")
    elif unitId == "unit1":
        client.publish("goal/2", f"stop")
    elif unitId == "unit2":
        client.publish("goal/3", f"stop")
    elif unitId == "unit3":
        client.publish("goal/4", f"stop")

# Get the time step of the current world
timestep = int(robot.getBasicTimeStep())
# Calculate a multiple of timestep close to one second
duration = (1000 // timestep) * timestep

broker = 'mq.nl.eu.org'
port = 8883

# Define the topic
topic = "move"

# Connect to MQTT broker
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.username_pw_set("connectedsystems", "tincos")
client.on_connect = on_connect
client.on_message = on_message
client.connect(broker, port)

# Enable sensors
sensors = [robot.getDevice(f"sensor{i}") for i in range(4)]
for sensor in sensors:
    sensor.enable(duration)
    
client.publish("obstacle/1", f"-3:1:1:3")
client.publish("obstacle/2", f"-1:1:1:3")
client.publish("obstacle/3", f"1:1:1:3")
client.publish("obstacle/4", f"3:1:1:3")
client.publish("obstacle/5", f"-3:-1:7:1")
client.publish("obstacle/6", f"3:-1:7:1")

# LED devices for indicating movement direction
leds = [robot.getDevice(f"led{i}") for i in range(4)]

# Set to store obstacle positions
obstacle_positions = set()

# Store original target position
original_target_position = supervisorNode.getField("target").getSFVec2f() if supervisorNode.getField("target") else None

# Store direction of movement
move_direction = [0, -1]  # Default: move down

unitId = supervisorNode.getField("name").getSFVec2f()

client.loop_start()

# Execute every second
while robot.step(duration) != -1:
    try:
        # Read sensor values
        sensor_values = [sensor.getValue() for sensor in sensors]

        # Check if any sensor detects an obstacle
        if any(value != 1000 for value in sensor_values):
            # Add obstacle positions to the set
            obstacle_positions.add(tuple(supervisorNode.getPosition()))
            # If obstacle detected, stop and move back
            posX, posY, posZ = supervisorNode.getPosition()
            if move_direction == [0, -1]:
                posY += 0.1  # Move left by 0.1 units
                leds[0].set(1)
                leds[1].set(0)
                leds[2].set(0)
                leds[3].set(0)
            elif move_direction == [0, 1]:
                posY -= 0.1  # Move right by 0.1 units
                leds[0].set(0)
                leds[1].set(1)
                leds[2].set(0)
                leds[3].set(0)
            elif move_direction == [-1, 0]:
                posX += 0.1  # Move up by 0.1 units
                leds[0].set(1)
                leds[1].set(0)
                leds[2].set(1)
                leds[3].set(0)
            elif move_direction == [1, 0]:
                posX -= 0.1  # Move down by 0.1 units
                leds[0].set(0)
                leds[1].set(0)
                leds[2].set(0)
                leds[3].set(1)
            supervisorNode.getField("translation").setSFVec3f([posX, posY, posZ])

        else:
            # Move towards the target or resume original path
            targetField = supervisorNode.getField("target")
            if targetField is not None:
                targetVec = targetField.getSFVec2f()
                targetX = targetVec[0] / 10
                targetY = targetVec[1] / 10

                if original_target_position is not None:
                    original_targetX = original_target_position[0] / 10
                    original_targetY = original_target_position[1] / 10

                posX, posY, posZ = supervisorNode.getPosition()

                # If original target position is stored and robot has reached it, resume original path
                if original_target_position is not None and posX == original_targetX and posY == original_targetY:
                    targetX = original_targetX
                    targetY = original_targetY
                    original_target_position = None  # Reset original target position

                # Move horizontally towards the target
                if posX < targetX:
                    posX = min(posX + 0.1, targetX)
                    leds[0].set(1)
                    leds[1].set(0)
                    leds[2].set(0)
                    leds[3].set(0)
                    move_direction = [0, 1]  # Move left
                elif posX > targetX:
                    posX = max(posX - 0.1, targetX)
                    leds[0].set(0)
                    leds[1].set(1)
                    leds[2].set(0)
                    leds[3].set(0)
                    move_direction = [0, -1]  # Move right

            # Update position
            supervisorNode.getField("translation").setSFVec3f([posX, posY, posZ])

            # If horizontal position is reached, move vertically towards the target
            if posX == targetX:
                if posY < targetY:
                    posY = min(posY + 0.1, targetY)
                    leds[0].set(0)
                    leds[1].set(0)
                    leds[2].set(1)
                    leds[3].set(0)
                    move_direction = [-1, 0]  # Move up
                elif posY > targetY:
                    posY = max(posY - 0.1, targetY)
                    leds[0].set(0)
                    leds[1].set(0)
                    leds[2].set(0)
                    leds[3].set(1)
                    move_direction = [1, 0]  # Move down

        # Update position
        supervisorNode.getField("translation").setSFVec3f([posX, posY, posZ])

        # Publish robot's position
        posX2 = int(posX * 10 -5)
        posY2 = int(posY * 10 -5)

        if unitId == "unit0":
            client.publish(topic + "/1", f"{posX2},{posY2}")
        elif unitId == "unit1":
            client.publish(topic + "/2", f"{posX2},{posY2}")
        elif unitId == "unit2":
            client.publish(topic + "/3", f"{posX2},{posY2}")
        elif unitId == "unit3":
            client.publish(topic + "/4", f"{posX2},{posY2}")
        
        if targetX == posX and targetY == posY:
            if unitId == "unit0":
                client.publish("goal/1", f"true")
            elif unitId == "unit1":
                client.publish("goal/2", f"true")
            elif unitId == "unit2":
                client.publish("goal/3", f"true")
            elif unitId == "unit3":
                client.publish("goal/4", f"true")
        else:
            if unitId == "unit0":
                client.publish("goal/1", f"false")
            elif unitId == "unit1":
                client.publish("goal/2", f"false")
            elif unitId == "unit2":
                client.publish("goal/3", f"false")
            elif unitId == "unit3":
                client.publish("goal/4", f"false")


    except KeyboardInterrupt:
        # Stop if user interrupts the program
        break
    except Exception as e:
        print("An error occurred:", e)

# Start the MQTT client's loop to process incoming messages
client.loop_forever()
