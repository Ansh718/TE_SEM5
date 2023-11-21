import socket

def main():
    try:
        # Create a socket to connect to the server at IP address "localhost" and port 12345
        with socket.create_connection(('localhost', 12345)) as socket_conn:
            # Create input and output streams for communication
            with socket_conn.makefile('r') as in_stream, socket_conn.makefile('w') as out_stream:
                # Send a message to the server
                out_stream.write("Hello from the client!\n")
                out_stream.flush()

                # Receive a response from the server
                response_from_server = in_stream.readline().strip()
                print("Server says:", response_from_server)

    except Exception as e:
        print("Error:", e)

if __name__ == "__main__":
    main()
