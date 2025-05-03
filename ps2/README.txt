2. Code description

1. “Why This Works" explanation: 
analyze.c implements a function that parses an input string url, populating host name, doc
name and port number, wgethtml assemles an html get request, set up a and connect to socket 
that is listening on port of nginx, and prints payload to stdout. Finally wgettext redirects 
the html text into stdin and calls html2text to extract the text inside the html file. 