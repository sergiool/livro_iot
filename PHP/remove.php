<html><body>

<?php

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

foreach($_POST as $val) {
	$sql = 	"DELETE FROM Usuarios WHERE TAG='" . $val . "'";
	$result = $conn->query($sql);
	if ($result) echo "Tag " . $val . " removida com sucesso. <BR>";
}
	
?>

Selecione as tags a serem excluídas:
<form action=remove.php method=post>

<?php

$sql = "SELECT TAG,Nome  FROM Usuarios";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) 
        echo "<input type=checkbox name=check" . $row["TAG"] . " value=" . $row["TAG"] . ">" . $row["TAG"] . "-" . $row["Nome"] . "<br>";
} else {
    echo "0 results";
}
$conn->close();
?>

<input type="submit" value="Submit">
</form>
</html></body>
