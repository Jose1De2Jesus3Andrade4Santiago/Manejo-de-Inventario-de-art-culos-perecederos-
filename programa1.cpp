//LIBRERIAS A INCLUIR
#include<iostream>
#include<stdlib.h>
#include<string.h>
//DEFINICIONES 

using namespace std;
struct producto{
    int id;
    char descripcion[64];
    float precio;
    struct producto* atras = NULL;
};typedef producto* Producto;

#define N 3

//PROTOTIPOS
producto pedirDatosProducto();
FILE* openFile();
FILE* createfile();
void ingresarDatosArchivo(Producto primero);
void menu();
char* nombreArchivo();
FILE* open_closeFile(char s,FILE** archivo,char* nombreArchivo);
producto* leerFromArchivo();
int colaVacia(Producto primero,Producto ultimo);
void agregarAcola(Producto* primero,Producto* ultimo);
void imprimirCola(Producto primero);
char* copiar(char* cadena2,int inicio,int final);
void imprimirProducto(producto producto);
producto stringToProducto(char* cadena);
void burbuja(Producto desordenado, int tamanio);
void Binaria(Producto ordenado);

//FUNCION PRINCIPAL MAIN
int main(){

    //Punteros de la cola diamica
    Producto primero = NULL;
    Producto ultimo = NULL;

    //Agregamos 3 productos a la cola
    for(int i = 0; i < N;i++){
    	agregarAcola(&primero,&ultimo);
	}

    //Cargamos lod datos en un archivo
    ingresarDatosArchivo(primero);


    //Extraemos los datosdel archivo
    producto* desordenado = leerFromArchivo();

    //ordenamos el arreglo desordenado con el metodo burbuja
    burbuja(desordenado,N);

    //imprimimos arreglo ordenado
    cout<<"ELEMENTOS ORDENADOS"<<endl;
    for(int i = 0; i < N; i++){
        cout<<"----------"<<" producto "<<i<<" "<<"----------"<<endl;
        imprimirProducto(desordenado[i]);
    }
    system("pause");
    system("cls");

    //Buscamos un elemento en el arreglo ordenado
    Binaria(desordenado);

    return 0;
}

//FUNCIONES 

//Pedir datos de un producto
producto pedirDatosProducto(){
    producto newProducto;
    cout<<"Ingresa el Id del producto: "<<endl;
    cin>>newProducto.id;
    cout<<"Ingresa la descripcion del producto: "<<endl;
    cin>>newProducto.descripcion;
    cout<<"Ingresa el precio del producto: "<<endl;
    cin>>newProducto.precio;
    return newProducto;
}

//Funcion que imprime los datos de un elemento tipo producto
void imprimirProducto(producto producto){
    cout<<"ID del producto: "<<producto.id<<endl;
    cout<<"Descripcion del producto: "<<producto.descripcion<<endl;
    cout<<"Precio del producto: "<<producto.precio<<endl;
}

//Verifica si la cola esta o no vacia
int colaVacia(Producto primero,Producto ultimo){
    //Si nuestra cola esta vacia retorna un 1 y si no lo esta, retorna un 0
    if(primero == NULL & ultimo == NULL  )
        return 1;
    else return 0;
}

//Agrega elementos a la cola
void agregarAcola(Producto* primero,Producto* ultimo){
    Producto nuevoProdcuto = new producto();
	*nuevoProdcuto = pedirDatosProducto();

    //Caso 1: cola vacia
    if(colaVacia(*primero,*ultimo) == 1){
        *primero = nuevoProdcuto;
        *ultimo = nuevoProdcuto;
    }else{
        //Caso 2: cola no vacia
        Producto aux = *ultimo;
        aux->atras = nuevoProdcuto;
        *ultimo = nuevoProdcuto;
    }
    
}

//Imprime los datos de la cola
void imprimirCola(Producto primero){
    Producto aux = primero;
    while(aux != NULL){
        cout<<"ID del producto: "<<aux->id<<endl;
        cout<<"Descripcion del producto: "<<aux->descripcion<<endl;
        cout<<"Precio del producto: "<<aux->precio<<endl;
        aux=aux->atras;
    }
}

//Pedimos el nombre del archivo 
char* nombreArchivo(){
    char* nombreArchivo = new char[64];
    cout<<"Nombre del archivo que se desea abrir o crear: ";
    cin>>nombreArchivo;
    return nombreArchivo;
}

//Creamos un elemento de tipo archivo para operar
FILE* createfile(char* nombreArchivo){
    FILE* archivo;
    return archivo;
}

//Abrimos y cerramos un archivo
FILE* open_closeFile(char s,FILE** archivo,char* nombreArchivo){

    if(s == 'O'){
        //Abrimos el archivo, si no existe , entonces lo creamos con el modo "a+"
        if( (*archivo = fopen(nombreArchivo,"a+")) == NULL)
            cout<<"No fue posible abrir/crear el archivo"<<endl;
    }else{
        //cerramos el archivo
        if( fclose(*archivo) == 0){
            cout<<"El archivo se cerro con exito"<<endl;
        }else
            cout<<"No fue posible cerrar el archivo"<<endl;
    } 

    return *archivo;   
}

//Ingresamos datos en un archivo 
void ingresarDatosArchivo(Producto primero){

    //Abrimos el archivo en el que vamos a escribir
    char* nombreFile = nombreArchivo();
    FILE* archivo = createfile(nombreFile);
	archivo = open_closeFile('O',&archivo,nombreFile);

    //Pedimos los datos del producto
    Producto aux = primero;
    while(aux != NULL){
        fprintf(archivo,"%d",aux->id);
        fputs(" ",archivo);
        fputs(aux->descripcion,archivo);
        fputs(" ",archivo);
        fprintf(archivo,"%.3f",aux->precio);
        fputs("\n",archivo);
        aux=aux->atras;
    }
    //cerramos el archivo 
     open_closeFile('C',&archivo,nombreFile);
}

//Leemos los datos de un archivo
producto* leerFromArchivo(){

    //Abrimos el archivo del cual vamos a leer la informacion
    char* nombreFile = nombreArchivo();
    FILE* archivo = createfile(nombreFile);
    archivo = fopen(nombreFile,"r");

    //variables auxiliares para almacenar la informacion
    char* cadenaAux = new char[64];
    producto* productos = new producto[N];
    int i = 0;

    /*leemos linea por linea la informacion del archivo
    y convertimos a un elemento de tipo producto*/
    while(i < N){
        fgets(cadenaAux,64,archivo);
        productos[i] = stringToProducto(cadenaAux);
        i++;
    }
    //Cerramos el archivo 
		open_closeFile('C',&archivo,nombreFile);
	
    //Imprimimos los datos obtenidos del Archivo
    cout<<"Datos del archivo"<<endl;
    for(int i = 0; i < N; i++){
        cout<<"----------"<<" producto "<< (i+1) <<" "<<"----------"<<endl;
        imprimirProducto(productos[i]);
    }
    system("pause");
    system("cls");
    return productos;
}

//convertir cadena a campos de tipo producto
producto stringToProducto(char* cadena){

    producto newProducto;

    //Dividiremos la cadena en subcadenas delimitadas por espacios
    string cadenaaux = cadena;
    int espacio1 = cadenaaux.find_first_of(" ");
    int espacio2 = cadenaaux.find_last_of(" ");
    int final = cadenaaux.length();

    string str = copiar(cadena, espacio1 +1, espacio2);

   //Asignamos los valores (castings)
    newProducto.id =  stoi( copiar(cadena, 0, espacio1));
    strcpy(newProducto.descripcion, str.c_str());
    newProducto.precio =  stof( copiar(cadena, espacio2+1, final));

    return newProducto;
}

//copia el intervalo de un char[] a otro 
char* copiar(char* cadena2,int inicio,int final){

	 char* cadena1 = new char[64];
	 int i = 0;

	 while(inicio <= final){
        cadena1[i] = cadena2[inicio];
        inicio++;
        i++;
    }
    //Agregamos el caracter que marca el final del arreglo
    cadena1[i] = '\0';
    
    return cadena1;
}

//Ordenamos un arreglo con elementos de tipo producto con el metodo burbuja
void burbuja(producto* desordenado, int tamanio){

    producto temporal;
    for (int i = 0; i < tamanio; i++){
        for(int j = 0; j < (tamanio-1); j++){

            if (desordenado[j].id > desordenado[j+1].id){
                temporal = desordenado[j];
                desordenado[j] = desordenado[j+1];
                desordenado[j+1] = temporal;
            }

        }
    }
} 

//buscamos elemento de manera binaria
void Binaria(Producto ordenado){

    int derecho,izquierdo,bandera,medio,IDbuscado,opc;

    do{
        izquierdo = 0;
        derecho = N-1;
        bandera = 0;
 
        //Pedimos el dato a buscar
        cout<<"\nINGRESE EL ID DEL PRODUCTO QUE DESEA BUSCAR "<<endl;
        cin>>IDbuscado;

        while ( ( izquierdo <= derecho ) && (bandera == 0) ){
            medio = (izquierdo + derecho)/2;

            //caso donde el Id esta en la parte derecha el arreglo
            if (ordenado[medio].id < IDbuscado)
                izquierdo = medio+1;
            else{
                //caso donde el Id esta en la prte derecha el arreglo
                if (ordenado[medio].id > IDbuscado)
                     derecho = medio-1;
                else
                     bandera = 1; //El elemento ha sido encontrado
            }           
        }

        //Lo encontramos?  
        if ( bandera == 1 ) {
            cout<<"\nLO ENCONTRE\n";
            imprimirProducto(ordenado[medio]);
        }
        else cout<<"\nNO ESTA\n";
        
        //Preguntar si quiere volver a buscar
        cout<<"\nOTRO? 1. si 2. no\n";
        cin>>opc;
        system("cls");

    }while(opc == 1);
}
