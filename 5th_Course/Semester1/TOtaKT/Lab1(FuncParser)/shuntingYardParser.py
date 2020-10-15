import math
import numpy as np

CONSTANTS = {
    'pi': np.pi,
    'e': np.e
}

OPERATORS = {
    np.sin: 1,
    np.cos: 1,
    np.tan: 1,
    np.abs: 1,
    np.exp : 1, 
    np.power: 1, 
    np.sqrt: 1,
    "+" : 2,
    "-" : 2,
    "*" : 3,
    "/" : 3,
}

numberType = "number"
variableType = "variable"
funcType = "function"
operatorType = "operator"
leftParType = "leftParenthesis"
rightParType = "rightParenthesis"

FUNCTIONS = {
    'sin': np.sin,
    'cos': np.cos,
    'tg': np.tan,
    'abs': np.abs,
    'exp': np.exp,
    'pow': np.power,
    'sqrt': np.sqrt
}

NUMBERS = "0123456789"

VARIABLES = "abcdefghijklmnopqrstuvwxyz"

class ShuntingYardParser:
    def __init__(self, expression):
        self.expression = expression
        self.currentIndex = 0
        self.operatorStack = []
        self.outputStack = []
        self.inputStack = []

    def getValue(self, values):
        self.values = {} if values == None else values.copy()
        self.createInputStack()
        self.parseInputStack()
        ans = self.calculateOutputStack()
        self.currentIndex = 0
        return ans[0]

    def calculateOutputStack(self):
        args = []
        ans = 0
        while len(self.outputStack) != 0:
            if len(self.outputStack) == 1 and self.outputStack[0][1] == numberType:
                ans = self.outputStack[0]
                break
            token = self.outputStack.pop(0)
            if token[1] == numberType:
                args.append(token[0])
            elif token[1] == operatorType:
                if token[0] == '+':
                    self.outputStack.insert(0, (args[-2] + args[-1], numberType))
                elif token[0] == '-':
                    self.outputStack.insert(0, (args[-2] - args[-1], numberType))
                elif token[0] == '*':
                    self.outputStack.insert(0, (args[-2] * args[-1], numberType))
                elif token[0] == '/':
                    if(args[-1] == 0):
                        raise Exception("Cannot divide by 0!")
                    self.outputStack.insert(0, (args[-2]/args[-1], numberType))
                args.pop()
                args.pop()
            elif token[1] == funcType:
                func = FUNCTIONS.get(token[0])
                if token[0] == 'pow':
                    self.outputStack.insert(0, (float(func(args[-2], args[-1])), numberType))
                    args.pop()
                    args.pop()
                else:
                    self.outputStack.insert(0, (float(func(args[-1])), numberType))
                    args.pop()
        return ans


    def createInputStack(self):
        self.skipWhiteSpaces()
        while self.hasNext():
            self.skipWhiteSpaces()
            char = self.getCurrentChar()
            if char == ',':
                self.currentIndex += 1
                self.skipWhiteSpaces()
                continue
            if char in '()':
                self.inputStack.append((char, leftParType if char == '(' else rightParType))
                self.currentIndex += 1
            elif char in OPERATORS.keys():
                self.inputStack.append((char, operatorType))
                self.currentIndex += 1
            elif char in NUMBERS:
                self.inputStack.append((self.parseNumber(), numberType))
            elif char.lower() in VARIABLES:
                self.inputStack.append(self.parseVariable())
            else: 
                raise Exception("Unexpected char met in input expression")
            
    def parseInputStack(self):
        while len(self.inputStack) != 0:
            token = self.inputStack.pop(0)
            if token[1] == numberType:
                self.outputStack.append(token)
            elif token[1] == funcType:
                self.operatorStack.append(token)
            elif token[1] == operatorType:
                while (len(self.operatorStack) != 0
                and self.operatorStack[-1][1] != leftParType
                and OPERATORS.get(self.operatorStack[-1][0]) >= OPERATORS.get(token[0])):
                    self.outputStack.append(self.operatorStack.pop())
                self.operatorStack.append(token)
            elif token[1] == leftParType:
                self.operatorStack.append(token)
            elif token[1] == rightParType:
                while self.operatorStack[-1][1] != leftParType:
                    self.outputStack.append(self.operatorStack.pop())
                if self.operatorStack[-1][1] == leftParType:
                    self.operatorStack.pop()
                    self.outputStack.append(self.operatorStack.pop())
        while len(self.operatorStack) != 0:
            self.outputStack.append(self.operatorStack.pop())

    def parseNumber(self):
        self.skipWhiteSpaces()
        numberStr = self.getCurrentChar()
        dotFound = False
        self.currentIndex +=1
        while self.hasNext():
            char = self.getCurrentChar()
            if char == '.':
                if dotFound:
                    raise Exception("Two dots met in one number!")
                else:
                    numberStr += char
                    self.currentIndex += 1
                    dotFound = True
            elif char in NUMBERS:
                numberStr += char
                self.currentIndex += 1
            else: 
                break
        
        return float(numberStr)


    def parseVariable(self):
        self.skipWhiteSpaces()
        variableStr = self.getCurrentChar().lower()
        self.currentIndex += 1
        while self.hasNext():
            char = self.getCurrentChar()
            if char.lower() in VARIABLES:
                variableStr += char.lower()
                self.currentIndex += 1
            else:
                break
        if variableStr in self.values.keys():
            return (self.values.get(variableStr), numberType)
        if variableStr in CONSTANTS.keys():
            return (CONSTANTS.get(variableStr), numberType)
        if variableStr in FUNCTIONS.keys():
            return (variableStr, funcType)
        
        raise Exception(f"Unknown variable name at {self.currentIndex}")

    def hasNext(self):
        return self.currentIndex < len(self.expression)

    def skipWhiteSpaces(self):
        while self.hasNext() and self.getCurrentChar() in ' \n':
            self.currentIndex += 1
    
    def getCurrentChar(self):
        return self.expression[self.currentIndex]

    # return (tokenValue, tokenType) tuple.
    def parseToken(self, token):
        pass