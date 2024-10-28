// Establish WebSocket connection
const socket = new WebSocket('ws://your-websocket-url');

// Handle connection events
socket.addEventListener('open', function() {
    console.log('WebSocket connection established.');
    updateConnectionStatus(true); // Update connection status
    resetActualSpeeds(); // Reset actual speeds when connection is established
});

socket.addEventListener('close', function() {
    console.log('WebSocket connection closed.');
    updateConnectionStatus(false); // Update connection status
});

// Function to update connection status
function updateConnectionStatus(isConnected) {
    const connectionStatusElements = document.querySelectorAll('.connection-status');
    connectionStatusElements.forEach(element => {
        element.textContent = isConnected ? 'Connection Established' : 'No Connection Established';
        element.classList.toggle('text-red-500', !isConnected); // Change color based on connection status
        element.classList.toggle('text-green-500', isConnected); // Change color if connected
    });

    if (!isConnected) {
        resetActualSpeeds(); // Reset actual speeds to zero if no connection
    }
}

// Function to reset actual speed displays
function resetActualSpeeds() {
    document.getElementById('car1-actual-speed').textContent = '0';
    document.getElementById('car2-actual-speed').textContent = '0';
    document.getElementById('car3-actual-speed').textContent = '0';
}

// Function to send desired speed to PWM
function sendDesiredSpeed(carId, speed) {
    // Implement the logic to send speed to PWM
    // Example: socket.send(JSON.stringify({ carId, speed }));
    console.log(`Sending desired speed for ${carId}: ${speed} KM/H`);
}

// JavaScript to handle slider input and update desired speed value
document.getElementById('car1-slider').addEventListener('input', function() {
    const speedValue = this.value;
    document.getElementById('car1-speed').textContent = speedValue; // Update desired speed display
    sendDesiredSpeed('Car 1', speedValue); // Send desired speed to PWM
});

document.getElementById('car2-slider').addEventListener('input', function() {
    const speedValue = this.value;
    document.getElementById('car2-speed').textContent = speedValue; // Update desired speed display
    sendDesiredSpeed('Car 2', speedValue); // Send desired speed to PWM
});

document.getElementById('car3-slider').addEventListener('input', function() {
    const speedValue = this.value;
    document.getElementById('car3-speed').textContent = speedValue; // Update desired speed display
    sendDesiredSpeed('Car 3', speedValue); // Send desired speed to PWM
});

// Listen for incoming messages from the WebSocket
socket.addEventListener('message', function(event) {
    const data = JSON.parse(event.data);
    
    // Assuming data structure is { carId, actualSpeed, distanceFront, distanceBack }
    if (data.carId === 'Car 1') {
        document.getElementById('car1-actual-speed').textContent = data.actualSpeed;
        document.getElementById('car1-front').textContent = data.distanceFront + ' m';
        document.getElementById('car1-back').textContent = data.distanceBack + ' m';
    } else if (data.carId === 'Car 2') {
        document.getElementById('car2-actual-speed').textContent = data.actualSpeed;
        document.getElementById('car2-front').textContent = data.distanceFront + ' m';
        document.getElementById('car2-back').textContent = data.distanceBack + ' m';
    } else if (data.carId === 'Car 3') {
        document.getElementById('car3-actual-speed').textContent = data.actualSpeed;
        document.getElementById('car3-front').textContent = data.distanceFront + ' m';
        document.getElementById('car3-back').textContent = data.distanceBack + ' m';
    }
});