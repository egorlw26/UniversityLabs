from flask import Flask, flash, redirect, render_template, request
from Chatbot import *
import os

app = Flask(__name__)
app.bot = Chatty()

def getCategories():
    categories = []
    for file in os.listdir("custom_corpus"):
        categories.append(file[:-4])
    return categories

@app.route("/", methods = ["GET"])
def index():
    return render_template('main.html', categories = getCategories())
        
@app.route("/test", methods = ["POST"])
def test():
    msg = request.form.get("msg")
    return {"response": str(app.bot.request(msg))}

@app.route("/addCategory/<categoryName>", methods = ["POST"])
def addCategory(categoryName):
    app.bot.add_dialogue(["Hi", "Hi"], categoryName)
    return ""    

@app.route("/addDialog/<category>/<input>/<output>", methods = ["POST"])
def addDialog(category, input, output):
    dialogue = [input, output]
    app.bot.add_dialogue(dialogue, category)
    return ""

@app.route("/update", methods = ["POST"])
def trainBot():
    app.bot.train()
    return ""

if __name__ == "__main__":
    trainBot()
    app.run(debug = True)