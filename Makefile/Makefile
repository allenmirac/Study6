cpp_srcs := ${shell find src -name *.cpp}
cpp_objs := ${patsubst cpp_srcs/%.cpp,objs/%.o,${cpp_srcs}}

include_paths := /home/mirac/Desktop/Github/Makefile/include

I_flag := ${include_paths:%=-I%}

compile_options := -g -O3 -Wall ${I_flag}

# objs/%.o: src/%.cpp
# 	mkdir -p ${dir $@}
# 	g++ -c $^ -o $@ 
# # ${compile_options}

workspace/exec: ${cpp_objs}
	mkdir -p ${dir $@}
	g++ $^ -o $@

run : workspace/exec
	./$<


debug:
	echo ${I_flag}

clean:
	rm -rf objs workspace/exec
.PHONY: debug

# cpp_srcs := $(shell find src -name *.cpp)
# cpp_objs := $(patsubst src/%.cpp,objs/%.o,$(cpp_srcs))

# # 你的头文件所在文件夹路径（建议绝对路径）
# include_paths := /home/mirac/Desktop/Github/Makefile/include
# I_flag        := $(include_paths:%=-I%)


# objs/%.o : src/%.cpp
# 	@mkdir -p $(dir $@)
# 	@g++ -c $^ -o $@ $(I_flag)

# compile : ${cpp_objs}

# workspace/exec : $(cpp_objs)
# 	@mkdir -p $(dir $@)
# 	@g++ $^ -o $@ 

# run : workspace/exec
# 	@./$<

# debug :
# 	@echo $(I_flag)

# clean :
# 	@rm -rf objs

# .PHONY : debug run