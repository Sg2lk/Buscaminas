//JOHAN ALEXANDER PULUPA ROMERO
/*La ventaja de emplear clases se encuentra en que podemos atribuirle el "valor" que queramos para emplearlo como objetos y gracias a la herencia nos aporta una mayor comodidad a la
 hora de crear o eliminar objetos más o menos específicos ya que siempre partimos de una clase base con los atributos similares de las demas clases derivas.
 A su vez la sobrecarga de operadores nos beneficia a la hora de ampliar las capacidades que tienen tales operadores de manera predeterminada a la vez que nos pueden ayudar a simplificar
 lineas de codigo similares para emplearlo en una misma clase de objeto.*/

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <fstream>

HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );//linea implementada solamente para añadir color al tablero en terminal para que sea mas intuitivo visualmente

using namespace std;

class Tablero{
private:
    int ff{0},cc{0};

public:

    Tablero();

    Tablero(int x, int y)
    {
        setff(x);
        setcc(y);
    }

    void setff(int x)
    {
        ff=x;
    }

    int getff() const
    {
        return ff;
    }

    void setcc(int y)
    {
        cc=y;
    }

    int getcc() const
    {
        return cc;
    }

    ~Tablero(){}

};

class TableroJugador:public Tablero{
private:
    string nombre;
    int victorias{0};
    vector<vector<char>> Jugador;
    vector<vector<float>> Numeros;
public:

    TableroJugador();

    TableroJugador(int x, int y):Tablero(x,y)
    {
        vector<char> aux;
        aux.resize(getcc(),'o');
        Jugador.resize(getff(),aux);

        vector<float> temp;
        temp.resize(getcc(),0);
        Numeros.resize(getff(),temp);
    }

    void setNombre(const string &n)
    {
        nombre=n;
    }

    ~TableroJugador()//antes de finalizar el programa y limpiar los tableros guarda el nombre y las victorias del jugador en un fichero de texto
    {
        Jugador.clear();
        Numeros.clear();

        ifstream Lectura;
        Lectura.open("registro.txt",ios::in);

        ofstream aux;
        aux.open("auxiliar.txt",ios::out);

        string c;

        bool encontrado{0};

        if(Lectura.good())//comprueba si el archivo exite, si no crea uno nuevo
        {
            string auxN;
            string auxV;
            int temp{0};
            int cont{0};

            while(getline(Lectura,c))
            {
                for(int i{0};i<c.size();i++)
                    {
                        if(c.at(i)==' ')
                        {
                            auxN=c.substr(0,i);
                            auxV=c.substr(i+1);
                        }
                    }

                if(auxN==nombre)//si existe el usuario, modifica las victorias
                {
                    encontrado=true;
                    temp=stoi(auxV);
                    temp+=victorias;
                    auxV=to_string(temp);
                }

                aux<<auxN<<" "<<auxV<<endl;
            }

            if(encontrado==false)//si no existe, añade el nuevo usuario
            {
                aux<<nombre<<" "<<victorias<<endl;
            }

            aux.close();
            Lectura.close();

            remove("registro.txt");
            rename("auxiliar.txt","registro.txt");

        }

        else//crea el archivo para guardar al jugador y sus victorias si no existe el fichero de texto
        {
            ofstream Escritura;
            Escritura.open("registro.txt",ios::out);

            if(Escritura.fail())
            {
                cout << "Error al crear archivo" << endl;
                exit(1);
            }

            Escritura<<nombre<<" "<<victorias<<endl;
            Escritura.close();
        }
    }

    const vector<vector<char>>& getTableroJugador() const
    {
        return Jugador;
    }

    const vector<vector<float>>& getNumeros() const
    {
        return Numeros;
    }

    bool CasillaSeleccionada(int x, int y, const vector<vector<bool>>& aux)//metodo para comprobar si la partida sigue en curso o ha finalizado con victoria o derrota
    {
        if(aux.at(x).at(y)==1)//si la casilla seleccionada coincide con el de una mina, muestra el tablero con las minas descubiertas y un mensaje de derrota
        {
            SetConsoleTextAttribute(hConsole, 12);//estas lineas solo sirven para cambiar el color de partes del tablero
            cout << "-- FIN DEL JUEGO --" << endl;

            SetConsoleTextAttribute(hConsole, 15);
                cout << "   |";
                for(int i{0};i<Jugador.size();i++)
                    {
                        cout << " " << i << " |";
                    }
                    cout << endl;

                cout << "----";
                SetConsoleTextAttribute(hConsole, 3);

                for(int i{0};i<Jugador.size()*4;i++)
                    {
                        cout << "-";
                    }
                    cout << endl;

            for(int i{0};i<Jugador.size();i++)
            {
                SetConsoleTextAttribute(hConsole, 15);
                        cout << " " << i << " |";

                for(int j{0};j<Jugador.at(i).size();j++)
                    {

                        if(aux.at(i).at(j)==1)
                        {
                            Jugador.at(i).at(j)='*';
                            SetConsoleTextAttribute(hConsole, 6);
                            cout << " " << Jugador.at(i).at(j) << " |";
                        }

                        else
                        {
                            SetConsoleTextAttribute(hConsole, 6);
                            cout << " " << Jugador.at(i).at(j) << " |";
                        }
                    }

                    cout << endl;
                    SetConsoleTextAttribute(hConsole, 15);
                    cout << "----";

                for(int j{0};j<getff()*4;j++)
                    {
                        SetConsoleTextAttribute(hConsole, 3);
                        cout << "-";
                    }
                    cout << endl;
            }
            SetConsoleTextAttribute(hConsole, 7);
                return true;
        }

        else//si no se ha perdido la partida comprueba si quedan casillas sin descubrir que no sean minas o si estan señaladas todas las minas
        {
            int cont{0};
            for(int i{0};i<Jugador.size();i++)
                {
                    for(int j{0};j<Jugador.at(i).size();j++)
                        {
                            if(aux.at(i).at(j)==1 and Jugador.at(i).at(j)=='F')
                            {
                                cont++;
                            }
                        }
                }

            if(cont==5 or cont==10)
            {
                SetConsoleTextAttribute(hConsole, 10);
                cout << "--- VICTORIA!!! ---" << endl;

                SetConsoleTextAttribute(hConsole, 15);
                cout << "   |";

                for(int i{0};i<Jugador.size();i++)
                    {
                        cout << " " << i << " |";
                    }
                    cout << endl;

                cout << "----";
                SetConsoleTextAttribute(hConsole, 3);

                for(int i{0};i<Jugador.size()*4;i++)
                    {
                        cout << "-";
                    }
                    cout << endl;

                for(int i{0};i<Jugador.size();i++)//descubre todo el tablero al jugador si ha ganado la partida
                    {
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << " " << i << " |";

                    for(int j{0};j<Jugador.at(i).size();j++)
                        {
                            if(aux.at(i).at(j)==1)
                            {
                                Jugador.at(i).at(j)='*';
                                SetConsoleTextAttribute(hConsole, 6);
                                cout << " " << Jugador.at(i).at(j) << " |";
                            }

                            else
                            {
                                SetConsoleTextAttribute(hConsole, 6);
                                cout << " " << Jugador.at(i).at(j) << " |";
                            }
                        }

                    cout << endl;
                    SetConsoleTextAttribute(hConsole, 15);
                    cout << "----";

                for(int j{0};j<getff()*4;j++)
                    {
                        SetConsoleTextAttribute(hConsole, 3);
                        cout << "-";
                    }

                        cout << endl;

                    }

                victorias++;
                SetConsoleTextAttribute(hConsole, 7);
                return true;
            }

            else
            {
                for(int i{0};i<Jugador.size();i++)
                {
                    for(int j{0};j<Jugador.at(i).size();j++)
                        {
                            if(Numeros.at(i).at(j)==1 or Numeros.at(i).at(j)==2 or Numeros.at(i).at(j)==3 or Numeros.at(i).at(j)==4)
                            {
                                return false;
                            }

                            else if(Numeros.at(i).at(j)==5 or Numeros.at(i).at(j)==6 or Numeros.at(i).at(j)==7 or Numeros.at(i).at(j)==8)
                            {
                                return false;
                            }
                        }
                }
            }
        }

        SetConsoleTextAttribute(hConsole, 10);
        cout << "--- VICTORIA!!! ---" << endl;

        SetConsoleTextAttribute(hConsole, 15);
                cout << "   |";
                for(int i{0};i<Jugador.size();i++)
                    {
                        cout << " " << i << " |";
                    }
                    cout << endl;

                cout << "----";
                SetConsoleTextAttribute(hConsole, 3);

                for(int i{0};i<Jugador.size()*4;i++)
                    {
                        cout << "-";
                    }
                    cout << endl;

        for(int i{0};i<Jugador.size();i++)//descubre todo el tablero al jugador si ha ganado la partida
            {
                SetConsoleTextAttribute(hConsole, 15);
                        cout << " " << i << " |";

                for(int j{0};j<Jugador.at(i).size();j++)
                    {
                        if(aux.at(i).at(j)==1)
                        {
                            Jugador.at(i).at(j)='*';
                            SetConsoleTextAttribute(hConsole, 6);
                            cout << " " << Jugador.at(i).at(j) << " |";
                        }

                        else
                        {
                            SetConsoleTextAttribute(hConsole, 6);
                            cout << " " << Jugador.at(i).at(j) << " |";
                        }
                    }

                    cout << endl;
                    SetConsoleTextAttribute(hConsole, 15);
                    cout << "----";

                for(int j{0};j<getff()*4;j++)
                    {
                        SetConsoleTextAttribute(hConsole, 3);
                        cout << "-";
                    }
                    cout << endl;
            }

        victorias++;
        SetConsoleTextAttribute(hConsole, 7);
        return true;
    }

    void DescubrirCasillas(int x, int y, vector<vector<bool>> &tablerominas)//metodo para descubrir las casillas de alrededor si son ceros (es decir, si en la casilla no hay nignun numero o mina)
    {
        this->CasillasAdyacentesJugador(x,y,tablerominas);
    }

    void CasillasAdyacentesJugador(int x,int y, const vector<vector<bool>> &tablerominas)//metodo que se complementa con el anterior y "limpia" las casillas que se encuentran fuera la casilla actual
    {
        int FilaLI{0}, ColumnaLI{0}, FilaLF{0}, ColumnaLF{0};

        if(x<=0)
           FilaLI=0;

        else
            FilaLI=x-1;

        if(y<=0)
            ColumnaLI=0;

        else
            ColumnaLI=y-1;

        if(x+1>=getff())
            FilaLF=getff()-1;

        else
            FilaLF=x+1;

        if(y+1>=getcc())
            ColumnaLF=getcc()-1;

        else
            ColumnaLF=y+1;

        for(int i{FilaLI};i<=FilaLF;i++)
            {
                for(int j{ColumnaLI};j<=ColumnaLF;j++)
                    {

                            if(tablerominas.at(i).at(j)!=1)
                            {
                                if(Numeros.at(x).at(y)==0)
                                {
                                    this->CambiarCero(x,y);
                                    Jugador.at(x).at(y)=' ';
                                    if(Numeros.at(x).at(y)==9)
                                    {
                                        this->DescubrirCeros(FilaLI, ColumnaLI, FilaLF, ColumnaLF, x, y, tablerominas);
                                        this->SeguirCeros(FilaLI, ColumnaLI, FilaLF, ColumnaLF, x, y, tablerominas);
                                    }
                                }

                                else if(Numeros.at(x).at(y)==1)
                                {
                                    Jugador.at(x).at(y)='1';
                                    this->CambiarCero(x,y);
                                }

                                else if(Numeros.at(x).at(y)==2)
                                {
                                    Jugador.at(x).at(y)='2';
                                    this->CambiarCero(x,y);
                                }

                                else if(Numeros.at(x).at(y)==3)
                                {
                                    Jugador.at(x).at(y)='3';
                                    this->CambiarCero(x,y);
                                }

                                else if(Numeros.at(x).at(y)==4)
                                {
                                    Jugador.at(x).at(y)='4';
                                    this->CambiarCero(x,y);
                                }

                                else if(Numeros.at(x).at(y)==5)
                                {
                                    Jugador.at(x).at(y)='5';
                                    this->CambiarCero(x,y);
                                }

                                else if(Numeros.at(x).at(y)==6)
                                {
                                    Jugador.at(x).at(y)='6';
                                    this->CambiarCero(x,y);
                                }

                                else if(Numeros.at(x).at(y)==7)
                                {
                                    Jugador.at(x).at(y)='7';
                                    this->CambiarCero(x,y);
                                }

                                else if(Numeros.at(x).at(y)==8)
                                {
                                    Jugador.at(x).at(y)='8';
                                    this->CambiarCero(x,y);
                                }
                            }
                    }
            }
    }

    void SeguirCeros(int nF, int nC, int uF, int uC, int x, int y, const vector<vector<bool>> &tablerominas)
    {
        for(int i{nF};i<=uF;i++)
            {
                for(int j{nC};j<=uC;j++)
                    {
                        if(Numeros.at(i).at(j)==0)
                        {
                            this->CasillasAdyacentesJugador(i,j,tablerominas);
                        }
                    }
            }
    }

    void DescubrirCeros(int nF, int nC, int uF, int uC, int x, int y, const vector<vector<bool>> &tablerominas)//metodo para revelar las casillas adyacentes de la casilla selecciona por el jugador
    {
        for(int i{nF};i<=uF;i++)
            {
                for(int j{nC};j<=uC;j++)
                    {
                        if(Numeros.at(i).at(j)==0)
                        {
                            Jugador.at(i).at(j)=' ';
                        }

                        else if(Numeros.at(i).at(j)==1)
                        {
                            Jugador.at(i).at(j)='1';
                            this->CambiarCero(i,j);
                        }

                        else if(Numeros.at(i).at(j)==2)
                        {
                            Jugador.at(i).at(j)='2';
                            this->CambiarCero(i,j);
                        }

                        else if(Numeros.at(i).at(j)==3)
                        {
                            Jugador.at(i).at(j)='3';
                            this->CambiarCero(i,j);
                        }

                        else if(Numeros.at(i).at(j)==4)
                        {
                            Jugador.at(i).at(j)='4';
                            this->CambiarCero(i,j);
                        }

                        else if(Numeros.at(i).at(j)==5)
                        {
                            Jugador.at(i).at(j)='5';
                            this->CambiarCero(i,j);
                        }

                        else if(Numeros.at(i).at(j)==6)
                        {
                            Jugador.at(i).at(j)='6';
                            this->CambiarCero(i,j);
                        }

                        else if(Numeros.at(i).at(j)==7)
                        {
                            Jugador.at(i).at(j)='7';
                            this->CambiarCero(i,j);
                        }

                        else if(Numeros.at(i).at(j)==8)
                        {
                            Jugador.at(i).at(j)='8';
                            this->CambiarCero(i,j);
                        }
                    }
            }
    }

    void CambiarCero(int x, int y)//metodo para cambiar una casilla sin descubrir (numeros del 0-8) a una casilla descubierta (9)
    {
        if(Numeros.at(x).at(y)>=0 && Numeros.at(x).at(y)<9)
        {
            Numeros.at(x).at(y)=9;
        }
    }

    void CasillasAdyacentes(const vector<vector<bool>> &tablerominas)//metodo para establecer el tablero numeros oculto con las minas cercanas de cada casilla
    {
        cout << endl;
        for(int i{0};i<tablerominas.size();i++)//tablero con numeros-------------------------------
            {
                for(int j{0};j<tablerominas.at(i).size();j++)
                    {
                        Numeros.at(i).at(j) = this->minasCercanas(i,j,tablerominas);
                    }
            }
    }

    int minasCercanas(int x,int y, const vector<vector<bool>> &tablerominas)//metodo que cuenta las minas adyacentes de una casilla y a la vez limpia las casillas que no se pueden contar porque estan fuera del tablero
    {
        int FilaLI{0}, ColumnaLI{0}, FilaLF{0}, ColumnaLF{0}, cont;

        if(x<=0)
           FilaLI=0;

        else
            FilaLI=x-1;

        if(y<=0)
            ColumnaLI=0;

        else
            ColumnaLI=y-1;

        if(x+1>=getff())
            FilaLF=getff()-1;

        else
            FilaLF=x+1;

        if(y+1>=getcc())
            ColumnaLF=getcc()-1;

        else
            ColumnaLF=y+1;

        for(int i{FilaLI};i<=FilaLF;i++)
            {
                for(int j{ColumnaLI};j<=ColumnaLF;j++)
                    {
                            cont+=tablerominas.at(i).at(j);
                    }
            }
        return cont;
    }

    void ModificarMinas(const vector<vector<bool>> &tablerominas)//Este metodo cambia las minas del tablero de numeros a 1.1 para no confundirlo en otros bucles con el numero 1 de una mina cercana
    {
        for(int i{0};i<Numeros.size();i++)
            {
                for(int j{0};j<Numeros.at(i).size();j++)
                    {

                        if(tablerominas.at(i).at(j)==1)
                            Numeros.at(i).at(j)=1.1;
                    }
            }
    }

    void PrintNumeros() const//Este metodo imprime cada casilla con el numero de minas que tiene alrededor
    {
        for(int i{0};i<Numeros.size();i++)//tablero numeros---------------------------------
            {
                for(int j{0};j<Numeros.at(i).size();j++)
                    {
                        cout << " " << Numeros.at(i).at(j);
                    }
                   cout << endl;
            }
    }

    void ColocarBandera()
    {
        int filaBandera{0}, columnaBandera{0};

        cout << "fila de bandera: ";
        cin >> filaBandera;
        cout << "columna de bandera: ";
        cin >> columnaBandera;

        if(filaBandera<0 or filaBandera>=getff())
        {
            cout << "Error al introducir datos, pruebe de nuevo." << endl;
            this->ColocarBandera();
        }

        else if(columnaBandera<0 or columnaBandera>=getcc())
        {
            cout << "Error al introducir datos, pruebe de nuevo." << endl;
            this->ColocarBandera();
        }

        else
        {
            if(Jugador.at(filaBandera).at(columnaBandera)=='1'){cout << "casilla invalida, intentelo de nuevo." << endl;    this->ColocarBandera();}
            else if(Jugador.at(filaBandera).at(columnaBandera)=='2'){cout << "casilla invalida, intentelo de nuevo." << endl;    this->ColocarBandera();}
            else if(Jugador.at(filaBandera).at(columnaBandera)=='3'){cout << "casilla invalida, intentelo de nuevo." << endl;    this->ColocarBandera();}
            else if(Jugador.at(filaBandera).at(columnaBandera)=='4'){cout << "casilla invalida, intentelo de nuevo." << endl;    this->ColocarBandera();}
            else if(Jugador.at(filaBandera).at(columnaBandera)=='5'){cout << "casilla invalida, intentelo de nuevo." << endl;    this->ColocarBandera();}
            else if(Jugador.at(filaBandera).at(columnaBandera)=='6'){cout << "casilla invalida, intentelo de nuevo." << endl;    this->ColocarBandera();}
            else if(Jugador.at(filaBandera).at(columnaBandera)=='7'){cout << "casilla invalida, intentelo de nuevo." << endl;    this->ColocarBandera();}
            else if(Jugador.at(filaBandera).at(columnaBandera)=='8'){cout << "casilla invalida, intentelo de nuevo." << endl;    this->ColocarBandera();}
            else if(Jugador.at(filaBandera).at(columnaBandera)==' '){cout << "casilla invalida, intentelo de nuevo." << endl;    this->ColocarBandera();}
            else
                Jugador.at(filaBandera).at(columnaBandera)='F';
        }
    }

    void EliminarBandera()
    {
        int filaBandera{0}, columnaBandera{0};

        cout << "fila de bandera a eliminar: ";
        cin >> filaBandera;
        cout << "columna de bandera a eliminar: ";
        cin >> columnaBandera;

        if(filaBandera<0 or filaBandera>=getff())
        {
            cout << "Error al introducir datos, pruebe de nuevo." << endl;
            this->EliminarBandera();
        }

        else if(columnaBandera<0 or columnaBandera>=getcc())
        {
            cout << "Error al introducir datos, pruebe de nuevo." << endl;
            this->EliminarBandera();
        }

        else
        {
            if(Jugador.at(filaBandera).at(columnaBandera)=='F')
                Jugador.at(filaBandera).at(columnaBandera)='o';

            else if(Jugador.at(filaBandera).at(columnaBandera)==' ' or Jugador.at(filaBandera).at(columnaBandera)=='1' or Jugador.at(filaBandera).at(columnaBandera)=='2')
            {
                return;
            }

            else if(Jugador.at(filaBandera).at(columnaBandera)=='3' or Jugador.at(filaBandera).at(columnaBandera)=='4' or Jugador.at(filaBandera).at(columnaBandera)=='5')
            {
                return;
            }

            else if(Jugador.at(filaBandera).at(columnaBandera)=='6' or Jugador.at(filaBandera).at(columnaBandera)=='7' or Jugador.at(filaBandera).at(columnaBandera)=='8')
            {
                return;
            }

            else if(Jugador.at(filaBandera).at(columnaBandera)=='o')
            {
                return;
            }

            else
            {
                cout << "Opcion invalida, vuelva a intentarlo." << endl;
                this->EliminarBandera();
            }
        }
    }
};

class TableroMinas:public Tablero{
private:
    vector<vector<bool>> Minas;

public:

    TableroMinas();

    TableroMinas(int x, int y):Tablero(x,y)
    {
        vector<bool> aux;
        aux.resize(getcc(),0);
        Minas.resize(getff(),aux);
    }

    const vector<vector<bool>>& getMinas() const
    {
        return Minas;
    }

    void ColocarMinas(int m)//metodo que coloca las minas en una casilla aleatoria
    {
        int minasColocadas{0};

        while(minasColocadas<m)
        {
            int f,c;
            f=rand()%(getff()-1);
            c=rand()%(getcc()-1);

            if(Minas.at(f).at(c)==1)
                continue;

            Minas.at(f).at(c)=1;
            minasColocadas++;
        }
    }

    ~TableroMinas()
    {
        Minas.clear();
    }
};

ostream& operator <<(ostream &os, const TableroMinas &m)//sobrecarga para mostrar el tablero booleano con minas si se desea
{
    vector<vector<bool>> aux;
    aux=m.getMinas();

    for(int i{0};i<aux.size();i++)
            {
                for(int j{0};j<aux.at(i).size();j++)
                    {

                        os << " " << aux.at(i).at(j);
                    }

                os << endl;
            }

    return os;
}

ostream& operator <<(ostream &os, const TableroJugador &m)//sobrecarga para mostrar como transcurre el tablero visible para el jugador
{
    vector<vector<char>> aux;
    aux=m.getTableroJugador();

    SetConsoleTextAttribute(hConsole, 15);
    cout << "   |";
    for(int i{0};i<aux.size();i++)
        {
            os << " " << i << " |";
        }
        os << endl;

    os << "----";
    SetConsoleTextAttribute(hConsole, 3);

    for(int i{0};i<aux.size()*4;i++)
        {
            os << "-";
        }
        os << endl;

    for(int i{0};i<aux.size();i++)//tablero visible para jugador
            {
                SetConsoleTextAttribute(hConsole, 15);
                os << " " << i << " |";

                for(int j{0};j<aux.at(i).size();j++)
                    {
                        SetConsoleTextAttribute(hConsole, 6);
                        os << " " << aux.at(i).at(j) << " |";
                    }

                    os << endl;
                    SetConsoleTextAttribute(hConsole, 15);
                    os << "----";

                for(int j{0};j<aux.size()*4;j++)
                    {
                        SetConsoleTextAttribute(hConsole, 3);
                        os << "-";
                    }
                    os << endl;
            }

            SetConsoleTextAttribute(hConsole, 7);

        return os;
}

int main()
{
    srand(time(NULL));
    int filas{0}, columnas{0}, nivel{0}, minas{0}, filaJugador{0}, columnaJugador{0};
    bool finalizar{0};
    int op{1};
    string nombre;

    cout << "   ** Bienvenido al buscaminas **" << endl;
    cout << "Introduzca su nombre: ";
    getline(cin,nombre);

    system("cls");

    while(op == 1)
    {
        cout << "---- Seleccione el nivel de dificultad ----" << endl;
        cout << "Facil      -> 1" << endl;
        cout << "Experto    -> 2" << endl;
        cin >> nivel;
        system("cls");

        if(nivel==1)
        {
            filas=6;
            columnas=6;
            minas=5;
            cout << "minas a encontrar: 5" << endl;
        }

        else if(nivel==2)
        {
            filas=9;
            columnas=9;
            minas=10;
            cout << "minas a encontrar: 10" << endl;
        }

        else
        {
            cout << "Opcion invalida!!" << endl;
            return 1;
        }

        TableroJugador jj(filas,columnas);//Crea espacio para tablero jugador y numeros
        TableroMinas mm(filas,columnas);//Crea espacio para tablero minas

        jj.setNombre(nombre);

        mm.ColocarMinas(minas);

        vector<vector<bool>> temp;
        temp=mm.getMinas();

        jj.CasillasAdyacentes(temp);//Crea el contenido del tablero numeros

        while(finalizar==false)
        {
            char bandera;
            char quitarBandera;
            bool cb{0}, qb{0};

            while(cb==0)
            {
                system("cls");
                cout << jj;
                //jj.PrintNumeros();//quitar el comentario de esta linea si se quiere visualizar como trabaja el tablero de numeros
                //cout << mm;//quitar el comentario de esta linea si se quiere visualizar el tablero booleano con minas

                cout << "Desea colocar una bandera [s/n]: ";
                cin >> bandera;
                if(bandera=='s')
                {
                    cin.ignore();
                    jj.ColocarBandera();
                }

                else if(bandera=='n')
                {
                    cin.ignore();
                    cb=1;
                }

                else
                {
                    cout << "Opcion incorrecta, vuelva a intentarlo." << endl << endl;
                    cin.ignore();
                }
            }

            while(qb==0)
            {
                system("cls");
                cout << jj;

                cout << "Desea quitar alguna bandera [s/n]: ";
                cin >> quitarBandera;
                if(quitarBandera=='s')
                {
                    cin.ignore();
                    jj.EliminarBandera();
                }

                else if(quitarBandera=='n')
                {
                    cin.ignore();
                    qb=1;
                }

                else
                {
                    cout << "Opcion incorrecta, vuelva a intentarlo." << endl << endl;
                    cin.ignore();
                }
            }

            cout << "Introduce la fila: ";
            cin >> filaJugador;
            cout << "Introduce la columna: ";
            cin >> columnaJugador;

            try{

                if(filaJugador>=filas or filaJugador<0 or columnaJugador>=columnas or columnaJugador<0)//Comprobar que la casilla seleccionada esta dentro del tamaño de la matriz
                {
                    throw 1;
                }
            }

            catch(int x)
            {
                system("cls");
                cout << "Casilla no encontrada, vuelva a intentarlo." << endl << endl;
                continue;
            }

            jj.ModificarMinas(temp);

            jj.DescubrirCasillas(filaJugador,columnaJugador,temp);

            cb=0;

            system("cls");

            finalizar=jj.CasillaSeleccionada(filaJugador,columnaJugador,temp);
        }

        cout << endl;
        cout << "Eliga una opcion:" << endl;
        cout << "1 - Volver a jugar" << endl;
        cout << "2 - Salir" << endl;
        cin>>op;

        while(op<1 or op>2)
        {
            cout <<"Error, vuelva a intentarlo: ";
            cin >> op;
        }

        if(op==1)
            finalizar=false;

        system("cls");
    }

    return 0;
}
