import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

port = 9888

s.connect(('192.168.100.20',port))

#a = 'hello!'
#s.send(a.encode())
#print(s.recv(1024).decode())

#s.close()
sstat = "[ss]" #asks server for status - prefix for every input line
shm = ">" #shell message thingie

print("Welcome to the Sooflea terminal")
print("input '(close)' or send empty command to close the connection\n")

while True:

    s.send(sstat.encode()) #asks server for current sstat
    shm2 = s.recv(1024).decode() + shm
    comm = input(shm2)
    
    if(comm == "(close)" or len(comm) == 0):
        s.close()
        break
    
    s.send(comm.encode())
    print(s.recv(1024).decode())
    print('\n')

print("goodbye!")
