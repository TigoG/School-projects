import mqtt from 'mqtt'
import { toast } from 'vue3-toastify'

const client = mqtt.connect('ws://mq.nl.eu.org:8884', {
    clean: true,
    connectTimeout: 4000,
    reconnectPeriod: 5000,
    username: 'connectedsystems',
    password: 'tincos',
    keepalive: 60,
    resubscribe: true
})

client.on('close', () => {
    console.log('[MQTT] Connection closed')
    toast.error('Verbinding verbroken met MQTT-service.')
})

client.on('error', (error) => {
    console.error('[MQTT] Connection failed', error)
})

export default client
