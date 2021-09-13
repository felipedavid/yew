if not exist ..\..\bin\ {
    mkdir ..\..\bin\
}
cl.exe /Zi /EHsc main.cpp -o ..\..\bin\tsundere.exe
