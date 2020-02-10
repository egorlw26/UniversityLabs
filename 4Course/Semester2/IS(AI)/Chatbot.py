import os
import yaml
from chatterbot import ChatBot
from chatterbot.trainers import ListTrainer, ChatterBotCorpusTrainer

class Chatty:
    def __init__(self):
        self.train_data_path = 'custom_corpus/'
        self.default_response = "I don't understand you :( Try to improve me with adding your dialogue"
        train = self.is_need_to_train()
        self.bot = ChatBot('Chatty',default_response = self.default_response)
        if train:
            self.train()
        
    def request(self, text):
        return self.bot.get_response(text)
        
    def train(self):
        self.trainer = ChatterBotCorpusTrainer(self.bot)
        self.trainer.train(self.train_data_path)
        
    def add_dialogue(self, dialogue, category = "undefined"):
        file_path = os.getcwd() + '/' + self.train_data_path + category + '.yml'
        data = self.read_to_dict(file_path) if os.path.exists(file_path) else self.create_new_category(category)
        self.add(data, dialogue)
        self.write(data, file_path)
                    
    def create_new_category(self, category):
        res = {}
        res['categories'] = [category]
        res['conversations'] = []
        return res
    
    def read_to_dict(self, file_path):
        res = {}
        with open(file_path, 'r') as f:
            lines = f.readlines()
            categories = False
            conversations = False
            for line in lines:
                replica = line[4:-1]
                if 'categories' in line:
                    res['categories'] = []
                    categories = True
                    conversations = False
                elif 'conversations' in line:
                    res['conversations'] = []
                    categories = False
                    conversations = True
                elif conversations and '- - ' in line:
                    res['conversations'].append([replica])
                elif conversations and '- ' in line:
                    res['conversations'][-1].append(replica)
                elif categories and '- ' in line:
                    res['categories'].append(line[2:-1])
        return res
        
    def write(self, data, file_path):
        with open(file_path, 'w') as f:
                f.write('categories:')
                for category in data['categories']:
                    f.write('\n- ' + category)
                f.write('\nconversations: ')
                for conv in data['conversations']:
                    f.write("\n- - " + conv[0])
                    for replica in conv[1:]:
                        f.write("\n  - " + replica)
                f.write('\n')
        
    def add(self, data, dialogue):
        conversations = data['conversations']
        if len(conversations) != 0:
            for conv in conversations:
                if dialogue != conv:
                    conversations.append(dialogue)
        else:
            conversations.append(dialogue)
    
    def is_need_to_train(self):
        db_path = os.getcwd() + '/db.sqlite3'
        return not os.path.exists(db_path)

if __name__ == "__main__":
    chatty = Chatty()
    chatty.train()
    running = True
    while running:
        request = input("You: ")
        response = chatty.request(request)
        if request == 'Bye':
            running = False
        print("Bot:", response)