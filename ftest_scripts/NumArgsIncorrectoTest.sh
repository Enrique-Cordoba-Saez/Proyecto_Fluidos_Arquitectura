#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0


# Limpiar el archivo de salida de errores
error_output_file="./error_output.txt"
> "$error_output_file"

# Ejecutar el programa y redirigir la salida de errores al archivo
../build/fluid/fluid 5 ../files/small.fld ../files/small_out.fld sobrante 2> "$error_output_file"

# Mensaje de error esperado
expected_error_message="Error: Invalid   number of arguments: 4"

# Imprimir mensajes reales y esperados
actual_error_message=$(cat "$error_output_file")

echo "Mensaje de error real: $actual_error_message"
echo "Mensaje de error esperado: $expected_error_message"

# Comparar el mensaje de error con el mensaje esperado
if echo "$actual_error_message" | grep -q "$expected_error_message"; then
    echo "El programa generó el mensaje de error esperado."
else
    echo "ERROR: El mensaje de error no coincide con el esperado."
fi

