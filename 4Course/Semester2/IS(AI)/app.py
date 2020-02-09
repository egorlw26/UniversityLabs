from flask import Flask, flash, redirect, render_template, request
from Chatbot import *

app = Flask(__name__)

def getCategories():
    categories = []
    filePath = "custom_corpus/english.corpus.json"
    with open(filePath, "r") as json_file:
        data = json.load(json_file)
        for category in data:
            categories.append(category)
    return categories

@app.route("/", methods = ["GET"])
def index():
    return render_template('main.html', categories = getCategories())
        
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

@app.route("/addDialog/<category>/<input>/<output>", methods = ["POST"])
def addDialog(category, input, output):
    data = {}
    filePath = "custom_corpus/english.corpus.json"
    with open(filePath, 'r') as json_file:
        data = json.load(json_file)
        
    data[category].append([input, output])
    with open(filePath, 'w') as json_file:
        json.dump(data, json_file, indent=2)
    return ""

@app.route("/update", methods = ["POST"])
def trainBot():
    app.bot = Chatty()
    return ""

if __name__ == "__main__":
    trainBot()
    app.run(debug = True)