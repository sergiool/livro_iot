<?php

if (!_POST['Data1']){

$servername = "mysql.hostinger.com.br";
$username = "u311824763_iot";
$password = "kedma256";
$dbname = "u311824763_iot";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT tag FROM Usuarios";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo $row["tag"] . "\r\n";
    }
} else {
    echo "0 results";
}
$conn->close();
?>