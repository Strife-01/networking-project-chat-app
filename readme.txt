This is a simple framework to communicate with the integration project audio interface and simulation server.
This framework comes with a simple demo part (in main.cpp) that sends text from stdin.

On Linux you can simply compile with:
g++ main.cpp network/*.cpp -pthread

On Windows make a new visual c++ project in visual studio from existing files and include the contents of this folder.
Make sure you make a 'console application' project!