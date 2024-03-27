#!/usr/bin/env python3

import base64
import cgi

form = cgi.FieldStorage()

filename = form.getvalue('file')
if not filename:
    print("Content-Type: text/html\r\n")
    cl = len("<html><body><h1>Error: no file matches</h1></body></html>")
    print("Content-Length: " + str(cl) + "\r\n\r")
    print("<html><body><h1>Error: no file matches</h1></body></html>")
    exit(1)

def generate_html_body(image_path):

    # Genera il corpo HTML con l'immagine
    html_body = """<!DOCTYPE html>
    <html>
    <body>
        <img src="%s"><img/>
    </body>
    </html>""" % image_path

    return html_body

file_path = './img/' + filename + '.jpeg'
html_content = generate_html_body(file_path)

header = "Content-Type: text/html\r\n"
header += "Content-Length: " + str(len(html_content)) + "\r\n\r"

print(header)
print(html_content)