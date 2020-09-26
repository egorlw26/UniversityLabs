import math
import numpy as np

CONSTANTS = {
    'pi': np.pi,
    'e': np.e
}

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

# Main idea: Expression -> Addition -> Multiplication -> Brackets -> Negative -> Inner Expression

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
        
        while self.hasNextSymbol():
            self.skipWhiteSpacesFromCurrentIndex()
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
        
        while self.hasNextSymbol():
            self.skipWhiteSpacesFromCurrentIndex()
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
        self.skipWhiteSpacesFromCurrentIndex()
        char = self.getCurrentSymbol()

        if char == '(':
            self.currentCharIndex += 1
            value = self.parseExpression()
            self.skipWhiteSpacesFromCurrentIndex()

            if not self.hasNextSymbol() or self.getCurrentSymbol() != ')':
                raise Exception("Can't find ')', something's wrong")
            self.currentCharIndex += 1
            return value
        else:
            return self.parseNegativeValue()

    def parseNegativeValue(self):
        self.skipWhiteSpacesFromCurrentIndex()
        if self.skipIfNext('-'):
            return -1 * self.parseBrackets()
        else:
            return self.parseValue()

    # returning either number or variable
    def parseValue(self):
        self.skipWhiteSpacesFromCurrentIndex()
        char = self.getCurrentSymbol()
        if char in NUMBERS:
            return self.parseNumber()
        else:
            return self.parseVariable()
        
    def parseVariable(self):
        self.skipWhiteSpacesFromCurrentIndex()
        variableValue = None
        strVariable = ''
        
        while self.hasNextSymbol():
            char = self.getCurrentSymbol()
            if char.lower() in VARIABLES:
                strVariable += char.lower()
                self.currentCharIndex += 1
            else:
                break

        if strVariable in FUNCTIONS:
            args = self.parseArguments()
            func = FUNCTIONS.get(strVariable.lower())
            return float(func(*args))
        elif strVariable in CONSTANTS:
            return CONSTANTS.get(strVariable)
        elif strVariable in self.values.keys():
            variableValue = self.values[strVariable]
            if variableValue != None:
                return variableValue
            else:
                raise Exception(f"Can't find value for {strVariable} variable")
        else:
            raise Exception(f"Unexpected name of function or constant: {strVariable}")
    
    def skipIfNext(self, symbol):
        if not self.hasNextSymbol():
            raise Exception(f"Can't find '{symbol}', something's wrong")
        if self.getCurrentSymbol() == symbol:
            self.currentCharIndex += 1
            return True
        else:
            return False

    def parseArguments(self):
        self.skipWhiteSpacesFromCurrentIndex()
        args = []
        char = self.getCurrentSymbol()
        if char != '(':
            raise Exception ("No open bracket for argument!")

        self.currentCharIndex += 1
        self.skipWhiteSpacesFromCurrentIndex()
        while not self.skipIfNext(')'):
            self.skipWhiteSpacesFromCurrentIndex()
            if len(args) > 0:
                if not self.skipIfNext(','):
                    raise Exception("Comma was not found")
                else:
                    self.skipWhiteSpacesFromCurrentIndex()                  
            args.append(self.parseExpression())
        return args
            
    def hasNextSymbol(self):
        return self.currentCharIndex < len(self.expression)

    def getCurrentSymbol(self):
        return self.expression[self.currentCharIndex]

    def parseNumber(self):
        self.skipWhiteSpacesFromCurrentIndex()
        strNumber = ''
        dotFound = False
        currentChar = ''

        # As we expecting number, than we expecting dot or some number
        # We don't allow dot without 0 at start, so '.25' isn't valid
        while self.hasNextSymbol():
            self.skipWhiteSpacesFromCurrentIndex()
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
        # print(f"Returning: {strNumber}, cur index: {self.currentCharIndex}")
        return float(strNumber)

    # skipping all white symbols until non-white found
    def skipWhiteSpacesFromCurrentIndex(self):
        while self.hasNextSymbol() and self.getCurrentSymbol() in ' \n':
            self.currentCharIndex += 1
        return

