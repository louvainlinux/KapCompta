#!/bin/sh
echo $PWD
cp KCCore/libKCCore.1.0.0.dylib KCApp/KapCompta.app/Contents/MacOs/libKCCore.1.dylib
install_name_tool -change libKCCore.1.dylib \
    @executable_path/libKCCore.1.dylib \
    KCApp/KapCompta.app/Contents/MacOs/KapCompta
