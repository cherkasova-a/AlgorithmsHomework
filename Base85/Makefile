# Настройки компилятора
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LD = $(CXX)

GTEST_LIBS = -lgtest -lgtest_main -lpthread

TARGET = base85
UTEST_TARGET = utest

.PHONY: all release debug clean run_utest run_itest depends

all: release run_utest run_itest

release: base85.o base85ed.o
	$(LD) -o $(TARGET) $^

debug: CXXFLAGS += -g
debug: base85.o base85ed.o
	$(LD) -o $(TARGET) $^

$(UTEST_TARGET): base85ed_test.o base85ed.o
	$(LD) -o $@ $^ $(GTEST_LIBS)

run_utest: $(UTEST_TARGET)
	./$(UTEST_TARGET)

run_itest: release
	@echo "Running integration tests..."
	@chmod +x integration_test.sh
	./integration_test.sh

clean:
	rm -f *.o $(TARGET) $(UTEST_TARGET) random.bin* random.b85*

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

depends:
	@echo "Dependencies updated"
