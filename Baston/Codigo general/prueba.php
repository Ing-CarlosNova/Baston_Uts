<?php
// Datos de conexión a la base de datos
$servername = "localhost";  // o la IP del servidor
$username = "root";         // usuario de la base de datos
$password = "";             // contraseña de la base de datos
$dbname = "baston_uts";

// Crear la conexión
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar la conexión
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Obtener los datos del ESP32
$longitud = $_GET['lon'];
$latitud = $_GET['lat'];
$posicion = $_GET['pos'];


// Insertar los datos en la base de datos
$sql = "INSERT INTO valores (longitud, latitud, posicion) VALUES ('$longitud', '$latitud', '$posicion' )";

if ($conn->query($sql) === TRUE) {
    echo "Data inserted successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

// Cerrar la conexión
$conn->close();
?>