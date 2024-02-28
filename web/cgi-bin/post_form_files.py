#!/usr/bin/env python3

import cgi
import sys

# Get the form data from the request
form = cgi.FieldStorage()

# Check if the 'file' field is present in the form data
if 'file' in form:
    # Extract the form field values
    fileitem = form['file']

    uploadPath = "./web/uploads/"
    postDataPath = uploadPath + "post_data.txt"

    with open(uploadPath + fileitem.filename, 'wb') as f:
        f.write(fileitem.file.read())

    with open (postDataPath, 'w') as f:
        f.write(f"filename: {fileitem.filename}")

    # Prepare the HTML response
    success_body = """<!DOCTYPE html>
    <html>
    <body>
    <h1>File Uploaded Successfully</h1>
    <p>Filename: %s</p>
    </body>
    </html>""" % (fileitem.filename)

    # Calculate the content length
    content_length = len(success_body)

    # Print headers, including Content-Length
    print("Content-Type: text/html; charset=utf-8\r")
    print("Content-Length: %d\r" % content_length)
    print("\r")

    # Print the HTML body
    print(success_body)
else:
    # Print error message if 'file' field is not present
    error_body = """
    <html>
    <body>
    <h1>Error</h1>
    <p>File field not found in the form data.</p>
    </body>
    </html>
    """

    # Calculate the content length
    content_length = len(error_body)

    # Print headers, including Content-Length
    print("Content-Type: text/html\r")
    print("Content-Length: %d\r" % content_length)
    print("\r")

    # Print the HTML body
    print(error_body)
    sys.exit(1)
sys.exit(0)
