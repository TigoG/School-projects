<template>
    <div class="dashboard">
        <div class="panel">
            <div class="legend" v-if="dashboard">
                <div v-for="i in blocks" class="legend-item">
                    <div class="legend-color" :style="`background-color: ${i.color}`"></div>
                    <div>
                        <span>Robot {{ i.id }}</span>
                        <p class="robot-status" :style="emergency.has(i.id)? 'color:red' : ''" v-text="currentStatus(i.id)"></p>
                    </div>
                </div>
            </div>

            <canvas id="field" width="660" height="660"/>
        </div>

        <div class="controls" v-if="dashboard">
            <div>
                <button @click="stop" id="stop">Noodstop (alle robots)</button>
            </div>
            <div>
                <span>Geef de ID van de robot, of klik op de robot.</span>
                <div class="coords">
                    <span>[</span>
                    <input type="text" v-model="blockId">
                    <span>]</span>
                </div>
            </div>
            <div>
                <span>Geef een coordinaat, of klik op een vlak.</span>
                <div class="coords">
                    <span>(</span>
                    <input type="text" v-model="x">
                    <span>,</span>
                    <input type="text" v-model="y">
                    <span>)</span>
                </div>
            </div>
            <div>
                <button @click="target">Verplaats robot</button>
            </div>
        </div>
    </div>
</template>

<script>
import Dashboard from '@/assets/dashboard.js'
import client from '@/assets/mqtt.js'
import { toast } from 'vue3-toastify'

export default {
    name: 'App',
    data() {
        return {
            dashboard: null,
            x: null,
            y: null,
            blockId: null,
            searching: new Set(),
            emergency: new Set()
        }
    },
    methods: {
        async target() {
            if (this.blockId === null || this.x === null || this.y === null) {
                toast.error('Kies een robot en vul de coordinaten in.')
                return
            }

            if (this.x < -5 || this.x > 5 || this.y < -5 || this.y > 5) {
                toast.error('Coordinaten moeten tussen -5 en 5 liggen.')
                return
            }

            if (this.blockId < 1 || this.blockId > this.blocks.length) {
                toast.error('Kies een geldige robot.')
                return
            }

            if (this.dashboard.isObstacle(this.x, this.y)) {
                toast.error('Kan robot niet sturen naar een obstakel.')
                return
            }

            await this.publish('target/' + this.blockId, this.x + ',' + this.y)
            toast.info(`Robot ${this.blockId} heeft nu (${this.x}, ${this.y}) als target.`)

            this.reset()
        },
        async stop() {
            for (let i = 1; i <= this.blocks.length; i++) {
                await this.publish('stop/' + i, 'true')
            }
            toast.info('Noodstop ingedrukt.')
        },
        async publish(topic, message) {
            await client.publish(topic, message, (error) => {
                if (error) {
                    toast.error('Kon taak niet versturen. MQTT-service is niet beschikbaar.')
                }
                else {
                    console.log(`[MQTT] Published message on topic '${topic}': ${message}`)
                }
            })
        },
        reset() {
            this.x = null
            this.y = null
            this.blockId = null
        },
        currentStatus(id) {
            if (this.emergency.has(id)) {
                return 'Noodstop'
            }

            return this.searching.has(id)? 'Onderweg' : 'Doel bereikt'
        }
    },
    computed: {
        blocks() {
            return this.dashboard.blocks
        }
    },
    mounted() {
        const canvas = document.getElementById('field')
        this.dashboard = new Dashboard(canvas, 60, 11, 11)

        this.dashboard.canvas.addEventListener('click', (event) => {
            const coords = this.dashboard.getCoordsOfEvent(event)
            if (this.dashboard.isObstacle(coords.x, coords.y)) {
                toast.error('Kan robot niet sturen naar een obstakel.')
                return
            }
            if (coords.robot === -1) {
                this.x = coords.x
                this.y = coords.y
                toast.info(`Coordinaat (${coords.x}, ${coords.y}) geselecteerd.`)
            }
            else {
                this.blockId = coords.robot
                toast.info(`Robot ${coords.robot} geselecteerd.`)
            }
        })

        client.on("connect", () => {
            console.log('[MQTT] Connection established')

            client.subscribe('move/#', (error) => {
                if (error) {
                    toast.error('Kon niet verbinden met MQTT-service.')
                    return
                }

                toast.success('Verbonden met MQTT-service.')
                console.log('[MQTT] Subscribed to topic: move/#')
            })

            client.subscribe('obstacle/#', (error) => {
                if (error) return
                console.log('[MQTT] Subscribed to topic: obstacle/#')
            })

            client.subscribe('target/#', (error) => {
                if (error) return
                console.log('[MQTT] Subscribed to topic: target/#')
            })

            client.subscribe('goal/#', (error) => {
                if (error) return
                console.log('[MQTT] Subscribed to topic: goal/#')
            })
        })

        client.on("message", (topic, message) => {
            if (topic.toString().startsWith('move/')) {
                const botId = parseInt(topic.toString().substring(5))
                const coords = message.toString().split(",")
                const x = parseInt(coords[0])
                const y = parseInt(coords[1])

                // Roep de moveBlock-functie aan met de opgehaalde waarden
                this.dashboard.moveBlock(botId, x, y)
            }
            else if (topic.toString().startsWith('obstacle/')) {
                console.log('Webots is opnieuw opgestart.')
                this.emergency.clear()
            }
            else if (topic.toString().startsWith('target/')) {
                const botId = parseInt(topic.toString().substring(7))
                this.searching.add(botId)
            }
            else if (topic.toString().startsWith('goal/')) {
                const botId = parseInt(topic.toString().substring(5))
                if (message.toString() === 'false') {
                    this.searching.add(botId)
                }
                else if (message.toString() === 'true') {
                    this.searching.delete(botId)
                }
                else if (message.toString() === 'stop') {
                    this.searching.delete(botId)
                    this.emergency.add(botId)
                }
            }
        })
    }
}
</script>
