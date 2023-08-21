CFLAGS=-Wall -Wextra -Ofast
BIN=cgen

default: $(BIN)

$(BIN):
	gcc $(CFLAGS) $(BIN).c -o $(BIN) 

install: $(BIN)
	cp cgen /usr/local/bin

clean:
	rm -rf $(BIN)
