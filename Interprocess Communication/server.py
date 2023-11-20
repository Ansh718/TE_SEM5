import socket
import threading

class ClientHandler(threading.Thread):
    def __init__(self, client_socket):
        super().__init__()
        self.client_socket = client_socket

    def run(self):
        try:
            # Create input and output streams for communication with the client
            with self.client_socket.makefile('r') as in_stream, self.client_socket.makefile('w') as out_stream:
                # Read a message from the client
                message_from_client = in_stream.readline().strip()
                print("Client says:", message_from_client)

                # Send a response to the client
                out_stream.write("Hello from the server!\n")
                out_stream.flush()

        except Exception as e:
            print("Error:", e)

        finally:
            # Close the client socket
            self.client_socket.close()

def main():
    # Create a server socket that listens on port 12345
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('localhost', 12345))
    server_socket.listen(5)

    print("Server is waiting for connections...")

    try:
        while True:
            # Wait for a client to connect
            client_socket, _ = server_socket.accept()
            print("Client connected.")

            # Create a new thread to handle the client
            client_handler = ClientHandler(client_socket)
            client_handler.start()

    except KeyboardInterrupt:
        pass

    finally:
        # Close the server socket
        server_socket.close()

if __name__ == "__main__":
    main()
