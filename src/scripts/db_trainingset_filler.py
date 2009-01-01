#!/usr/bin/python

import sqlite3, os, sys, pprint

# Variables
TRAINING_SET_DIR = "/Users/nico/projects/dev/nfp/original_data/training_set"
DESTINATION_SQLITE_FILE = "/Users/nico/projects/dev/nfp/data/nfp.sqlite"

# Classes
class FileNotFound(Exception):
    pass

class mv_file:
    def __init__(self, path):
        if not os.path.exists(path):
            raise FileNotFound
        else:
            FILE = open(path)
            self.__movie_id = FILE.readline().split(':')[0]
            self.__data = (i.replace('\n', '').split(',') for i in FILE.readlines())
            FILE.close()
    
    def g_data(self):
        return self.__data
    
    def data(self):
        return [(self.__movie_id, i[0], i[1], i[2]) for i in self.__data]   
        
    def movie_id(self):
        return self.__move_id

       
class db_handler:
    def __init__(self, db_file):
        #if not os.path.exists(db_file):
        #    #raise FileNotFound
        #    print db_file
        #    sys.exit(2)
        #else:
            self.__conn = sqlite3.connect(db_file)
            self.__cursor = self.__conn.cursor()
            print "Connection OK"
            
    def cursor(self):
        return self.__cursor
    
    def commit(self):
        self.__conn.commit()

    def rollback(self):
        self.__conn.rollback()
            
    def __delete__(self):
        self.__conn.close()
        
    def close(self):
        self.__cursor.close()
        
class db_model(db_handler):
    __drop_query = """DROP TABLE RATING"""
    __create_query = """CREATE TABLE RATING
                        (
                            id INTEGER PRIMARY KEY AUTOINCREMENT,
                            movie_id INTEGER NOT NULL,
                            user INTEGER NOT NULL,
                            rate INTEGER,
                            date TEXT
                        )"""
    __insert_query = """INSERT INTO RATING (movie_id, user, rate, date)
                        VALUES (?, ?, ?, ?)"""
    __select_query = """SELECT * FROM RATING"""
    
    def __init__(self, db_file):
        db_handler.__init__(self, db_file)
        
    def create_table(self):
        try:
            self.cursor().execute(self.__drop_query)
        except sqlite3.OperationalError, detail:
            print detail
        self.cursor().execute(self.__create_query)
    
    def insert(self, records):
        print "Inserting %d records" % len(records)
        for rec in records:
            self.cursor().execute(self.__insert_query, rec)
        self.commit()
        
    def get_all(self):
        c = self.cursor()
        c.execute(self.__select_query)
        return [row for row in c]
        
        
                
                
            
# Main
def main():
    # Some basic checks...
    if not os.path.exists(TRAINING_SET_DIR):
        print "TRAINING_SET_DIR <%s> is not a valid path." % TRAINING_SET_DIR
        sys.exit(1)
    
    dsf_path = os.path.split(DESTINATION_SQLITE_FILE)[0]
    if not os.path.exists(dsf_path):
        print "DESTINATION_SQLITE_FILE <%s> does not point to a valid path." % dsf_path
        sys.exit(1)
    
    if not os.path.exists(dsf_path):
        print "DESTINATION_SQLITE_FILE already exists." % DESTINATION_SQLITE_FILE
        sys.exit(1)
    
    # Here we go baby...
    
    db = db_model(DESTINATION_SQLITE_FILE)
    db.create_table()
    
    FILES = []
    i = 0
    for file in os.listdir(TRAINING_SET_DIR):
        file_path = os.path.join(TRAINING_SET_DIR, file)
        print "Processing %s" % os.path.split(file_path)[1]
        #pprint.pprint(mv_file(file_path).data())
        db.insert(mv_file(file_path).data())
        i += 1
        #if i == 10:
        #    break
            
    res = db.get_all()
    
    
    db.close()
        
        
        
        
if __name__ == "__main__":
    main()