import math

CONSTANTS = {
    'PI': math.pi,
    'E': math.e
}

FUNCTIONS = {
    'sin': math.sin,
    'cos': math.cos,
    'tg': math.tan,
    'ctg': 1/math.tan,
    'abs': abs,
    'exp': math.exp,
    'pow': math.pow
}

# Example: 2*x + 7*(3 + 3) 

class FuncParser:
    def __init__(self, expression, values = None):
        self.expression = expression
        self.currentCharIndex = 0
        self.values = {} if values == None else values.copy()

    def parseExpression(self):
        pass

    def parseAdditions(self):
        pass

    def parseMultiplication(self):
        pass

    def parseBrackets(self):
        pass

    def hasNextSymbol(self):
        return self.currentCharIndex < len(self.expression)

    def getCurrentSymbol(self):
        return self.expression[self.currentCharIndex]

    def skipWhitespaces(self):
        while self.hasNextSymbol() and self.getCurrentSymbol() in ' \n':
            self.currentCharIndex += 1
        return



