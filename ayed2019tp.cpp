#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

struct Empleado {
	char codEmp[9 + 1];
	char nombYApe[50 + 1];
	int cantProdVend;
};

struct Venta {
	char codEmp[9 + 1];
	int codProd;
	int fecha;
	float precioVenta;
};

void crearEmpleados() {

	FILE* empleados = fopen("Empleados.dat","wb+");

	Empleado vecE[4] = {
		{ "EE", "Juan Gomez", 50 },
		{ "AA", "Pablo Perez", 100 },
		{ "ZZ", "Ariel Gimenez", 10 },
		{ "NN", "Roberto Diaz", 200 }
	};

	fwrite(vecE, sizeof(Empleado), 4, empleados);
	fclose(empleados);
}

void mostrarEmpleados() {
	FILE* empleados = fopen("Empleados.dat","rb+");

	cout << "Codigo Empleado, Nombre y Apellido, Cantidad de productos Vendidos" << endl;
	Empleado e;
	fread(&e, sizeof(Empleado), 1, empleados);

	while (!feof(empleados)) {
		cout << e.codEmp << ", " << e.nombYApe << ", " << e.cantProdVend << endl;
		fread(&e, sizeof(Empleado), 1, empleados);
	}

	cout << endl;
	fclose(empleados);
}

void crearVentas() {

	FILE* ventas = fopen("Ventas.dat","wb+");

	Venta vecV[11] = {
		{ "EE", 1, 20141001, 40.0 },
		{ "ZZ", 5, 20141001, 30.0 },
		{ "EE", 2, 20141002, 15.0 },
		{ "AA", 5, 20141002, 30.0 },
		{ "AA", 5, 20141002, 30.0 },
		{ "EE", 2, 20141002, 15.0 },
		{ "ZZ", 3, 20141004, 70.0 },
		{ "ZZ", 3, 20141010, 70.0 },
		{ "EE", 2, 20141011, 15.0 },
		{ "ZZ", 4, 20141015, 60.0 },
		{ "ZZ", 5, 20141015, 30.0 }
	};

	fwrite(vecV, sizeof(Venta), 11, ventas);
	fclose(ventas);
}

void mostrarVentas() {
	FILE* ventas = fopen("Ventas.dat","rb+");

	cout << "Codigo Empleado, Codigo Producto, Fecha, Precio de Venta" << endl;
	Venta v;
	fread(&v, sizeof(Venta), 1, ventas);

	while (!feof(ventas)) {
		cout << v.codEmp << ", " << v.codProd << ", " << v.fecha << ", " << v.precioVenta << endl;
		fread(&v, sizeof(Venta), 1, ventas);
	}

	cout << endl;
	fclose(ventas);
}

void ordenarDescendente(Empleado arrEmpleados[], int arrEmpleadosSize, float recaudacionPorEmpleado[]) {
	for (int i = 0; i < arrEmpleadosSize; i++)
	{
		for (int j = 0; j < arrEmpleadosSize; j++)
		{
			if (recaudacionPorEmpleado[j] < recaudacionPorEmpleado[i])
			{
				float tmpRecaudacion = recaudacionPorEmpleado[i];
				Empleado tmpEmpleado = arrEmpleados[i];
				recaudacionPorEmpleado[i] = recaudacionPorEmpleado[j];
				recaudacionPorEmpleado[j] = tmpRecaudacion;
				arrEmpleados[i] = arrEmpleados[j];
				arrEmpleados[j] = tmpEmpleado;
			}
		}
	}
}

void resolucionTp() {
	//TODO completar aquí con la resolución del TP
	// recordar usar la libreria string.h para el manejo de comparación y copia de valores de cadenas
	// funciones útiles para usar: strcmp y stcpy

	cout << "Parte A: Reporte de empleados usando vector estatico" << endl << endl;

	FILE* empleados = fopen("Empleados.dat","rb+");
	FILE* ventas = fopen("Ventas.dat","rb+");

	if (!ventas || !empleados) {
		cout << "No se pudo abrir uno de los archivos, saliendo.." << endl;
		return;
	}

	Venta v;
	Empleado arrEmpleados[50]= {};
	float recaudacionPorEmpleado[50] = {};
	size_t arrEmpleadosSize = sizeof(arrEmpleados)/sizeof(arrEmpleados[0]);

	fread(&arrEmpleados, sizeof(Empleado), 50, empleados);
	fclose(empleados);

	fread(&v, sizeof(Venta), 1, ventas);
	
	while (!feof(ventas)) {
		for (int i=0; i < arrEmpleadosSize; i++) {
			if (strcmp(arrEmpleados[i].codEmp, v.codEmp) == 0) {
				arrEmpleados[i].cantProdVend++;
				recaudacionPorEmpleado[i] += v.precioVenta;
			}
		}
		fread(&v, sizeof(Venta), 1, ventas);
	}

	fclose(ventas);

	ordenarDescendente(arrEmpleados, arrEmpleadosSize, recaudacionPorEmpleado);

	for (int i=0; i < arrEmpleadosSize; i++) {
		if (strcmp(arrEmpleados[i].codEmp, "")) {
			cout.precision(2);
			cout << "Codigo Empleado: " << arrEmpleados[i].codEmp << endl
			<< "Nombre y apellido: " << arrEmpleados[i].nombYApe << endl
			<< "Total de productos vendidos: " << arrEmpleados[i].cantProdVend << endl
			<< "Total recaudado: $" << fixed << recaudacionPorEmpleado[i] << endl << "\n";
		}
	}
}

int main() {
	crearEmpleados();
	mostrarEmpleados();
	crearVentas();
	mostrarVentas();

	resolucionTp();

	return 0;
}