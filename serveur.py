import socket, cv2, pickle, struct

# Création du socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host_name = socket.gethostname()
host_ip = socket.gethostbyname(host_name)
print('HOST IP:', host_ip)
port = 999
socket_address = (host_ip, port)

# Assignation du socket
server_socket.bind(socket_address)

# choix du port
server_socket.listen(5)
print("LISTENING AT:", socket_address)

# server_socket.listen(5) renvoie TRUE (connection)
while True:
    client_socket, addr = server_socket.accept()
    print('GOT CONNECTION FROM:', addr)
    if client_socket:
        vid = cv2.VideoCapture(0)
        while (vid.isOpened()):
            img, frame = vid.read()
            a = pickle.dumps(frame)
            message = struct.pack("Q", len(a)) + a
            client_socket.sendall(message)
            cv2.imshow('TRANSMITTING VIDEO', frame)
            key = cv2.waitKey(100) & 0xFF
            if key == ord('q'):
                client_socket.close()


print('HOST IP:', print('HOST IP:', host_ip))
