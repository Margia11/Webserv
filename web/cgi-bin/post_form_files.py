#!/usr/bin/env python3

import cgi
import os

# Print the content type to HTML
print("Content-type: text/html\r")
print()

# Get the form data from the request
form = cgi.FieldStorage()

# Check if the 'file' field is present in the form data
if 'file' in form:
    # Extract the form field values
    fileitem = form['file']

    # Rest of your script to process the file...

    # Print success message
    print("<html><body>")
    print("<h1>File Uploaded Successfully</h1>")
    print("<p>Filename: %s</p>" % fileitem.filename)
    print("<p>Content:</p>")
    print("<pre>%s</pre>" % fileitem.file.read().decode('utf-8'))
    print("</body></html>")
else:
    # Print error message if 'file' field is not present
    print("<html><body>")
    print("<h1>Error</h1>")
    print("<p>File field not found in the form data.</p>")
    print("</body></html>")
