#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>

using namespace std;

/********************************************************************************/
/* CONFIGURACIÓN DEL ARCHIVO */
const char rutaPrograma[] = ".\\Archivos\\codigoFuente.txt";
FILE *fprograma;
bool archivoCorrecto(FILE *archivo) // Comprobar que el archivo se abre correctamente.
{

    if (archivo == NULL)
    {
        cout << "Error: Archivo no encontrado...\n"
             << endl;
        cout << "No se pudo leer la ruta: \"" << rutaPrograma << "\"" << endl;
        return false;
    }

    return true;
}

/* VARIABLES GLOBALES */
string reservadas[] = {"inicio", "fin", "int", "if", "then", "print"};
string separadores1 = "+*=!); \n";
string separadores2 = "+*=!); \n";
string separadores3 = "( \n";
string separadores4 = "( ";
string separadores5 = " ";

/********************************************************************************/
/* FUNCIONES PARA LOS SEPARADORES */
bool comparar_Sn(char c, string sN) // Busca en el Conjunto de Separadores de cada Estado Final
{
    bool encontrado = false;
    for (char &s : sN)
    {
        if (c == s)
        {
            encontrado = true;
        }
    }
    return encontrado;
}

/********************************************************************************/
/* MÉTODO PARA IMPRIMIR */
void imprimirLinea(int linea) // (OMITIR) Impresión para mejor legibilidad.
{
    cout << "----------------------------------------------------" << endl;
    cout << "                       LINEA " << linea << endl;
    cout << "----------------------------------------------------" << endl;
}

/* M�TODOS PARA COMPROBAR */
bool isReservada(string r) // Depende de Separadores1
{
    for (string reservada : reservadas)
    {
        if (reservada.compare(r) == 0)
        {
            return true;
        }
    }
    return false;
}

bool isIdentificador(string i) // Depende de Separadores1
{
    if (isalpha(i.at(0)))
    {
        for (char &c : i)
        {
            if (!isdigit(c) && !isalpha(c))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool isNumero(string n) // Depende de Separadores2
{
    int contadorPuntosDecimal = 0;
    for (char &c : n)
    {
        if (!isdigit(c))
        {
            if (c == '.')
            {
                contadorPuntosDecimal++;
            }
            else
            {
                return false;
            }
        }
    }
    if (contadorPuntosDecimal >= 0 && contadorPuntosDecimal < 2)
    {
        return true;
    }
    return false;
}

bool isOpAritmetico(string s) // Depende de Separadores3
{
    return s.at(0) == '+' || s.at(0) == '*';
}

bool isAsignacion(string s) // Depende de Separadores4
{
    return s.at(0) == '=';
}

bool isOpRelacional(string o) // Depende de Separadores5
{
    if (o.compare("==") == 0 || o.compare("!=") == 0)
    {
        return true;
    }
    return false;
}

/********************************************************************************/

/* PARA MÁS INFORMACIÓN:
 * REVISAR LA DOCUMENTACIÓN Y EL DIAGRAMA DEL AUTÓMATA.
 */

/********************************************************************************/
int main()
{

    fprograma = fopen(rutaPrograma, "r");

    if (!archivoCorrecto(fprograma))
    {
        exit(1);
    }
    else
    {

        cout << "****************************************************" << endl;
        cout << "                     INICIALIZADO" << endl;
        cout << "****************************************************\n"
             << endl;

        int linea = 0;      // Contador de Líneas
        string cadena = ""; // Para guardar la cadena que representa un Token
        char c;             // Carácter para leer el archivo carácter por carácter
        int e = 0;          // Estado Inicial

        while (!feof(fprograma)) // Mientras sea diferente del Final del Archivo
        {
            c = getc(fprograma); // Obtiene un carácter del Archivo

            switch (e) // Determina el Estado en el que se encuentra el Autómata
            {
            case 0: // El Caso 0: solo puede ir hacia ( -> ) {1,2,5,6,7}
                if (isalpha(c))
                {
                    e = 1;
                    cadena += c;
                }
                if (isdigit(c))
                {
                    e = 2;
                    cadena += c;
                }
                if (c == '+' || c == '*')
                {
                    e = 5;
                    cadena += c;
                }
                if (c == '=')
                {
                    e = 6;
                    cadena += c;
                }
                if (c == '!')
                {
                    e = 7;
                    cadena += c;
                }
                break;
            case 1:
                /* El Caso 1 -> {1, 0}
                 * Se vuelve al estado cero para analizar un nuevo Token;
                 */
                if (isalpha(c) || isdigit(c))
                {
                    e = 1;
                    cadena += c;
                }
                else if (comparar_Sn(c, separadores1)) // Se determina si se construyó un Token dependiendo de sus Separadores
                {
                    e = 0;
                    ungetc(c, fprograma);    // Se retrocede un carácter para continuar con el correcto análisis
                    if (isReservada(cadena)) // Si: es Palabra Reservada comprueba que sea válido el Token
                    {
                        // (OMITIR) La impresión se realiza por cuestión de estética.
                        if (cadena.compare("inicio") == 0)
                        {
                            linea++;
                            imprimirLinea(linea);
                            cout << "Res: " + cadena << endl;
                            linea++;
                            imprimirLinea(linea);
                        }
                        else
                        {
                            cout << "Res: " + cadena << endl;
                        }
                    }
                    else if (isIdentificador(cadena)) // Caso cntrario Si: es Identificador comprueba que sea válido el Token
                    {
                        cout << "Iden: " + cadena << endl;
                    }
                    else // Caso contrario: para los elementos desconocidos
                    {
                        cout << "----: " + cadena << endl;
                    }
                    /* (OMITIR) Si el carácter es un ";" se imprime una nueva línea.
                     * Esto solo sucede para los Identificadores y Números que son
                     * las dos instancias en que puede existir el punto y coma.
                     */
                    if (c == ';')
                    {
                        linea++;
                        imprimirLinea(linea);
                    }
                    cadena = ""; // Se vacía la cadena para construir el nuevo Token
                }
                else if (c == EOF) // (OMITIR) Si: es el final del Archivo se imprime "fin" como Palabra Reservada
                {
                    if (isReservada(cadena))
                    {
                        cout << "Res: " + cadena << endl;
                    }
                }
                else // Para todos los casos en los que no se encuentre en el Lenguaje Definido
                {
                    cout << "ERROR: No reconocido" << endl;
                }
                break;
            case 2:
                if (isdigit(c))
                {
                    e = 2;
                    cadena += c;
                }
                else if (c == '.')
                {
                    e = 3;
                    cadena += c;
                }
                else if (comparar_Sn(c, separadores2))
                {
                    e = 0;
                    ungetc(c, fprograma);
                    if (isNumero(cadena))
                    {
                        cout << "Num: " + cadena << endl;
                    }
                    else
                    {
                        cout << "----: " + cadena << endl;
                    }
                    if (c == ';')
                    {
                        linea++;
                        imprimirLinea(linea);
                    }
                    cadena = "";
                }
                else
                {
                    cout << "ERROR: No reconocido" << endl;
                }
                break;
            case 3:
                if (isdigit(c))
                {
                    e = 4;
                    cadena += c;
                }
                break;
            case 4:
                if (isdigit(c))
                {
                    e = 4;
                    cadena += c;
                }
                else if (comparar_Sn(c, separadores2))
                {
                    e = 0;
                    ungetc(c, fprograma);
                    if (isNumero(cadena))
                    {
                        cout << "Num: " + cadena << endl;
                    }
                    else
                    {
                        cout << "----: " + cadena << endl;
                    }
                    if (c == ';')
                    {
                        linea++;
                        imprimirLinea(linea);
                    }
                    cadena = "";
                }
                else
                {
                    cout << "ERROR: No reconocido" << endl;
                }
                break;
            case 5:
                if (comparar_Sn(c, separadores3))
                {
                    e = 0;
                    ungetc(c, fprograma);
                    if (isOpAritmetico(cadena))
                    {
                        cout << "OpAr: " + cadena << endl;
                    }
                    else
                    {
                        cout << "----: " + cadena << endl;
                    }
                    cadena = "";
                }
                else
                {
                    cout << "ERROR: No reconocido" << endl;
                }
                break;
            case 6:
                if (c == '=')
                {
                    e = 8;
                    cadena += c;
                }
                else if (comparar_Sn(c, separadores4))
                {
                    e = 0;
                    ungetc(c, fprograma);
                    if (isOpRelacional(cadena))
                    {
                        cout << "OpRe: " + cadena << endl;
                    }
                    else if (isAsignacion(cadena))
                    {
                        cout << "Asig: " + cadena << endl;
                    }
                    else
                    {
                        cout << "----: " + cadena << endl;
                    }
                    cadena = "";
                }
                else
                {
                    cout << "ERROR: No reconocido" << endl;
                }
                break;
            case 7:
                if (c == '=')
                {
                    e = 8;
                    cadena += c;
                }
                break;
            case 8:
                if (comparar_Sn(c, separadores5))
                {
                    e = 0;
                    ungetc(c, fprograma);
                    if (isOpRelacional(cadena))
                    {
                        cout << "OpRe: " + cadena << endl;
                    }
                    else
                    {
                        cout << "----: " + cadena << endl;
                    }
                    cadena = "";
                }
                else
                {
                    cout << "ERROR: No reconocido" << endl;
                }
                break;
            }
        }

        fclose(fprograma);

        cout << "\n****************************************************" << endl;
        cout << "                     FINALIZADO" << endl;
        cout << "****************************************************" << endl;
    }

    return 0;
}
/********************************************************************************/
