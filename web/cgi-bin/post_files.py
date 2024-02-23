#!/usr/bin/env python3

import cgi

# Set the content type to HTML
# print()
#print("Script CGI")
# Get the form data from the request
form = cgi.FieldStorage()

# Extract the form field values
name = form.getvalue('name')
file = form.getvalue('file')
fileitem = form['file']

uploadPath = "./web/uploads"
postDataPath = uploadPath + "/post_data.txt"

if fileitem.filename:
   fn = os.path.basename(fileitem.filename)
   with open(uploadPath + fn, 'wb') as f:
      f.write(fileitem.file.read())
   message = "The file '" + fn + "' was uploaded successfully"
else:
    message = "No file was uploaded"

with open(postDataPath, 'w') as f :
   f.write(name)
   f.write("\n")
   f.write(file)

# Print out the form data
body = """
<html>
<body>
<h1>Form Data</h1>
<p>Name: %s</p>
<p>File: %s</p>
</body>
</html>
""" % (name, file)

print("Content-type: text/html\r\n")
print("Content-Length: %d\r\n" % len(body))
print("\r\n")
print(body)
