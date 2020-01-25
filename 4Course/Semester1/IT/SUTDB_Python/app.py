from flask import Flask, flash, redirect, render_template, request
from random import randint
from database import *
from mongo_database import *
import pymongo



app = Flask(__name__)

# mongo_client = pymongo.MongoClient("mongodb://localhost:27017/")

# mydb = mongo_client["mongo_database"]

# mycol = mydb["collection"]

# mydict = {"name" : "collection", "tables":[{"name": "table_1", "columns" : [{"name": "col_1", "type": 1}], "rows" : [[{"data": "first", "type" : 1}]]}]}

database = DataBase("Test")
database.addTable("TestTableFirst")
database.addColumnToTable("TestTableFirst", "test_col_1", 1)
database.addColumnToTable("TestTableFirst", "test_col_2", 1)
database.addColumnToTable("TestTableFirst", "test_col_3", 1)
database.addRowToTable("TestTableFirst")
database.addRowToTable("TestTableFirst")
database.updateCellData("TestTableFirst", 0, 0, "00")
database.updateCellData("TestTableFirst", 0, 1, "01")
database.updateCellData("TestTableFirst", 0, 2, "02")
database.updateCellData("TestTableFirst", 1, 0, "10")
database.updateCellData("TestTableFirst", 1, 1, "11")
database.updateCellData("TestTableFirst", 1, 2, "12")

database2 = DataBase("Test2")

# mongo_databases = [mydict]
databases = [database, database2]

# mycol.delete_many({})

# mycol.insert(mydict)

def get_db(database_name):
    for db in databases:
        if db.db["name"] == database_name:
            return db

@app.route("/database/<db_name>", methods = ["GET"])
def open_db(db_name):
    return render_template('database.html', database = get_db(db_name))

@app.route("/", methods = ["GET"])
def index():
    return render_template('main.html', databases = databases, mongo_db = databases)

@app.route("/databases/<db_name>/tables/<table_name>/delete_row/<index>", methods=["POST"])
def delete_row_from_table(db_name, table_name, index):
    get_db(db_name).removeRowFromTable(table_name, int(index))
    return ""
    
@app.route("/databases/<db_name>/tables/<table_name>/delete", methods = ["POST"])
def delete_table(db_name, table_name):
    get_db(db_name).removeTable(table_name)
    return ""
    
@app.route("/databases/<db_name>/tables/<table_name>/add_row", methods = ["POST"])
def add_row(db_name, table_name):
    data = request.form.getlist('vals[]')
    print(data)
    get_db(db_name).addRowToTableWithData(table_name, data)
    return ""
   
@app.route("/databases/<db_name>/tables/<table_name>/add_column", methods = ["POST"])
def add_column(db_name, table_name):
    name = request.form.get("name")
    get_db(db_name).addColumnToTable(table_name, name, "1")
    return ""
   
@app.route("/databases/<db_name>/tables/add_table", methods = ["POST"])
def add_table(db_name):
    name = request.form.get("name")
    print(name)
    get_db(db_name).addTable(name)
    return ""

@app.route("/databases/<db_name>/tables/<table_name>/remove_column/<column_name>", methods = ["POST"])
def remove_column(db_name, table_name, column_name):
    get_db(db_name).removeColumnFromTable(table_name, column_name)
    return ""

@app.route("/databases/<db_name>/tables/union_table", methods = ["POST"])
def add_union_table(db_name):
    f_name = request.form.get("f_name")
    s_name = request.form.get("s_name")
    c_name = request.form.get("c_name")
    n_name = request.form.get("n_name")
    get_db(db_name).unionTableByField(f_name, s_name, c_name, n_name)
    return ""

# !!!!!!!!!!!!!!!!
#   MONGO SECTOR
# !!!!!!!!!!!!!!!!


def refresh_mongo():
    mycol.delete_many({})
    mycol.insert_many(mongo_databases)

@app.route("/mongo_databases/<db_name>", methods = ["GET"])
def open_mongo_db(db_name):
    return render_template('mongo_db.html', database = mydict)
    
@app.route("/mongo_databases/<db_name>/tables/<table_name>/delete", methods = ["POST"])
def delete_table_mongo(db_name, table_name):
    removeTableMongo(mydict, table_name) #!!!!!!!!!!!!!!!!!!
    refresh_mongo()
    return ""

@app.route("/mongo_databases/<db_name>/tables/<table_name>/delete_column/<column_name>", methods = ["POST"])
def remove_column_mongo(db_name, table_name, column_name):
    removeColumnFromTableMongo(mydict, table_name, column_name)
    refresh_mongo()
    return ""

@app.route("/mongo_databases/<db_name>/tables/<table_name>/delete_row/<row_index>", methods = ["POST"])
def remove_row_mongo(db_name, table_name, row_index):
    removeRowFromTableMongo(mydict, table_name, int(row_index))
    refresh_mongo()
    return ""

@app.route("/mongo_databases/<db_name>/tables/<table_name>/add_row", methods = ["POST"])
def add_row_mongo(db_name, table_name):
    data = request.form.getlist('vals[]')
    print(data)
    addRowToTableWithDataMongo(mydict, table_name, data)
    refresh_mongo()
    return ""
    
@app.route("/mongo_databases/<db_name>/tables/add_table", methods = ["POST"])
def add_table_mongo(db_name):
    name = request.form.get("name")
    addTableMongo(mydict, name)
    refresh_mongo()
    return ""
    
@app.route("/mongo_databases/<db_name>/tables/<table_name>/add_column", methods = ["POST"])
def add_column_mongo(db_name, table_name):
    name = request.form.get("name")
    addColumnToTableMongo(mydict, table_name, name, "1")
    refresh_mongo()
    return ""

@app.route("/mongo_databases/<db_name>/tables/union_table", methods = ["POST"])
def union_table_mongo(db_name):
    f_name = request.form.get("f_name")
    s_name = request.form.get("s_name")
    c_name = request.form.get("c_name")
    n_name = request.form.get("n_name")
    unionTableByFieldMongo(mydict, f_name, s_name, c_name, n_name)
    refresh_mongo()
    return ""

if __name__ == "__main__":
    app.run(debug = True)