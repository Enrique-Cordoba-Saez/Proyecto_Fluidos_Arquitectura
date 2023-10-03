#include <iostream>
#include <fstream>
#include <cstring>

int main() {
    using namespace std;
    const string archivoEntrada = "read_binary/files/small.fld";
    ifstream archivo(archivoEntrada, ios::binary);

    if (!archivo) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    char buffer[4];
    int i = 0;
    while (archivo.read(buffer, sizeof(buffer))&& (i < 28))  {
        float data_simple;
        memcpy(&data_simple, buffer, sizeof(data_simple));
        double data = data_simple;
        cout << "Número leído " << i << ": " << data << endl;
        i++;
    }
    archivo.close();
    return 0;
}
