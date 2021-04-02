#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>

using namespace std;

/********************************************************************************/
/* CONFIGURACI�N DEL ARCHIVO */
FILE *fprograma;
bool archivoCorrecto (FILE *archivo, const char rutaPrograma []) // Comprobar que el archivo se abre correctamente.
{

    if (archivo == NULL)
    {
        cout<<"Error: Archivo no encontrado...\n"<<endl;
        cout<<"No se pudo leer la ruta: \""<<rutaPrograma<<"\""<<endl;
        return false;
    }

    return true;
}

/********************************************************************************/
/* CONFIGURACI�N PARA SEPARADORES */
string reservadas [] = {"inicio","fin","int","if","then","print"};
string separadores1 = "+*=!); \n"; // Identificadores
string separadores2 = "+*=!); \n"; // N�meros
string separadores3 = "( \n"; // Operadores Aritm�ticos
string separadores4 = "( "; // Operador de Asignaci�n
string separadores5 = " "; // Operadores Relacionales
string separadores6 = " "; // Par�ntesis Izquierdo
string separadores7 = "+*; "; // Par�ntesis Derecho
string separadores8 = " \n"; // Para ";" � Final de Sentencia

bool comparar_Sn (char c, string sN) // Busca en el Conjunto de Separadores de cada Estado Final
{
    bool encontrado = false;
    for (char& s : sN)
    {
        if (c == s)
        {
            encontrado = true;
        }
    }
    return encontrado;
}

/********************************************************************************/
/* FUNCIONES PARA COMPROBACIONES */
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
        for (char& c: i)
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
    for (char& c: n)
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

bool isOpAritmetico(string c) // Depende de Separadores3
{
    return (c.at(0) == '+' || c.at(0) == '*') && c.length() == 1;
}

bool isAsignacion(string c) // Depende de Separadores4
{
    return c.at(0) == '=' && c.length() == 1;
}

bool isOpRelacional(string o) // Depende de Separadores5
{
    if (o.compare("==") == 0 || o.compare("!=") == 0)
    {
        return true;
    }
    return false;
}

bool isParentesisIzq(string c) // Depende de Separadores6
{
    return c.at(0) == '(' && c.length() == 1;
}

bool isParentesisDer(string c) // Depende de Separadores7
{
    return c.at(0) == ')' && c.length() == 1;
}

bool isFinalSentencia(string c) // Depende de Separadores8
{
    return c.at(0) == ';' && c.length() == 1;
}

/********************************************************************************/
/* ESTRUCTURAS PARA LOS DATOS */
struct Token
{
    string tipo;
    string valor;
    Token* siguiente;
};

struct Iden
{
    int posicion;
    string valor;
    Iden* siguiente;
};

struct Num
{
    int posicion;
    string valor;
    Num* siguiente;
};

Token* tokenInicial = new Token();
Token* tokenFinal = new Token();
Iden* idenInicial = new Iden();
Num* numInicial = new Num();
Iden* idenFinal = new Iden();
Num* numFinal = new Num();

/********************************************************************************/
/* FUNCIONES PARA PRESENTACI�N */
void imprimirTokens(Token* primero, Token* ultimo)
{
    while(primero != ultimo)
    {
        cout<<primero->tipo<<" "<<primero->valor<<endl;
        primero = primero->siguiente;
    }
}

void imprimirIDs(Iden* primero, Iden* ultimo)
{
    while(primero != ultimo)
    {
        cout<<primero->posicion<<" "<<primero->valor<<endl;
        primero = primero->siguiente;
    }
}

void imprimirNums(Num* primero, Num* ultimo)
{
    while(primero != ultimo)
    {
        cout<<primero->posicion<<" "<<primero->valor<<endl;
        primero = primero->siguiente;
    }
}

/********************************************************************************/

/* PARA M�S INFORMACI�N:
 * REVISAR LA DOCUMENTACI�N Y EL DIAGRAMA DEL AUT�MATA.
 */

/********************************************************************************/
int analizadorLexico (const char rutaPrograma [])
{

    fprograma = fopen(rutaPrograma, "r"); // Apertura de Archivo

    if (!archivoCorrecto(fprograma, rutaPrograma))
    {
        exit(1);
    }
    else // Si el archivo est� correcto
    {
        /************************* (CADENA) *************************/
        Token* tokenAux = tokenInicial; // Para crear una pila de Tokens
        Iden* idenAux = idenInicial; // Para crear una pila de Identificadores
        Num* numAux = numInicial; // Para crear una pila de N�meros
        int contIds = 0; // Para contar los Identificadores
        int contNums = 0; // Para contar los N�meros
        /************************************************************/

        string cadena = "";	// Para guardar la cadena que representa un Token
        char c; // Car�cter para leer el archivo car�cter por car�cter
        int e = 0; // Estado Inicial

        while (!feof(fprograma)) // Mientras sea diferente del Final del Archivo
        {
            c =  getc(fprograma); // Obtiene un car�cter del Archivo

            switch(e) // Determina el Estado en el que se encuentra el Aut�mata
            {
            case 0: // El Caso 0: solo puede ir hacia ( -> ) {1,2,5,6,7}
                if(isalpha(c)) // Va a e = 1 con Letras
                {
                    e = 1;
                    cadena += c;
                }
                if (isdigit(c)) // Va a e = 2 con N�meros
                {
                    e = 2;
                    cadena += c;
                }
                if (c == '+' || c == '*') // Va a e = 5 con Op. Ar.
                {
                    e = 5;
                    cadena += c;
                }
                if (c == '=') // Va a e = 6 con Op. As.
                {
                    e = 6;
                    cadena += c;
                }
                if (c == '!') // Va a e = 7 con el s�mbolo "!"
                {
                    e = 7;
                    cadena += c;
                }
                if (c == '(') // Va a e = 9 con Par. Izq.
                {
                    e = 9;
                    cadena += c;
                }
                if (c == ')') // Va a e = 10 con Par. Der.
                {
                    e = 10;
                    cadena += c;
                }
                if (c == ';') // Va a e = 11 con s�mbolo ";"
                {
                    e = 11;
                    cadena += c;
                }
                break;
            case 1:
                /* El Caso 1 -> {1, 0}
                 * Se vuelve al estado cero para analizar un nuevo Token;
                 */
                if(isalpha(c) || isdigit(c))
                {
                    e = 1;
                    cadena += c;
                }
                else if (comparar_Sn(c, separadores1)) // Se determina si se construy� un Token dependiendo de sus Separadores
                {
                    e = 0;
                    ungetc(c, fprograma); // Se retrocede un car�cter para continuar con el correcto an�lisis

                    /************************* (CADENA) *************************/
                    if (isReservada(cadena)) // Si: es Palabra Reservada comprueba que sea v�lido el Token
                    {
                        tokenAux->tipo = "res";
                        tokenAux->valor = cadena;
                        tokenAux->siguiente = new Token();
                        tokenAux = tokenAux->siguiente;
                        tokenFinal = tokenAux;
                    }
                    else if (isIdentificador(cadena)) // Caso cntrario Si: es Identificador comprueba que sea v�lido el Token
                    {
                        contIds++;
                        idenAux->posicion = contIds;
                        idenAux->valor = cadena;
                        idenAux->siguiente = new Iden();
                        idenAux = idenAux->siguiente;
                        idenFinal = idenAux;

                        tokenAux->tipo = cadena;
                        tokenAux->valor = "id";
                        tokenAux->siguiente = new Token();
                        tokenAux = tokenAux->siguiente;
                        tokenFinal = tokenAux;
                    }
                    /************************************************************/

                    cadena = ""; // Se vac�a la cadena para construir el nuevo Token
                }
                else if (c == EOF) // (OMITIR) Si: es el final del Archivo se imprime "fin" como Palabra Reservada
                {
                    /************************* (CADENA) *************************/
                    if (isReservada(cadena))
                    {
                        tokenAux->tipo = "res";
                        tokenAux->valor = cadena;

                        tokenAux->siguiente = new Token();
                        tokenAux = tokenAux->siguiente;
                        tokenAux->tipo = "eof"; // Al final se agrega el s�mbolo "$" como delimitador
                        tokenAux->valor = "$";

                        tokenFinal = tokenAux->siguiente; // Tener en cuenta la �ltima instancia para la presentaci�n
                    }
                    else if (isIdentificador(cadena))   // Caso cntrario Si: es Identificador comprueba que sea v�lido el Token
                    {
                        contIds++;
                        idenAux->posicion = contIds;
                        idenAux->valor = cadena;
                        idenAux->siguiente = new Iden();
                        idenAux = idenAux->siguiente;
                        idenFinal = idenAux;

                        tokenAux->tipo = cadena;
                        tokenAux->valor = "id";

                        tokenAux->siguiente = new Token();
                        tokenAux = tokenAux->siguiente;
                        tokenFinal = tokenAux;
                        tokenAux->tipo = "eof"; // Al final se agrega el s�mbolo "$" como delimitador
                        tokenAux->valor = "$";

                        tokenFinal = tokenAux->siguiente; // Tener en cuenta la �ltima instancia para la presentaci�n
                    }
                    /************************************************************/
                }
                else // Para todos los casos en los que no se encuentre en el Lenguaje Definido
                {
                    return 1;
                }
                break;
            case 2: /** Los dem�s casos son similares por lo que no se comentan. **/
                if(isdigit(c))
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
                        contNums++;
                        numAux->posicion = contNums;
                        numAux->valor = cadena;
                        numAux->siguiente = new Num();
                        numAux = numAux->siguiente;
                        numFinal = numAux;

                        tokenAux->tipo = cadena;
                        tokenAux->valor = "num";
                        tokenAux->siguiente = new Token();
                        tokenAux = tokenAux->siguiente;
                        tokenFinal = tokenAux;
                    }
                    cadena = "";
                }
                else
                {
                    return 1;
                }
                break;
            case 3:
                if(isdigit(c))
                {
                    e = 4;
                    cadena += c;
                }
                break;
            case 4:
                if(isdigit(c))
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
                        contNums++;
                        numAux->posicion = contNums;
                        numAux->valor = cadena;
                        numAux->siguiente = new Num();
                        numAux = numAux->siguiente;
                        numFinal = numAux;

                        tokenAux->tipo = cadena;
                        tokenAux->valor = "num";
                        tokenAux->siguiente = new Token();
                        tokenAux = tokenAux->siguiente;
                        tokenFinal = tokenAux;
                    }
                    cadena = "";
                }
                else
                {
                    return 1;
                }
                break;
            case 5:
                if (comparar_Sn(c, separadores3) || isalpha(c) || isdigit(c))
                {
                    e = 0;
                    ungetc(c, fprograma);
                    if (isOpAritmetico(cadena))
                    {
                        tokenAux->tipo = "opar";
                        tokenAux->valor = cadena;
                        tokenAux->siguiente = new Token();
                        tokenAux = tokenAux->siguiente;
                        tokenFinal = tokenAux;
                    }
                    cadena = "";
                }
                else
                {
                    return 1;
                }
                break;
            case 6:
                if(c == '=')
                {
                    e = 8;
                    cadena += c;
                }
                else if (comparar_Sn(c, separadores4) || isalpha(c) || isdigit(c))
                {
                    e = 0;
                    ungetc(c, fprograma);
                    if (isAsignacion(cadena))
                    {
                        tokenAux->tipo = "asig";
                        tokenAux->valor = cadena;
                        tokenAux->siguiente = new Token();
                        tokenAux = tokenAux->siguiente;
                        tokenFinal = tokenAux;
                    }
                    cadena = "";
                }
                else
                {
                    return 1;
                }
                break;
            case 7:
                if(c == '=')
                {
                    e = 8;
                    cadena += c;
                }
                break;
            case 8:
                if (comparar_Sn(c, separadores5) || isalpha(c) || isdigit(c))
                {
                    e = 0;
                    ungetc(c, fprograma);
                    if (isOpRelacional(cadena))
                    {
                        tokenAux->tipo = "opre";
                        tokenAux->valor = cadena;
                        tokenAux->siguiente = new Token();
                        tokenAux = tokenAux->siguiente;
                        tokenFinal = tokenAux;
                    }
                    cadena = "";
                }
                else
                {
                    return 1;
                }
                break;
            case 9:
                if (comparar_Sn(c, separadores6) || isalpha(c) || isdigit(c))
                {
                    e = 0;
                    ungetc(c, fprograma);
                    if (isParentesisIzq(cadena))
                    {
                        tokenAux->tipo = "pizq";
                        tokenAux->valor = cadena;
                        tokenAux->siguiente = new Token();
                        tokenAux = tokenAux->siguiente;
                        tokenFinal = tokenAux;
                    }
                    cadena = "";
                }
                else
                {
                    return 1;
                }
                break;
            case 10:
                if (comparar_Sn(c, separadores7) || isalpha(c) || isdigit(c))
                {
                    e = 0;
                    ungetc(c, fprograma);
                    if (isParentesisDer(cadena))
                    {
                        tokenAux->tipo = "pder";
                        tokenAux->valor = cadena;
                        tokenAux->siguiente = new Token();
                        tokenAux = tokenAux->siguiente;
                        tokenFinal = tokenAux;
                    }
                    cadena = "";
                }
                else
                {
                    return 1;
                }
                break;
            case 11:
                if (comparar_Sn(c, separadores8) || isalpha(c))
                {
                    e = 0;
                    ungetc(c, fprograma);
                    if (isFinalSentencia(cadena))
                    {
                        tokenAux->tipo = "fise";
                        tokenAux->valor = cadena;
                        tokenAux->siguiente = new Token();
                        tokenAux = tokenAux->siguiente;
                        tokenFinal = tokenAux;
                    }
                    cadena = "";
                }
                else
                {
                    return 1;
                }
                break;
            }

        }
        /************************* (OMITIR) *************************/
        //imprimirTokens(tokenInicial, tokenFinal); // (OMITIR)
        //imprimirIDs(idenInicial, idenFinal); // (OMITIR)
        //imprimirNums(numInicial, numFinal); // (OMITIR)

        delete idenAux; // (OMITIR) Se libera la memoria ocupada por el puntero
        delete numAux; // (OMITIR) Se libera la memoria ocupada por el puntero
        /************************************************************/

        fclose(fprograma); // Cierre de Archivo
    }

    return 0;
}
/********************************************************************************/
