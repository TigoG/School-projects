import './assets/main.css'
import 'vue3-toastify/dist/index.css'

import { createApp } from 'vue'
import App from './App.vue'

import Vue3Toastify from 'vue3-toastify'

createApp(App)
    .use(Vue3Toastify)
    .mount('#app')
