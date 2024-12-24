# BUILD INSTRUCTIONS
## UTILS
```
g++ -c utils.cc -lWs2_32
```
## SERVER
```
g++ -o server main.cpp utils.o -lWs2_32
```
## CLIENT
### Listener
```
    g++ -o listener -DCLIENT -DLISTENER main.cpp utils.o -lWs2_32
```
### Talker
```
g++ -o talker -DCLIENT main.cpp utils.o -lWs2_32
```

### EXTRAS
For client: .\client host port
    or .\client host
    or .\client
For server: .\server port 