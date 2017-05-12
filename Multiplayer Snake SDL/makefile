CC = g++
CFLAGS = -Wall -g
INCLUDES = -I/usr/include/SDL
LIBS = -lSDL -lSDL_net

SRCS = main.cpp Bullet.cpp HandleEvents.cpp level.cpp NetAppClient.cpp NetAppServer.cpp setup.cpp snake.cpp

OBJ_DIR = obj
OBJS = $(SRCS:.cpp=.o)
OBJS_W_DIR = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

OUT_DIR = "bin/Debug/Multiplayer Snake SDL"
MAIN = MultiplayerSnakeGame


all: obj $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUT_DIR) $(OBJS_W_DIR) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $(OBJ_DIR)/$@

obj:
	mkdir obj
