#!/usr/bin/env python3

import cgi

# Get the form data from the request
form = cgi.FieldStorage()

# Check if the 'file' field is present in the form data
if 'file' in form:
    # Extract the form field values
    fileitem = form['file']

    # Rest of your script to process the file...

    # Prepare the HTML response
    success_body = """
    <html>
    <body>
    <h1>File Uploaded Successfully</h1>
    <p>Filename: %s</p>
    <p>Content:</p>
    <pre>%s</pre>
    </body>
    </html>
    """ % (fileitem.filename, fileitem.file.read().decode('utf-8'))

    # Calculate the content length
    content_length = len(success_body)

    # Print headers, including Content-Length
    print("Content-type: text/html; charset=utf-8\r")
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
    print("Content-type: text/html\r")
    print("Content-Length: %d\r" % content_length)
    print("\r")

    # Print the HTML body
    print(error_body)
