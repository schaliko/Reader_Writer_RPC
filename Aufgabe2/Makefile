all: remo_math math_server

remo_math: math_clnt.o math_xdr.o client.o
	cc -o remo_math math_clnt.o math_xdr.o client.o -I/usr/include/tirpc -lnsl -ltirpc

math_clnt.o: math_clnt.c
	cc -c math_clnt.c -I/usr/include/tirpc -lnsl -ltirpc

math_xdr.o: math_xdr.c
	cc -c math_xdr.c -I/usr/include/tirpc -lnsl -ltirpc

client.o: client.c
	cc -c client.c -I/usr/include/tirpc -lnsl -ltirpc

math_server: math_svc.o math_xdr.o server.o
	cc -o math_server math_svc.o math_xdr.o server.o -I/usr/include/tirpc -lnsl -ltirpc

math_svc.o: math_svc.c
	cc -c math_svc.c -I/usr/include/tirpc -lnsl -ltirpc

server.o: server.c
	cc -c server.c -I/usr/include/tirpc -lnsl -ltirpc

clean:
	rm -f math_clnt.o math_xdr.o client.o remo_math math_svc.o server.o math_server

# To run the client (optional)
run_client: remo_math
	./remo_math

# To run the server (optional)
run_server: math_server
	./math_server

