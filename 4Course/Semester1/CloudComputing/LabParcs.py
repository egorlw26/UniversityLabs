from Pyro4 import expose

class Solver:
    def __init__(self, workers=None, input_file_name=None, output_file_name=None):
        self.input_file_name = input_file_name
        self.output_file_name = output_file_name
        self.workers = workers
        print("Inited, Parcs started")
        
    def solve(self):
        print("Job started!")
        print("Workers %d" % len(self.workers))
        word, text = self.read_input_file()
        
        step = len(text) / len(self.workers)
        
        out = open(self.output_file_name, 'w')
        out.write(word+'\n')
        
        results = []
        for i in range(len(self.workers)):
            results.append(self.workers[i].word_count(text[i*step:(i+1)*step], word))
        
        for val in results:
            out.write(str(val.value)+'\n')
        
    @staticmethod
    @expose
    def word_count(text, word):
        return text.count(word[:-2])
    
    
    def read_input_file(self):
        f = open(self.input_file_name, 'r')
        word = f.readline()
        text = f.read()
        return word, text
        