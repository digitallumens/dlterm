Installation strategy as of 2015.05.29
------------------------------------------------------
We install to /tmp, and then the postinstall script:
1 - Sets owner to root:wheel
2 - Sets perms to 755
3 - Moves to /Library/Extensions
4 - Does a 'touch /Library/Extensions"


To build this installer package from the command line:
------------------------------------------------------
/Developer/usr/bin/packagemaker -d "Drivers.pmdoc" -v -o "./Install Drivers.pkg"
