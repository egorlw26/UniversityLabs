import json

class DataBase:
    def __init__(self, i_name):
        self.db = {"name": i_name, "tables": []} 
        
    def addTable(self, i_name):
        self.db["tables"].append({"name": i_name,
                                  "columns" : [],
                                  "rows": []})   
    
    def getTableIndex(self, table_name):
        for i, table in enumerate(self.db["tables"]):
            if table["name"] == table_name:
                return i
    
    def removeTable(self, i_name):
        index = self.getTableIndex(i_name)
        self.db["tables"].remove(self.db["tables"][index])
    
    def addColumnToTable(self, table_name, column_name, column_type):
        table_index = self.getTableIndex(table_name)
        self.db["tables"][table_index]["columns"].append(
        {"name": column_name, "type": column_type})
        for i in enumerate(self.db["tables"][table_index]["rows"]):
            self.db["tables"][table_index]["rows"][i[0]].append({"data": "", "type": column_type})
    
    def removeColumnFromTable(self, table_name, column_name):
        table_index = self.getTableIndex(table_name)
        col_index = 0
        for col_index, col in enumerate(self.db["tables"][table_index]["columns"]):
            if col["name"] == column_name:
                break
        self.db["tables"][table_index]["columns"].remove(
        self.db["tables"][table_index]["columns"][col_index])
        for i in enumerate(self.db["tables"][table_index]["rows"]):
            self.db["tables"][table_index]["rows"][i[0]].remove(
            self.db["tables"][table_index]["rows"][i[0]][col_index])
        
    def addRowToTable(self, table_name):
        table_index = self.getTableIndex(table_name)
        row = []
        for i in enumerate(self.db["tables"][table_index]["columns"]):
            type = self.db["tables"][table_index]["columns"][i[0]]["type"]
            row.append({"data": "", "type": type})
        self.db["tables"][table_index]["rows"].append(row)
    
    def addRowToTableWithData(self, table_name, data):
        table_index = self.getTableIndex(table_name)
        row = []
        for i in data:
            row.append({"data": i, "type": 1})
        self.db["tables"][table_index]["rows"].append(row)
    
    def getCellFromTable(self, table_name, row, column):
        table_index = self.getTableIndex(table_name)
        return self.db["tables"][table_index]["rows"][row][column]
    
    def removeRowFromTable(self, table_name, row_index):
        table_index = self.getTableIndex(table_name)
        self.db["tables"][table_index]["rows"].remove(
        self.db["tables"][table_index]["rows"][row_index])
        
    def updateCellData(self, table_name, row, column, n_data):
        table_index = self.getTableIndex(table_name)
        self.db["tables"][table_index]["rows"][row][column]["data"] = n_data
        
    def unionTableByField(self, table_first_name, table_second_name, field_name,
                          n_table_name):
        f_index = self.getTableIndex(table_first_name)
        s_index = self.getTableIndex(table_second_name)
        col_ind_f = -1
        col_ind_s = -1
        for i in enumerate(self.db["tables"][f_index]["columns"]):
            if self.db["tables"][f_index]["columns"][i[0]]["name"] == field_name:
                col_ind_f = i[0]
                break
                
        for i in enumerate(self.db["tables"][s_index]["columns"]):
            if self.db["tables"][s_index]["columns"][i[0]]["name"] == field_name:
                col_ind_s = i[0]
                break
        
        if(col_ind_f != -1 and col_ind_s != -1):
            n_table = {"name": n_table_name, "columns": [], "rows": []}
            n_table["columns"].append(self.db["tables"][s_index]["columns"][col_ind_s])
            
            for i in enumerate(self.db["tables"][f_index]["rows"]):
                row = [self.db["tables"][f_index]["rows"][i[0]][col_ind_f]]
                n_table["rows"].append(row)
            
            for i in enumerate(self.db["tables"][s_index]["rows"]):
                n_table["rows"].append([self.db["tables"][s_index]["rows"][i[0]][col_ind_s]])
        
        self.db["tables"].append(n_table)
