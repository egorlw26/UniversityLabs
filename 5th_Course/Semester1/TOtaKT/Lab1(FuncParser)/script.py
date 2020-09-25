import math

CONSTANTS = {
    'PI': math.pi,
    'E': math.e
}

FUNCTIONS = {
    'sin': math.sin,
    'cos': math.cos,
    'tg': math.tan,
    'abs': abs,
    'exp': math.exp,
    'pow': math.pow
    'sqrt': math.sqrt
}

NUMBERS = "0123456789"

VARIABLES = "abcdefghijklmnopqrstuvwxyz"

# Example: -(7*4) + 3 
# Main idea: Expression -> Addition -> Multiplication -> Brackets -> Inner Expression

class FuncParser:
    def __init__(self, expression, values = None):
        self.expression = expression
        self.currentCharIndex = 0
        self.values = {} if values == None else values.copy()

    def parseExpression(self):
        return self.parseAdditions()

    def parseAdditions(self):
        # we get an array of values before first '+' or '-' symbol (left part)
        values = [self.parseMultiplication()]

        self.skipWhitespacesFromCurrentIndex()
        
        while self.hasNextSymbol():
            char = self.getCurrentSymbol()
            if char == '+':
                self.currentCharIndex += 1
                values.append(self.parseMultiplication())
            elif char == '-':
                self.currentCharIndex += 1
                values.append(-1 * self.parseMultiplication())
            else: 
                break

        return sum(values)
        

    def parseMultiplication(self):
        # left side from first '*' or '/' symbol
        values = [self.parseBrackets()]
        
        while True:
            self.skipWhitespacesFromCurrentIndex()
            char = self.getCurrentSymbol()
            if char == '*':
                self.currentCharIndex += 1
                values.append(self.parseBrackets())
            elif char == '/':
                self.currentCharIndex += 1
                denim = self.parseBrackets()
                if denim == 0:
                    raise Exception("Can't divide by 0.")
                values.append(1.0/denim)
            else:
                break
        resVal = 1.0
        for value in values:
            resVal *= value
        
        return resVal

    def parseBrackets(self):
        self.skipWhitespacesFromCurrentIndex()
        char = self.getCurrentSymbol()

        if char == '(':
            self.currentCharIndex += 1
            value = self.parseExpression()
            self.skipWhitespacesFromCurrentIndex()

            if self.getCurrentSymbol() != ')':
                raise Exception("Can't find closing bracket, something's wrong")
            self.currentCharIndex += 1
            return value
        else:
            return self.parseValue()

    # returning either number or variable
    def parseValue(self):
        self.skipWhitespacesFromCurrentIndex()
        char = self.getCurrentSymbol()
        if char in NUMBERS:
            return self.parseNumber()
        else:
            return self.parseArgument()
        
    def parseArgument(self):
        pass

    def hasNextSymbol(self):
        return self.currentCharIndex < len(self.expression)

    def isNext(self, string):
        return self.expression[self.currentCharIndex:self.currentCharIndex + len(string)] == string

    def getCurrentSymbol(self):
        return self.expression[self.currentCharIndex]

    def parseNumber(self):
        self.skipWhitespacesFromCurrentIndex()
        strNumber = ''
        dotFound = False
        currentChar = ''

        # As we expecting number, than we expecting dot or some number
        # We don't allow dot without 0 at start, so '.25' isn't valid
        while self.hasNextSymbol():
            currentChar = self.getCurrentSymbol()
            if currentChar == '.':
                if dotFound:
                    raise Exception("Additional dot found!")
                if len(strNumber) == 0:
                    raise Exception("Dot at start of number found, you've missed something")
                dotFound = True
                strNumber += currentChar
            elif currentChar in NUMBERS:
                strNumber += currentChar
            else:
                break
            self.currentCharIndex += 1
        
        if len(strNumber) == 0:
            raise Exception("Unexpected length of number")

        return float(strNumber)

    # skipping all white symbols until non-white found
    def skipWhitespacesFromCurrentIndex(self):
        while self.hasNextSymbol() and self.getCurrentSymbol() in ' \n':
            self.currentCharIndex += 1
        return

