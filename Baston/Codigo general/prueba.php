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
$ultrasonido = $_GET['ult'];


// Insertar los datos en la base de datos
$sql = "INSERT INTO valores (longitud, latitud, posicion, ultrasonido) VALUES ('$longitud', '$latitud', '$posicion', '$ultrasonido')";

if ($conn->query($sql) === TRUE) {
    echo "Data inserted successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

// Cerrar la conexión
$conn->close();
?>

<?php

// Codigo 2 para railway 

// Conectar a la base de datos MySQL
$host = 'tmysql.railway.internal';  // El host de tu base de datos MySQL en Railway
$db = 'datos_baston';  // Nombre de tu base de datos
$user = 'root';  // Usuario de tu base de datos
$pass = 'dmzLWlPlOIiMGMjokiKpkrQIWrokPQsq';  // Contraseña de tu base de datos

$dsn = "mysql:host=$host;dbname=$db;charset=utf8mb4";
$options = [
    PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    PDO::ATTR_EMULATE_PREPARES => false,
];

try {
    $pdo = new PDO($dsn, $user, $pass, $options);
} catch (PDOException $e) {
    echo json_encode(['status' => 'error', 'message' => 'Error en la conexión a la base de datos: ' . $e->getMessage()]);
    exit;
}

// Verificar si la solicitud es POST
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    // Obtener los datos JSON enviados por Arduino
    $json_data = file_get_contents('php://input');
    $data = json_decode($json_data, true);
    
    // Verificar si los datos son válidos
    if ($data && isset($data['lon']) && isset($data['lat']) && isset($data['pos']) && isset($data['ult'])) {
        // Preparar la consulta SQL para insertar los datos en la tabla `datos_baston`
        $stmt = $pdo->prepare("INSERT INTO datos_baston (longitud, latitud, posicion, ultrasonido) VALUES (:sensor1, :sensor2, :sensor3, :sensor4)");
        $stmt->execute([
            ':sensor1' => $data['lon'],
            ':sensor2' => $data['lat'],
            ':sensor3' => $data['pos'],
            ':sensor4' => $data['ult'],
        ]);
        
        // Responder con éxito
        echo json_encode(['status' => 'success', 'message' => 'Datos almacenados correctamente en datos_baston']);
    } else {
        // Responder con error si los datos no son válidos
        echo json_encode(['status' => 'error', 'message' => 'Datos inválidos']);
    }
} else {
    // Si no es una solicitud POST, devolver un mensaje de error
    echo json_encode(['status' => 'error', 'message' => 'Método de solicitud inválido']);
}
?>
