1): How to build sqlite shared object.
    gcc sqlite3.c -lpthread -ldl -fPIC -shared -o libsqlite3.so	

2): How to build sqlite command tools
    gcc shell.c sqlite3.c -lpthread -ldl -o sqlite3
