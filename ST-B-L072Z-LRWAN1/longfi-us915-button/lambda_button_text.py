import base64
import json
import os
from botocore.vendored import requests

TWILIO_ACCOUNT_SID = os.environ.get("TWILIO_ACCOUNT_SID")
TWILIO_AUTH_TOKEN = os.environ.get("TWILIO_AUTH_TOKEN")
TWILIO_SMS_URL = "https://api.twilio.com/2010-04-01/Accounts/" + TWILIO_ACCOUNT_SID + "/Messages.json"
TWILIO_ASSIGNED_NUMBER = os.environ.get("TWILIO_ASSIGNED_NUMBER")
USER_NUMBER = os.environ.get("USER_NUMBER")

def lambda_handler(event, context):
    
    byte_arr = list(base64.b64decode(event['payload']))
    
    if byte_arr[0] == 1 :
        to_number =USER_NUMBER
        from_number = TWILIO_ASSIGNED_NUMBER
        body = "Button pressed!"
        data = {"To": to_number, "From": from_number, "Body": body}
    
        response = requests.post(TWILIO_SMS_URL, data = data, auth=(TWILIO_ACCOUNT_SID, TWILIO_AUTH_TOKEN))
        print(response.text)
        return "Button pushed - Message Sent"
    else: 
        print("Heartbeat")
        return "Heartbeat"
