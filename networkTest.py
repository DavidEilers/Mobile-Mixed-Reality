# python 3

import socket
import _thread
import time

google = "216.58.210.3"

HOST = '192.168.43.1'
HOST = "192.168.178.20"
MASTER_PORT = 7080
SLAVE_PORT = 7081

FIELD_EMPTY = 0
PLAYER_X = 1
PLAYER_O = 2

slave = {"ip":"0.0.0.0", "port": 0, "name": ""}

def buildConnectMessage(port, name):
    port_bytes = port.to_bytes(2, "little")

    return bytes([4]) + port_bytes + name.encode("ascii")

def getConnectMessage(msg):
    addr = msg[0]
    data = msg[1]
    
    connection_info = None
    
    if data[0] == 4:
        connection_info = {}
        
        connection_info["ip"] = addr
        
        connection_info["port"] = int.from_bytes(data[1:3], "little")

        connection_info["name"] = data[3:].decode("ascii")

    return connection_info


def buildClickMessage(x, y):
    return bytes([100, x, y])

def getClickMessage(msg):
    data = msg[1]

    if data[0] == 100:
        x = data[1]
        y = data[2]

        return (x, y)

    return None

def buildBoardMessage(board, hosts_turn):
    ints = [102, hosts_turn]

    for x in board:
        for y in x:
            ints.append(y)

    return bytes(ints)

def getBoardMessage(msg):
    data = msg[1]
    
    if data[0] == 102:
        board = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
        i = 2

        hosts_turn = data[1]
        
        for x in range(3):
            for y in range(3):
                board[x][y] = data[i]
                i += 1

        return (board, hosts_turn)
                
    return None

def make_move(board, player = 1):
    x, y = get_move()
            
    if board[x][y] == 0:
        board[x][y] = player
    
    return board

def get_move():
    valid = False

    while not valid:
        move = input('Make move ("0 0" -> left top): ')

        try:
            tmp = move.split(" ")
            x = int(tmp[0]) % 3
            y = int(tmp[1]) % 3

            valid = True
        except:
            print("No valid input!")
    
    return x, y

def run_master():
    n = Network(MASTER_PORT)

    n.start()

    board = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
    hosts_turn = 1

    slave_ip = ""

    #wait for connection from slave
    print("waiting for connection from slave...")
    waiting = True
    while waiting:
        time.sleep(0.1)
        for msg in n.received_data:
            connection_info = getConnectMessage(msg)
            if connection_info:
                waiting = False
                print("Slave connected:")
                print(connection_info)
                slave_ip = connection_info["ip"]
                n.send(slave_ip, SLAVE_PORT, buildConnectMessage(MASTER_PORT, "master"))

    while True:
        for i in board:
            print(i)

        board = make_move(board, PLAYER_X)

        hosts_turn = 0

        #send board update message
        n.send(slave_ip, SLAVE_PORT, buildBoardMessage(board, hosts_turn))

        #wait for messages and act accordingly
        for msg in n.received_data:
            val = getClickMessage(msg)

            if val:
                x = val[0]
                y = val[1]
                board = make_move(board, x, y, PLAYER_O)
                hosts_turn = 1
                n.send(slave_ip, SLAVE_PORT, buildBoardMessage(board, hosts_turn))
    

def run_slave(ip):
    n = Network(SLAVE_PORT)
    n.start()

    n.send(ip, MASTER_PORT, buildConnectMessage(MASTER_PORT, "slave"))

    board = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
    hosts_turn = 1

    #wait for master
    print("connecting to master...")
    waiting = True
    while waiting:
        time.sleep(0.1)
        for msg in n.received_data:
            connection_info = getConnectMessage(msg)
            if connection_info:
                #connected
                print("Connected:")
                print(connection_info)
                waiting = False
                    
    while True:
        #wait for board update message
        waiting = True
        while waiting:
            time.sleep(0.1)
            for msg in n.received_data:
                val = getBoardMessage(msg)
                if val:
                    board = val[0]
                    hosts_turn = val[1]
                    if hosts_turn == 0:
                        waiting = False

        for i in board:
            print(i)
        #make own move with ClickMessage
        x, y = get_move()
        n.send(ip, MASTER_PORT, buildClickMessage(x, y))

class Network:
    def __init__(self, port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(1)
        self.sock.bind(("", port))
        self.sock.listen()

        self.received_data = []

        self.running = False

    def send(self, ip, port, data):
        tmp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        tmp.settimeout(1)
        
        try:
            tmp.connect((ip, port))
            tmp.send(data)
            tmp.close()
            return True
        except:
            return False

    def start(self):
        if not self.running:
            _thread.start_new_thread(self.receive_thread, ())
            self.running = True

    def receive_thread(self):
        while self.running:
            try:
                conn, addr = self.sock.accept()
                with conn:
                    self.received_data.append((addr, conn.recv(1024)))
            except:
                pass
    def stop(self):
        self.running = False

if __name__ == "__main__":
    ip = input("Enter IP-address. Empty for Host: ")

    if ip == "":
        run_master()
    else:
        run_slave(ip)
