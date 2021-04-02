#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
#include "AnalizadorLexico.h"

using namespace std;

/********************************************************************************/
/* CONFIGURACIÓN DEL ARCHIVO */
const char rutaPrograma [] = ".\\Archivos\\codigoFuente.txt";

/********************************************************************************/
/* CONFIGURACIÓN DE LAS TABLAS */
/**/
const int tablaAccionRows = 96;
const int tablaIr_ARows = 36;
const int tablaProduccionesRows = 23;

// Estado, Token, Acción
string tablaAccion[tablaAccionRows][3] =
{

    {"0","inicio","s02"}, {"1","$","acc"}, {"2","print","s08"}, {"2","id","s11"}, {"2","int","s10"}, {"2","if","s09"},
    {"3","fin","s12"}, {"4",";","s13"}, {"5","fin","r04"}, {"5","print","s08"}, {"5","id","s11"}, {"5","int","s10"}, {"5","if","s09"},
    {"6","id","s11"}, {"7","=","s16"}, {"8","id","s17"}, {"9","(","s18"}, {"10","id","r09"}, {"11","=","r10"}, {"12","$","r01"},
    {"13","fin","r02"}, {"13","print","s08"}, {"13","id","s11"}, {"13","int","s10"}, {"13","if","s09"}, {"14","fin","r05"}, {"15","=","s20"},
    {"16","id","s26"}, {"16","(","s25"}, {"16","num","s27"}, {"17",";","r08"}, {"18","id","s29"}, {"19","fin","r03"}, {"20","id","s26"},
    {"20","(","s25"}, {"20","num","s27"}, {"21",";","r07"}, {"22",";","r11"}, {"22","+","s31"}, {"23",";","r13"}, {"23","+","r13"},
    {"23","*","s32"}, {"23",")","r13"}, {"24",";","r15"}, {"24","+","r15"}, {"24","*","r15"}, {"24",")","r15"}, {"25","id","s26"},
    {"25","(","s25"}, {"25","num","s27"}, {"26",";","r17"}, {"26","+","r17"}, {"26","*","r17"}, {"26",")","r17"}, {"27",";","r18"},
    {"27","+","r18"}, {"27","*","r18"}, {"27",")","r18"}, {"28",")","s34"}, {"29","==","s36"}, {"29","!=","s37"}, {"30",";","r06"},
    {"31","id","s26"}, {"31","(","s25"}, {"31","num","s27"}, {"32","id","s26"}, {"32","(","s25"}, {"32","num","s27"}, {"33","+","s31"},
    {"33",")","s40"}, {"34","then","s41"}, {"35","id","s42"}, {"36","id","r21"}, {"37","id","r22"}, {"38",";","r12"}, {"38","+","r12"},
    {"38","*","s32"}, {"38",")","r12"}, {"39",";","r14"}, {"39","+","r14"}, {"39","*","r14"}, {"39",")","r14"}, {"40",";","r16"},
    {"40","+","r16"}, {"40","*","r16"}, {"40",")","r16"}, {"41","print","s08"}, {"41","id","s11"}, {"41","int","s10"}, {"42",")","r20"},
    {"43",";","s44"}, {"44","fin","r19"}, {"44","print","r19"}, {"44","id","r19"}, {"44","int","r19"}, {"44","if","r19"}

};

// Estado, EncabezadoProducción, SiguienteEstado
string tablaIr_A[tablaIr_ARows][3] =
{

    {"0","P","1"}, {"2","B","3"}, {"2","S","4"}, {"2","TV","6"}, {"2","UV","7"}, {"2","CTRL","5"}, {"5","B","14"},
    {"5","S","4"}, {"5","TV","6"}, {"5","UV","7"}, {"5","CTRL","5"}, {"6","UV","15"}, {"13","B","19"}, {"13","S","4"},
    {"13","TV","6"}, {"13","UV","7"}, {"13","CTRL","5"}, {"16","VAL","21"}, {"16","E","22"}, {"16","T","23"}, {"16","F","24"},
    {"18","COND","28"}, {"20","VAL","30"}, {"20","E","22"}, {"20","T","23"}, {"20","F","24"}, {"25","E","33"}, {"25","T","23"},
    {"25","F","24"}, {"29","OR","35"}, {"31","T","38"}, {"31","F","24"}, {"32","F","39"}, {"41","S","43"}, {"41","TV","6"},
    {"41","UV","7"}

};

// NúmeroProducción, EncabezadoProducción, CuerpoProducción
string tablaProducciones[tablaProduccionesRows][3] =
{
    {"0","P\'","P"}, {"1","P","inicio B fin"}, {"2","B","S ;"}, {"3","B","S ; B"}, {"4","B","CTRL"}, {"5","B","CTRL B"},
    {"6","S","TV UV = VAL"}, {"7","S","UV = VAL"}, {"8","S","print id"}, {"9","TV","int"}, {"10","UV","id"},
    {"11","VAL","E"}, {"12","E","E + T"}, {"13","E","T"}, {"14","T","T * F"}, {"15","T","F"}, {"16","F","( E )"},
    {"17","F","id"}, {"18","F","num"}, {"19","CTRL","if ( COND ) then S ;"}, {"20","COND","id OR id"}, {"21","OR","=="},
    {"22","OR","!="}
};
/**/

/**
const int tablaAccionRows = 36;
const int tablaIr_ARows = 9;
const int tablaProduccionesRows = 6;

string tablaAccion[tablaAccionRows][3] =
{
    {"0","id","s05"}, {"0","(","s04"}, {"1","+","s06"}, {"1","$","acc"}, {"2","+","r02"}, {"2","*","s07"}, {"2",")","r02"},
    {"2","$","r02"}, {"3","+","r04"}, {"3","*","r04"}, {"3",")","r04"}, {"3","$","r04"}, {"4","id","s05"}, {"4","(","s04"},
    {"5","+","r06"}, {"5","*","r06"}, {"5",")","r06"}, {"5","$","r06"}, {"6","id","s05"}, {"6","(","s04"}, {"7","id","s05"},
    {"7","(","s04"}, {"8","+","s06"}, {"8",")","s11"}, {"9","+","r01"}, {"9","*","s07"}, {"9",")","r01"}, {"9","$","r01"},
    {"10","+","r03"}, {"10","*","r03"}, {"10",")","r03"}, {"10","$","r03"}, {"11","+","r05"}, {"11","*","r05"}, {"11",")","r05"},
    {"11","$","r05"}
};

string tablaIr_A[tablaIr_ARows][3] =
{
    {"0","E","1"}, {"0","T","2"}, {"0","F","3"}, {"4","E","8"}, {"4","T","2"},
    {"4","F","3"}, {"6","T","9"}, {"6","F","3"}, {"7","F","10"}
};

string tablaProducciones[tablaProduccionesRows][3] =
{
    {"1","E","E + T"}, {"2","E","T"}, {"3","T","T * F"}, {"4","T","F"}, {"5","F","( E )"},{"6","F","id"}
};
/**/

/********************************************************************************/
/* VARIABLES GLOBALES Y FUNCIONES DE PILA*/
struct Pila
{
    int estado = 0;
    Pila* siguiente = NULL;
};

Pila* nodoFondo = new Pila();
Pila* nodoSuperior = nodoFondo;

/*#####         FUNCIONES DE PILA         ######*/
void agregaNodo (Pila* &nodo)
{
    Pila* nodoAux = new Pila();
    nodoAux->siguiente = nodo;
    nodo = nodoAux;
}

int reduceNodos (int cant, Pila* &nodo)
{
    Pila* nodoAux;
    for (int i = 0; i < cant; i++)
    {
        nodoAux = nodo;
        nodo = nodo->siguiente;
        delete nodoAux;
    }
    return nodo->estado; // Devuelve el SiguienteEstado antes de Reducir
}

/********************************************************************************/
/* VARIABLES GLOBALES Y FUNCIONES DE PRESENTACIÓN */
int lineas = 0;
string sentencia = "";
string integridad = "";

struct Movimiento
{
    int num;
    string pila;
    string simbolo;
    string entrada;
    string accion;
    Movimiento* siguiente;
};

Movimiento* movInicial = new Movimiento();
Movimiento* movFinal = movInicial;

/*#####         FUNCIONES PARA PRESENTACIÓN         ######*/

void presentar ()
{
    cout<<"("<<movFinal->num<<")\t| "<<movFinal->pila<<movFinal->simbolo<<"\t\t"<<movFinal->entrada<<"\t | "<<movFinal->accion<<endl;
}

void imprimirPila (Pila* inicio, Pila* fin, string token)
{
    string pila = "";

    while (inicio != fin)
    {
        pila = to_string(inicio->estado) + " " + pila;
        inicio = inicio->siguiente;
    }
    pila = to_string(inicio->estado) + " " + pila;

    movFinal->pila = pila;
    presentar();
}

/********************************************************************************/
/* FUNCIONES DE ACCIÓN() E IR_A() */
int accionToInt (string accion) // Elimina la primera letra (Acción) y devuelve el Estado
{
    string acc = "";
    acc += accion.at(1);
    acc += accion.at(2);
    return stoi(acc); // Recupera la accion de tablaAccion[e][2] en Tipo de Dato Entero;
}

/*#####         ARGUMENTOS DE LA FUNCIÓN REDUCIR         ######*/
int calculaReducciones (int pN)
{
    int r = 0;

    for (int i = 0; i < tablaProduccionesRows; i++)
    {
        if (stoi(tablaProducciones[i][0]) == pN && tablaProducciones[i][2].length() != 0)
        {

            for (int j = 0; j < tablaProducciones[i][2].length(); j++)
            {
                if (tablaProducciones[i][2].at(j) == ' ')
                {
                    r++;
                }
            }
            r++;
            return r;
        }
    }

    return -1;
}

string obtenerEncabezadoProduccion (int pN)
{
    for (int i = 0; i < tablaProduccionesRows; i++)
    {
        if (stoi(tablaProducciones[i][0]) == pN)
        {
            movFinal->accion += tablaProducciones[i][1] + " -> " + tablaProducciones[i][2]; // (OMITIR)
            movFinal->simbolo = tablaProducciones[i][1]; // (OMITIR)
            return tablaProducciones[i][1];
        }
    }

    return "";
}

/*#####         ACCIÓN() E IR_A()         ######*/
int ir_A(int e, string encabezadoProduccion) // Número de Estado y Cabecera de Producción
{
    for (int i = 0; i < tablaIr_ARows; i++)
    {
        if (e == stoi(tablaIr_A[i][0]) && tablaIr_A[i][1].compare(encabezadoProduccion) == 0)
        {
            return stoi(tablaIr_A[i][2]); // Devuelve el SiguienteEstado después de Reducir
        }
    }

    return -1;
}

int realizarAccion(int e) // Obtiene la siguiente Acción que debe realizar de la TablaAcción
{
    if (tablaAccion[e][2].at(0) == 's')
    {
        movFinal->accion = "desplazar " + tablaAccion[e][1]; // (OMITIR)
        agregaNodo(nodoSuperior); // Se agrega el nodo para el nuevo Estado
        nodoSuperior->estado = accionToInt(tablaAccion[e][2]); // Se agrega el nuevo Estado
        movFinal->simbolo = tablaAccion[e][1]; // (OMITIR)
        return 0;
    }
    else if (tablaAccion[e][2].at(0) == 'r')
    {
        int pN = accionToInt(tablaAccion[e][2]); // Obtiene el NúmeroProducción de la TablaProducciones
        int r = calculaReducciones(pN); // Calcula cuántos nodos debe Reducir
        if (r != -1)
        {
            movFinal->accion = "reducir " + to_string(r) + " mediante "; // (OMITIR)
            int pilaReducida = reduceNodos(r, nodoSuperior); // Se obtiene el nuevo Estado antes de Reducir
            agregaNodo(nodoSuperior); // Se agrega el nodo para el nuevo Estado
            string encabezadoProduccion = obtenerEncabezadoProduccion(pN); // Obtiene el EncabezadoProducción
            nodoSuperior->estado = ir_A(pilaReducida, encabezadoProduccion); // Se agrega el nuevo Estado después de Reducir
            if (nodoSuperior->estado == -1)
            {
                return -1;
            }
            return 1;
        }
    }
    else if (tablaAccion[e][2] == "acc")
    {
        Pila* nodoAux = nodoSuperior; // Se iguala el nodoFondo de la pila al nodoSuperior
        nodoSuperior = nodoFondo;
        delete nodoAux;
        return 2;
    }

    return -1;
}

/********************************************************************************/
/* FUNCIONES PARA CONTROL Y EJECUCIÓN */

/*#####         PROGRAMA CONTROLADOR         ######*/
int siguienteEstado (Pila* nodo, Token* entrada) // Busca el siguiente Estado en TablaAcción
{

    for (int i = 0; i < tablaAccionRows; i++)
    {
        if (nodo->estado == stoi(tablaAccion[i][0]) && entrada->valor.compare(tablaAccion[i][1]) == 0)
        {
            //movFinal->accion = tablaAccion[i][2]; // Imprime las Acciones de cada Movimiento
            return i;
        }

    }

    return -1;

}

/********************************************************************************/
/* ANALIZADOR SINTÁCTICO */
int analizadorSintactico (Token* inicio, Token* fin)
{
    int e = 0;
    int contador = 0; // (OMITIR)

    while (inicio != fin)
    {
        contador++; // (OMITIR)

        e = siguienteEstado(nodoSuperior, inicio);

        /************************* (NO-DEL) *************************/
        movFinal->num = contador;
        movFinal->entrada = inicio->valor;
        imprimirPila(nodoSuperior, nodoFondo, inicio->valor);
        /************************************************************/

        if (e != -1)
        {
            switch(realizarAccion(e))
            {
            case 0:
                /************************* (OMITIR) *************************
                if (inicio->valor.compare("inicio") == 0 || inicio->valor.compare(";") == 0)
                {
                    lineas++;
                    sentencia += inicio->valor;
                    integridad = "[ok!]";

                    cout<<lineas<<" "<<sentencia<<" "<<integridad<<endl;
                    sentencia = "";
                }
                else
                {
                    sentencia += inicio->valor + " ";
                }
                /************************************************************/

                inicio = inicio->siguiente; // Para Desplazamiento se prepara el siguiente Símbolo de Entrada (Token)
                break;
            case 1:
                break;
            case 2:
                /************************* (OMITIR) *************************
                lineas++;
                integridad = "[okay]";

                cout<<lineas<<" "<<sentencia<<integridad<<endl;
                /************************************************************/

                /************************* (NO-DEL) *************************/
                movFinal->num = 0;
                movFinal->pila = "0";
                movFinal->simbolo = "$";
                movFinal->entrada = "acc";
                movFinal->accion = "Aceptar";
                imprimirPila(nodoSuperior, nodoFondo, inicio->valor);
                /************************************************************/
                return 0;
                break;
            default:
                return -1;
                break;
            }
        }
        else
        {
            /************************* (OMITIR) *************************
            lineas++;
            sentencia += " >> " + inicio->valor;
            integridad = "[err!]";

            cout<<lineas<<" "<<sentencia<<" << "<<integridad<<"\n\n"<<endl;
            cout<<"ERRORES en linea: "<<lineas<<endl;
            /************************************************************/

            return -1;
        }
    }

    return -1;
}

/********************************************************************************/
int main ()
{
    if (!analizadorLexico(rutaPrograma))
    {
        if (!analizadorSintactico(tokenInicial, tokenFinal))
        {
            cout<<"*******************"<<endl;
            cout<<"COMPILADO CON EXITO"<<endl;
            cout<<"*******************"<<endl;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}
/********************************************************************************/
