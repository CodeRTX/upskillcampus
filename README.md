# Mongo Secure Climate Logger

## Overview
The **Mongo Secure Climate Logger** is an IoT-based environmental monitoring system that captures real-time temperature, humidity, and heat index data. This project utilizes an **ESP8266** microcontroller and a **DHT11** sensor to collect data, which is then sent via Wi-Fi to a **RESTful API** built with **Node.js and Express.js**. The data is securely stored in **MongoDB Atlas**, ensuring scalability and persistence. Additionally, **pfSense** is used as a firewall solution to enhance network security.

## Features
- Real-time temperature and humidity monitoring
- Data transmission via Wi-Fi using ESP8266
- REST API for data storage and retrieval
- Secure data storage with MongoDB Atlas
- Interactive visualization using ThingSpeak
- pfSense firewall integration for enhanced security

## Technologies Used
- **Hardware:** ESP8266, DHT11 Sensor
- **Software & Tools:**
  - **Programming Languages:** JavaScript (Node.js)
  - **Backend Framework:** Express.js
  - **Database:** MongoDB Atlas
  - **Firewall:** pfSense
  - **Data Visualization:** ThingSpeak
  - **Development Environment:** Arduino IDE, VS Code

## Installation & Setup

### 1. Clone the Repository
```sh
git clone https://github.com/yourusername/mongo-secure-climate-logger.git
cd mongo-secure-climate-logger
```

### 2. Install Dependencies
```sh
npm install
```

### 3. Configure Environment Variables
Create a `.env` file and set up your MongoDB URI:
```sh
MONGO_URI=mongodb+srv://yourusername:yourpassword@yourcluster.mongodb.net/IoT_db
PORT=5000
```

### 4. Run the Server
```sh
node new.js
```
The server will start at `http://localhost:5000`

### 5. Deploy the Arduino Code
- Upload the arduino_code.ino to your ESP8266 using the Arduino IDE.
- Ensure the Wi-Fi credentials and API endpoint match the server settings.

## API Endpoints
| Method | Endpoint       | Description         |
|--------|----------------|---------------------|
| POST   | /api/sensors   | Stores sensor data  |

## Security Enhancements
- **pfSense Firewall**: Protects the REST API server and database.
- **Encrypted API Calls**: Ensures data integrity during transmission.
- **MongoDB Security Rules**: Restricts unauthorized access.

## Contributors
- Ankan Debnath
- Ravijyoti Motak

## License
This project is licensed under the **Apache License**.

## Acknowledgments
Special thanks to IoT Academy, IIT Guwahati, and Assam Skill Development Mission for their guidance and support.
