import json
from chatterbot import ChatBot
from chatterbot.trainers import ListTrainer, ChatterBotCorpusTrainer

class Chatty:
    def __init__(self):
        self.bot = ChatBot('Chatty', 
            default_response = "I don't understand you :(")
        self.data_path = 'custom_corpus/english.corpus.json'
        self.trainer = ChatterBotCorpusTrainer(self.bot)
        self.trainer.train(self.data_path)
        
    def request(self, text):
        return self.bot.get_response(text)
        
    def add_to_corpus(self, request, response, category = "undefined"):
        with open(self.data_path, 'r') as f:
            data = json.load(f) 
            if category in data:
                data[category].append([request,response])
            else:
                data[category] = [[request, response]]
            with open(self.data_path, 'w') as f:
                json.dump(data,f,indent=2)

if __name__ == "__main__":
    chatty = Chatty()
    print(chatty.request(input()))