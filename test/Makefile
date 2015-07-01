all:
	g++ -o debug curlopt_debugfunction.cpp -lcurl
	g++ -o post curlopt_postfields.cpp -lcurl
	g++ -o delete file_delete.cpp -lcurl
	g++ -o download download.cpp -lcurl
	g++ -o chkspeed chkspeed.c -lcurl
	gcc -o postinmemory postinmemory.c -lcurl
	g++ -o progressfunc progressfunc.c -lcurl
	
	g++ -o rename renameFile.cpp
	g++ -o https https.c -lcurl

# Housekeeping
.PHONY : clean
clean:
	rm -fr debug post delete download chkspeed postinmemory rename progressfunc https

