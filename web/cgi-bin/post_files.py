#!/usr/bin/python3

import cgi, os

form = cgi.FieldStorage()

# Get a filename here
fileitem = form['file']

# Test if the file was uploaded
if fileitem.file:
   open('/cgi-bin/tmp/' + os.path.basename(fileitem.file), 'wb').write(fileitem.file.read())
   message = 'The file "' + os.path.basename(fileitem.file) + '" was uploaded successfully'
else:
   message = 'Uploading Failed'

print("Content-Type: text/html;charset=utf-8")
print ("Content-type:text/html\r\n")
print("<H1> " + message + " </H1>")
