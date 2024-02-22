#!/usr/bin/env python3

import cgi

# Set the content type to HTML
# print()
#print("Script CGI")
# Get the form data from the request
form = cgi.FieldStorage()

# Extract the form field values
name = form.getvalue('name')
email = form.getvalue('email')

uploadPath = "./web/uploads"
postDataPath = uploadPath + "/post_data.txt"
with open(postDataPath, 'w') as f :
    f.write(name)
    f.write(email)

# Print out the form data
body = """
<html>
<body>
<h1>Form Data</h1>
<p>Name: %s</p>
<p>Email: %s</p>
</body>
</html>
""" % (name, email)

print("Content-type: text/html\r\n")
print("Content-Length: %d\r" % len(body))
print("\r")
print(body)
