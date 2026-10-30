import { toast } from 'vue3-toastify'

class Block {
    constructor(id, x, y, color) {
        this.id = id
        this.x = x
        this.y = y
        this.color = color
    }

    // Verplaats de positie van het blok
    move(newX, newY) {
        this.x = newX
        this.y = newY
    }
}

class Dashboard {
    constructor(canvas, cellSize, rows, columns) {
        this.canvas = canvas
        this.ctx = this.canvas.getContext('2d')
        this.cellSize = cellSize
        this.rows = rows
        this.columns = columns

        // Initial blokken posities en kleuren
        this.blocks = [
            new Block(1, -5, -5, 'red'),
            new Block(2, 5, -5, 'orange'),
            new Block(3, 5, 5, 'green'),
            new Block(4, -5, 5, 'blue')
        ]

        // Posities van obstakels
        this.obstacles = [
            { x: -3, y: 1, width: 1, height: 3 },
            { x: -1, y: 1, width: 1, height: 3 },
            { x: 1, y: 1, width: 1, height: 3 },
            { x: 3, y: 1, width: 1, height: 3 },

            { x: -3, y: -1, width: 7, height: 1 },
            { x: -3, y: -3, width: 7, height: 1 }
        ]

        this.drawBlocks()
    }

    drawGrid() {
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height)
        for (let i = 0; i < this.rows; i++) {
            for (let j = 0; j < this.columns; j++) {
                this.ctx.strokeRect(i * this.cellSize, j * this.cellSize, this.cellSize, this.cellSize)
            }
        }
    }

    drawBlocks() {
        this.drawGrid()
        const centerX = Math.floor(this.columns / 2)
        const centerY = Math.floor(this.rows / 2)

        this.obstacles.forEach(obstacle => {
            const x = (obstacle.x + centerX) * this.cellSize
            const y = (centerY - obstacle.y - obstacle.height + 1) * this.cellSize
            this.ctx.fillStyle = 'gray'
            this.ctx.fillRect(x, y, obstacle.width * this.cellSize, obstacle.height * this.cellSize)
        })

        this.blocks.forEach(block => {
            const x = (block.x + centerX) * this.cellSize
            const y = (centerY - block.y) * this.cellSize
            this.ctx.fillStyle = block.color
            this.ctx.fillRect(x, y, this.cellSize, this.cellSize)
        })
    }

    moveBlock(blockId, newX, newY) {
        const block = this.blocks.find(block => block.id === blockId)

        if (!block) {
            toast.error(`Kon robot ${blockId} niet verplaatsen op (${newX}, ${newY}).`)
            return
        }

        // if (this.isObstacle(newX, newY)) {
        //     toast.error(`Kon robot ${blockId} niet op obstakel (${newX}, ${newY}) plaatsen.`)
        //     return
        // }

        block.move(newX, newY)
        this.drawBlocks()
    }

    isObstacle(x, y) {
        return this.obstacles.some(obstacle => {
            return x >= obstacle.x && x < obstacle.x + obstacle.width &&
                y >= obstacle.y && y < obstacle.y + obstacle.height
        })
    }

    getRobotAt(x, y) {
        const block = this.blocks.find(block => block.x === x && block.y === y)
        return block? block.id : -1
    }

    getCoordsOfEvent(event) {
        const rect = this.canvas.getBoundingClientRect()
        const x = event.clientX - rect.left
        const y = event.clientY - rect.top

        const centerX = Math.floor(this.columns / 2)
        const centerY = Math.floor(this.rows / 2)

        const fieldX = Math.floor(x / this.cellSize) - centerX
        const fieldY = centerY - Math.floor(y / this.cellSize)
        const robot = this.getRobotAt(fieldX, fieldY)

        return {
            robot,
            x: fieldX,
            y: fieldY
        }
    }
}

export default Dashboard
