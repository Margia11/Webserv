#!/usr/bin/env python

import cgi

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
name = form.getvalue('name')
email = form.getvalue('email')

# Print the response headers
print("Content-type: text/html")
print()

# Print the response body
print("<html>")
print("<head>")
print("<title>Form Data</title>")
print("</head>")
print("<body>")
print("<h1>Form Data</h1>")
print("<p>Name: " + name + "</p>")
print("<p>Email: " + email + "</p>")
print("</body>")
print("</html>")
