demo : demo.cc libOptions.so
	g++ -L"." -lOptions demo.cc -o demo
libOptions.so : Options.hh Options.cc
	g++ -shared -fPIC Options.cc -o libOptions.so
clean:
	rm -f demo libOptions.so
