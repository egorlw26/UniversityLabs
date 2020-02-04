import json
import codecs

if __name__ == "__main__":
    
    myName = "Delikatnyi"
    
    dataToWrite = {}
    dataToWrite["conversations"] = []
    
    with codecs.open("dialogTelegram.json", 'r', errors='ignore', encoding='utf-8') as json_file:
        data = json.load(json_file)
        for dialog in data["chats"]["list"]:
            messages = dialog["messages"]
            for i in range(len(messages)-1):
                if messages[i]["type"] == "message" and\
                not isinstance(messages[i]["text"], list) and\
                messages[i]["from"] == myName and \
                messages[i+1]["from"] != myName:
                    dataToWrite["conversations"].append([
                    messages[i]["text"],
                    messages[i+1]["text"]])
    
    
    
    with codecs.open("custom_corpus/yehor.corpus.json", 'w', encoding='utf-8') as output_json_file:
        json.dump(dataToWrite, output_json_file, indent=2, ensure_ascii=False)