#!/usr/bin/env python3

import cgi

# Set the content type to HTML
# print()
#print("Script CGI")
# Get the form data from the request
form = cgi.FieldStorage()

# Extract the form field values
name = form.getvalue('name')
email = form.getvalue('file')

uploadPath = "./web/uploads"
postDataPath = uploadPath + "/post_data.txt"
with open(postDataPath, 'w') as f :
    f.write(name)
    f.write("\n")
    f.write(email)

# Print out the form data
body = """<!DOCTYPE html>
<html>
<body>
<h1>Form Data</h1>
<p>Name: %s</p>
<p>Email: %s</p>
</body>
</html>""" % (name, email)

print("Content-Type: text/html\r")
print("Content-Length: %d\r" % len(body))
print("\r")
print(body)
