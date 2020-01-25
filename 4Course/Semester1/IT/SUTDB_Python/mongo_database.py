import json

def addTableMongo(mydict, i_name):
    mydict["tables"].append({"name": i_name,
                              "columns" : [],
                              "rows": []})   

def getTableIndexMongo(mydict, table_name):
    for i, table in enumerate(mydict["tables"]):
        if table["name"] == table_name:
            return i

def removeTableMongo(mydict, i_name):
    index = getTableIndexMongo(mydict, i_name)
    mydict["tables"].remove(mydict["tables"][index])

def addColumnToTableMongo(mydict, table_name, column_name, column_type):
    table_index = getTableIndexMongo(mydict, table_name)
    mydict["tables"][table_index]["columns"].append(
    {"name": column_name, "type": column_type})
    for i in enumerate(mydict["tables"][table_index]["rows"]):
        mydict["tables"][table_index]["rows"][i[0]].append({"data": "", "type": column_type})

def removeColumnFromTableMongo(mydict, table_name, column_name):
    table_index = getTableIndexMongo(mydict, table_name)
    col_index = 0
    for col_index, col in enumerate(mydict["tables"][table_index]["columns"]):
        if col["name"] == column_name:
            break
    mydict["tables"][table_index]["columns"].remove(
    mydict["tables"][table_index]["columns"][col_index])
    for i in enumerate(mydict["tables"][table_index]["rows"]):
        mydict["tables"][table_index]["rows"][i[0]].remove(
        mydict["tables"][table_index]["rows"][i[0]][col_index])
    
def addRowToTableMongo(mydict, table_name):
    table_index = getTableIndexMongo(mydict, i_name)
    row = []
    for i in enumerate(mydict["tables"][table_index]["columns"]):
        type = mydict["tables"][table_index]["columns"][i[0]]["type"]
        row.append({"data": "", "type": type})
    mydict["tables"][table_index]["rows"].append(row)

def addRowToTableWithDataMongo(mydict, table_name, data):
    table_index = getTableIndexMongo(mydict, table_name)
    row = []
    for i in data:
        row.append({"data": i, "type": 1})
    mydict["tables"][table_index]["rows"].append(row)

def getCellFromTableMongo(mydict, table_name, row, column):
    table_index = getTableIndexMongo(mydict, i_name)
    return mydict["tables"][table_index]["rows"][row][column]

def removeRowFromTableMongo(mydict, table_name, row_index):
    table_index = getTableIndexMongo(mydict, table_name)
    mydict["tables"][table_index]["rows"].remove(
    mydict["tables"][table_index]["rows"][row_index])
    
def updateCellDataMongo(mydict, table_name, row, column, n_data):
    table_index = getTableIndexMongo(mydict, i_name)
    mydict["tables"][table_index]["rows"][row][column]["data"] = n_data
    
def unionTableByFieldMongo(mydict, table_first_name, table_second_name, field_name,
                      n_table_name):
    f_index = getTableIndexMongo(mydict, table_first_name)
    s_index = getTableIndexMongo(mydict, table_second_name)
    col_ind_f = -1
    col_ind_s = -1
    for i in enumerate(mydict["tables"][f_index]["columns"]):
        if mydict["tables"][f_index]["columns"][i[0]]["name"] == field_name:
            col_ind_f = i[0]
            break
            
    for i in enumerate(mydict["tables"][s_index]["columns"]):
        if mydict["tables"][s_index]["columns"][i[0]]["name"] == field_name:
            col_ind_s = i[0]
            break
    
    if(col_ind_f != -1 and col_ind_s != -1):
        n_table = {"name": n_table_name, "columns": [], "rows": []}
        n_table["columns"].append(mydict["tables"][s_index]["columns"][col_ind_s])
        
        for i in enumerate(mydict["tables"][f_index]["rows"]):
            row = [mydict["tables"][f_index]["rows"][i[0]][col_ind_f]]
            n_table["rows"].append(row)
        
        for i in enumerate(mydict["tables"][s_index]["rows"]):
            n_table["rows"].append([mydict["tables"][s_index]["rows"][i[0]][col_ind_s]])
    
    mydict["tables"].append(n_table)
