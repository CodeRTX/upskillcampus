const express = require('express');
const mongoose = require('mongoose');
const bodyParser = require('body-parser');

const app = express();
app.use(bodyParser.json());

// MongoDB Atlas Connection URI
const mongoURI = "mongodb+srv://ravocage:nF3yHXbritMQzoxJ@iitg.g15ff.mongodb.net/IoT_db?retryWrites=true&w=majority";

// Connect to MongoDB Atlas
mongoose.connect(mongoURI, {
    useNewUrlParser: true,
    useUnifiedTopology: true
}).then(() => console.log("✅ Connected to MongoDB Atlas"))
  .catch(err => console.error("❌ MongoDB Atlas connection error:", err));

// Define a Mongoose schema and model for sensor readings
const sensorSchema = new mongoose.Schema({
    timestamp: { type: Date, default: Date.now },
    temperature_in_c: Number,
    temperature_in_f: Number,
    humidity: Number,
    heat_index_in_c: Number,
    heat_index_in_f: Number
});

const SensorReading = mongoose.model('SensorReading', sensorSchema);

// Route to handle sensor data
app.post('/api/sensors', async (req, res) => {
    try {
        const sensorData = req.body;
        console.log("Received sensor readings:", sensorData);
        
        // Create a new document and save it to MongoDB
        const newReading = new SensorReading(sensorData);
        await newReading.save();
        
        res.status(201).json({ Status: "OK" });
    } catch (error) {
        console.error("Failed to insert data into MongoDB:", error);
        res.status(500).json({ Status: "Error inserting data", Error: error.message });
    }
});

// Start the server
const PORT = 5000;
app.listen(PORT, () => {
    console.log(`Server is running on http://0.0.0.0:${PORT}`);
});
