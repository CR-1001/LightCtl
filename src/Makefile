CXX=g++ -std=gnu++20 -g
RM=rm -f

SOURCES=starter.cpp output_formatter.cpp command_interpreter.cpp bridge_connector.cpp wget_rest_client.cpp state.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: lightc

lightc: $(OBJECTS)
	$(CXX) -o lightctl $(SOURCES)

.cpp.o:
	$(CXX) -c $< -o $@

clean: 
	$(RM) $(OBJECTS) lightctl
