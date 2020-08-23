# OBJS specifies which files to compile as part of the project
OBJS = ray_main.c ray_sdl.c lib/util.c lib/ellipse.c

# CC specifies which compiler we're using
CC = gcc

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w supresses all warnings
COMPILER_FLAGS = -w

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lm -lSDL2 #-lSDL2_mixer #-lSDL2_ttf#-lSDL2_image

# OBJ_NAME specifies the name of our executable
OBJ_NAME = raycast

# This is the target that compiles our executables
all : $(OBJS)
	$(CC) $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)
