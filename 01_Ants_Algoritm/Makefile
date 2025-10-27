CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Указываем компилятору, где искать заголовочные файлы
INCLUDES = -I../My_graph

# Находим все .cpp файлы в текущей директории и в ../My_graph
SRCS = $(wildcard *.cpp) $(wildcard ../My_graph/*.cpp)

# Заменяем расширения .cpp на .o для получения списка объектных файлов
OBJS = $(patsubst %.cpp,%.o,$(notdir $(SRCS)))

# Имя исполняемого файла
TARGET = main

# Правило по умолчанию
all: $(TARGET)

# Правило для сборки исполняемого файла
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Правило для компиляции .cpp файлов из текущей директории
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Правило для компиляции .cpp файлов из ../My_graph
%.o: ../My_graph/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Правило для очистки
clean:
	-del /F /Q $(OBJS) $(TARGET).exe 2>nul
