#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
     int id;
     char titulo[50];
     int anio;
     char genero[10];
     char actores[3][50];
     int cantidadDeActores; // Agrego este campo para poder almacenar la cantidad valida de actores
     int rating;
     int estado;
} pelicula;

typedef struct {
     int id;
     char nombreCompleto[50];
     char mail[50];
     char celular[30]; // Uso un string porque los numero superan lo permitido en int y long, ademas el usuario puede agregar el + en caso de que aclare el pais
     char password[8];
     pelicula miLista[50];
     int cantPeliculas; // Agrego este campo para poder almacenar la cantidad valida de peliculas en la lista del usuario
     int estado;
} usuario;

void red();
void green();
void white();
void yellow();
void menu();
void menuPeliculas(int option1);
int validarTitulo(char archivoPeliculas[], char titulo[]);
int validarGenero(char genero[]);
int validarRating(int rating);
int validarEstado(int estado);
int validarMail(char archivoUsuarios[], char mail[]);
int validarPassword(char password[]);
int validarLista(usuario currentUser, char pelicula[]);

// ------------------------------------------------

void cargarPelicula(char archivoPeliculas[]);
int cargarActores(char actores[][50]);
void recorrerArchivoPelicula(char archivoPeliculas[]);
void recorrerArchivoPeliculasActivas(char archivoPeliculas[]);
void mostrarActores(char actores[][50], int cantidadDeActores);
void mostrarPelicula(pelicula peliculaActual);
int ultimoIdPelicula(char archivoPeliculas[]);
void arregloPorAnio(char archivoPeliculas[], pelicula arrayAnio[]);
int menorAnio(pelicula arrayAnio[], int pos, int validos );
void ordenarPorAnio(pelicula arrayAnio[], int validos);
void arregloPorTitulo(char archivoPeliculas[], pelicula arrayTitulo[]);
int menorTitulo(pelicula arrayTitulo[], int pos, int validos);
void ordenarPorTitulo(pelicula arrayTitulo[], int validos);
void mostrarArray(pelicula array[], int validos);
void generoPreferido(char archivoUsuarios[], usuario currentUser);

// ------------------------------------------------

void cargarUsuario(char archivoUsuarios[]);
void recorrerArchivoUsuario(char archivoUsuarios[]);
void mostrarUsuario(usuario usuarioActual);
int ultimoIdUsuario(char archivoUsuarios[]);
usuario encontrarUsuario(char archivoUsuarios[], char mail[], char password[]);
usuario login(usuario userToLogIn);
usuario logout(usuario currentUser);
void registro(char archivoUsuarios[]);
void mostrarPerfil(usuario usuarioActual);
usuario darDeBaja(char archivoUsuarios[], usuario currentUser);
usuario modificarUsuario(char archivoUsuarios[], usuario currentUser);
pelicula buscarPelicula(char archivoPeliculas[], char peliculaAAgregar[]);
usuario agregarALista(char archivoUsuarios[], char archivoPeliculas[], usuario currentUser);
void mostrarLista(pelicula miLista[], int validos);
usuario bajaPeliDeUsuario(char archivoUsuarios[], usuario currentUser);
void buscarPorNombre(char archivoPeliculas[],char aBuscar[]);
void mostrarPorGenero(char archivoPeliculas[], char genero[]);

int main()
{
    menu();
    return 0;
}

/// ----------------------------------------------------------------------------------- PELICULAS ---------------------------------------------------------------------------------------

void cargarPelicula(char archivoPeliculas[]){
    char control = 's';
    int i = ultimoIdPelicula(archivoPeliculas);
    pelicula nuevaPelicula;
    FILE *archivo = fopen(archivoPeliculas, "ab");

    if(archivo){
        printf("\nQuiere cargar una Pelicula? (s/n)\n");
        fflush(stdin);
        scanf("%c", &control);

        while(control == 's'){
            printf("\n-----------------------CARGANDO PELICULAS---------------------------\n");

            int flagTitulo = 1;
            while(flagTitulo == 1){
                printf("\nTitulo de la Pelicula:\n");
                fflush(stdin);
                gets(nuevaPelicula.titulo);
                flagTitulo = validarTitulo("archivoPeliculas.dat", nuevaPelicula.titulo);
            }

            printf("\nAnio de estreno:\n");
            scanf("%i", &nuevaPelicula.anio);

            int flagGenero = 1;
            while(flagGenero == 1){
                printf("\nGenero de la Pelicula (ACCION, COMEDIA, DRAMA, ROMANTICA, SUSPENSO o TERROR):\n");
                fflush(stdin);
                gets(nuevaPelicula.genero);
                flagGenero = validarGenero(nuevaPelicula.genero);
            }

            printf("\nActores de la Pelicula (Maximo 3):\n");
            nuevaPelicula.cantidadDeActores = cargarActores(&nuevaPelicula.actores); // Guardo la cantidad de Actores que se cargaron

            printf("\nRating de la Pelicula (1-5):\n");
            int rating;
            scanf("%i", &rating);
            nuevaPelicula.rating = validarRating(rating);

            printf("\nEstado de la Pelicula (1 = Activa / 0 = No Activa):\n");
            int estado;
            scanf("%i", &estado);
            nuevaPelicula.estado = validarEstado(estado);


            printf("\n--------------------------------------------------------------------\n");

            i++; // El incremento va antes de setear el ID !!!
            nuevaPelicula.id = i;

            fwrite(&nuevaPelicula, sizeof(pelicula), 1, archivo);

            green();
            printf("\nPelicula Cargada!\n");
            white();
            printf("\nCargar otra Pelicula? (s/n)\n");
            fflush(stdin);
            scanf("%c", &control);
        }
        fclose(archivo);
    }else {
        printf("El archivo en NULL");
    }
}

int cargarActores(char actores[][50]){
    int f = 0;
    char letra = 's';
    while(f<3 && letra == 's'){
        printf("\nIngrese un Actor\n");
        fflush(stdin);
        gets(actores[f]);
        f++;
        if(f<3){
            printf("\nDesea cargar otro Actor? (s/n)\n");
            fflush(stdin);
            scanf("%c", &letra);
        }
    }
   return f;
}

void recorrerArchivoPelicula(char archivoPeliculas[]){
    FILE *archivo = fopen(archivoPeliculas, "rb");
    pelicula peliculaActual;
    if(archivo){
        while(fread(&peliculaActual, sizeof(pelicula), 1, archivo) > 0){
            mostrarPelicula(peliculaActual);
        }
        fclose(archivo);
    }
}

void recorrerArchivoPeliculasActivas(char archivoPeliculas[]){
    FILE *archivo = fopen(archivoPeliculas, "rb");
    pelicula peliculaActual;
    if(archivo){
        while(fread(&peliculaActual, sizeof(pelicula), 1, archivo) > 0){
            if(peliculaActual.estado == 1){
                mostrarPelicula(peliculaActual);
            }
        }
        fclose(archivo);
    }
}

void mostrarActores(char actores[][50], int cantidadDeActores){
    int f = 0;
    while(f<cantidadDeActores){
        printf("\n%s", actores[f]);
        f++;
    }
}

void mostrarPelicula(pelicula peliculaActual){
    printf("\nTitulo: %s\n", peliculaActual.titulo);
    printf("ID: %d\n", peliculaActual.id);
    printf("Anio: %d\n", peliculaActual.anio);
    printf("Genero: %s\n", peliculaActual.genero);
    printf("Actores: \n");
    mostrarActores(peliculaActual.actores, peliculaActual.cantidadDeActores);
    printf("\n\nRating: %d\n", peliculaActual.rating);
    if(peliculaActual.estado == 1){
        green();
        printf("Activa\n");
        white();
    }else{
        red();
        printf("No Activa\n");
        white();
    }
    printf("\n***************************************************\n");
}

int ultimoIdPelicula(char archivoPeliculas[]){ // PARA AUTO-GENERAR EL ID CORRECTAMENTE
    FILE *archivo = fopen(archivoPeliculas, "r+b");
    pelicula peliculaActual;
    int id = 0;

    if(archivo){
        fseek(archivo, -1 * sizeof(pelicula), SEEK_END); // Uso el fseek() para bucar desde el indice que le indique, SEEK_END(2) pondria el indice desde el final del archivo.
        if(fread(&peliculaActual, sizeof(pelicula), 1, archivo) > 0){ // En caso de que el archivo exista pero este vacio, para no retornar basura hago la condicion.
            id = peliculaActual.id;
        }
        fclose(archivo);
    }
    return id;
}

void arregloPorAnio(char archivoPeliculas[], pelicula arrayAnio[]){
    FILE *archivo = fopen(archivoPeliculas, "rb");
    pelicula peliculaActual;
    int i = 0;

    if(archivo){
        while(fread(&peliculaActual, sizeof(pelicula), 1, archivo) > 0){
            if(peliculaActual.estado == 1){
                arrayAnio[i] = peliculaActual;
                i++;
            }
        }

        fclose(archivo);
    }

    ordenarPorAnio(arrayAnio, i);
}

int menorAnio(pelicula arrayAnio[], int pos, int validos ){
    pelicula menor = arrayAnio[pos];
    int posmenor = pos;
    int index = pos+1;
    while(index<validos){
        if(menor.anio < arrayAnio[index].anio){
            menor = arrayAnio[index];
            posmenor = index;
        }
        index++;
    }
    return posmenor;
}

void ordenarPorAnio(pelicula arrayAnio[], int validos){
    pelicula burbuja;
    int posmenor = 0;
    int i =0 ;
    while(i<(validos)){
        posmenor = menorAnio(arrayAnio, i, validos);
        burbuja = arrayAnio[posmenor];
        arrayAnio[posmenor] = arrayAnio[i];
        arrayAnio[i] = burbuja;
        i++;
    }
    mostrarArray(arrayAnio, validos);
}

void arregloPorTitulo(char archivoPeliculas[], pelicula arrayTitulo[]){
    FILE *archivo = fopen(archivoPeliculas, "rb");
    pelicula peliculaActual;
    int i = 0;

    if(archivo){
        while(fread(&peliculaActual, sizeof(pelicula), 1, archivo) > 0){
            if(peliculaActual.estado == 1){
                arrayTitulo[i] = peliculaActual;
                i++;
            }
        }

        fclose(archivo);
    }

    ordenarPorTitulo(arrayTitulo, i);
}

int menorTitulo(pelicula arrayTitulo[], int pos, int validos){
    pelicula menor = arrayTitulo[pos];
    int posmenor = pos;
    int index = pos+1;
    while(index<validos){
        if(strcmp(menor.titulo, arrayTitulo[index].titulo) > 0){
            menor = arrayTitulo[index];
            posmenor = index;
        }
        index++;
    }
    return posmenor;
}

void ordenarPorTitulo(pelicula arrayTitulo[], int validos){
    pelicula burbuja;
    int posmenor = 0;
    int i =0 ;
    while(i<(validos)){
        posmenor = menorTitulo(arrayTitulo, i, validos);
        burbuja = arrayTitulo[posmenor];
        arrayTitulo[posmenor] = arrayTitulo[i];
        arrayTitulo[i] = burbuja;
        i++;
    }
    mostrarArray(arrayTitulo, validos);
}

void mostrarArray(pelicula array[], int validos){
    int i = 0;
    while(i<validos){
        mostrarPelicula(array[i]);
        i++;
    }
}

/// ----------------------------------------------------------------------------------- USUARIOS ---------------------------------------------------------------------------------------

void cargarUsuario(char archivoUsuarios[]){
    char control = 's';
    int i = ultimoIdUsuario(archivoUsuarios);
    usuario nuevoUsuario;
    FILE *archivo = fopen(archivoUsuarios, "ab");

    if(archivo){
        printf("\nQuiere cargar un Usuario? (s/n)\n");
        fflush(stdin);
        scanf("%c", &control);

        while(control == 's'){
            printf("\n----------------------CARGANDO USUARIOS----------------------------\n");

            printf("\nNombre Completo:\n");
            fflush(stdin);
            gets(nuevoUsuario.nombreCompleto);

            int flagMail = 1;
            while(flagMail == 1){
                printf("\nMail:\n");
                fflush(stdin);
                gets(nuevoUsuario.mail);
                flagMail = validarMail("archivoUsuarios.dat", nuevoUsuario.mail);
            }

            printf("\nCelular:\n");
            fflush(stdin);
            gets(nuevoUsuario.celular);

            int flagPassword = 1;
            while(flagPassword == 1){
                printf("\Password:\n");
                fflush(stdin);
                gets(nuevoUsuario.password);
                flagPassword = validarPassword(nuevoUsuario.password);
            }

            printf("\nEstado del Usuario (1 = Activo / 0 = No Activo):\n");
            scanf("%i", &nuevoUsuario.estado);

            printf("\n---------------------------------------------------------------------\n");

            i++; // El incremento va antes de setear el ID !!!
            nuevoUsuario.id = i;
            nuevoUsuario.cantPeliculas = 0;

            fwrite(&nuevoUsuario, sizeof(usuario), 1, archivo);

            printf("\nCargar otro Usuario? (s/n)\n");
            fflush(stdin);
            scanf("%c", &control);
        }
        fclose(archivo);
    }else {
        printf("El archivo en NULL");
    }
}

void registro(char archivoUsuarios[]){
    int i = ultimoIdUsuario(archivoUsuarios);
    usuario nuevoUsuario;
    FILE *archivo = fopen(archivoUsuarios, "ab");

    if(archivo){
        printf("\n---------------------- CREAR UNA CUENTA ----------------------------\n");

        printf("\nNombre Completo:\n");
        fflush(stdin);
        gets(nuevoUsuario.nombreCompleto);

        printf("\nCelular:\n");
        fflush(stdin);
        gets(nuevoUsuario.celular);

        int flagMail = 1;
        while(flagMail == 1){
            printf("\nMail:\n");
            fflush(stdin);
            gets(nuevoUsuario.mail);
            flagMail = validarMail("archivoUsuarios.dat", nuevoUsuario.mail);
        }

        int flagPassword = 1;
        while(flagPassword == 1){
            printf("\nPassword:\n");
            fflush(stdin);
            gets(nuevoUsuario.password);
            flagPassword = validarPassword(nuevoUsuario.password);
        }

        printf("\n---------------------------------------------------------------------\n");

        i++;
        nuevoUsuario.id = i;
        nuevoUsuario.estado = 1;
        nuevoUsuario.cantPeliculas = 0;

        fwrite(&nuevoUsuario, sizeof(usuario), 1, archivo);

        fclose(archivo);
    }

    green();
    printf("\nRegistro Completo!\n");
    white();
}

void recorrerArchivoUsuario(char archivoUsuarios[]){
    FILE *archivo = fopen(archivoUsuarios, "rb");
    usuario usuarioActual;
    if(archivo){
        while(fread(&usuarioActual, sizeof(usuario), 1, archivo) > 0){
            mostrarUsuario(usuarioActual);
        }
        fclose(archivo);
    }
}

void mostrarUsuario(usuario usuarioActual){
    printf("\Nombre Completo: %s\n", usuarioActual.nombreCompleto);
    printf("ID: %i\n", usuarioActual.id);
    printf("Mail: %s\n", usuarioActual.mail);
    printf("Celular: %s\n", usuarioActual.celular);
    printf("Password: %s\n", usuarioActual.password); /// Deberia mostrarla?
    printf("Tus Peliculas \n"); /// Recorrer el array de las peliculas guardadas
    printf("Cantidad de Peliculas: %i\n", usuarioActual.cantPeliculas);
    if(usuarioActual.estado == 1){
        green();
        printf("Activo\n");
        white();
    }else{
        red();
        printf("No Activo\n");
        white();
    }
    printf("\n***************************************************\n");
}

void mostrarPerfil(usuario usuarioActual){
    printf("\n********************** TU PERFIL *******************\n\n");
    printf("\Nombre Completo: %s\n", usuarioActual.nombreCompleto);
    printf("Celular: %s\n", usuarioActual.celular);
    printf("Mail: %s\n", usuarioActual.mail);
    printf("\n\n********************** PELICULAS *******************\n");
    if(usuarioActual.cantPeliculas == 0){
        printf("\nTodavia no agregaste peliculas a tu lista\n\n");
    }else{
        mostrarLista(usuarioActual.miLista, usuarioActual.cantPeliculas);
    }

}

int ultimoIdUsuario(char archivoUsuarios[]){
    FILE *archivo = fopen(archivoUsuarios, "r+b");
    usuario usuarioActual;
    int id = 0;

    if(archivo){
        fseek(archivo, -1 * sizeof(usuario), SEEK_END); // Uso el fseek() para bucar desde el indice que le indique, SEEK_END(2) pondria el indice desde el final del archivo.
        if(fread(&usuarioActual, sizeof(usuario), 1, archivo) > 0){ // En caso de que el archivo exista pero este vacio, para no retornar basura hago la condicion.
            id = usuarioActual.id;
        }
        fclose(archivo);
    }
    return id;
}

usuario encontrarUsuario(char archivoUsuarios[], char mail[], char password[]){
    FILE *archivo = fopen(archivoUsuarios, "rb");
    int flag = 0;
    usuario usuarioABuscar;
    if(archivo){
        while( (flag == 0) && (fread(&usuarioABuscar, sizeof(usuario), 1, archivo)) > 0 ){
            if((strcmp(usuarioABuscar.mail, mail) == 0) && (strcmp(usuarioABuscar.password, password) == 0)){
                flag = 1;
            }
        }
        fclose(archivo);
    }
    fseek(archivo, (-1 * sizeof(usuario)), SEEK_CUR);
    if(flag == 0){
        usuarioABuscar.id = -1;
    }
    return usuarioABuscar;
}

usuario login(usuario userToLogIn){
    printf("****************************");
    printf("\nLog In\n");
    printf("****************************");
    char control = 's';
    while(control == 's'){

        char mail[60];
        printf("\nMail: ");
        fflush(stdin);
        gets(mail);

        char password[10];
        printf("\nPassword: ");
        fflush(stdin);
        gets(password);

        usuario usuarioBuscado = encontrarUsuario("archivoUsuarios.dat", mail, password);
        if(usuarioBuscado.id == -1){
            red();
            printf("\n\nLas credenciales que ingresaste son incorrectas\n\n");
            white();
            yellow();
            printf("\nQuieres intentar de vuelta? (s/n)\n");
            white();
            fflush(stdin);
            scanf("%c", &control);
        }else if(usuarioBuscado.estado == 0){
            red();
            printf("\n\nTu usuario esta dado de baja\n\n");
            white();
            control = 'n';
        }else{
            printf("\033[0;32m");
            printf("\nBienvenido %s!\n", usuarioBuscado.nombreCompleto);
            white();
            userToLogIn = usuarioBuscado;
            control = 'n';
        }
    }
    return userToLogIn;
}

usuario logout(usuario currentUser){
    red();
    printf("\nCerraste Sesion\n");
    white();
    currentUser.id = 0;
    return currentUser;
}

usuario darDeBaja(char archivoUsuarios[], usuario currentUser){
    FILE *archivo = fopen(archivoUsuarios, "r+b");
    int flag = 0;
    char control;
    usuario usuarioABuscar;
    if(archivo){
        while( (flag == 0) && (fread(&usuarioABuscar, sizeof(usuario), 1, archivo)) > 0 ){
            if(usuarioABuscar.id == currentUser.id){
                flag = 1;
            }
        }
        fseek(archivo, (-1 * sizeof(usuario)), SEEK_CUR);
        yellow();
        printf("\nEstas seguro que quieres dar de baja tu Usuario? (s/n)\n");
        white();
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's'){
            green();
            printf("\nDiste de baja tu Usuario\n");
            white();
            usuarioABuscar.estado = 0;
            fwrite(&usuarioABuscar, sizeof(usuario), 1, archivo);
            currentUser.id = 0;
        }

        fclose(archivo);
    }

    return currentUser;
}

usuario modificarUsuario(char archivoUsuarios[], usuario currentUser){
    FILE *archivo = fopen(archivoUsuarios, "r+b");
    int flag = 0;
    char control;
    usuario usuarioABuscar;
    if(archivo){
        while( (flag == 0) && (fread(&usuarioABuscar, sizeof(usuario), 1, archivo)) > 0 ){
            if(usuarioABuscar.id == currentUser.id){
                flag = 1;
            }
        }
        fseek(archivo, (-1 * sizeof(usuario)), SEEK_CUR);

        yellow();
        printf("\nModificar Nombre? (s/n)\n");
        white();
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's'){
            printf("\Nombre Completo:\n");
            fflush(stdin);
            gets(usuarioABuscar.nombreCompleto);
        }

        yellow();
        printf("\nModificar Mail? (s/n)\n");
        white();
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's'){
            int flagMail = 1;
            while(flagMail == 1){
                printf("\nMail:\n");
                fflush(stdin);
                gets(usuarioABuscar.mail);
                flagMail = validarMail("archivoUsuarios.dat", usuarioABuscar.mail);
            }
        }

        yellow();
        printf("\nModificar Celular? (s/n)\n");
        white();
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's'){
            printf("\nCelular:\n");
            fflush(stdin);
            gets(usuarioABuscar.celular);
        }

        yellow();
        printf("\nModificar Password? (s/n)\n");
        white();
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's'){
            printf("\Password:\n");
            fflush(stdin);
            gets(usuarioABuscar.password);
        }

        fwrite(&usuarioABuscar, sizeof(usuario), 1, archivo);
        currentUser = usuarioABuscar;

        fclose(archivo);
    }

    return currentUser;
}

usuario agregarALista(char archivoUsuarios[], char archivoPeliculas[], usuario currentUser){
    FILE*archivo = fopen(archivoUsuarios, "r+b");

    usuario usuarioABuscar;
    char peliculaAAgregar[100];
    pelicula peliculaEncontrada;
    int flag = 0;

    if(archivo)
    {
        while( (flag == 0) && (fread(&usuarioABuscar, sizeof(usuario), 1, archivo)) > 0 ){
            if(usuarioABuscar.id == currentUser.id){
                flag = 1;
            }
        }
        fseek(archivo, (-1 * sizeof(usuario)), SEEK_CUR);

        printf("\nQue pelicula quieres agregar?\n");
        fflush(stdin);
        gets(peliculaAAgregar);
        peliculaEncontrada = buscarPelicula(archivoPeliculas, peliculaAAgregar);
        int repetido = validarLista(currentUser, peliculaAAgregar);

        if(repetido == 1){
            yellow();
            printf("\nLa pelicula que buscaste ya esta en tu lista!\n");
            white();
        }else{
            if(peliculaEncontrada.id != 0){
                usuarioABuscar.miLista[usuarioABuscar.cantPeliculas] = peliculaEncontrada;
                usuarioABuscar.cantPeliculas = usuarioABuscar.cantPeliculas + 1;
                green();
                printf("\nAgregaste la pelicula a tu lista\n");
                white();
            }else{
                yellow();
                printf("\nLa pelicula que buscaste no existe o esta dada de baja!\n");
                white();
            }
        }

        fwrite(&usuarioABuscar, sizeof(usuario), 1, archivo);
        currentUser = usuarioABuscar;
        fclose(archivo);
    }

    return currentUser;
}

void mostrarLista (pelicula miLista[], int validos){
    int i = 0;
    for(i=0; i<validos; i++){
        if(miLista[i].estado == 1){
            mostrarPelicula(miLista[i]);
        }
    }
}

pelicula buscarPelicula(char archivoPeliculas[], char peliculaAAgregar[]){
    pelicula peliculaABuscar;
    int flag = 0;
    FILE*archivo = fopen(archivoPeliculas, "rb");

    if(archivo){
        while( (flag == 0) && (fread(&peliculaABuscar, sizeof(pelicula), 1, archivo)) > 0 ){
            if(strcmp(peliculaABuscar.titulo, peliculaAAgregar) == 0 && peliculaABuscar.estado == 1){
                flag = 1;
            }
        }
        if(flag == 1){
            fseek(archivo, (-1 * sizeof(pelicula)), SEEK_CUR);
        }else{
            peliculaABuscar.id = 0;
        }
        fclose(archivo);
    }
    return peliculaABuscar;
}

usuario bajaPeliDeUsuario(char archivoUsuarios[], usuario currentUser){
    FILE *archivoUser = fopen(archivoUsuarios, "r+b");
    int flag = 0;
    int i = 0;
    char pelicula[30];
    usuario usuarioABuscar;

    if(archivoUser){
        while( (flag == 0) && (fread(&usuarioABuscar, sizeof(usuario), 1, archivoUser)) > 0 ){
            if(usuarioABuscar.id == currentUser.id){
                flag = 1;
            }
        }
        fseek(archivoUser, (-1 * sizeof(usuario)), SEEK_CUR);
        printf("\nIngrese el nombre la pelicula que quiere quitar de su lista:\n");
        fflush(stdin);
        gets(pelicula);
        i = 0;
        flag = 0;
        if(validarLista(currentUser, pelicula) == 1){
           while(flag == 0){
                if(strcmp(currentUser.miLista[i].titulo, pelicula) == 0){
                    flag = 1;
                }else{
                    i++;
                }
           }
            usuarioABuscar.miLista[i].estado = 0;
            fwrite(&usuarioABuscar, sizeof(usuario), 1, archivoUser);
            currentUser.miLista[i].estado = 0;
            green();
            printf("\nLa pelicula %s fue quitada de la lista\n", currentUser.miLista[i].titulo);
            white();
        }
        else{
            yellow();
            printf("\nEsa pelicula no esta en tu lista!\n");
            white();
        }
         fclose(archivoUser);
    }
    return currentUser;
}


void buscarPorNombre(char archivoPeliculas[],char aBuscar[]){
    pelicula peliBuscada;
    pelicula resultado[50];
    int i=0;

    FILE *archivo = fopen(archivoPeliculas, "rb");

    if(archivo){
        while(fread(&peliBuscada, sizeof(pelicula), 1, archivo) > 0){
            if(strstr(peliBuscada.titulo, aBuscar)!= NULL && peliBuscada.estado == 1){
                resultado[i] = peliBuscada;
                i++;
            }
        }
        fclose(archivo);
    }
    if(i>0){
        mostrarArray(resultado, i);
    }else{
        yellow();
        printf("\nNo encontramos ninguna pelicula\n");
        white();
    }
}

void mostrarPorGenero(char archivoPeliculas[], char genero[]){
    FILE *pArchi = fopen(archivoPeliculas, "rb");
    pelicula aux;
    if(pArchi){
        while(fread(&aux,sizeof(pelicula),1 ,pArchi) > 0){
            if(strcmpi(aux.genero, genero) == 0 && aux.estado == 1){
                mostrarPelicula(aux);
            }
        }
        fclose(pArchi);
    }
}

void generoPreferido(char archivoUsuarios[], usuario currentUser){
    FILE *archivo = fopen(archivoUsuarios, "rb");
    usuario aux;
    int i = 0;
    int flag = 0;
    int accion = 0;
    int comedia = 0;
    int drama = 0;
    int romantica = 0;
    int suspenso = 0;
    int terror = 0;

    if(archivo){
        while(flag == 0 && fread(&aux, sizeof(usuario), 1, archivo) > 0){
            if(aux.id == currentUser.id){
                flag = 1;
            }
        }

        while(i < aux.cantPeliculas){
            if(strcmp(aux.miLista[i].genero, "ACCION") == 0){
                accion++;
            }else if(strcmp(aux.miLista[i].genero, "DRAMA") == 0){
                drama++;
            }else if(strcmp(aux.miLista[i].genero, "ROMANTICA") == 0){
                romantica++;
            }else if(strcmp(aux.miLista[i].genero, "SUSPENSO") == 0){
                suspenso++;
            }else if(strcmp(aux.miLista[i].genero, "TERROR") == 0){
                terror++;
            }else if(strcmp(aux.miLista[i].genero, "COMEDIA") == 0){
                comedia++;
            }
            i++;
        }

        fclose(archivo);
    }

    if(accion>drama && accion>terror && accion>comedia && accion>romantica && accion>suspenso){
        mostrarPorGenero("archivoPeliculas.dat", "ACCION");
    }else if(comedia>accion && comedia>terror && comedia>drama && comedia>romantica && comedia>suspenso){
        mostrarPorGenero("archivoPeliculas.dat", "COMEDIA");
    }else if(drama>accion && drama>terror && drama>comedia && drama>romantica && drama>suspenso){
        mostrarPorGenero("archivoPeliculas.dat", "DRAMA");
    }else if(romantica>accion && romantica>terror && romantica>drama && romantica>comedia && romantica>suspenso){
        mostrarPorGenero("archivoPeliculas.dat", "ROMANTICA");
    }else if(suspenso>accion && suspenso>terror && suspenso>drama && suspenso>romantica && suspenso>comedia){
        mostrarPorGenero("archivoPeliculas.dat", "SUSPENSO");
    }else if(terror>accion && terror>comedia && terror>drama && terror>romantica && terror>suspenso){
        mostrarPorGenero("archivoPeliculas.dat", "TERROR");
    }else if(accion == 0 && comedia == 0 && drama == 0 && romantica == 0 && suspenso == 0 && terror == 0){
        yellow();
        printf("\nNo tienes peliculas en tu lista!\n");
        printf("\nAgrega peliculas a tu lista para poder recomendarte mas\n\n");
        white();
    }
}


/// ----------------------------------------------------------------------------------- VALIDACIONES ---------------------------------------------------------------------------------------

int validarLista(usuario currentUser, char pelicula[]){
    for(int i=0; i<currentUser.cantPeliculas; i++){
        if(strcmp(currentUser.miLista[i].titulo, pelicula) == 0){
            return 1;
        }
    }
    return 0;
}

int validarPassword(char password[]){
    if(strlen(password) > 8 ){
        yellow();
        printf("\nLa password no puede tener mas de 8 caracteres\n");
        printf("Intenta con otra password\n");
        white();
        return 1;
    }
    return 0;
}

int validarMail(char archivoUsuarios[], char mail[]){
    if(strstr(mail, "@") == NULL){
        yellow();
        printf("\nEste mail no es valido\n");
        printf("Intenta con otro mail\n");
        white();
        return 1;
    }

    FILE *archivo = fopen(archivoUsuarios, "rb");
    usuario usuarioActual;
    if(archivo){
        while(fread(&usuarioActual, sizeof(usuario), 1, archivo) > 0){
            if(strcmp(usuarioActual.mail, mail) == 0){
                yellow();
                printf("\nEste mail ya se encuentra registrado\n");
                printf("Intenta con otro mail\n");
                white();
                return 1;
            }
        }
        fclose(archivo);
    }
    return 0;
}

int validarTitulo(char archivoPeliculas[], char titulo[]){
    FILE *archivo = fopen(archivoPeliculas, "rb");
    int flag = 0;
    pelicula peliculaActual;
    if(archivo){
        while(flag == 0 && fread(&peliculaActual, sizeof(pelicula), 1, archivo) > 0){
            if(strcmp(peliculaActual.titulo, titulo) == 0){
                yellow();
                printf("\nEsta Pelicula ya se encuentra en la Base de Datos\n");
                printf("Intenta cargar otra Pelicula\n");
                white();
                flag = 1;
            }
        }
        fclose(archivo);
    }
    return flag;
}

int validarGenero(char genero[]){
    if(strcmp(genero, "ACCION") == 0 || strcmp(genero, "COMEDIA") == 0 || strcmp(genero, "DRAMA") == 0 || strcmp(genero, "ROMANTICA") == 0 || strcmp(genero, "SUSPENSO") == 0 || strcmp(genero, "TERROR") == 0){
        return 0;
    }else{
        yellow();
        printf("\nEl genero que ingresaste no es correcto\n");
        printf("Generos Permitidos: ACCION, COMEDIA, DRAMA, ROMANTICA, SUSPENSO o TERROR\n");
        printf("Intenta de vuelta\n");
        white();
        return 1;
    }
}

int validarRating(int rating){
    while( rating < 1 || rating > 5){
        yellow();
        printf("\nEl rating debe ser entre 1 y 5\n");
        printf("Intentalo de vuelta\n");
        white();
        printf("\nRating de la Pelicula (1-5):\n");
        scanf("%i", &rating);
    }
    return rating;
}

int validarEstado(int estado){
    while( estado != 0 && estado != 1){
        yellow();
        printf("\nEl estado debe ser 1 (Activa) o 0 (No Activa)\n");
        printf("Intentalo de vuelta\n");
        white();
        printf("\nEstado de la Pelicula (1 = Activa / 0 = No Activa):\n");
        scanf("%i", &estado);
    }
    return estado;
}

/// ----------------------------------------------------------------------------------- MENU ---------------------------------------------------------------------------------------

void red() {
  printf("\033[1;31m");
}

void green() {
  printf("\033[0;32m");
}

void white() {
  printf("\033[0;37m");
}

void yellow() {
  printf("\033[0;33m");
}

void menu(){
    system("cls");
    char control = 's';
    int option, option1, option2;
    usuario currentUser;

    while(control == 's'){
        printf("\nBIENVENIDO A PRIME VIDEO\n");
        printf("-----------------------------------------\n\n");
        printf("Ingrese una opcion\n\n");
        printf("[1] - Peliculas\n");
        printf("[2] - Usuario\n");

        fflush(stdin);
        scanf("%i", &option);
        system("cls;");

        switch(option){
        case 1: ;
        printf("\PELICULAS\n");
        printf("-----------------------------------------\n");
        menuPeliculas(option1);

        system("cls");
        break;

        case 2: ;
            if(currentUser.id == 0){
                printf("\nUSUARIOS NO LOGUEADOS\n");
                printf("-----------------------------------------\n\n");
                printf("Ingrese una opcion\n\n");
                printf("[1] - Log In\n");
                printf("[2] - Registro\n");

                fflush(stdin);
                scanf("%i", &option2);
                system("cls;");
                switch(option2){
                    case 1: ;
                    currentUser = login(currentUser);

                    system("Pause");
                    system("cls");
                    break;

                    case 2: ;
                    registro("archivoUsuarios.dat");

                    system("Pause");
                    system("cls");
                    break;

            }
            }else if(currentUser.id == 1){
                printf("\nADMIN PRIME VIDEO\n");
                printf("-----------------------------------------\n\n");
                printf("Ingrese una opcion\n\n");
                printf("[1] - Cargar Peliculas\n");
                printf("[2] - Mostrar listado de Peliculas (Activas y No Activas)\n");
                printf("[3] - Mostrar listado de Usuarios\n");
                printf("[4] - Cargar Usuarios\n");
                printf("[5] - Log Out\n");

                fflush(stdin);
                scanf("%i", &option2);
                system("cls;");
                switch(option2){
                    case 1: ;
                    cargarPelicula("archivoPeliculas.dat");

                    system("Pause");
                    system("cls");
                    break;

                    case 2: ;
                    printf("\nListado de Peliculas\n");
                    recorrerArchivoPelicula("archivoPeliculas.dat");

                    system("Pause");
                    system("cls");
                    break;

                    case 3: ;
                    printf("\nListado de Usuarios\n");
                    recorrerArchivoUsuario("archivoUsuarios.dat");

                    system("Pause");
                    system("cls");
                    break;

                    case 4: ;
                    cargarUsuario("archivoUsuarios.dat");

                    system("Pause");
                    system("cls");
                    break;

                    case 5: ;
                    currentUser = logout(currentUser);

                    system("Pause");
                    system("cls");
                    break;
                }
            }else{
                green();
                printf("\Hola %s\n", currentUser.nombreCompleto);
                printf("-----------------------------------------\n\n");
                white();
                printf("Ingrese una opcion\n\n");
                printf("[1] - Ver Perfil\n");
                printf("[2] - Modificar el Usuario\n");
                printf("[3] - Ver Mis Recomendados\n");
                printf("[4] - Agregar una pelicula a mi lista\n");
                printf("[5] - Quitar una pelicula de mi lista\n");
                printf("[6] - Log Out\n");
                printf("[7] - Dar de baja el Usuario\n");

                fflush(stdin);
                scanf("%i", &option2);
                system("cls;");
                switch(option2){
                    case 1: ;
                    mostrarPerfil(currentUser);

                    system("Pause");
                    system("cls");
                    break;

                    case 2: ;
                    currentUser = modificarUsuario("archivoUsuarios.dat", currentUser);

                    system("Pause");
                    system("cls");
                    break;

                    case 3: ;
                    printf("\nRecomendados\n\n");
                    generoPreferido("archivoUsuarios.dat", currentUser);

                    system("Pause");
                    system("cls");
                    break;

                    case 4: ;
                    char opcion;
                    printf("\nQuiere ver el listado de Peliculas? (s/n)\n");
                    fflush(stdin);
                    scanf("%c", &opcion);
                    if(opcion == 's'){
                        menuPeliculas(option1);
                    }

                    control = 's';
                    while(control == 's'){
                        currentUser = agregarALista("archivoUsuarios.dat", "archivoPeliculas.dat", currentUser);
                        yellow();
                        printf("\nCargar otra pelicula? (s/n)\n");
                        white();
                        fflush(stdin);
                        scanf("%c", &control);
                    }


                    system("Pause");
                    system("cls");
                    break;

                    case 5: ;
                    printf("\n\n********************** PELICULAS FAVORITAS *******************\n");
                    if(currentUser.cantPeliculas == 0){
                        printf("\nTodavia no agregaste peliculas a tu lista\n\n");
                    }else{
                        mostrarLista(currentUser.miLista, currentUser.cantPeliculas);
                        control = 's';
                        while(control == 's'){
                            currentUser = bajaPeliDeUsuario("archivoUsuarios.dat", currentUser);
                            yellow();
                            printf("\Dar de baja otra pelicula? (s/n)\n");
                            white();
                            fflush(stdin);
                            scanf("%c", &control);
                        }
                    }



                    system("Pause");
                    system("cls");
                    break;

                    case 6: ;
                    currentUser = logout(currentUser);

                    system("Pause");
                    system("cls");
                    break;

                    case 7: ;
                    currentUser = darDeBaja("archivoUsuarios.dat" ,currentUser);

                    system("Pause");
                    system("cls");
                    break;
                }
            }
        system("cls");
        break;

        default: ;
        yellow();
        printf("\nElegiste una opcion incorrecta\n");
        white();
        }

        yellow();
        printf("\nVolver al menu principal? (s/n)\n");
        white();
        fflush(stdin);
        scanf("%c", &control);
        system("cls");
    }
}

void menuPeliculas(int option1){
    printf("\nIngrese una opcion\n\n");
    printf("[1] - Listado completo de Peliculas\n");
    printf("[2] - Buscar una pelicula\n");
    printf("[3] - Ordenadar por Estreno\n");
    printf("[4] - Ordenadar por Titulo\n");
    printf("[5] - Ordenadar por Genero\n");

    fflush(stdin);
    scanf("%i", &option1);
    system("cls;");
    switch(option1){
        case 1: ;
        printf("\nListado de Peliculas\n");
        recorrerArchivoPeliculasActivas("archivoPeliculas.dat");

        system("Pause");
        system("cls");
        break;

        case 2: ;
        char busqueda[100];
        printf("\nBuscar una pelicula:\n");
        fflush(stdin);
        gets(busqueda);
        buscarPorNombre("archivoPeliculas.dat", busqueda);

        system("Pause");
        system("cls");
        break;

        case 3: ;
        pelicula arrayAnio[50];
        printf("\nOrdenadar por Estreno\n");
        arregloPorAnio("archivoPeliculas.dat", arrayAnio);

        system("Pause");
        system("cls");
        break;

        case 4: ;
        pelicula arrayTitulo[50];
        printf("\nOrdenadar por Titulo\n");
        arregloPorTitulo("archivoPeliculas.dat", arrayTitulo);

        system("Pause");
        system("cls");
        break;

        case 5: ;
        char generoAFiltrar[50];
        int generoValidado = 0;
        printf("\nOrdenadar por Genero\n");
        while(generoValidado == 0){
            printf("\nQue genero quiere buscar?\n");
            fflush(stdin);
            gets(generoAFiltrar);
            generoValidado = validarGenero(generoAFiltrar);
            if(generoValidado == 0){
                mostrarPorGenero("archivoPeliculas.dat", generoAFiltrar);
                generoValidado = 1;
            }else{
                generoValidado = 0;
            }
        }

        system("Pause");
        system("cls");
        break;
    }
}
