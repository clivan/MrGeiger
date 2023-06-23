import pandas as pd
import matplotlib.pyplot as plt
import tkinter as tk
from tkinter import filedialog
from tkinter import ttk

# Función para graficar los datos
def plot_data():
    # Obtener las columnas seleccionadas por el usuario
    column_x = combo_x.get()
    column_y = combo_y.get()

    # Graficar los datos
    plt.plot(df[column_x], df[column_y])
    plt.xlabel(column_x)
    plt.ylabel(column_y)
    plt.show()

# Crear la interfaz gráfica de usuario
root = tk.Tk()
root.title("Graficador de datos")

# Crear el botón para seleccionar el archivo CSV
button_file = tk.Button(root, text="Seleccionar archivo", command=lambda: load_data())
button_file.pack()

# Función para cargar los datos del archivo CSV y mostrar las columnas en los menús desplegables
def load_data():
    # Obtener la ruta del archivo CSV
    filepath = filedialog.askopenfilename(title="Seleccionar archivo", filetypes=[("CSV", "*.csv")])

    # Cargar los datos del archivo CSV en un dataframe de pandas
    global df
    df = pd.read_csv(filepath)

    # Agregar las columnas del dataframe a los menús desplegables
    combo_x['values'] = df.columns.tolist()
    combo_y['values'] = df.columns.tolist()

# Crear el menú desplegable para seleccionar la columna X
label_x = tk.Label(root, text="Seleccionar columna X:")
label_x.pack()
combo_x = ttk.Combobox(root)
combo_x.pack()

# Crear el menú desplegable para seleccionar la columna Y
label_y = tk.Label(root, text="Seleccionar columna Y:")
label_y.pack()
combo_y = ttk.Combobox(root)
combo_y.pack()

# Crear el botón para graficar los datos
button_plot = tk.Button(root, text="Graficar", command=plot_data)
button_plot.pack()

root.mainloop()

