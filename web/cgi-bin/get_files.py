#!/usr/bin/env python3
import os

# Set the path to the directory you want to list
directory_path = 'Server/'

# Get the list of files in the directory
files = os.listdir(directory_path)

# Print the HTML header
header = ("Content-Type: text/html\r\n")

body = (("""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        :root {
            --silver-lake-blue: #6290c3ff;
            --mint-green: #c2e7daff;
            --honeydew: #f1ffe7ff;
            --space-cadet: #1a1b41ff;
            --lime: #baff29ff;

            --silver-lake-blue-hsl: hsla(212, 45%, 57%, 1);
            --mint-green-hsl: hsla(159, 44%, 83%, 1);
            --honeydew-hsl: hsla(95, 100%, 95%, 1);
            --space-cadet-hsl: hsla(238, 43%, 18%, 1);
            --lime-hsl: hsla(79, 100%, 58%, 1);

            --silver-lake-blue-rgba: rgba(98, 144, 195, 1);
            --mint-green-rgba: rgba(194, 231, 218, 1);
            --honeydew-rgba: rgba(241, 255, 231, 1);
            --space-cadet-rgba: rgba(26, 27, 65, 1);
            --lime-rgba: rgba(186, 255, 41, 1);

            --gradient-top: linear-gradient(0deg, var(--silver-lake-blue), var(--mint-green), var(--honeydew), var(--space-cadet), var(--lime));
            --gradient-right: linear-gradient(90deg, var(--silver-lake-blue), var(--mint-green), var(--honeydew), var(--space-cadet), var(--lime));
            --gradient-bottom: linear-gradient(180deg, var(--silver-lake-blue), var(--mint-green), var(--honeydew), var(--space-cadet), var(--lime));
            --gradient-left: linear-gradient(270deg, var(--silver-lake-blue), var(--mint-green), var(--honeydew), var(--space-cadet), var(--lime));
            --gradient-top-right: linear-gradient(45deg, var(--silver-lake-blue), var(--mint-green), var(--honeydew), var(--space-cadet), var(--lime));
            --gradient-bottom-right: linear-gradient(135deg, var(--silver-lake-blue), var(--mint-green), var(--honeydew), var(--space-cadet), var(--lime));
            --gradient-top-left: linear-gradient(225deg, var(--silver-lake-blue), var(--mint-green), var(--honeydew), var(--space-cadet), var(--lime));
            --gradient-bottom-left: linear-gradient(315deg, var(--silver-lake-blue), var(--mint-green), var(--honeydew), var(--space-cadet), var(--lime));
            --gradient-radial: radial-gradient(var(--silver-lake-blue), var(--mint-green), var(--honeydew), var(--space-cadet), var(--lime));
        }

        body {
            margin: 0;
            height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            background: var(--gradient-radial);
        }

        h1 {
            color: var(--space-cadet);
        }
        h2 {
            color: var(--lime);
        }
    </style>
    <title>CSS Gradients Page</title>
</head>
<body>
"""))

# Print the page title
body += (("""\
    <h2>Files in directory: {}</h2>
    """).format(directory_path))

# Print the list of files as an unordered list
body += (("""\
    <ul>
    """))

for file in files:
    body += (("""\
        <li>{}</li>
    """.format(file)))

body += (("""\
    </ul>
"""))

# Print the HTML footer
body += (("""\
</body>
</html>"""))

header += "Content-Length: " + str(len(body)) + "\r\n\r\n"

print(header + body)
