verbose:
	gcc -Wl,--verbose ./hello.c

layout:
	echo "Global Data and Static" && gcc -O0 ./global_static.c && objdump -t ./a.out | grep global_var && objdump -t ./a.out | grep static_var
	echo "Global Data and Initialized Static" && gcc -O0 ./global_static_init.c && objdump -t ./a.out | grep global_var && objdump -t ./a.out | grep static_var
	echo "Initialized Global Data and Initialized Static" && gcc -O0 ./global_init_static_init.c && objdump -t ./a.out | grep global_var && objdump -t ./a.out | grep static_var
