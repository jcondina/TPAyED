#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

struct Venta
{
	char codEmp[9 + 1];
	int codProd;
	int fecha;
	float precioVenta;
};

struct ListaProducto
{
	int codProd;
	int fecha;
	struct ListaProducto *sig;
};

struct Empleado
{
	char codEmp[9 + 1];
	char nombYApe[50 + 1];
	int cantProdVend;
	float recaudacion;
	ListaProducto *productos;
};

typedef struct ListaProducto *nodoProducto;

void crearEmpleados()
{

	FILE *empleados = fopen("Empleados.dat", "wb+");

	Empleado vecE[4] = {
		{"EE", "Juan Gomez", 50},
		{"AA", "Pablo Perez", 100},
		{"ZZ", "Ariel Gimenez", 10},
		{"NN", "Roberto Diaz", 200}};

	fwrite(vecE, sizeof(Empleado), 4, empleados);
	fclose(empleados);
}

void mostrarEmpleados()
{
	FILE *empleados = fopen("Empleados.dat", "rb+");

	cout << "Codigo Empleado, Nombre y Apellido, Cantidad de productos Vendidos" << endl;
	Empleado e;
	fread(&e, sizeof(Empleado), 1, empleados);

	while (!feof(empleados))
	{
		cout << e.codEmp << ", " << e.nombYApe << ", " << e.cantProdVend << endl;
		fread(&e, sizeof(Empleado), 1, empleados);
	}

	cout << endl;
	fclose(empleados);
}

void crearVentas()
{

	FILE *ventas = fopen("Ventas.dat", "wb+");

	Venta vecV[11] = {
		{"EE", 1, 20141001, 40.0},
		{"ZZ", 5, 20141001, 30.0},
		{"EE", 2, 20141002, 15.0},
		{"AA", 5, 20141002, 30.0},
		{"AA", 5, 20141002, 30.0},
		{"EE", 2, 20141002, 15.0},
		{"ZZ", 3, 20141004, 70.0},
		{"ZZ", 3, 20141010, 70.0},
		{"EE", 2, 20141011, 15.0},
		{"ZZ", 4, 20141015, 60.0},
		{"ZZ", 5, 20141015, 30.0}};

	fwrite(vecV, sizeof(Venta), 11, ventas);
	fclose(ventas);
}

void mostrarVentas()
{
	FILE *ventas = fopen("Ventas.dat", "rb+");

	cout << "Codigo Empleado, Codigo Producto, Fecha, Precio de Venta" << endl;
	Venta v;
	fread(&v, sizeof(Venta), 1, ventas);

	while (!feof(ventas))
	{
		cout << v.codEmp << ", " << v.codProd << ", " << v.fecha << ", " << v.precioVenta << endl;
		fread(&v, sizeof(Venta), 1, ventas);
	}

	cout << endl;
	fclose(ventas);
}

void ordenarDescendente(Empleado arrEmpleados[], int arrEmpleadosSize)
{
	for (int i = 0; i < arrEmpleadosSize; i++)
	{
		for (int j = 0; j < arrEmpleadosSize; j++)
		{
			if (arrEmpleados[j].recaudacion < arrEmpleados[i].recaudacion)
			{
				Empleado tmpEmpleado = arrEmpleados[i];
				arrEmpleados[i] = arrEmpleados[j];
				arrEmpleados[j] = tmpEmpleado;
			}
		}
	}
}

nodoProducto crearNodo()
{
	nodoProducto aux;
	aux = (nodoProducto)malloc(sizeof(struct ListaProducto));
	aux->sig = NULL;
	return aux;
}

void agregarOrdenado(nodoProducto &cabeza, nodoProducto nuevoNodo)
{
	nodoProducto current;
	if (cabeza == NULL || cabeza->fecha <= nuevoNodo->fecha)
	{
		nuevoNodo->sig = cabeza;
		cabeza = nuevoNodo;
	}
	else
	{
		current = cabeza;
		while (current->sig != NULL &&
			   current->sig->fecha < nuevoNodo->fecha)
		{
			current = current->sig;
		}
		nuevoNodo->sig = current->sig;
		current->sig = nuevoNodo;
	}
}

void agregarVenta(nodoProducto &cabeza, Venta v)
{
	nodoProducto temp, p, aux;
	temp = crearNodo();
	temp->codProd = v.codProd;
	temp->fecha = v.fecha;

	agregarOrdenado(cabeza, temp);
}

void mostrarProductos(nodoProducto productos)
{
	nodoProducto aux = productos;
	while (aux != NULL)
	{
		cout << aux->codProd << "\t\t" << aux->fecha << endl;
		aux = aux->sig;
	}
};

void liberarLista(nodoProducto &cabeza)
{
	nodoProducto actual;
	while ((actual = cabeza) != NULL)
	{
		cabeza = cabeza->sig;
		free(actual);
	}
}

void resolucionTp()
{
	//TODO completar aquí con la resolución del TP
	// recordar usar la libreria string.h para el manejo de comparación y copia de valores de cadenas
	// funciones útiles para usar: strcmp y stcpy

	FILE *empleados = fopen("Empleados.dat", "rb+");
	FILE *ventas = fopen("Ventas.dat", "rb+");

	if (!ventas || !empleados)
	{
		cout << "No se pudo abrir alguno de los archivos, saliendo.." << endl;
		return;
	}

	Venta v;
	Empleado arrEmpleados[50] = {};
	size_t arrEmpleadosSize = sizeof(arrEmpleados) / sizeof(arrEmpleados[0]);

	fread(&arrEmpleados, sizeof(Empleado), 50, empleados);
	fclose(empleados);

	fread(&v, sizeof(Venta), 1, ventas);

	while (!feof(ventas))
	{
		for (int i = 0; i < arrEmpleadosSize; i++)
		{
			if (strcmp(arrEmpleados[i].codEmp, v.codEmp) == 0)
			{
				arrEmpleados[i].cantProdVend++;
				arrEmpleados[i].recaudacion += v.precioVenta;
				agregarVenta(arrEmpleados[i].productos, v);
			}
		}
		fread(&v, sizeof(Venta), 1, ventas);
	}

	fclose(ventas);

	ordenarDescendente(arrEmpleados, arrEmpleadosSize);

	for (int i = 0; i < arrEmpleadosSize; i++)
	{
		if (strcmp(arrEmpleados[i].codEmp, ""))
		{
			cout.precision(2);
			cout << "Codigo Empleado: " << arrEmpleados[i].codEmp << endl
				 << "Nombre y apellido: " << arrEmpleados[i].nombYApe << endl
				 << "Total de productos vendidos: " << arrEmpleados[i].cantProdVend << endl
				 << "Total recaudado: $" << fixed << arrEmpleados[i].recaudacion << endl;
			cout << "Productos Vendidos:" << endl
				 << "=================================" << endl
				 << "Codigo Producto\tFecha" << endl;
			mostrarProductos(arrEmpleados[i].productos);
			cout << "=================================" << endl
				 << endl;
		}
		liberarLista(arrEmpleados[i].productos);
	}
}

int main()
{
	crearEmpleados();
	mostrarEmpleados();
	crearVentas();
	mostrarVentas();

	resolucionTp();

	return 0;
}
