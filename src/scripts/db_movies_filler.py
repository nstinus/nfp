#!/usr/bin/python

import sqlite3, os, sys, pprint

# Variables
MOVIES_FILE = "/Users/nico/projects/dev/nfp/original_data/movie_titles.txt"
DESTINATION_SQLITE_FILE = "/Users/nico/projects/dev/nfp/data/nfp.sqlite"

# Classes
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
    __insert_query = """INSERT INTO MOVIES (id, title, year)
                        VALUES (?, ?, ?)"""
    __select_query = """SELECT * FROM MOVIES"""
    
    def __init__(self, db_file):
        db_handler.__init__(self, db_file)
        
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
    if not os.path.exists(MOVIES_FILE):
        print "MOVIES_FILE <%s> does not exist." % MOVIES_FILE
        sys.exit(1)
    
    if not os.path.exists(DESTINATION_SQLITE_FILE):
        print "DESTINATION_SQLITE_FILE <%s> does not exist." % dsf_path
        sys.exit(1)
    
    # Here we go baby...
    
    db = db_model(DESTINATION_SQLITE_FILE)
    
    FILE = open(MOVIES_FILE)
    
    db.insert(map(lambda x: (x[0], x[2].replace('\n', ''), x[1]), [line.split(',')[:3] for line in FILE.readlines()]))
    
    res = db.get_all()
    
    db.close()
        
if __name__ == "__main__":
    main()