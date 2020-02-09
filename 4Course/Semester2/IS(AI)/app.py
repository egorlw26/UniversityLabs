from flask import Flask, flash, redirect, render_template, request
from Chatbot import *

app = Flask(__name__)
app.bot = Chatty()

categories = []
def getCategories():
    filePath = "custom_corpus/english.corpus.json"
    with open(filePath, "r") as json_file:
        data = json.load(json_file)
        for category in data:
            categories.append(category)

@app.route("/", methods = ["GET"])
def index():
    getCategories()
    print(categories)
    return render_template('main.html', categories = categories)
        
@app.route("/test", methods = ["POST"])
def test():
    msg = request.form.get("msg")
    print(msg)
    return {"response": str(app.bot.request(msg))}

@app.route("/addCategory/<categoryName>", methods = ["POST"])
def addCategory(categoryName):
    data = {}
    filePath = "custom_corpus/english.corpus.json"
    with open(filePath, "r") as json_file:
        data = json.load(json_file)
    
    data[categoryName] = []
    
    with open(filePath, "w") as json_file:
        json.dump(data, json_file, indent=2)
    
    return ""    

if __name__ == "__main__":
    app.run(debug = True)