from flask import Flask, flash, redirect, render_template, request
from Chatbot import *

app = Flask(__name__)
app.bot = Chatty()

@app.route("/", methods = ["GET"])
def index():
    return render_template('main.html')
        
@app.route("/test", methods = ["POST"])
def test():
    msg = request.form.get("msg")
    print(msg)
    return {"response": str(app.bot.request(msg))}
    
if __name__ == "__main__":
    app.run(debug = True)