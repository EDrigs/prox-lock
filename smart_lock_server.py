from flask import Flask, request, jsonify
from twilio.rest import Client
from twilio.twiml.messaging_response import MessagingResponse
from flask_cors import CORS
from dotenv import load_dotenv
import random
import requests
import time
import os

app = Flask(__name__)
CORS(app)

# Twilio Configuration
load_dotenv()
ACCOUNT_SID = os.getenv("TWILIO_ACCOUNT_SID")
AUTH_TOKEN = os.getenv("TWILIO_AUTH_TOKEN")
TWILIO_PHONE_NUMBER = os.getenv("TWILIO_PHONE_NUMBER")
ADMIN_PHONE_NUMBER = os.getenv("ADMIN_PHONE_NUMBER")

client = Client(ACCOUNT_SID, AUTH_TOKEN)

# Arduino / Smart Lock Setup
ARDUINO_IP = os.getenv("ARDUINO_IP")    
API_SECRET = os.getenv("API_SECRET")

# Temporary In-Memory Storage
current_otp = None
reset_requests = {}

@app.route("/", methods=["GET"])
def home():
    return jsonify({
        "message": "Smart Lock Server API",
        "status": "running",
        "endpoints": {
            "GET /get_status": "Check server status",
            "POST /relay_otp": "Send OTP to phone",
            "POST /sms": "Twilio webhook for SMS"
        }
    }), 200

# Basic Status Endpoint
@app.route("/get_status", methods=["GET"])
def status():
    return jsonify({"status": "online", "message": "Smart Lock Server running"}), 200


@app.route("/relay_otp", methods=["POST"])
def relay_otp():
    data = request.get_json()
    otp = data.get("otp")
    secret = data.get("secret")

    if secret != API_SECRET:
        return jsonify({"error": "Unauthorized"}), 403
    
    if not otp:
        return jsonify({"error": "OTP missing"}), 400
    
    client.messages.create(
        body=f"Smart Lock: Your one-time passcode is {otp}. Enter this code within 2 minutes to unlock the door.",
        from_=TWILIO_PHONE_NUMBER,
        to=ADMIN_PHONE_NUMBER
    )
    print(f"📤 Sent OTP to admin: {otp}")
    return jsonify({"status": "sent"}), 200




# ===============================
# 🚀 Run the Server
# ===============================
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
