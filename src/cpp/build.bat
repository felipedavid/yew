if not exist ..\..\bin\ {
    mkdir ..\..\bin\
}
cl.exe /Zi /EHsc main.cpp scanner.cpp token.cpp -o ..\..\bin\tsundere.exe
