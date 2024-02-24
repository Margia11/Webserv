#!/usr/bin/env python3

import base64

def generate_html_body(image_path):
    # Apre il file in modalità binaria
    with open(image_path, 'rb') as file:
        # Legge i dati binari del file
        binary_data = file.read()

    # Converte i dati binari in una stringa base64
    base64_data = base64.b64encode(binary_data).decode('utf-8')

    # Genera il corpo HTML con l'immagine
    html_body = f"""
    <html>
    <body>
        <img src="./img/Gatto.jpeg"><img/>
    </body>
    </html>
    """

    return html_body

file_path = './web/img/Gatto.jpeg'
html_content = generate_html_body(file_path)

header = "HTTP/1.1 200 OK\r\n"
header += "Content-Type: text/html\r\n"
header += "Content-Length: " + str(len(html_content)) + "\r\n\r"

print(header)
print(html_content)

