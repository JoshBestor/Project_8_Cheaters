final: plagerismCatcher.o
	g++ -o plagiarismCatcher  plagerismCatcher.o
driver: plagerismCatcher.cpp
	g++ -c plagerismCatcher.cpp